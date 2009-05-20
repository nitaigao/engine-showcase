/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickConstraintListener.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureCollisionListener.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit2/hkFixedConstraint.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include<Common/Base/Monitor/hkMonitorStream.h>

WallSimulation::WallSimulation(hkpWorld* mainWorld, hkpWorld* parallelWorld, hkReal timestep):
m_timeStep(timestep),
m_nextWallID(1),
m_mainWorld(mainWorld),
m_parallelWorld(parallelWorld),
m_collisionListener(0),
m_constraintListener(0),
m_wallsToBeChecked(0)
{
	// update reference counting
	mainWorld->addReference();
	parallelWorld->addReference();
}

void WallSimulation::autoSetup()
{
	
	// LOCK parallel wall
	m_parallelWorld->lock();

	// the parallel world doesn't need gravity
	m_parallelWorld->setGravity( hkVector4::getZero() );
	
	// dummy collision filter to avoid all collisions in the parallel simulation
	BrickFilterNoCollisions* bf = new BrickFilterNoCollisions();
	m_parallelWorld->setCollisionFilter( bf );
	bf->removeReference();

	// constraint listener that will pass info about broken constraints to the wall simulation
	m_constraintListener = new BrickConstraintListenerNotify( this );
	m_parallelWorld->addConstraintListener( m_constraintListener );

	// Collision listener, used to catch collisions in the main
	// world and apply them to the parallel simulation	
	m_collisionListener = new WallFractureCollisionListener(this, true);

	m_parallelWorld->unlock();
}

WallSimulation::~WallSimulation()
{
	{
		m_mainWorld->lock();
		hkPointerMap<int, hkpRigidBody*>::Iterator rbiter = m_wallsInSimulationBodies.getIterator();
		while(m_wallsInSimulationBodies.isValid(rbiter)) 
		{
			hkpRigidBody* wb = m_wallsInSimulationBodies.getValue(rbiter);
			wb->removeCollisionListener( m_collisionListener );
			rbiter = m_wallsInSimulationBodies.getNext(rbiter);
		}
		m_collisionListener->removeReference();
		m_mainWorld->removeReference();

		m_mainWorld->unlock();
	}

	{
		// remove all walls from the parallel world and delete the wrappers, (main world cleanup must be done elsewhere by the original owner)
		hkPointerMap<int, WallWrapper*>::Iterator iter = m_wallsInSimulationWrappers.getIterator();
		while(m_wallsInSimulationWrappers.isValid(iter)) 
		{
			WallWrapper* ww = m_wallsInSimulationWrappers.getValue(iter);
			delete m_wallsLastImpactVelocities.getWithDefault( ww->getID(), HK_NULL );
			delete ww; //m_wallsInSimulationWrappers.getValue(iter);
			iter = m_wallsInSimulationWrappers.getNext(iter);
		}
	}

	m_parallelWorld->lock();
	// remove added listeners and release the worlds
	m_parallelWorld->removeConstraintListener( m_constraintListener );
	m_constraintListener->removeReference();
	m_parallelWorld->unlock();
	
	m_parallelWorld->markForWrite();
	m_parallelWorld->removeReference();
	m_parallelWorld = HK_NULL;

	//
}

void WallSimulation::reserveWallIDs(int numIds, hkArray<int>& IDs)
{
	int lastID = m_nextWallID;
	m_nextWallID += numIds;
	for(int i=lastID; i< m_nextWallID; ++i)
	{
		IDs.pushBack(i);
	}
}

void WallSimulation::posOnGround(hkpWorld* world, const hkVector4& pos, hkVector4& newPosOut) const 
{
	// cast a ray to find exact position
	hkpWorldRayCastInput ray;
	ray.m_from = pos;
	ray.m_to = pos;
	ray.m_from(1) += 20.0f;
	ray.m_to(1) -= 20.0f;
	hkpWorldRayCastOutput rayOutput;
	world->castRay(ray,rayOutput);

	newPosOut.setInterpolate4(ray.m_from, ray.m_to, rayOutput.m_hitFraction);
}

WallWrapper* WallSimulation::createWallWrapper( const BrickwallBuilderDescriptor& wbDescriptor )
{
	hkArray<hkpRigidBody*> bricks;
	hkArray<hkpConstraintInstance*> constraints;

	// create the bricks of the wall
	BrickWallBuilder::buildBrickWall(wbDescriptor, bricks, constraints);

	// create the wall
	hkVector4 extents;
	extents.setMul4(2.0f, wbDescriptor.m_brickShape->getHalfExtents());
	WallWrapper* ww = new WallWrapper( bricks, constraints, extents );

	return ww;
}

WallWrapper* WallSimulation::addWallToSimulation(  const BrickwallBuilderDescriptor& bwDescriptor  )
{
	m_parallelWorld->lock();
		
	WallWrapper* wall;
	{
		// the wall in the parallel simulation has slightly different parameters
		BrickwallBuilderDescriptor parallelDescr( bwDescriptor );

		{
			// the bricks in the parallel must be centered in 0...
			hkVector4 origin( hkVector4::getZero() );
			posOnGround(m_mainWorld, origin, origin);
			parallelDescr.m_transform.set(hkQuaternion::getIdentity(), origin);
			//parallelDescr.m_position = origin;
			//// ...and have identity rotation
			//parallelDescr.m_orientation = hkQuaternion::getIdentity();
		}
		
		if( parallelDescr.m_attachToGround )
		{
			// attach the parallel brick wall to the "fixed" world 
			parallelDescr.m_theGround = m_parallelWorld->getFixedRigidBody();
		}
		// create the wall wrapper...
		wall = createWallWrapper( parallelDescr );

		if( bwDescriptor.m_attachToGround && bwDescriptor.m_theGround!=HK_NULL )
		{
			// the wall wrapper must be "logically attached" to the main world as 
			// this information is used to attach the rigid bodies created by the wrapper during fracture
			wall->attachToGround( bwDescriptor.m_theGround );
		}
	}

	// rigid body stiff representation of the wall
	hkpRigidBody* newWall=HK_NULL;
	// build the wall rigid body
	hkpConstraintInstance* groundConstraint = wall->generateBodyForThisWrapper(&newWall, bwDescriptor);
	// add the wrapper and the body
	storeWrapperAndBody( wall, newWall );
	// add the wrapper bricks to the parallel simulation
	addWallBricksToParallelWorld( wall );
	// add the rigid body to the main world
	addWallBodyToMainWorld( newWall );
	if(groundConstraint!=HK_NULL)
	{
		m_mainWorld->addConstraint( groundConstraint );
		groundConstraint->removeReference();
	}

	newWall->removeReference();

	if(m_collisionListener)
	{
		// set the collision listener
		newWall->addCollisionListener( m_collisionListener );
	}

	m_parallelWorld->unlock();

	return wall;
}

void WallSimulation::storeWrapperAndBody(WallWrapper* newWallWrapper, hkpRigidBody* newWallBody)
{
	// check if a new ID must be assigned
	if( !WallWrapper::isValidID(newWallWrapper->getID()) ) 
	{
		newWallWrapper->setID( reserveWallID() );
	}


	// check if another wall with the same ID is already present
	HK_ASSERT2(0x18a0fb9f, !(m_wallsInSimulationWrappers.hasKey(newWallWrapper->getID()) || m_wallsInSimulationBodies.hasKey(newWallWrapper->getID())) , "You are trying to add a wall that is already present");

	// insert into wrappers list
	m_wallsInSimulationWrappers.insert(newWallWrapper->getID(), newWallWrapper);
	m_wallsLastImpactVelocities.insert(newWallWrapper->getID(), new BodyVelocities() );
	// and in the wall bodies list
	m_wallsInSimulationBodies.insert(newWallWrapper->getID(), newWallBody);

	//if( newWallWrapper->IsOneBrick() )
	//	// 1 brick bodies won't generate collision events (i.e. they will be discarded)
	//	wallID = 0;

	newWallBody->setCollisionFilterInfo( BrickFilter::calcFilterInfo( 0, 0, newWallWrapper->getID() ) );
}

void WallSimulation::replaceWallWrapperAndBody(int wallID, WallWrapper* newWallWrapper, hkpRigidBody* newWallBody)
{
	WallWrapper* oldWallWrapper = getWallFromParallel(wallID);
	// the old wall can be deleted only if it's in the list
	HK_ASSERT2(0x18e01f04, (oldWallWrapper!=HK_NULL && m_wallsInSimulationBodies.getWithDefault(wallID, HK_NULL)!=HK_NULL ), "you are trying to replace something that wasn't in the list");

	HK_ASSERT2(0x5c2e1d6a, wallID==newWallWrapper->getID() ,"ID not set!!");
	HK_ASSERT2(0x5c2e1d6a, wallID==BrickFilter::getWallID(*newWallBody) ,"ID not set!!");
	
	// replace the old wall wrapper 
	m_wallsInSimulationWrappers.remove(wallID);
	m_wallsInSimulationWrappers.insert(wallID, newWallWrapper);

	// replace the old wall body 
	m_wallsInSimulationBodies.remove(wallID);
	m_wallsInSimulationBodies.insert(wallID, newWallBody);

	if( newWallWrapper->isOneBrick() )
		// 1 brick bodies won't generate collision events (i.e. they will be discraded)
		wallID = 0;
	// set the new rigid body collision filter info
	newWallBody->setCollisionFilterInfo( BrickFilter::calcFilterInfo(0,0,wallID) );

	// the old wrapper isn't useful anymore
	delete oldWallWrapper;
}


void WallSimulation::addWallBricksToParallelWorld(WallWrapper* wall)
{
	HK_ASSERT2(0x51e02060, wall != HK_NULL, "WallSimulation : You are trying to add a HK_NULL to the simulation");

	m_parallelWorld->lock();

	// Add wall bricks
	m_parallelWorld->addEntityBatch(reinterpret_cast<hkpEntity * const *>(wall->getBricks()), wall->getNumBricks(), HK_ENTITY_ACTIVATION_DO_NOT_ACTIVATE);
	// get wall constraints
	const hkArray<hkpConstraintInstance*> * constraints = wall->getConstraints();

	// Add constraints to the world
	for(int i=0; i<constraints->getSize(); ++i)
		m_parallelWorld->addConstraint((*constraints)[i]);

	wall->releaseWorldData();

	m_parallelWorld->unlock();
}


// removes the wall from the simulated world
void WallSimulation::removeWrapperAndBricks(int wallID)
{
	// check if the wall wrapper is actually present
	HK_ASSERT2(0x443d5803, m_wallsInSimulationWrappers.hasKey(wallID) , "You are trying to remove a non existent wall");

	m_parallelWorld->lock();

	WallWrapper* wall = getWallFromParallel( wallID );
	// remove the bricks from the parallel world
	m_parallelWorld->removeEntityBatch(reinterpret_cast<hkpEntity * const *>(wall->getBricks()), wall->getNumBricks());
	m_wallsInSimulationBodies.remove(wallID);
	// remove from wall wrapper
	removeWrapper( wallID );

	m_parallelWorld->unlock();
}

void WallSimulation::removeWrapper(int wallID)
{
	delete getWallFromParallel( wallID );
	m_wallsInSimulationWrappers.remove( wallID );
	delete m_wallsLastImpactVelocities.getWithDefault( wallID, HK_NULL );
	m_wallsLastImpactVelocities.remove(wallID);
}


void WallSimulation::notifyCollisionEvent(int wallID1, int wallID2, const hkVector4& appliedImpulse, const hkVector4& contactPoint, const CollisionBodiesVelocities& preCollisionVelocities)
{
	// store collision data for first wall
	if( WallWrapper::isValidID(wallID1) )
	{
		addCollisionEvent(wallID1, appliedImpulse, contactPoint, preCollisionVelocities.bodyALinear, preCollisionVelocities.bodyAAngular);
	}
	// and second wall (reversed)
	if( WallWrapper::isValidID(wallID2) )
	{
		hkVector4 negImpulse( appliedImpulse );
		negImpulse.mul4( -1.0f );
		addCollisionEvent(wallID2, negImpulse, contactPoint, preCollisionVelocities.bodyBLinear, preCollisionVelocities.bodyBAngular);
	}
}


void WallSimulation::addCollisionEvent(int wallID, const hkVector4& impulse, const hkVector4& contactPoint, const hkVector4& linearVelocity, const hkVector4& angularVelocity)
{
	// create collision event data
	WallCollisionEventData* data = new WallCollisionEventData(impulse, contactPoint);
	if(m_wallsCollisions.hasKey(wallID))
	{
		// there's already an entry for this wall, add the collision event
		hkPointerMap< int, WallCollisions* >::Iterator collisionIter = m_wallsCollisions.findKey(wallID);
		WallCollisions* collisions = m_wallsCollisions.getValue(collisionIter);
		collisions->eventsData.pushBack( data );
	} else
	{
		// this wall has no entries, create a new entry and add the collision event
		WallCollisions* collEvent = new WallCollisions(wallID, data, linearVelocity, angularVelocity);
		m_wallsCollisions.insert( wallID, collEvent );
	}
}

void WallSimulation::clearCollisionEventData()
{
	hkPointerMap< int, WallCollisions* >::Iterator iter = m_wallsCollisions.getIterator();
	
	HK_TIMER_BEGIN("ClearCollisionEvents", HK_NULL);
	
	while(m_wallsCollisions.isValid(iter)) 
	{
		HK_TIMER_BEGIN_LIST("ResetPositionsAndVelocities", "Retrieve Wall");

		WallCollisions* wallcollisions = m_wallsCollisions.getValue(iter);
		
		HK_TIMER_SPLIT_LIST("Reset Velocities and positions");

		getWallFromParallel( wallcollisions->wallID )->resetBricksPositionOrientationAndVelocities();

		HK_TIMER_SPLIT_LIST("SaveImpactVelocities");

		setWallLastImpactVelocities(wallcollisions->wallID, wallcollisions->preCollisionLinearVelocity, wallcollisions->preCollisionAngularVelocity);

		HK_TIMER_SPLIT_LIST("Cleanup");

		wallcollisions->clear();

		delete wallcollisions;
		
		HK_TIMER_END_LIST();
		
		iter = m_wallsCollisions.getNext(iter);
	}
	m_wallsCollisions.clear();
	
	HK_TIMER_END();
}

void WallSimulation::applyImpulses()
{
	m_mainWorld->lock();

	hkVector4 contactPoint;
	hkVector4 impulseTransformed; 

	// for each wall with associated collision events
	hkPointerMap< int, WallCollisions* >::Iterator iter = m_wallsCollisions.getIterator();
	while( m_wallsCollisions.isValid(iter) )
	{
		// retrieve collisions
		WallCollisions* collisions = m_wallsCollisions.getValue( iter );

		WallWrapper* wallwrapper = getWallFromParallel( collisions->wallID );
		
		// for each collision event of this wall
		for(int i = 0; i< collisions->eventsData.getSize(); ++i)
		{
			// transform the point to object space
			transformPointFromWorldToWallLocal(*(getWallFromMain(collisions->wallID)), collisions->eventsData[i]->contactPointPosition, contactPoint);
			// transform the impulse from world to object space
			impulseTransformed.setRotatedInverseDir( getWallFromMain(collisions->wallID)->getRotation(), collisions->eventsData[i]->impulseApplied);
			impulseTransformed.zeroElement(3);
			// find the brick to which the impulse must be applied
			
			if(wallwrapper->m_fracturable && m_wallsToBeChecked.indexOf(wallwrapper->getID())==-1)
				m_wallsToBeChecked.pushBack(wallwrapper->getID());
			if(wallwrapper->applyPointImpulseToBrick( impulseTransformed, contactPoint ))
			{
				debugImpulses.pushBack( contactPoint );
				debugImpulses.pushBack( impulseTransformed );
			}
		}
		// get next wall collisions list 
		iter = m_wallsCollisions.getNext( iter );
	}

	m_mainWorld->unlock();

}

void WallSimulation::updateWalls()
{
	m_parallelWorld->lock();

	// check if there are pending collision events 
	if(m_wallsCollisions.getSize()!=0)
	{
		HK_TIMER_BEGIN("ApplyImpulses and Simulate", HK_NULL);
		
		// if so..

		// ...apply collision impulses
		applyImpulses();
	
		// ...and perform simulation
		simulate();	

		// finally clean processed data
		clearCollisionEventData();

		HK_TIMER_END();
	}
	else
	{
		HK_TIMER_BEGIN("Just Simulate", HK_NULL);
		// otherwise just perform simulation
		simulate();
		HK_TIMER_END();
	}

	m_parallelWorld->unlock();

	HK_TIMER_BEGIN("Fracture Walls", HK_NULL);
	// check if any constraint breaking was notified
	if(!m_wallsToBeChecked.isEmpty())
	{	
		HK_TIMER_BEGIN("PreFracture", HK_NULL);
		
		// LOCK WORLD
		m_mainWorld->lock();
		
		// some constraints were broken
		// here we'll keep all the created and removed walls
		hkArray<hkpRigidBody*> addedWalls;
		hkArray<hkpRigidBody*> removedWalls;
		bool found = false;
		hkArray<hkpConstraintInstance*> addedConstraints(0);
		HK_TIMER_END();
		// check if each of the potentially broken walls is actually broken and collect pieces
		HK_TIMER_BEGIN("ComputeFracture", HK_NULL);

		for(int i=0; i<m_wallsToBeChecked.getSize(); ++i)
		{
			// if the wall has broken pieces add it to the list of the walls to be removed
			hkpRigidBody* removed = HK_NULL;
			
			if( collectWallBrokenPieces(m_wallsToBeChecked[i], addedWalls, &(removed), addedConstraints ) )
			{
				removedWalls.pushBack(removed);
				found = true;
			}
		}

		HK_TIMER_END();
		HK_TIMER_BEGIN("PostFracture", HK_NULL);
		if(found)
		{
			// remove all the old walls
			if(!removedWalls.isEmpty())
			{
				m_mainWorld->removeEntityBatch( reinterpret_cast<hkpEntity * const *>(removedWalls.begin()), removedWalls.getSize() );
				removedWalls.clearAndDeallocate();
			}

			if(!addedWalls.isEmpty())	
			{
				HK_MONITOR_ADD_VALUE( "NumNewWalls", static_cast<float>(addedWalls.getSize()), HK_MONITOR_TYPE_INT );
				// add the new walls
				m_mainWorld->addEntityBatch( reinterpret_cast<hkpEntity * const *>(addedWalls.begin()), addedWalls.getSize()/*, HK_ENTITY_ACTIVATION_DO_NOT_ACTIVATE*/ );
				for(int i=0; i< addedWalls.getSize(); ++i)
				{
					addedWalls[i]->removeReference();
				}
				HK_MONITOR_ADD_VALUE( "NumNewConstraints", static_cast<float>(addedConstraints.getSize()), HK_MONITOR_TYPE_INT );
				// add ground constraints
				for(int i=0; i<addedConstraints.getSize(); ++i)
				{
					m_mainWorld->addConstraint( addedConstraints[i] );
					addedConstraints[i]->removeReference();
				}
			}
		}
		// clear ids from the this simulation step
		m_wallsToBeChecked.clear();

		// UNLOCK WORLD
		m_mainWorld->unlock();
		
		HK_TIMER_END(); // post fracture

		HK_TIMER_END(); // fracture walls
	} // walls2bechecked != empty
}

// Collects the walls that broke down during the last simulation
hkBool WallSimulation::collectWallBrokenPieces(int wallID, hkArray<hkpRigidBody*>& wallsAddedOut, hkpRigidBody** wallRemovedOut, hkArray<hkpConstraintInstance*>& wgConstraintsOut)
{
	hkBool res = false;

	m_parallelWorld->lock();

	HK_ASSERT2(0x217b7fb5, m_wallsInSimulationWrappers.hasKey(wallID), "The wall is not in the simulation");
	
	// TMP CODE FOR FRACTURE OF A SINGLE BRICK ----------------------------------------------------------------------------- |||
	WallWrapper* wall = getWallFromParallel(wallID);
	if(!wall->m_fracturedBodies.isEmpty())
	{
		if(!wall->m_fracturedBodies.isEmpty())
		{
			wallsAddedOut.insertAt(wallsAddedOut.getSize(), wall->m_fracturedBodies.begin(), wall->m_fracturedBodies.getSize());
			*wallRemovedOut = getWallFromMain(wallID);
			
			hkVector4 oldCenter;
			{
				/*hkAabb bbox;
				(*wallRemovedOut)->getCollidable()->getShape()->getAabb(hkTransform::getIdentity(), .0f, bbox);
				oldCenter.setSub4(bbox.m_max, bbox.m_min);
				oldCenter.mul4(0.5f);*/

				oldCenter = (*wallRemovedOut)->getCenterOfMassLocal();
			}


			for(int i=0; i<wall->m_fracturedBodies.getSize(); ++i)
			{
				hkpRigidBody* wb = wall->m_fracturedBodies[i];
				//hkAabb nBbox;
				//wb->getCollidable()->getShape()->getAabb(hkTransform::getIdentity(), .0f, nBbox);
				hkVector4 newBodyCenter;
				//newBodyCenter.setSub4(nBbox.m_max, nBbox.m_min);
				//newBodyCenter.mul4(0.5f);
				newBodyCenter = wb->getCenterOfMassLocal();
				hkVector4 pos;
				pos.setSub4(newBodyCenter, oldCenter);
				// add a little separating velocity
				wb->setLinearVelocity(pos);
				pos.setRotatedDir((*wallRemovedOut)->getRotation(), pos);
				pos.add4( (*wallRemovedOut)->getPosition() );
				wb->setPosition(pos);
				wb->setRotation((*wallRemovedOut)->getRotation());
			}
			
			if( !wall->m_fracturePlanes.isEmpty() )
			{
				m_debugFracturePlanes.insertAt(m_debugFracturePlanes.getSize(), &(wall->m_fracturePlanes[0]), wall->m_fracturePlanes.getSize() );
				m_debugcPoints.pushBack(wall->m_contactPoint);
				m_debugImpulses.pushBack(wall->m_impulse);
			}
			removeWrapperAndBricks(wallID);
			m_parallelWorld->unlock();
			return true;
		}
	}
	// END OF TMP CODE FOR FRACTURE OF A SINGLE BRICK ----------------------------------------------------------------------------- |||


	int wallsAddedOffset = wallsAddedOut.getSize();
	hkArray<WallWrapper*> brokenPiecesWrappers;
	res = getWallFromParallel(wallID)->fractureWall( brokenPiecesWrappers, wallsAddedOut, wgConstraintsOut, *(getWallFromMain(wallID)) );
	if(res)
	{	
		// keep the wall to be removed
		(*wallRemovedOut) = getWallFromMain(wallID);
		m_wallsInSimulationBodies.remove(wallID);

		// set new bodies velocities velocities
		BodyVelocities* vel = m_wallsLastImpactVelocities.getWithDefault(wallID, HK_NULL);
		HK_ASSERT2(0x412ec3c4, vel!=HK_NULL, "there are no stored velocities for this wall");

		computeVelocitiesForNewBodies( brokenPiecesWrappers, vel->linear, vel->angular, wallsAddedOut, wallsAddedOffset, **wallRemovedOut);

		getAllDebugEdges(debugEdges);

		removeWrapper(wallID);

		// add each newly created wall to the lists
		for(int i=0; i<brokenPiecesWrappers.getSize(); ++i) // the first wall has just replaced the original one
		{
			if( !brokenPiecesWrappers[i]->isOneBrick() )
			{
				storeWrapperAndBody( brokenPiecesWrappers[i], wallsAddedOut[i + wallsAddedOffset] );
				if(m_collisionListener)
				{
					wallsAddedOut[i + wallsAddedOffset ]->addCollisionListener( m_collisionListener );
				}
			}
			else
			{
				m_parallelWorld->removeEntity( static_cast<hkpEntity*>(*(brokenPiecesWrappers[i]->getBricks())) );
				delete brokenPiecesWrappers[i];
			}
		}
	}

	m_parallelWorld->unlock();

	return res;
}

void WallSimulation::computeVelocitiesForNewBodies( const hkArray<WallWrapper*>& wrappers, const hkVector4& oldLinearVelocity, const hkVector4& oldAngularVelocity, hkArray<hkpRigidBody*>& newBodies, int offset, const hkpRigidBody& oldBody )
{
	hkVector4 zeroVec( hkVector4::getZero() );

	hkVector4 totalLinearMomentum( zeroVec );
	hkVector4 totalAngularMomentum( zeroVec );

	for(int i=0; i<wrappers.getSize(); ++i)
	{
		// reset
		hkVector4 newBodylinear( zeroVec );
		hkVector4 newBodyAngular( zeroVec ); 
		hkVector4 newBodyLinearMomentum( zeroVec );
		hkVector4 newBodyAngularMomentum( zeroVec );

		// for each brick
		if(!wrappers[i]->isOneBrick())
		{
			// get a sample of the bricks
			hkArray<int> sampleBricks;
			findSample( wrappers[i], sampleBricks );

			hkVector4 relBrickPosition( zeroVec );
			hkVector4 angularContribution( zeroVec );
			for(int k=0; k<sampleBricks.getSize(); ++k)
			{
				//add linear contribution
				newBodylinear.add4( wrappers[i]->getBrick(k).getLinearVelocity() );	
				//compute brick position relative to the center of mass..
				relBrickPosition.setSub4( wrappers[i]->getBrick(k).getPosition(), wrappers[i]->getCenterOfMass() );
				//and compute angular contribution
				angularContribution.setCross( relBrickPosition, wrappers[i]->getBrick(k).getLinearVelocity() );
				//angularContribution.mul4( wrappers[i]->GetBrick(k).getPosition().lengthInverse3() );
				//add the angular contribution
				newBodyAngular.add4( angularContribution );
			}  
			// average the velocities to compute the linear velocity for this body
			newBodylinear.mul4( static_cast<hkReal>( 1.0f / sampleBricks.getSize() ) );
			//newBodyAngular.mul4( static_cast<hkReal>( 1.0f / sampleBricks.getSize() ) );
		} 
		else
		{
			// just one brick, get the velocities
			newBodylinear = wrappers[i]->getBrick(0).getLinearVelocity();	
			newBodyAngular = wrappers[i]->getBrick(0).getAngularVelocity();	
		}
	
		// add the original body's velocities
		newBodylinear.add4( oldLinearVelocity );
		//newBodyAngular.add4( oldAngularVelocity ); // WARNING : this shouldn't be here
		
		// compute momentum for this body
		newBodyLinearMomentum.setMul4( newBodies[i+offset]->getMass(), newBodylinear );  // Pi = Mi*Vi
		// add it to the global momentum
		totalLinearMomentum.add4( newBodyLinearMomentum );  // P += Pi

		// compute angular momentum for this body
		//newBodyAngularMomentum.setMul4( newBodies[i+offset]->getMass(), newBodyAngular );
		hkVector4 bodyRelPos;
		bodyRelPos.setSub4( newBodies[i+offset]->getCenterOfMassLocal(), oldBody.getCenterOfMassLocal() ); 
		newBodyAngularMomentum.setCross( bodyRelPos, newBodyLinearMomentum );  // Li = Ri x Pi
		// and add it to the global angular momentum
		totalAngularMomentum.add4( newBodyAngularMomentum ); // L += Li

		newBodies[i+offset]->setLinearVelocity( newBodylinear );
		newBodies[i+offset]->setAngularVelocity( newBodyAngular );
	}
	
	// add the angular momentum component of the old velocity
	totalAngularMomentum.addMul4( oldBody.getMass(), oldAngularVelocity );

	// to prevent small blocks coming from a big wall from having too big velocities we
	// compute all the velocities from the global linear and angular momentum 
	hkVector4 totalLinear;
	{
		hkReal invTotMass = oldBody.getMassInv();
		totalLinear.setMul4( invTotMass, totalLinearMomentum ); //  V = P / M
	}	
	hkVector4 totalAngular;
	{
		// get inverse of inertia tensor of the original body
		hkMatrix3 invInertiaT;
		oldBody.getInertiaInvLocal( invInertiaT );
		// and compute the angular velocity
		totalAngular.setMul3(invInertiaT, totalAngularMomentum); // W = invInertiaTensor * L 
	}

	// now compute and set the final velocities for each body
	for(int i=offset; i<newBodies.getSize(); ++i)
	{
		hkReal ratio = /*1.0f - newBodies[i]->getMass()*oldBody.getMassInv();*/ 0.8f;
		// preserve a part of the originally computed velocity to achieve a "separating" velocity
		hkVector4 finalLinear;
		finalLinear.setInterpolate4( newBodies[i]->getLinearVelocity(), totalLinear, ratio ); // Vi = Vi*(1-r)+V*r
		// and set it to the body
		newBodies[i]->setLinearVelocity( finalLinear );

		// preserve part of the originally computed angular velocity
		hkVector4 finalAngular;
		finalAngular.setInterpolate4( newBodies[i]->getAngularVelocity(), totalAngular, ratio ); // Wi = Wi*(1-r)+W*r
		// and set the final angular
		newBodies[i]->setAngularVelocity( finalAngular );
	}   
}


void WallSimulation::getAllBricksPositions(hkArray<hkVector4>& positions) const
{
	hkPointerMap<int, WallWrapper*>::Iterator iter = m_wallsInSimulationWrappers.getIterator();
	while(m_wallsInSimulationWrappers.isValid(iter)) 
	{
		m_wallsInSimulationWrappers.getValue(iter)->getAllBricksPositions( positions );
		iter = m_wallsInSimulationWrappers.getNext(iter);
	}
}


void WallSimulation::getAllDebugEdges(hkArray<hkVector4>& edges) const
{
	edges.clear();

	hkPointerMap<int, WallWrapper*>::Iterator iter = m_wallsInSimulationWrappers.getIterator();
	while(m_wallsInSimulationWrappers.isValid(iter)) 
	{
		if( !m_wallsInSimulationWrappers.getValue(iter)->debugEdges.isEmpty() )
		{
			edges.insertAt(0, &(m_wallsInSimulationWrappers.getValue(iter)->debugEdges[0]), m_wallsInSimulationWrappers.getValue(iter)->debugEdges.getSize() ); 
		}
		iter = m_wallsInSimulationWrappers.getNext(iter);
	}
}

void WallSimulation::getAllDebugfracturePlanes(hkArray<hkVector4>& planes) const
{
	planes.clear();
	planes = m_debugFracturePlanes;
}

void WallSimulation::getAllDebugImpulsesAndContactPoints(hkArray<hkVector4>& impulsesArray, hkArray<hkVector4>& pointsArray) const
{
	impulsesArray.clear();
	pointsArray.clear();
	impulsesArray = m_debugImpulses;
	pointsArray = m_debugcPoints;
}


/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/

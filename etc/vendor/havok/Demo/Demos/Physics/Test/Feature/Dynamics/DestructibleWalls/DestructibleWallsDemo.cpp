/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickConstraintListener.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureCollisionListener.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallWrapper.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockWrapper.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/DestructibleWallsDemo.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureUtility.h>


DEMO_OPTIONS_DEFINE(DestructibleWallsDemo,Options);

struct BreakableWallVariant
{
	const char*	 m_name;
	bool singleBrick;
	const char* m_details;

};

static const char helpString[] = \
"This demo shows how to simulate stiff, destructible walls. The displayed walls are simplified representations" \
" of the real brick walls, which are kept in a Parallel Simulation." \
" When the brick wall constraints break, the representation is updated to display the broken walls. ";

static const char helpStringTmp[] = "Just one brick to test fracture";

static const BreakableWallVariant g_variants[] =    
{
	{ "Destructible Walls", false, helpString },
	{ "Single Brick Tmp", true, helpStringTmp }
};


DestructibleWallsDemo::DestructibleWallsDemo(hkDemoEnvironment* env)
:hkDefaultPhysicsDemo(env),
m_fractureUtility(HK_NULL),
m_constraintListener(HK_NULL),
m_shootingDirX(.0f),
m_shootingDirY(.0f),
m_gunCounter(0),
m_centerOfScene( hkVector4::getZero() ),
m_brickHalfExtents( 0.5f, 0.5f, 0.5f )
{
	// General setup
	m_frameToSimulationFrequencyRatio = 1.0f;
	m_timestep = 1.0f / 60.0f;

	// read demo options
	//const BreakableWallVariant& variant =  g_variants[m_variantId];

	m_collisionDetectionType = (m_options.m_useParallelSimulation)? PARALLEL : SIMPLE;  //variant.m_Type;

	// wall shape options 
	int wallWidth  = m_options.m_WallsWidth;  
	int wallHeight = m_options.m_WallsHeight; 

	// setup camera
	{
		hkReal tot = static_cast<hkReal>( m_options.m_WallsHeight + m_options.m_WallsWidth );
		if(tot < 5)
			tot = 5;
		//hkVector4 from(-25.0f, 30.0f, 30.0f);
		hkVector4 from(-tot*1.0f, tot*1.25f, tot*1.25f);
		hkVector4 to( -5.0f, 15.0f, 15.0f);
		//hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up( 0.0f,  1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	// Main world initialization
	// define world info..

	hkpWorldCinfo worldInfo;
	worldInfo.setBroadPhaseWorldSize(500.0f);
	worldInfo.setupSolverInfo(hkpWorldCinfo/*::SOLVER_TYPE_8ITERS_HARD*/::SOLVER_TYPE_4ITERS_HARD);// using 8 iterations is much stiffer and realistic..	
	worldInfo.m_collisionTolerance = 0.01f;
	worldInfo.m_gravity = hkVector4( 0.0f,-9.8f,0.0f);
	worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
	worldInfo.m_enableDeactivation = false;
	// .. create the world..
	m_world = new hkpWorld(worldInfo);

	// ..and LOCK it
	m_world->lock();

	// register agents and setup graphics
	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	// to enable wireframe display remove comment from next line
	if(m_options.m_wireFrameMode)
		m_env->m_window->getViewport(0)->toggleState(HKG_ENABLED_WIREFRAME);  
	setupGraphics();

	// Create and add the ground
	hkpRigidBody* ground = createGround();
	m_world->addEntity( ground , HK_ENTITY_ACTIVATION_DO_NOT_ACTIVATE);
	ground->removeReference();

	if(g_variants[m_variantId].singleBrick) // TMP SINGLE BRICK FRACTURE VARIANT
	{
		// geometry of a wall brick
		hkpBoxShape* brickGeom = new hkpBoxShape(m_brickHalfExtents, 0);
		// wall builder settings
		BrickwallBuilderDescriptor bwDescriptor;
		bwDescriptor.m_brickShape = brickGeom;
		bwDescriptor.m_width = 1;
		bwDescriptor.m_height = 1;
		bwDescriptor.m_brickMass = 10.0f;
		// Set positions and orientations for the walls
		m_centerOfScene(1) += 10.0f; 
		bwDescriptor.m_transform.set(hkQuaternion::getIdentity(), m_centerOfScene);
		//bwDescriptor.m_position = m_centerOfScene;
		//bwDescriptor.m_orientation.setIdentity();
		
		// create the fracture utility
		m_fractureUtility = new WallFractureUtility( m_world );
		// ...and add the wall
		WallWrapper* addedWall = m_fractureUtility->addWallToSimulation(bwDescriptor);
		// make this wall fracturable
		addedWall->toggleFracturableState(true);

		brickGeom->removeReference();
	} else
	 // WORLD AND WALLS SETUP
	{
		// common to both parallel and simple
		// geometry of a wall brick
		hkpBoxShape* brickGeom = new hkpBoxShape(m_brickHalfExtents, 0);
		// wall builder settings
		BrickwallBuilderDescriptor bwDescriptor;
		bwDescriptor.m_brickShape = brickGeom;
		bwDescriptor.m_width = wallWidth;
		bwDescriptor.m_height = wallHeight;
		bwDescriptor.m_brickMass = 10.0f;
		bwDescriptor.m_strength = m_options.m_bottomConstraintStrength;
		bwDescriptor.m_lowerThreshold = m_options.m_topConstraintStrength;
		bwDescriptor.m_attachToGround = m_options.m_attachToGround;
		bwDescriptor.m_theGround = m_world->getFixedRigidBody();
		// Set positions and orientations for the walls
		//hkArray< hkVector4 > wallsPositions(3);
		//hkArray< hkQuaternion > wallsOrientations(3); 
		hkArray< hkTransform > wallTransforms(3);
		{
			posOnGround(m_world, m_centerOfScene, m_centerOfScene);
			// wall 1
			m_centerOfScene(2) += wallWidth*m_brickHalfExtents(0); 
			wallTransforms[0].set(hkQuaternion::getIdentity(), m_centerOfScene);
			//wallsPositions[0] = m_centerOfScene;
			//wallsOrientations[0].setIdentity();
			// wall 2
			m_centerOfScene(2) -= wallWidth*m_brickHalfExtents(0)*2.0f; 
			wallTransforms[1].set(hkQuaternion::getIdentity(), m_centerOfScene);
			//wallsPositions[1] = m_centerOfScene;
			//wallsOrientations[1].setIdentity();
			// wall 3
			m_centerOfScene(2) += wallWidth*m_brickHalfExtents(0);
			m_centerOfScene(0) += wallWidth*m_brickHalfExtents(0);
			m_centerOfScene(0) += m_brickHalfExtents(0);
			//wallsPositions[2] = m_centerOfScene;
			hkVector4 axis(0,1,0); 
			hkQuaternion rot( axis, HK_REAL_PI/2.0f );
			//wallsOrientations[2].setAxisAngle( axis, HK_REAL_PI/2.0f );
			wallTransforms[2].set(rot, m_centerOfScene);
		}
		//
		
		// Differentiate simple and parallel simulation versions
		switch(m_collisionDetectionType)
		{
		case SIMPLE :
			{
				// Set up a collision filter so we can disable collisions between the bricks we constrain together
				hkpCollisionFilter* brickFilter = new BrickFilter();
				m_world->setCollisionFilter( brickFilter );
				brickFilter->removeReference();

				// constraint listener to enable collision detection between bricks whose constraints are broken
				m_constraintListener = new BrickConstraintListenerEntities();
				m_world->addConstraintListener( m_constraintListener );

				for(int i=0; i<3; ++i)
				{
					// build walls
					bwDescriptor.m_transform = wallTransforms[i];
					//bwDescriptor.m_position = wallsPositions[i];
					//bwDescriptor.m_orientation = wallsOrientations[i];
					bwDescriptor.m_wallID = i+1;
					createAddBrickWall(m_world, bwDescriptor);
				}
			}	
			break;
		case PARALLEL :
			{
				// create the fracture manager utility
				m_fractureUtility  = new WallFractureUtility( m_world );

				for(int i=0; i< 3; ++i)
				{
					// set positions and ask the utility to create and add the walls
					bwDescriptor.m_transform = wallTransforms[i];
					/*bwDescriptor.m_position = wallsPositions[i];
					bwDescriptor.m_orientation = wallsOrientations[i];*/
					m_fractureUtility->addWallToSimulation( bwDescriptor );
				}
			}
		} //  end of switch

		brickGeom->removeReference();
	}

	// UNLOCK the main world
	m_world->unlock();
}



hkpRigidBody* DestructibleWallsDemo::createGround() 
{
	// build ground box
	hkVector4 extents(50.f, 2.0f, 50.0f);
	hkpConvexShape* groundBoxShape= new hkpBoxShape(extents);
	hkpRigidBodyCinfo groundInfo;
	groundInfo.m_shape = groundBoxShape;
	groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
	groundInfo.m_position = hkVector4(0.0f, -2.0f, 0.0f);
	groundInfo.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

	hkpRigidBody* ground = new hkpRigidBody(groundInfo);
	groundBoxShape->removeReference();
	return ground;
}

WallWrapper* DestructibleWallsDemo::createWallWrapper( const BrickwallBuilderDescriptor& bwDescriptor )
{
	hkArray<hkpRigidBody*> bricks;
	hkArray<hkpConstraintInstance*> constraints;

	// create the bricks of the wall
	BrickWallBuilder::buildBrickWall( bwDescriptor, bricks, constraints );

	// create the wall
	hkVector4 bExt;
	bExt.setMul4(2.0f, bwDescriptor.m_brickExtents);
	WallWrapper* ww = new WallWrapper( bricks, constraints, bExt );

	return ww;
}

void DestructibleWallsDemo::createAddBrickWall( hkpWorld* world, const BrickwallBuilderDescriptor& wbDescriptor )
{
	hkArray<hkpRigidBody*> bricks;
	hkArray<hkpConstraintInstance*> constraints;

	BrickWallBuilder::buildBrickWall(wbDescriptor, bricks, constraints);

	// Add all entities... 
	world->addEntityBatch(reinterpret_cast<hkpEntity*const*>(bricks.begin()), bricks.getSize(), HK_ENTITY_ACTIVATION_DO_NOT_ACTIVATE);

	// ...and constraints
	for(int i=0; i<constraints.getSize(); i++)
	{
		world->addConstraint( constraints[i]);
		constraints[i]->removeReference();
	}

	// cleanup
	for(int i=0; i<bricks.getSize(); i++)
	{
		bricks[i]->removeReference();
	}
}

hkpRigidBody* DestructibleWallsDemo::createBall(const hkVector4& position, const hkVector4& velocity) 
{
	const hkReal radius = 1.0f;
	const hkReal mass = 100.0f;

	hkpMassProperties result;
	hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, mass, result);

	hkpSphereShape* sphereShape = new hkpSphereShape(radius); 
	hkVector4 spherePos(-20.0f, 0.0f + radius, 200.0f);

	hkpRigidBodyCinfo sphereInfo;
	sphereInfo.m_mass = result.m_mass;
	sphereInfo.m_centerOfMass = result.m_centerOfMass;
	sphereInfo.m_inertiaTensor = result.m_inertiaTensor;
	sphereInfo.m_shape = sphereShape;
	sphereInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	sphereInfo.m_position.setAdd4(position, spherePos);
	sphereInfo.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;

	hkpRigidBody* sphereBody = new hkpRigidBody(sphereInfo);

	sphereBody->setLinearVelocity(velocity);

	sphereShape->removeReference();

	return sphereBody;
}

DestructibleWallsDemo::~DestructibleWallsDemo()
{
	if(m_constraintListener && m_collisionDetectionType==SIMPLE)
	{
		m_world->lock();
		m_world->removeConstraintListener( m_constraintListener );
		m_world->unlock();

		m_constraintListener->removeReference();
	}

	if(m_fractureUtility)
		delete m_fractureUtility;

	m_world->markForWrite();
	delete m_world;
	m_world = HK_NULL;
}

void DestructibleWallsDemo::posOnGround(hkpWorld* world, const hkVector4& pos, hkVector4& newPos) const 
{
	// cast a ray to find exact position
	hkpWorldRayCastInput ray;
	ray.m_from = pos;
	ray.m_to = pos;
	ray.m_from(1) += 20.0f;
	ray.m_to(1) -= 20.0f;
	hkpWorldRayCastOutput rayOutput;
	world->castRay(ray,rayOutput);

	newPos.setInterpolate4(ray.m_from, ray.m_to, rayOutput.m_hitFraction);
}


hkDemo::Result DestructibleWallsDemo::stepDemo()
{
	m_world->lock();

	{
		const hkgPad* pad = m_env->m_gamePad;

		// Cannon control + drawing
		// check to see if the user has pressed one of the control keys:
		int x = ((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0)? -1:0;
		x = ((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)? 1:x;
		int y = ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)? -1:0;
		y = ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)?    1:y;

		m_shootingDirX += x * 0.015f;
		m_shootingDirY += y * 0.015f;

		hkVector4 canonStart( m_centerOfScene );
		canonStart(2) += m_options.m_WallsWidth*2.0f;

		hkVector4 canonDir( m_shootingDirX, m_shootingDirY, -1.0f );
		canonDir.normalize3();

		// display the canon direction 
		{
			hkpWorldRayCastInput in;
			in.m_from = canonStart;
			in.m_to.setAddMul4( in.m_from, canonDir, 100.0f );
			in.m_filterInfo = 0;
			hkpWorldRayCastOutput out;
			m_world->castRay( in , out );
			hkVector4 hit; hit.setInterpolate4( in.m_from, in.m_to, out.m_hitFraction );
			HK_DISPLAY_LINE( in.m_from, hit, 0x600000ff );
			if ( out.hasHit() )
			{
				HK_DISPLAY_ARROW( hit, out.m_normal, 0x60ff00ff );
			}
			HK_DISPLAY_ARROW( canonStart, canonDir, hkColor::CYAN );
		}

		// Shooting bullets
		{
			hkBool shooting = false;

			if ( pad->wasButtonPressed(HKG_PAD_BUTTON_1)!= 0 )
			{
				shooting = true;
			}

			if (  pad->isButtonPressed(HKG_PAD_BUTTON_2)!= 0)
			{
				if ( m_gunCounter-- < 0 )
				{
					shooting = true;
					m_gunCounter = 5;
				}
			}  

			if ( shooting )
			{
				hkpMassProperties result;
				hkpInertiaTensorComputer::computeSphereVolumeMassProperties(m_options.m_cannonBallRadius, m_options.m_cannonBallMass, result);

				hkpSphereShape* sphereShape = new hkpSphereShape(m_options.m_cannonBallRadius); 
				hkVector4 spherePos(-20.0f, 0.0f + m_options.m_cannonBallRadius, 200.0f);

				hkpRigidBodyCinfo sphereInfo;
				sphereInfo.m_mass = result.m_mass;
				sphereInfo.m_centerOfMass = result.m_centerOfMass;
				sphereInfo.m_inertiaTensor = result.m_inertiaTensor;
				sphereInfo.m_shape = sphereShape;
				sphereInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
				sphereInfo.m_position = canonStart;
				sphereInfo.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;
				sphereInfo.m_linearVelocity.setMul4( 100.0f, canonDir );

				hkpRigidBody* bullet = new hkpRigidBody( sphereInfo );
				sphereInfo.m_shape->removeReference();

				m_world->addEntity( bullet );
				bullet->removeReference();
			}
		}
	}

	// release the world
	m_world->unlock();

	// step demo
	hkDemo::Result res = hkDefaultPhysicsDemo::stepDemo();


	//HK_TIMER_BEGIN_LIST( "Update Walls", "Update Walls"/*HK_NULL*/);
	HK_TIMER_BEGIN( "Update Walls", "update walls"/*HK_NULL*/);
	if(m_collisionDetectionType == PARALLEL)
	{
		HK_ASSERT2(0x7274e9ec, m_fractureUtility!=HK_NULL ,"The parallel simulation wasn't set!!");
		// and update walls
		m_fractureUtility->Update();
	}
	HK_TIMER_END();

	HK_TIMER_BEGIN("DebugDisplay", HK_NULL);
	// DEBUG DISPLAY
	if(m_fractureUtility->getSimulation() && m_options.m_showDebugDisplay)
	{
		// applied impulses
		for(int i=0; i<m_fractureUtility->getSimulation()->debugImpulses.getSize(); i+=2)
		{
			hkVector4 start( m_fractureUtility->getSimulation()->debugImpulses[i] );
			hkVector4 end( m_fractureUtility->getSimulation()->debugImpulses[i+1] );
			end.mul4( 0.01f );
			HK_DISPLAY_ARROW(start, end , 0x00000000);			
		}
		
		if(m_fractureUtility->getSimulation()->debugImpulses.getSize() > 100 )
			m_fractureUtility->getSimulation()->debugImpulses.clear();

		// bricks positions in parallel simulation
		hkArray<hkVector4> positions;
		m_fractureUtility->getSimulation()->getAllBricksPositions( positions );
		for(int i=0; i<positions.getSize(); ++i)
		{
			drawBrick(positions[i] , m_brickHalfExtents);
		}
		positions.clear();

		// edges of union find
		for(int i=0; i<m_fractureUtility->getSimulation()->debugEdges.getSize(); i+=2)
		{
			HK_DISPLAY_LINE(m_fractureUtility->getSimulation()->debugEdges[i], m_fractureUtility->getSimulation()->debugEdges[i+1] , 0x00000000);
		}
	}
	HK_TIMER_END();

	if(m_collisionDetectionType == PARALLEL)
	{
		hkArray< hkVector4 > planes;
		m_fractureUtility->getSimulation()->getAllDebugfracturePlanes(planes);
		for(int i=0; i< planes.getSize()-2; ++i)
		{
			if(planes[i].length3()!=0)
			{
				hkVector4 offset(.0f, .5f, .0f/*.5f, .5f, .5f*/);
				HK_DISPLAY_PLANE(planes[i], offset, 0.5f, 0xffffffff);
			}
		}
		if(!planes.isEmpty() && planes[planes.getSize()-1].length3()!=0)
		{
			hkVector4 offset(.0f, .5f, .0f/*.5f, .5f, .5f*/);
			HK_DISPLAY_PLANE(planes[planes.getSize()-2], offset, 0.5f, 0x00000000);
			HK_DISPLAY_PLANE(planes[planes.getSize()-1], offset, 0.5f, 0xffff0000);
		}
		hkArray<hkVector4> cpts;
		hkArray<hkVector4> impulses;
		m_fractureUtility->getSimulation()->getAllDebugImpulsesAndContactPoints(impulses, cpts);
		for(int i=0; i< cpts.getSize(); ++i)
		{
			HK_DISPLAY_ARROW(cpts[i], impulses[i] , 0x00000000);	
		}
	}
	return res;
}

void DestructibleWallsDemo::drawBrick( const hkVector4& position, const hkVector4& halfsize) const
{
	hkVector4 posA=position;
	hkVector4 posC=position;
	posA.sub4( halfsize );
	posC.add4( halfsize );
	hkVector4 posB=posA;
	posB(0) = posC(0);
	hkVector4 posD=posA;
	posD(1) = posC(1);
	posA(2) = posB(2) = posC(2)= posD(2) = .0f;
	HK_DISPLAY_LINE(posA, posB , 0xffffffff);
	HK_DISPLAY_LINE(posB, posC , 0xffffffff);
	HK_DISPLAY_LINE(posC, posD , 0xffffffff);
	HK_DISPLAY_LINE(posD, posA , 0xffffffff);
	HK_DISPLAY_LINE(posA, posC , 0xffffffff);
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( DestructibleWallsDemo, HK_DEMO_TYPE_PRIME, BreakableWallVariant, g_variants, helpString ); 


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

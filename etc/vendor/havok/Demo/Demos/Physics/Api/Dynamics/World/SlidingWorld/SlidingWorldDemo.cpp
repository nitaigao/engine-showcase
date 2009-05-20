/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Demo support
#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/World/SlidingWorld/SlidingWorldDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhaseHandlePair.h>
#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

// hkMath support
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>

// hkCollide support
#include <Physics/Dynamics/Collide/hkpDynamicsContactMgr.h>

// hkDynamics support
#include <Physics/Dynamics/World/hkpSimulationIsland.h>
#include <Physics/Dynamics/Phantom/hkpPhantom.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>
#include <Physics/Dynamics/Phantom/hkpShapePhantom.h>

// hkInternal support, for updating the contact data
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Internal/Collide/Agent3/Machine/Nn/hkpAgentNnTrack.h>

// Debug display support
#include <Common/Visualize/hkDebugDisplay.h>

// For Havok timers
#include <Common/Base/Monitor/hkMonitorStream.h>


// Demo variants
struct SlidingWorldDemoVariant
{
	enum Mode
	{
		RECENTER_BROAD_PHASE_ONLY,
		SHIFT_COORDINATE_SPACE
	};

	Mode m_mode;
	const char* m_name;
	const char* m_details;
};

static const SlidingWorldDemoVariant g_variants[] =
{
	{ SlidingWorldDemoVariant::RECENTER_BROAD_PHASE_ONLY, "Recenter Broadphase Only", "This demo shows how to recenter the broadphase so that the character is always in its \"center\". It can be useful if floating point error is not a concern and you only want to simulate a small subset of your full game world. One natural application of this is world streaming. If floating point precision is an issue, please look at the other version of this demo called \"Shift Coordinate Space.\"\nBodies not yet in simulation are drawn as gray shells.\nNewly added bodies are colored green." },
	{ SlidingWorldDemoVariant::SHIFT_COORDINATE_SPACE, "Shift Coordinate Space", "This demo shows how to shift the entire world. It can be useful if you are building very large worlds and are worried about floating point precision problems. If you are not worried about floating point precision problems please see the other version of this demo called \"Recenter Broadphase only.\"\nBodies not yet in simulation are drawn as gray shells.\nNewly added bodies are colored green." }
};

class SlidingWorldBroadphaseBorder : public hkpBroadPhaseBorder
{
	public:

		SlidingWorldBroadphaseBorder( hkpWorld* world );

		virtual void maxPositionExceededCallback( hkpEntity* body );
};


SlidingWorldBroadphaseBorder::SlidingWorldBroadphaseBorder( hkpWorld* world )
:   hkpBroadPhaseBorder( world )
{
}

//   If a body moves outside the world size (broadphase), remove it, unless it's the environment
void SlidingWorldBroadphaseBorder::maxPositionExceededCallback( hkpEntity* entity )
{
	hkpRigidBody* body = static_cast<hkpRigidBody*>(entity);

	// Remove the bodies from simulation, unless it's the fixed body (assumed to be a large, level-encompassing landscape tile).
	// N.B. We will only get this callback for a body when it is added tot eh world, or moved (integrated, or explicit setPosition)
	// Thus, for a static (fixed) body, we expect to get this callback only once, on body addition, if the body extend outside the
	// broadphase. For a 'landscape' body this is likely to be acceptable
	if (body->getMotionType() != hkpMotion::MOTION_FIXED)
	{
		m_world->removeEntity( entity );
	}

}

/// Sliding world demo. Extensive notes are at the bottom of this file.
/// Caveats:
///   - This demo overcomes resolution problems for objects being too far away from the origin.
///   - The only Havok objects which are not shifted are constraints connected to the world using
///	 the hkpWorld::getFixedRigidBody(). Instead the body returned by hkpWorld::getFixedRigidBody() is moved.
///	 An alternative to this would be to iterate over all constraints and fix up the local space
///	 positions if connected to this single fixed rigid body, however this current solution should
///	 work well up to a world size of 400*400*400 kilometers. If this is not sufficient you need
///	 to extend this demo to iterate over all constraints as well. TODO
///   - Each time you shift your world, you introduce some small error in each object.
///	 To prevent this error accumulating, don't shift your world too often. Rule: One object
///	 should not be shifted more than 10000 times. As you probably create and destroy objects
///	 all the time, this should not be a serious restriction.

SlidingWorldDemo::SlidingWorldDemo(hkDemoEnvironment* env)
:   hkDefaultPhysicsDemo(env), m_time(0.0f), m_ticks(1), m_currentMode(MANUAL_SHIFT), m_delayBetweenAutomaticShifts(90)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xf0ff00a1, false); //'Attempting to remove an entity twice)'


	// Set up the camera.
	{
		hkVector4 from(0.0f, 75.0f, 30.0f);
		hkVector4 to(0.0f, 3.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	m_centers[0].set(10, 0, 0);
	m_centers[1].set(10, 0, 10);
	m_centers[2].set(0, 0, 10);
	m_centers[3].set(-10, 0, 10);
	m_centers[4].set(-10, 0, 0);
	m_centers[5].set(-10, 0, -10);
	m_centers[6].set(0, 0, -10);
	m_centers[7].set(10, 0, -10);

	m_currentCenter.setAll(0);


	// Create the world.
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);

		//info.setBroadPhaseWorldSize( 120.0f );
		info.setBroadPhaseWorldSize( 30.0f );

		m_world = new hkpWorld( info );
		m_world->m_wantDeactivation = true;
		m_world->lock();

		// N.B. We need this border 'for safety': it should play no part in the 'broadphase moving' or the 'coordinate shfting' -
		// the border callbacks are *not* fired as a result of calls to hkBroadphase::shiftBroadPhase() or hkBroadphase::shiftAllObjects().
		// They will be fired as a result of body addition or world stepping, so this will ensure that bodies added or moved 
		// (under integration) get correctly removed from simulation so that the only body not fully contained inside the broadphase
		// is a single fixed body (assumed to be a large, level-encompassing landscape tile.
		SlidingWorldBroadphaseBorder *border = new SlidingWorldBroadphaseBorder( m_world );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		// Setup the rest of the default viewers:
		setupGraphics();

		// Register all collision agents
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}


	// Create the fixed floor box.
	{
		hkVector4 fixedBoxSize(30.0f, .5f , 30.0f );

		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( fixedBoxSize , 0 );
		info.m_motionType = hkpMotion::MOTION_FIXED;

		// Create fixed box.
		hkpRigidBody* box = new hkpRigidBody(info);
		m_world->addEntity(box)->removeReference();

		info.m_shape->removeReference();
	}

	{
		// Create a single shape, and reuse it for all bodies.
		hkVector4 boxSize( .75f, .75f ,.75f );
		hkpShape* boxShape = new hkpBoxShape( boxSize , 0 );

		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = boxShape;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Compute the box inertia tensor.
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, 5.0f, boxInfo);

		boxInfo.m_rigidBodyDeactivatorType = hkpRigidBodyDeactivator::DEACTIVATOR_NEVER;

		for (int i = -20; i <= 20; i += 5)
		{
			for (int j = -20; j <= 20; j += 5)
			{
				boxInfo.m_position.set( (hkReal)i, 2.0f, (hkReal)j );

				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				m_world->addEntity( boxRigidBody);
				m_boxes.pushBack(boxRigidBody);
			}
		}

		boxShape->removeReference();
				
	}

	m_world->unlock();
}

SlidingWorldDemo::~SlidingWorldDemo()
{
	// Re-enable warning									
	hkError::getInstance().setEnabled(0xf0ff00a1, true); 

	m_world->lock();
	for (int i = 0; i < m_boxes.getSize(); i++)
	{
		m_boxes[i]->removeReference();
	}
	m_world->unlock();
}

void SlidingWorldDemo::recalcAabbsAfterBroadphaseRecenter( hkpWorld* world )
{
	HK_TIMER_BEGIN("RecalcAabbs", HK_NULL );

	// Iterate over all simulation islands.
	{
		// Fixed island
		const hkpSimulationIsland* island = world->getFixedIsland();
		for (int b = 0; b < island->getEntities().getSize(); b++ )
		{
			hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
			body->updateCachedAabb();
		}
	}
	{
		// Inactive islands
		const hkArray<hkpSimulationIsland*>& islands = world->getInactiveSimulationIslands();
		for (int i = 0; i < islands.getSize(); i++ )
		{
			hkpSimulationIsland* island = islands[i];
			for (int b = 0; b < island->getEntities().getSize(); b++ )
			{
				hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
				body->updateCachedAabb();
			}
		}
	}
	{
		// Active Islands
		const hkArray<hkpSimulationIsland*>& islands = world->getActiveSimulationIslands();
		for (int i = 0; i < islands.getSize(); i++ )
		{
			hkpSimulationIsland* island = islands[i];
			for (int b = 0; b < island->getEntities().getSize(); b++ )
			{
				hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
				body->updateCachedAabb();
			}
		}
	}

	HK_TIMER_END();
}


void SlidingWorldDemo::shiftAllGameObjectDataSilently( hkpWorld* world, const hkVector4& effectiveShiftDistance )
{
	HK_TIMER_BEGIN("ShiftObjects", HK_NULL );

	// Iterate over all simulation islands.
	{
		// Fixed island
		const hkpSimulationIsland* island = world->getFixedIsland();
		for (int b = 0; b < island->getEntities().getSize(); b++ )
		{
			hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
			hkVector4 newPos = body->getPosition();
			newPos.add4( effectiveShiftDistance );
			body->getRigidMotion()->setPosition( newPos );
			body->updateCachedAabb();
			HK_UPDATE_GEOMETRY( body->getTransform(), (hkUlong)body->getCollidable() );
		}
	}
	{
		// Inactive islands
		const hkArray<hkpSimulationIsland*>& islands = world->getInactiveSimulationIslands();
		for (int i = 0; i < islands.getSize(); i++ )
		{
			hkpSimulationIsland* island = islands[i];
			for (int b = 0; b < island->getEntities().getSize(); b++ )
			{
				hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
				hkVector4 newPos = body->getPosition();
				newPos.add4( effectiveShiftDistance );
				body->getRigidMotion()->setPosition( newPos );
				body->updateCachedAabb();
				HK_UPDATE_GEOMETRY( body->getTransform(), (hkUlong)body->getCollidable() );
			}
		}
	}
	{
		// Active Islands
		const hkArray<hkpSimulationIsland*>& islands = world->getActiveSimulationIslands();
		for (int i = 0; i < islands.getSize(); i++ )
		{
			hkpSimulationIsland* island = islands[i];
			for (int b = 0; b < island->getEntities().getSize(); b++ )
			{
				hkpRigidBody* body =  static_cast<hkpRigidBody*>( island->getEntities()[b]);
				hkVector4 newPos = body->getPosition();
				newPos.add4( effectiveShiftDistance );
				body->getRigidMotion()->setPosition( newPos );
				body->updateCachedAabb();
				HK_UPDATE_GEOMETRY( body->getTransform(), (hkUlong)body->getCollidable() );

				// You also need to shift contact points to prevent them from becoming stale.
				// To do this iterate over all contact points maintained by the entity's collision agents.
				//
				// Each entity, if in contact with another entity, has a collision agent to manage contacts.
				// The collision agents in turn manage contact points via a contact point
				// manager.  To shift contact points we shift all contact points in the manager by the
				// effectiveShiftDistance passed in.
				hkpLinkedCollidable& collidableEx = *body->getLinkedCollidable();
				for (int collidableIndex = 0; collidableIndex < collidableEx.m_collisionEntries.getSize(); collidableIndex++)
				{
					hkpAgentNnEntry* entry = collidableEx.m_collisionEntries[collidableIndex].m_agentEntry;

					hkpDynamicsContactMgr* contactManager = static_cast<hkpDynamicsContactMgr*>(entry->m_contactMgr);

					HK_ASSERT(0x0, contactManager != HK_NULL);

					hkArray<hkContactPointId> contactPointIds;
					contactManager->getAllContactPointIds( contactPointIds );

					for( int contactPointIndex = 0; contactPointIndex < contactPointIds.getSize(); ++contactPointIndex )
					{
						hkContactPoint* contactPoint = contactManager->getContactPoint( contactPointIds[contactPointIndex] );

						hkVector4 nPos = contactPoint->getPosition();
						nPos.add4( effectiveShiftDistance );
						contactPoint->setPosition( nPos );
					}
				}
			}
		}
	}

	// Iterate over all phantoms, *except for the ones attached to the broadphase*, ie the borders
	{
		for ( int p = 0; p < world->getPhantoms().getSize(); p++ )
		{
			hkpPhantom* phantom = world->getPhantoms()[p];

			if( phantom->getCollidable()->getType() != hkpWorldObject::BROAD_PHASE_BORDER ) 
			{
				switch( phantom->getType())
				{
				case HK_PHANTOM_AABB:
					{
						hkpAabbPhantom* aabbPhantom = static_cast<hkpAabbPhantom*>( phantom );
						// Now do a bad trick to move the aabb.
						hkAabb& aabb = const_cast<hkAabb&>( aabbPhantom->getAabb() );
						aabb.m_min.add4( effectiveShiftDistance );
						aabb.m_max.add4( effectiveShiftDistance );
						break;
					}
				case HK_PHANTOM_SIMPLE_SHAPE:
				case HK_PHANTOM_CACHING_SHAPE:
					{
						hkpShapePhantom* shapePhantom = static_cast<hkpShapePhantom*>( phantom );
						hkTransform& transform = const_cast<hkTransform&>( shapePhantom->getTransform());
						transform.getTranslation().add4( effectiveShiftDistance );
						break;
					}
				default:
					HK_ASSERT2(0xf041604,0,"Unknown Phantom Type" );
				}
			}
		}
	}
	HK_TIMER_END();
}


void HK_CALL SlidingWorldDemo::removeDuplicatesFromArray(hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder )
{
	hkArray<hkpBroadPhaseHandle*> noDups;
	noDups.reserve( objectsEnteringBroadphaseBorder.getSize() );
	for (int i = 0; i < objectsEnteringBroadphaseBorder.getSize(); i++)
	{
		if( noDups.indexOf( objectsEnteringBroadphaseBorder[i] ) == -1)
		{
			noDups.pushBack( objectsEnteringBroadphaseBorder[i] );
		}
	}

	objectsEnteringBroadphaseBorder.swap( noDups );
	
}

void SlidingWorldDemo::handleKeys(hkBool& doShift, hkVector4& newCenter)
{

	const int height = m_env->m_window->getHeight() ;	
	if( m_currentMode == AUTOMATIC_SHIFT)
	{
		m_env->m_textDisplay->outputText("Shift is AUTOMATIC. Hit \221 to change to MANUAL", 10, height - 80);
		hkString s;
		s.printf("Time to next shift: %d", m_delayBetweenAutomaticShifts - (m_ticks % m_delayBetweenAutomaticShifts));
		m_env->m_textDisplay->outputText(s, 10, height - 40);
	}
	else
	{
		m_env->m_textDisplay->outputText("Shift is MANUAL. Hit \221 to change to AUTOMATIC", 10, height - 80);
		m_env->m_textDisplay->outputText("Hit \226,\227,\224,\225 to shift world", 10, height - 40);
	}
	m_env->m_textDisplay->outputText("(Newly added bodies colored green)", 300, height - 40);

	const hkgPad* pad = m_env->m_gamePad;
	if( pad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		m_currentMode = (m_currentMode == MANUAL_SHIFT )? AUTOMATIC_SHIFT : MANUAL_SHIFT;
		if(m_currentMode == MANUAL_SHIFT)
		{
			m_ticks = 0;
		}
	}

	if( m_currentMode == MANUAL_SHIFT )
	{
		newCenter = m_currentCenter;
		if( pad->wasButtonPressed( HKG_PAD_DPAD_RIGHT ) )
		{
			newCenter(0) += 10.0f;
			doShift = true;
		}
		if( pad->wasButtonPressed( HKG_PAD_DPAD_LEFT ) )
		{
			newCenter(0) -= 10.0f;
			doShift = true;
		}
		if( pad->wasButtonPressed( HKG_PAD_DPAD_UP ) )
		{
			newCenter(2) -= 10.0f;
			doShift = true;
		}
		if( pad->wasButtonPressed( HKG_PAD_DPAD_DOWN ) )
		{
			newCenter(2) += 10.0f;
			doShift = true;
		}
	}

	const int numCenters = sizeof(m_centers) / sizeof( hkVector4);

	if( (m_currentMode == AUTOMATIC_SHIFT) && (m_ticks % m_delayBetweenAutomaticShifts == 0))
	{
		doShift = true;
		newCenter = m_centers[((m_ticks-1) / m_delayBetweenAutomaticShifts) % numCenters];
	}

}

void SlidingWorldDemo::makeFakeInput()
{
	m_currentMode = AUTOMATIC_SHIFT;
	m_delayBetweenAutomaticShifts = 10;
}

hkDemo::Result SlidingWorldDemo::stepDemo()
{
	makeFakeInput();

	hkBool doShift = false;
	hkVector4 newCenter;
	handleKeys(doShift, newCenter);



	m_world->lock();

 	if(doShift)
	{
		   // reset the box colors to a light grey
		for (int i = 0; i < m_boxes.getSize(); i++)
		{
			HK_SET_OBJECT_COLOR(hkUlong(m_boxes[i]->getCollidable()), hkColor::LIGHTGREY);
		}

		{
			//hkVector4 currentCenter = m_centers[((m_ticks / delay) + (numCenters-1)) % numCenters];
			//HK_DISPLAY_STAR(currentCenter, 2.5f, 0xFF00FF00);
			//HK_DISPLAY_LINE(nextCenter, m_currentCenter, 0xFF00FF00);
		}

		hkVector4 diff; 
		diff.setSub4(newCenter, m_currentCenter);
		
		hkArray<hkpBroadPhaseHandle*> objectsEnteringBroadphaseBorder;
		
		hkVector4 effectiveShift;
		if( g_variants[m_variantId].m_mode == SlidingWorldDemoVariant::RECENTER_BROAD_PHASE_ONLY )
		{
			recenterBroadPhaseVariant(diff, objectsEnteringBroadphaseBorder, effectiveShift);
			
		}
		else // SlidingWorldDemoVariant::SHIFT_COORDINATE_SPACE)
		{
			shiftCoordinateSystemVariant(diff, objectsEnteringBroadphaseBorder, effectiveShift);
			
		} 

		removeDuplicatesFromArray(objectsEnteringBroadphaseBorder);

		// Some bodies may have to be removed from simulation, some may need to be added.
		{		
			removeBodiesLeavingBroadphaseFromSimulation(objectsEnteringBroadphaseBorder);
			addBodiesNewlyInsideSimulationAreaToSimulation();
		}

		m_currentCenter = newCenter;
	
	}

	
	// draw the grid in cyan
	{
		hkVector4 minOffset; minOffset.set(-5, 1, -5);
		hkVector4 maxOffset; maxOffset.set(5,1,5);
		hkVector4 min, max;

		for (int i = -2; i <= 2; i++)
		for (int j = -2; j <= 2; j++)
		{
			min.set((hkReal)10*i, 0, (hkReal)10*j);
			max = min;
			min.add4(minOffset);
			max.add4(maxOffset);
			drawAabb(min, max, hkColor::CYAN );
		}
	}

	 // draw the broadphase extents in red
	displayCurrentBroadPhaseAabb(m_world, hkColor::RED );

	// draw locations of all 'unsimulated' bodies
	drawUnsimulatedBodies();

	if( m_currentMode == AUTOMATIC_SHIFT)
	{
		m_ticks++;
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


void SlidingWorldDemo::recenterBroadPhaseVariant(const hkVector4& requestedShift, hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder, hkVector4& effectiveShift)
{
	// Move broadphase only, i.e. recenter the broadphase around a new location in world space, and do so without
	// addition/removal of overlaps for those objects remaining inside the new location.
	// We may however get objects entering the border (leaving the broadphase) if they are not contained in the new location.
	m_world->shiftBroadPhase( requestedShift, effectiveShift, hkpWorld::SHIFT_BROADPHASE_UPDATE_ENTITY_AABBS );

	// If broadphase phantoms are present, they come along for the ride too. Note that this is not done automatically by the 
	// shiftBroadPhase function above, as the broadphase does not know if it has a border or not!
	if (m_world->getBroadPhaseBorder() && m_world->getBroadPhaseBorder()->m_phantoms[0])	
	{
		for (int i=0; i<6; i++)
		{
			hkpAabbPhantom* phant = (hkpAabbPhantom*) m_world->getBroadPhaseBorder()->m_phantoms[i];
			hkAabb& aabb = const_cast<hkAabb&>( phant->getAabb() );
			aabb.m_min.add4(effectiveShift);
			aabb.m_max.add4(effectiveShift);
		}
	}

	recalcAabbsAfterBroadphaseRecenter( m_world );
}


void SlidingWorldDemo::shiftCoordinateSystemVariant(const hkVector4& requestedShift, hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder, hkVector4& effectiveShift)
{
	// Shift all objects in the world by updating their object data (transforms, contact points) and their broadphase data

	// Shift the broadphase
	hkArray<hkpBroadPhaseHandlePair> newCollisionPairs;
	m_world->getBroadPhase()->shiftAllObjects( requestedShift, effectiveShift, newCollisionPairs );

	// Shift all objects in the scene "silently"
	shiftAllGameObjectDataSilently( m_world, effectiveShift );

	// Shift all objects currently outside simulation as well since presumably the coordinate change is supposed to be global
	// and they may be readded
	for (int i = 0; i < m_boxes.getSize(); i++)
	{
			// Check if in simulation
		if( m_boxes[i]->getWorld() == HK_NULL )
		{
			hkVector4 pos = m_boxes[i]->getPosition();
			pos.add4(effectiveShift);
				// we can call setPosition() here with no real overhead as the bodies aren't actually in the simulation!
			m_boxes[i]->setPosition(pos);
		}
	}

}



//////////////////////////////////////////////////
// Below here are only helper display functions
//////////////////////////////////////////////////


void SlidingWorldDemo::displayCurrentBroadPhaseAabb(hkpWorld* world, int color)
{
	world->lock();
	hkpBroadPhase* bp = world->getBroadPhase();
	world->unlock();

	hkAabb broadphaseAabb;
	bp->getExtents(broadphaseAabb.m_min, broadphaseAabb.m_max);

	// Expand broadphase slightly so we can see it even if the broadphase phantoms are also displayed
	const hkVector4 smallOffset(0.05f, 0.05f, 0.05f, 0.0f);
	broadphaseAabb.m_min.sub4(smallOffset);
	broadphaseAabb.m_max.add4(smallOffset);

	drawAabb(broadphaseAabb.m_min, broadphaseAabb.m_max, color);
	
}

void SlidingWorldDemo::removeBodiesLeavingBroadphaseFromSimulation(const hkArray<hkpBroadPhaseHandle*>& objectsEnteringBroadphaseBorder)
{
	for (int i = 0; i < objectsEnteringBroadphaseBorder.getSize(); i++)
	{
		hkpBroadPhaseHandle* handle = objectsEnteringBroadphaseBorder[i];
		hkpCollidable* collidable = static_cast<hkpCollidable*>(static_cast<hkpTypedBroadPhaseHandle*>(handle)->getOwner());

		// It might be a rigid body, or a phantom, so check both possibilities
		hkpRigidBody* rigidBody = hkGetRigidBody(collidable);
		if(rigidBody)
		{
			m_world->removeEntity( rigidBody );
		}

		hkpPhantom* phantom = hkGetPhantom(collidable);
		if(phantom)
		{
			m_world->removePhantom( phantom );
		}

	}

}

void SlidingWorldDemo::addBodiesNewlyInsideSimulationAreaToSimulation()
{
	m_world->lock();
	hkpBroadPhase* bp = m_world->getBroadPhase();

	hkAabb broadphaseAabb;
	bp->getExtents( broadphaseAabb.m_min, broadphaseAabb.m_max);

	for (int i = 0; i < m_boxes.getSize(); i++)
	{
		hkpRigidBody* rb = m_boxes[i];

		// If we're not in simulation, check if we're now inside the simulation area/broadphase
		if( rb->getWorld() == HK_NULL )
		{		
			// In general you'd have some world object management code here. In our case, we'll just check if the current
			// aabb  of the body would be fully contained inside the new broadphase location.
			hkAabb rbAabb;
			rb->getCollidable()->getShape()->getAabb(rb->getTransform(), 0, rbAabb);

			if (broadphaseAabb.contains(rbAabb)) 
			{
				m_world->addEntity( rb );
					// Color them slightly green so they are clearly marked as newly added
				HK_SET_OBJECT_COLOR(hkUlong(rb->getCollidable()), hkColor::rgbFromChars(200, 255, 200));
			}
		}
	}

	m_world->unlock();

}



void SlidingWorldDemo::drawAabb(const hkVector4& m_minExtent, const hkVector4& m_maxExtent, int color)
{
	hkArray<hkVector4> lines;
	lines.setSize(24);

	lines[0].set(m_minExtent(0),m_minExtent(1),m_minExtent(2));
	lines[1].set(m_minExtent(0),m_maxExtent(1),m_minExtent(2));

	lines[2].set(m_minExtent(0),m_minExtent(1),m_minExtent(2));
	lines[3].set(m_minExtent(0),m_minExtent(1),m_maxExtent(2));

	lines[4].set(m_minExtent(0),m_minExtent(1),m_minExtent(2));
	lines[5].set(m_maxExtent(0),m_minExtent(1),m_minExtent(2));

	lines[6].set(m_maxExtent(0),m_maxExtent(1),m_maxExtent(2));
	lines[7].set(m_maxExtent(0),m_maxExtent(1),m_minExtent(2));

	lines[8].set(m_maxExtent(0),m_maxExtent(1),m_maxExtent(2));
	lines[9].set(m_minExtent(0),m_maxExtent(1),m_maxExtent(2));

	lines[10].set(m_maxExtent(0),m_maxExtent(1),m_maxExtent(2));
	lines[11].set(m_maxExtent(0),m_minExtent(1),m_maxExtent(2));

	lines[12].set(m_minExtent(0),m_maxExtent(1),m_minExtent(2));
	lines[13].set(m_maxExtent(0),m_maxExtent(1),m_minExtent(2));

	lines[14].set(m_minExtent(0),m_maxExtent(1),m_minExtent(2));
	lines[15].set(m_minExtent(0),m_maxExtent(1),m_maxExtent(2));

	lines[16].set(m_maxExtent(0),m_maxExtent(1),m_minExtent(2));
	lines[17].set(m_maxExtent(0),m_minExtent(1),m_minExtent(2));

	lines[18].set(m_minExtent(0),m_maxExtent(1),m_maxExtent(2));
	lines[19].set(m_minExtent(0),m_minExtent(1),m_maxExtent(2));

	lines[20].set(m_minExtent(0),m_minExtent(1),m_maxExtent(2));
	lines[21].set(m_maxExtent(0),m_minExtent(1),m_maxExtent(2));

	lines[22].set(m_maxExtent(0),m_minExtent(1),m_maxExtent(2));
	lines[23].set(m_maxExtent(0),m_minExtent(1),m_minExtent(2));

	for (int i=0; i<24; i+=2)
	{
		HK_DISPLAY_LINE(lines[i], lines[i+1], color);
	}
}

void SlidingWorldDemo::drawUnsimulatedBodies()
{
	for (int i = 0; i < m_boxes.getSize(); i++)
	{
		hkpRigidBody* rb = m_boxes[i];

			// Check if in simulation
		if( rb->getWorld() == HK_NULL )
		{
			// Get center, and draw a gray box here
			hkVector4 pos = rb->getPosition();
			const hkReal halfSize = 0.5f;
			hkVector4 halfExtents(halfSize, halfSize, halfSize);
			hkVector4 min, max;
			min.setSub4(pos, halfExtents);
			max.setAdd4(pos, halfExtents);

			drawAabb(min, max, hkColor::rgbFromChars(170, 170, 170, 150) );
		}
	}
	
}


#if defined(HK_COMPILER_MWERKS)
#   pragma force_active on
#   pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows how to recenter the broadphase or how to change the world coordinate system.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SlidingWorldDemo, HK_DEMO_TYPE_PRIME, SlidingWorldDemoVariant, g_variants, helpString );


/*
Basically if you have a 'huge' world and you are using the Havok physics SDK you are likely to hit two problems:

	 A. The accuracy of floating point numbers is limited. Therefore you will get serious physics artifacts if objects
	    are positioned too far away from the origin. This is not a Havok-specific problem - It is likely that you will 
		see artifacts in other areas of your game such as graphics or AI calculations.
	 B. The size of the Havok Broadphase is limited (because it quantizes space) and changing this size to be huge
		in order to bound your entire game world means that the bounding box (AABB) representation of bodies and phantoms 
		is overly coarse and hence likely to be a performance hit.


To solve:

	 A: To prevent floating point accuracy problems, we have to keep the position of the objects as small as possible.
		You can do this by periodically shifting your coordinate space when you detect that your character/area of interaction
		has moved too far.
		Note that while doing so the position of the broadphase always stays around the origin.

		To shift the coordinate space in Havok, you have to reposition all objects in the world, which you can do by calling
		hkpRigidBody::setPosition(). However this has some nasty side effects:
			- It is very CPU intensive (overlaps and hence agents are destroyed and recreated)
			- It can cause inactive objects to wake up as active objects can temporarily hit those inactive objects.
			- You also have to move the fixed objects. However if you call setPosition on a fixed object,
			  the engine won't be able track the movement of this objects and has to disable a collision detection
			  optimization called tims. This can cause a 100% permanent performance hit.

		To solve this, we need a 'silent' way to move all objects. We do this in two steps:
			 - 1. As the Havok SDK provides even low level access to all data structures,
			   you can simply iterate over all objects and move them. This is illustrated in the method 
			   SlidingWorldDemo::shiftAllGameObjectDataSilently() which updates everything apart from the broadphase information.
			 - 2. To silently update the broadphase information, we use hkpBroadPhase::shiftAllObjects();

		This solution :
			  - solves problems A and B
			  - is very fast
			  - is simple, but we have to iterate over all objects in the world
			  - does not work with broadphase marker optimizations
			  - N.B. You need to remove objects hitting the broadphase boundary

	 B: Havok added a new function: hkpBroadPhase::shiftBroadPhase() to the SDK. It simply moves/recenters the broadphase.
		So you can move the broadphase with the character/area of interaction (and remove all objects falling outside this area).
		As the broadphase can't be shifted by any arbitrary distance, this function returns a valid shift
		distance which is closest to the input shift.
		This is:
		  - simple
		  - fast
		However:
		  - it does not work with broadphase marker optimizations
		  - you need to remove objects hitting the boundary
		  - it does not solve problem A
	 B: Another solution is to use the 32bit implementation of hkBroadphase.  It supports more objects (2^31 compared to
		2^14 of the default 16bit hkBroadphase) and also has an internal resolution of 31 bits.  This means that objects will
		have tighter fitting AABB extents even at large coordinates.  The caveat with the 32bit implementation is that it is roughly
		twice as slow and takes up twice as much memory.

		The 32bit broadphase will not prevent floating point accuracy problems and should be used when:
		   - you need support for more than 2^14 objects in the world
		   - you need tighter fitting AABBs around world objects to increase the performance of narrowphase collision detection

		To enable the 32bit hkBroadphase implementation put the following macro *before* world construction.

		#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
		hkpBroadPhase::Enable32BitBroadphase broadphaseHelper
	 B: Another solution would be to somehow resize the broadphase: This is pretty much identical to rebuilding the broadphase in
		terms of algorithmic complexity.
		The only way to do this in Havok is to create a new hkpWorld and move all objects over.


FAQs:


1. To solve problem A, we must do 2 steps. Why isn't the first step ("Iterate over all objects and move them in a silent way").
   part of the hkpBroadPhase::shiftAllObjects() function call?

		The reason is modularity. The broadphase has no access to the rest of the system.
		In Havok we had to make a choice whether to
		- (a): have all the code in an external demo
		- (b): or to integrate the shift functionality as a core feature of the engine.
		(b) would have made the SDK little bit more complicated. Also the advantage of
		going with solution (a) was that everything we do is explicit. So if a Havok user
		has to add extra code to shift his own actions and controllers he can do so.
		So for now most of the desired functionality is in the demo, not in the SDK.

*/

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

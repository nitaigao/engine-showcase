/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpActiveContactPointViewer.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/ToiCollisionsForKeyframedBodies/ToiCollisionsForKeyframedBodiesDemo.h>



	// This specific implementation is not referenced by any other class.
	// It adds itself as a listener to the entity and automatically deletes itself, 
	// when the entity gets deleted.	
class KeyframedBulletCollisionListener : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{
   	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		KeyframedBulletCollisionListener( hkpRigidBody* rigidBody)
		{
			rigidBody->addCollisionListener( this );
			rigidBody->addEntityListener( this );
		}
        
			//
			// hkpCollisionListener interface implementation
			//

			// Draws the contact point as a little red star.
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event ) {}

		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
		{
			if (event.m_type == hkpContactPointAddedEvent::TYPE_TOI)
			{
				// Color the stationary colliding body red
				hkUlong otherBodyId = hkUlong(event.m_callbackFiredFrom->getCollidable()) ^ hkUlong(&event.m_collidableA) ^ hkUlong(&event.m_collidableB);
				HK_SET_OBJECT_COLOR(otherBodyId, 0xffff0000U);

				hkpRigidBody* bullet = static_cast<hkpRigidBody*>(event.m_callbackFiredFrom);
				hkpWorld* world = bullet->getWorld();

				// Reverse the horizontal velocity of the body
				hkVector4 vel = bullet->getLinearVelocity();
				vel(0) *= -1.0f;
				bullet->setLinearVelocity(vel);

				// And reintegrate the body for immediate collision-response effect
				world->reintegrateAndRecollideEntities(&event.m_callbackFiredFrom, 1);
			}
		}


		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {}

		virtual void contactProcessCallback( hkpContactProcessEvent& event ) {}

			//
			// hkpEntityListener interface implementation
			//

			// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
		void entityDeletedCallback( hkpEntity* entity )
		{
			entity->removeCollisionListener( this ); 
			entity->removeEntityListener( this );

			// Now that we're removed from our entity, we can remove ourselves, too.
			delete this;
		}

			// Do nothing (we'll remove the collision event listener only when the entity is deleted)
		void entityRemovedCallback( hkpEntity* entity ) {}

};

ToiCollisionsForKeyframedBodies::ToiCollisionsForKeyframedBodies(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	enableDisplayingToiInformation(true);

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f,  -20.0f, 7.5f);
		hkVector4 to  (0.0f,  20.0f,  0.0f);
		hkVector4 up  (0.0f,  0.0f,  1.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Setup and create the hkpWorld
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(300.0f);
		info.m_collisionTolerance = .03f; 
		info.m_gravity.setZero4();
		info.m_simulationType = info.SIMULATION_TYPE_CONTINUOUS;
		info.m_expectedMaxLinearVelocity = 700.0f;

		m_world = new hkpWorld( info );
		m_world->lock();

		m_debugViewerNames.pushBack(hkpActiveContactPointViewer::getName());
		
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		setupGraphics();
	}

	// Create the fast keyframed bullet
	{
		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
			// Were setting the type to KEYFRAMED_REPORTING in order to receive events from collisions with
			// other keyframed and fixed bodies.
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_KEYFRAMED_REPORTING;
		ci.m_shape = new hkpSphereShape( 0.5f ); 
		ci.m_mass = 1.0f;
		ci.m_inertiaTensor.setDiagonal( 1.0f, 1.0f, 1.0f );
		ci.m_angularDamping = 0.0f;
		ci.m_linearDamping = 0.0f;
		ci.m_allowedPenetrationDepth = 0.5f; // value of 0.5f allows for penetration !!
		ci.m_allowedPenetrationDepth = 0.1f; 
		ci.m_position.set( - 5.0f, -7.0f, 0);

			// As this body will only collide with fixed/keyframed bodies (i.e. in this demo)
			// we can choose whether to override the default collisionResponse type with reporting-response only.
		ci.m_collisionResponse = hkpMaterial::RESPONSE_REPORTING;

		hkpRigidBody* bullet = new hkpRigidBody(ci);
		m_world->addEntity( bullet );

		ci.m_shape->removeReference();

		//
		//	Heavily push the bullet
		//
		const hkVector4 velocity( 500.0f, 500.0f, 0.0f );
		bullet->setLinearVelocity( velocity );
			// This object adds itself as a listener to the entity and deletes itself when
			// the 'owning' entity gets deleted.
		new KeyframedBulletCollisionListener( bullet );

		HK_SET_OBJECT_COLOR(hkUlong(bullet->getCollidable()), 0xff0000ffU);
		bullet->removeReference();
	}

	// Create two rows of balls
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_KEYFRAMED;
			ci.m_shape = new hkpSphereShape( 0.5f ); 
			ci.m_mass = 1.0f;
			ci.m_inertiaTensor.setDiagonal( 1.0f, 1.0f, 1.0f );
			ci.m_angularDamping = 0.0f;
			ci.m_linearDamping = 0.0f;
			ci.m_position.set( !i?-3.0f:3.0f, j * 1.0f, 0);
			ci.m_allowedPenetrationDepth = 0.1f; 

			hkpRigidBody* body = new hkpRigidBody(ci);
			m_world->addEntity( body );

			body->removeReference();
			ci.m_shape->removeReference();
		}
	}


	m_world->unlock();
}

ToiCollisionsForKeyframedBodies::~ToiCollisionsForKeyframedBodies()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true);
}

hkDemo::Result ToiCollisionsForKeyframedBodies::stepDemo()
{
	const hkReal physicsDeltaTime = 1.0f / 60.0f;
	const hkReal frameDeltaTime = physicsDeltaTime / 10.0f;

	hkDefaultPhysicsDemo::stepAsynchronously(m_world, frameDeltaTime, physicsDeltaTime );

	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A keyframed bullet is shot at a group of objects. " \
"By default this would result in zero response, and the trajectory of the bullet would remain unchanged. " \
"Here, a collision listener is connected to the bullet. It changes the horizontal direction of the bullet "\
"upon each Toi-collision (high-speed collision) and colors the other body red.";

HK_DECLARE_DEMO(ToiCollisionsForKeyframedBodies, HK_DEMO_TYPE_PRIME, "Keyframed bullet reacting to collisions with other keyframed or fixed bodies", helpString);

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

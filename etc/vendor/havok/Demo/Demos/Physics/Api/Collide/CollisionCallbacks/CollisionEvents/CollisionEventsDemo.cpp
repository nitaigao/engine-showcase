/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/CollisionCallbacks/CollisionEvents/CollisionEventsDemo.h>

// In this example we do not use the hkpAgentRegisterUtil
// but register the hkpBoxBoxAgent ourselves
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

// We need to process and access some collision based info
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>

// We need to display some collision based info
#include <Common/Visualize/hkDebugDisplay.h>


CollisionEventsDemo::CollisionEventsDemo( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env) 
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(3.0f, 4.0f, 8.0f);
		hkVector4 to(0.0f, 1.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_simulationType = info.SIMULATION_TYPE_CONTINUOUS;

		// Turn off deactivation so we can see continuous contact point processing
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the box-box collision agent
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}




	//
	// Create the floor
	//
	{
		hkpRigidBodyCinfo info;
		hkVector4 fixedBoxSize(5.0f, 0.5f , 5.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		info.m_shape = fixedBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position.setZero4();

		// Add some bounce.
		info.m_restitution = 0.8f;
		info.m_friction = 1.0f;

		// Create fixed box
		hkpRigidBody* floor = new hkpRigidBody(info);
		m_world->addEntity(floor);

		floor->removeReference();
		fixedBoxShape->removeReference();
	}

	// For this demo we simply have two box shapes which are constructed in the usual manner using a hkpRigidBodyCinfo 'blueprint'.
	// The dynamic box creation code in presented below. There are two key things to note in this example;
	// the 'm_restitution' member variable, which we have explicitly set to value of 0.9.
	// The restitution is over twice the default value of
	// 0.4 and is set to give the box (the floor has been set likewise) a more 'bouncy' nature. 

	//
	// Create a moving box
	//
	{
		hkpRigidBodyCinfo boxInfo;
		hkVector4 boxSize( .5f, .5f ,.5f );
		boxInfo.m_shape = new hkpBoxShape( boxSize , 0 );


		// Compute the box inertia tensor
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( boxInfo.m_shape, 1.0f, boxInfo );

		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Place the box so it bounces interestingly
		boxInfo.m_position.set(0.0f, 5.0f, 0.0f);
		hkVector4 axis(1.0f, 2.0f, 3.0f);
		axis.normalize3();
		boxInfo.m_rotation.setAxisAngle(axis, -0.7f);

		// Add some bounce.
		boxInfo.m_restitution = 0.5f;
		boxInfo.m_friction = 0.1f;

		hkpRigidBody* boxRigidBody = new hkpRigidBody( boxInfo );

		// remove reference from boxShape since rigid body "owns" it
		boxInfo.m_shape->removeReference();

		m_world->addEntity( boxRigidBody );
		boxRigidBody->removeReference();
		
		// Add the collision event listener to the rigid body
		MyCollisionListener* listener = new MyCollisionListener( boxRigidBody );
		listener->m_reportLevel = m_env->m_reportingLevel;
	}

	m_world->unlock();

}

// Our myCollisionListener class inherits from both hkpCollisionListener and hkpEntityListener and so implementations for each
// of the virtual methods in both base classes must be present. However, for the purposes of this demo we are only interested
// in the contactPointAddedCallback(...) and contactProcessCallback(...) methods. It is within these that we are able to use
// the newly acquired contact information and display our addition graphical data.


MyCollisionListener::MyCollisionListener( hkpRigidBody* rigidBody)
{
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_uniqueIdCounter = 0;
}


MyCollisionListener::~MyCollisionListener()
{
}

// The contactPointAddedCallback(...) code is given below:

void MyCollisionListener::contactPointAddedCallback(	hkpContactPointAddedEvent& event )
{
	//
	// draw the contact point as a little red star
	//
	{
		const hkVector4& start = event.m_contactPoint->getPosition();
		for ( int i = 0; i < 20; i++ )
		{
			hkVector4 dir( hkMath::sin( i * 1.0f ), hkMath::cos( i * 1.0f ), hkMath::sin(i * 5.0f ) );
			dir.setMul4(0.3f, dir);
			hkVector4 end;		end.setAdd4(start, dir);
			HK_DISPLAY_LINE(start, end, hkColor::RED);
		}
	}

	//
	//	collect all information in our own data structure
	//  as the havok memory manager is really really fast,
	//  allocating lots of small structures is acceptable
	//
	if ( event.m_contactPointProperties->getUserData() == HK_NULL )
	{
		ContactPointInfo* info = new ContactPointInfo;
		info->m_uniqueId = m_uniqueIdCounter++;
		event.m_contactPointProperties->setUserData( reinterpret_cast<hkUlong>(info) );

		//
		//	printf some information 
		//
		if (m_reportLevel >= hkDemoEnvironment::REPORT_INFO)
		{
			if ( event.isToi() )
			{
 				hkprintf("Toi userId=%i created\n", info->m_uniqueId );
			}
			else
			{
				int cpId = event.asManifoldEvent().m_contactPointId;
				hkprintf("Contact Point userId=%i created: contactId=%i\n", info->m_uniqueId, cpId );
			}
		}
	}

	// By setting the  ProcessContactCallbackDelay to 0 we will receive callbacks for 
	// any collisions processed for this body every frame (simulation step), i.e. the delay between
	// any such callbacks is 0 frames.

	// If you wish to only be notified every N frames simply set the delay to be N-1.
	// The default is 65536, i.e. (for practical purpose) once for the first collision only, until
	// the bodies separate to outside the collision tolerance. 
	event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
}


	// We obtain a handle to the contact point through the hkpContactPointAddedEvent structure and from this we can extract
	// the position in World Space. 
void MyCollisionListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
	ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties->getUserData() );
	if ( !info )
	{
		return;
	}

	if (m_reportLevel >= hkDemoEnvironment::REPORT_INFO)
	{
		if ( event.isToi() )
		{
			hkprintf("Toi userId=%i deleted\n", info->m_uniqueId );
		}
		else
		{
			int cpId = event.m_contactPointId;
			hkprintf("Contact Point userId=%i deleted. contactId=%i\n", info->m_uniqueId, cpId );
		}
	}
	delete info;
	event.m_contactPointProperties->setUserData( HK_NULL );
}

void MyCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{
	ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties->getUserData() );
	if (!info )
	{
		return;
	}

	if (m_reportLevel >= hkDemoEnvironment::REPORT_INFO)
	{
		if ( event.isToi() )
		{
			hkprintf("Toi userId=%i confirmed\n", info->m_uniqueId );
		}
		else
		{
			hkContactPointId id = event.getContactPointId();

			// you can get the contactMgr here by calling:  event.getContactMgr();
			hkprintf("Contact userId=%i confirmed. contactId=%i\n", info->m_uniqueId, id );
		}
	}
}




//
// contactProcessCallback
//
void MyCollisionListener::contactProcessCallback( hkpContactProcessEvent& event )
{
	hkpProcessCollisionData& result = *event.m_collisionData;
	int size = result.getNumContactPoints();

	for (int i = 0; i < size; i++ )
	{
		hkpProcessCdPoint& cp = result.m_contactPoints[i];
		{
			ContactPointInfo* info = reinterpret_cast<ContactPointInfo*>( event.m_contactPointProperties[i]->getUserData() );
			if ( (info) && (m_reportLevel >= hkDemoEnvironment::REPORT_INFO) )
			{
				int contactId = cp.m_contactPointId;
				hkprintf("Contact userId=%i processed. Impulse %f. Contact Point Id=%i\n", info->m_uniqueId, event.m_contactPointProperties[i]->getImpulseApplied(), contactId );
			}
		}

		// draw the contact points and normals in white
		{
			const hkVector4& start = result.m_contactPoints[i].m_contact.getPosition();
			hkVector4 normal       = result.m_contactPoints[i].m_contact.getNormal();

			// For ease of display only, we'll always draw the normal "up" (it points from entity 'B'
			// to entity 'A', but the order of A,B is arbitrary) so that we can see it. Thus, if it's 
			// pointing "down", flip its direction (and scale), only for display.
			normal.mul4(5.0f * normal(1));
			HK_DISPLAY_ARROW(start, normal, hkColor::WHITE);
		}
	}
}

// Here we use the hkpContactProcessEvent structure to access the hkpProcessCollisionOutput which has an array containing all of the contacts
// points. We then simply iterate through each of these points in turn and read the contact point position and normal and display
// a WHITE line at this point. We'd like the normal to always be drawn 'upwards' away from the floor, and since the
// order of the two collidables A and B is arbitrary, we flip the sign of the normal if it points 'downwards'.


//
// entityDeletedCallback
//
void MyCollisionListener::entityDeletedCallback( hkpEntity* entity )
{
	// Remove the collision event listener
	entity->removeCollisionListener( this ); 

	// Now that we're removed from our entity, we can remove ourselves, too.
	delete this;
}


void MyCollisionListener::entityRemovedCallback( hkpEntity* entity )
{
	// Do nothing (we'll remove the collision event listener only when the entity is deleted)
}


hkDemo::Result CollisionEventsDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates the use of Collision Events (for a specific Entity). "\
"In this demo these callbacks have been implemented by the myCollisionListener class which, "\
"upon the addition of a new contact point to the manifold, displays a RED star at that point. It also displays "\
"a WHITE line to represent the 'normal' whenever the contact is processed by the solver. Also it printfs " \
"other information like contact point id etc";

HK_DECLARE_DEMO(CollisionEventsDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Using Collision Points 'added' and 'processed' callbacks", helpString);

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

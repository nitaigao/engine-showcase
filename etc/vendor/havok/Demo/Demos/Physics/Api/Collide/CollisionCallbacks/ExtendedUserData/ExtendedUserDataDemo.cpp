/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/CollisionCallbacks/ExtendedUserData/ExtendedUserDataDemo.h>

// In this example we do not use the hkpAgentRegisterUtil
// but register the hkpBoxBoxAgent ourselves
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

// We need to process and access some collision based info
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>

// We need to display some collision based info
#include <Common/Visualize/hkDebugDisplay.h>


ExtendedUserDataDemo::ExtendedUserDataDemo( hkDemoEnvironment* env) 
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
		
		// Force this to collide on PPU and raise all callbacks
		info.m_forceCollideOntoPpu = true;

		// Force this to collide on PPU and raise all callbacks
		info.m_forceCollideOntoPpu = true;

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
	// Create a moving object
	//
	{
		hkpRigidBodyCinfo multiSpheresInfo;
		hkVector4 boxSize( .5f, .5f ,.5f );		

		// Build listShape of spheres
		// 
		{
			hkpSphereShape* sphere = new hkpSphereShape(0.2f);
			hkReal size = 0.5f;
			hkpShape* shapes[8];
			for (int i = 0; i < 8; i++)
			{
				hkReal px = i&0x1 ? -size : size;
				hkReal py = i&0x2 ? -size : size;
				hkReal pz = i&0x4 ? -size : size;

				hkVector4 translation(px, py, pz);
				
				shapes[i] = new hkpConvexTranslateShape(sphere, translation);

			}
			sphere->removeReference();

			hkpListShape* list = new hkpListShape(shapes, 8);
			for(int i = 0; i < 8; i++)
			{
				shapes[i]->removeReference();
			}

			multiSpheresInfo.m_shape = list;
		}




		// Compute the box inertia tensor
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( multiSpheresInfo.m_shape, 1.0f, multiSpheresInfo );

		multiSpheresInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		multiSpheresInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Place the box so it bounces interestingly
		multiSpheresInfo.m_position.set(0.0f, 5.0f, 0.0f);
		hkVector4 axis(1.0f, 2.0f, 3.0f);
		axis.normalize3();
		multiSpheresInfo.m_rotation.setAxisAngle(axis, -0.7f);

		// Add some bounce.
		multiSpheresInfo.m_restitution = 0.5f;
		multiSpheresInfo.m_friction = 0.1f;
		multiSpheresInfo.m_numUserDatasInContactPointProperties = 3;

		hkpRigidBody* multiSphereRigidBody = new hkpRigidBody( multiSpheresInfo );

		// remove reference from boxShape since rigid body "owns" it
		multiSpheresInfo.m_shape->removeReference();

		m_world->addEntity( multiSphereRigidBody );
		multiSphereRigidBody->removeReference();
		
		// Add the collision event listener to the rigid body
		m_listener = new MyExtendedUserDataListener( multiSphereRigidBody );
	}

	m_world->unlock();

}

// Our MyExtendedUserDataListener class inherits from both hkpCollisionListener and hkpEntityListener and so implementations for each
// of the virtual methods in both base classes must be present. However, for the purposes of this demo we are only interested
// in the contactPointAddedCallback(...) and contactProcessCallback(...) methods. It is within these that we are able to use
// the newly acquired contact information and display our addition graphical data.


MyExtendedUserDataListener::MyExtendedUserDataListener( hkpRigidBody* rigidBody)
{
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_uniqueIdCounter = 0;
}


MyExtendedUserDataListener::~MyExtendedUserDataListener()
{
}

void MyExtendedUserDataListener::contactPointAddedCallback(	hkpContactPointAddedEvent& event )
{
	//
	// Read the shape key hierarchy -- this is done identically for both Psi and Toi contact points.
	//
	hkpShapeKey shapeKey;
	{
		// This is the body to which the listener is attached.
		hkpEntity* body = event.m_callbackFiredFrom;

		// Extended user data only works when you use the default hkpSimpleConstraintContactMgr.
		// The function below will assert otherwise.

		// The atom caches information on how many extended user datas we store for each body.
		// Each body stores its data independently.
		const int numDatas = event.getNumExtendedUserDatas(event.m_callbackFiredFrom);

		// Now we can read the data.
		// The first user data stores the hkpShapeKey of the bottom most hkpCdBody in the shape hierarchy, 
		// the next one stores hkpShapeKey of its parent, and so on, till we store '-1' as the hkpShapeKey
		// of the root collidable, or we run out of extended user data slots, in which case only a part
		// of the hierarchy is stored.
		//
		// In this demo we expect to have:
		// extendedUserDatas[0] in the [0,7] range -- this is the hkpShapeKey of one of the 8 transformed sphere shapes grouped under a hkpListShape
		// extendedUserDatas[1] equal -1 -- this is the root hkpListShape, with no parent, and therefore no hkpShapeKey
		//
		// Note that we only have two levels of hierarchy, while our shape is composed of hkpListShape->hkpConvexTransformShape->hkpSphereShape.
		// That's because hkpConvexTransform/TranslateShapes don't create a corresponding hkpCdBody during collision detection.
		// Note that the above is not the case for the deprecated hkpTransformShape.
		hkInplaceArray<hkpContactPointProperties::UserData,8> data; data.setSize(numDatas);
		event.getExtendedUserDatas(body, data.begin(), numDatas);

		// Let us store our custom contact point id. Let's store it in the last data slot to avoid overwriting the hkpShapeKey hierarchy.
		// We know we have enough room, as we set entity->m_numUserDatasInContactPointProperties = 3 which is more that the max hierarchy depth for our shape.
		data[numDatas-1] = m_uniqueIdCounter++;

		// And write back all datas
		event.setExtendedUserDatas(body, data.begin(), numDatas);

		// Get the hkpShapeKey
		shapeKey = data[0];
	}

	// Sample demo structure holding history about callbacks triggered for this contact point.
	{
		ContactPointInfo& info = m_contactInfos.expandOne();
		new (&info) ContactPointInfo();
		HK_ASSERT2(0xad7853aa, m_contactInfos.getSize() == m_uniqueIdCounter, "Unique ID is not in synch with m_contactInfos array.");
		info.m_type = event.isToi() ? ContactPointInfo::TOI : ContactPointInfo::PSI;
		info.m_added = true;
		info.m_key = int(shapeKey);
	}

	// By setting the  ProcessContactCallbackDelay to 0 we will receive callbacks for 
	// any collisions processed for this body every frame (simulation step), i.e. the delay between
	// any such callbacks is 0 frames.

	// If you wish to only be notified every N frames simply set the delay to be N-1.
	// The default is 65536, i.e. (for practical purpose) once for the first collision only, until
	// the bodies separate to outside the collision tolerance. 
	event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
}

void MyExtendedUserDataListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{
	// Get num extra user datas for that body
	const int numDatas = event.getNumExtendedUserDatas(event.m_callbackFiredFrom);

	// Create a temp array to store the data
	hkInplaceArray<hkpContactPointProperties::UserData, 8> data; data.setSize(numDatas);

	// Get the data
	event.getExtendedUserDatas(event.m_callbackFiredFrom, data.begin(), numDatas);

	int id = data[numDatas-1];
	HK_ASSERT2(0xad755512, m_contactInfos[id].m_confirmed == false 
		                && m_contactInfos[id].m_processed >=1, "Callbacks don't match."); // We might have multiple process callbacks already if the point was involved in a toi
	m_contactInfos[id].m_confirmed = true;
}

void MyExtendedUserDataListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
	// Get num extra user datas for that body
	const int numDatas = event.getNumExtendedUserDatas(event.m_callbackFiredFrom);

	// Create a temp array to store the data
	hkInplaceArray<hkpContactPointProperties::UserData, 8> data; data.setSize(numDatas);

	// Get the data
	event.getExtendedUserDatas(event.m_callbackFiredFrom, data.begin(), numDatas);

	int id = data[numDatas-1];
	m_contactInfos[id].m_removed = true;

}

void MyExtendedUserDataListener::contactProcessCallback( hkpContactProcessEvent& event )
{
	// Get num extra user datas for that body
	const int numDatas = event.getNumExtendedUserDatas(event.m_callbackFiredFrom);

	// Create a temp array to store the data
	hkInplaceArray<hkpContactPointProperties::UserData, 8> data; data.setSize(numDatas);

	// Process PSI points
	for (int i = 0; i < event.m_collisionData->getNumContactPoints(); i++)
	{
		hkpProcessCdPoint& point = event.m_collisionData->m_contactPoints[i];

		// Get extended user datas
		event.getExtendedUserDatas(i, event.m_callbackFiredFrom, data.begin(), numDatas);

		// Get our custom contact id from the last extended user data
		int id = data[numDatas-1];
		int key = data[0];

		// Display info about the point
		char text[64];
		hkString::sprintf(text, "Id: %d Key: %d", id, key);
		HK_DISPLAY_3D_TEXT(text, point.m_contact.getPosition(), 0xffffffff);

		// Modify external contact-info structure
		m_contactInfos[id].m_processed ++;

	}

	// Process TOI point
	if (event.m_collisionData->hasToi())
	{
		// Get extended user datas
		event.getToiExtendedUserDatas(event.m_callbackFiredFrom, data.begin(), numDatas);
		int id = data[numDatas-1];
		m_contactInfos[id].m_processed ++;
	}

}

// Here we use the hkpContactProcessEvent structure to access the hkpProcessCollisionOutput which has an array containing all of the contacts
// points. We then simply iterate through each of these points in turn and read the contact point position and normal and display
// a WHITE line at this point. We'd like the normal to always be drawn 'upwards' away from the floor, and since the
// order of the two collidables A and B is arbitrary, we flip the sign of the normal if it points 'downwards'.


//
// entityDeletedCallback
//
void MyExtendedUserDataListener::entityDeletedCallback( hkpEntity* entity )
{
	// Remove the collision event listener
	entity->removeCollisionListener( this ); 

	// Now that we're removed from our entity, we can remove ourselves, too.
	delete this;
}


void MyExtendedUserDataListener::entityRemovedCallback( hkpEntity* entity )
{
	// Do nothing (we'll remove the collision event listener only when the entity is deleted)
}


hkDemo::Result ExtendedUserDataDemo::stepDemo()
{
	Result result = hkDefaultPhysicsDemo::stepDemo();

	char text[10000];
	int len = hkString::sprintf(text, " Id Key Type Callbacks triggered per point\r\n");

	// Display all the contact point info
	for (int c = hkMath::max2(0, m_listener->m_contactInfos.getSize()-20); c < m_listener->m_contactInfos.getSize(); c++)
	{
		MyExtendedUserDataListener::ContactPointInfo& info = m_listener->m_contactInfos[c];
		if (unsigned(len) > sizeof(text) - 1000)
		{
			break;
		}
		len += hkString::sprintf(text+len, "%3d %2d   %s %s Num.Processed:%3d %s %s\r\n", 
																			c, 
																			info.m_key, 
																			info.m_type == MyExtendedUserDataListener::ContactPointInfo::PSI ? "PSI" : "TOI",
																			info.m_added ? "Added" : "     ", 
																			info.m_processed,
																			info.m_confirmed ? "Confirmed" : "         ",
																			info.m_removed ? "Removed" : " ");
	}

	m_env->m_textDisplay->outputText(text, 10, 160);
	//HK_DISPLAY_TEXT(text, 0xffffffff);
	
	return result;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates the use of Collision Events (for a specific Entity). "\
"In this demo these callbacks have been implemented by the MyExtendedUserDataListener class which, "\
"upon the addition of a new contact point to the manifold, displays a RED star at that point. It also displays "\
"a WHITE line to represent the 'normal' whenever the contact is processed by the solver. Also it printfs " \
"other information like contact point id etc";

HK_DECLARE_DEMO(ExtendedUserDataDemo, HK_DEMO_TYPE_PRIME, "Using Collision Points 'added' and 'processed' callbacks", helpString);

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

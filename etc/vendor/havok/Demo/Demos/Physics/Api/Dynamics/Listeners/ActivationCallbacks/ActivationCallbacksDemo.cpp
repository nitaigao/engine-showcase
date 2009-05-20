/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Listeners/ActivationCallbacks/ActivationCallbacksDemo.h>

#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Entity/hkpEntityActivationListener.h>
#include <Physics/Dynamics/World/hkpSimulationIsland.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Base/Types/Color/hkColor.h>



class ActivationCallbacksDemoEntityActivationListener : public hkReferencedObject, public hkpEntityActivationListener, public hkpEntityListener
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	virtual void entityDeactivatedCallback(hkpEntity* entity)
	{
		HK_SET_OBJECT_COLOR( hkUlong(entity->getCollidable()), 0xffff5555 );
	}

	virtual void entityActivatedCallback(hkpEntity* entity)
	{
		HK_SET_OBJECT_COLOR( hkUlong(entity->getCollidable()), 0xff55ff55 );
	}

	/// Called when an entity is deleted. hkpEntityListener subclasses <b>must</b> implement this function.
	virtual void entityDeletedCallback( hkpEntity* entity ) 
	{
		delete this;
	}

};



ActivationCallbacksDemo::ActivationCallbacksDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 7.0f, 30.0f);
		hkVector4 to  (0.0f, 3.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld( info );
		m_world->lock();

		m_debugViewerNames.pushBack( hkpSimulationIslandViewer::getName()  );

		setupGraphics();
	}

	//
	// Register the agents
	//
	{
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}


	//
	// Create the fixed box
	//
	{
		// Position of the box
		hkVector4 boxPosition(0.0f, 0.0f, 0.0f);

		// Set up the construction info parameters for the box
		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_FIXED;

		info.m_friction = 1.0f;

		info.m_shape = new hkpBoxShape( hkVector4(60.0f, 0.2f, 60.0f), 0.05f );
		info.m_position = boxPosition;

		// Create fixed box
		hkpRigidBody* box = new hkpRigidBody(info);
		m_world->addEntity(box);
		box->removeReference();

		info.m_shape->removeReference();
	}

	//
	// Create the moving boxes
	//

	
	for (int i = 0; i < 20; i++)
	{
		// Create some object

		hkpRigidBodyCinfo info;
		info.m_friction = 1.0f;
		info.m_shape = new hkpBoxShape( hkVector4(0.5f, 0.5f, 0.5f), 0.0f );
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, 1.0f, info );
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Position of the box
		info.m_position.setZero4();
		info.m_position(0) = - hkReal(i) * 1.5f + 2.0f; //left
		info.m_position(1) = 1.0f;
		info.m_position(2) = hkMath::sin( hkReal(i) / 3.0f ) * 0.7f;

		if (i == 0)
		{
			info.m_motionType = hkpMotion::MOTION_KEYFRAMED;
			info.m_linearVelocity = hkVector4( -1.0f, 0.0f, 0.0f );
		}

		// Create a box
		hkpRigidBody* box = new hkpRigidBody(info);
		info.m_shape->removeReference();

		// Create a listener and attach it to the box
		// NOTE: The listener attaches itself to the body as an hkpEntityListener 
		// and deletes itself automatically when the entity is deleted.
		ActivationCallbacksDemoEntityActivationListener* listener = new ActivationCallbacksDemoEntityActivationListener();
		box->addEntityActivationListener(listener);
		box->addEntityListener(listener);

		// Insert the box into the world
		m_world->addEntity(box);
		box->removeReference();
		
	}

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo showing the usage of entity activation and deactivation callbacks. " \
"Deactivated bodies are marked red. Reactivated bodies are marked green.";

HK_DECLARE_DEMO(ActivationCallbacksDemo, HK_DEMO_TYPE_PRIME, "Usage of entity activation/deactivation callbacks", helpString);


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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/Modifiers/SurfaceVelocity/Filtered/FilteredSurfaceVelocityDemo.h>

#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

#include <Physics/Utilities/Collide/ContactModifiers/SurfaceVelocity/Filtered/hkpFilteredSurfaceVelocityUtil.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

#include <Common/Visualize/hkDebugDisplay.h>


class FilteredSurfaceVelocityDemo_PhantomListener: public hkReferencedObject, public hkpPhantomOverlapListener
{
public:

	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

	FilteredSurfaceVelocityDemo_PhantomListener(hkpFilteredSurfaceVelocityUtil* su)
	{
		this->m_filteredSurfaceVelocityUtil = su;
		this->m_filteredSurfaceVelocityUtil->addReference();
	}

	~FilteredSurfaceVelocityDemo_PhantomListener()
	{
		this->m_filteredSurfaceVelocityUtil->removeReference();
	}

	//
	// hkpPhantom interface implementation
	//
	virtual void collidableAddedCallback(   const hkpCollidableAddedEvent& event )
	{
		hkpRigidBody* owner = hkGetRigidBody( event.m_collidable );

		// enable surface velocity for the entity that has just entered the phantom
		this->m_filteredSurfaceVelocityUtil->enableEntity(owner);

		// recolor entity to red
		HK_SET_OBJECT_COLOR(hkUlong(event.m_collidable), hkColor::rgbFromChars(255, 0, 0));
	}

	//
	// hkpPhantom interface implementation
	//
	virtual void collidableRemovedCallback( const hkpCollidableRemovedEvent& event )
	{
		hkpRigidBody* owner = hkGetRigidBody(event.m_collidable);

		// disable surface velocity for the entity that has just left the phantom
		this->m_filteredSurfaceVelocityUtil->disableEntity(owner);

		// recolor entity to white
		HK_SET_OBJECT_COLOR(hkUlong(event.m_collidable), hkColor::rgbFromChars(255, 255, 255));
	}

protected:

	hkpFilteredSurfaceVelocityUtil* m_filteredSurfaceVelocityUtil;
};


FilteredSurfaceVelocityDemo::FilteredSurfaceVelocityDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xf0de4356, false); // 'Your m_contactRestingVelocity seems to be too small'

	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 4.0f, 3.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.81f, 0.0f);
		info.setBroadPhaseWorldSize(200.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_HARD);
		info.m_collisionTolerance = 0.05f;
		info.m_contactRestingVelocity = 0.0001f;

		m_world = new hkpWorld(info);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

    
	int propertyIdForEnabledEntities = 100;

	// 
	// Create ground box
	//
	hkpFilteredSurfaceVelocityUtil* filteredSurfaceVelocityUtil;
	{
		hkVector4 boxSize(10.0f, 0.5f, 10.0f);
		hkpBoxShape* baseShape = new hkpBoxShape(boxSize, 0);

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_shape = baseShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position = hkVector4(0.0f, 0.0f, 0.0f);
		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);

		m_world->addEntity(baseRigidBody);
		baseRigidBody->removeReference();
		baseShape->removeReference();

		//
		// add surface velocity
		//
		hkVector4 surfaceVelocity(0, 0, 1.0f);
		filteredSurfaceVelocityUtil = new hkpFilteredSurfaceVelocityUtil( baseRigidBody, surfaceVelocity, propertyIdForEnabledEntities);
	}


	//
	// create two small boxes
	//
	{
		hkpBoxShape* boxShape;
		{
			hkVector4 boxSize(0.2f, 0.2f, 0.2f);
			boxShape = new hkpBoxShape(boxSize, 0);
		}

		hkpRigidBodyCinfo boxInfo;
		{
			boxInfo.m_shape = boxShape;
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
			boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
			boxInfo.m_rotation.setAxisAngle( hkVector4( 1, 0, 0 ), 45.0f * (HK_REAL_PI/180.0f) );
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, 1.0f, boxInfo);
		}

		//
		// left box
		//
		{
			boxInfo.m_position.set(0.0f, 3.0f, 0.0f);

			hkpRigidBody* rigidBody = new hkpRigidBody(boxInfo);

			// we need to add this property to every object that is potentially filtered
			rigidBody->addProperty(propertyIdForEnabledEntities, hkpPropertyValue(0) );

			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
		}

		//
		// right box
		//
		{
			boxInfo.m_position.set(0.0f, 3.0f, -3.0f);

			hkpRigidBody* rigidBody = new hkpRigidBody(boxInfo);
			rigidBody->addProperty(propertyIdForEnabledEntities, hkpPropertyValue(0) );
			m_world->addEntity(rigidBody);
			rigidBody->removeReference();
		}
		boxShape->removeReference();
	}


	//
	// Create a phantom to trigger the enable entity
	//
	{
		hkVector4 boxSize(  1.5f, 0.5f ,1.0f);
		hkVector4 position( 1.0f, 1.1f, 0.0f);

		hkAabb aabb;
		aabb.m_min.setSub4( position, boxSize );
		aabb.m_max.setAdd4( position, boxSize );
		m_phantom = new hkpAabbPhantom(aabb);

		m_listener = new FilteredSurfaceVelocityDemo_PhantomListener(filteredSurfaceVelocityUtil);
		filteredSurfaceVelocityUtil->removeReference();
		m_phantom->addPhantomOverlapListener( m_listener );

		m_world->addPhantom( m_phantom );
	}

	filteredSurfaceVelocityUtil->removeReference();
	m_world->unlock();
}



FilteredSurfaceVelocityDemo::~FilteredSurfaceVelocityDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xf0de4356, true);

	m_world->lock();
	m_phantom->removePhantomOverlapListener( m_listener );
	m_phantom->removeReference();
	delete m_listener;
	m_world->unlock();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Filtered Surface Velocity Demo";

HK_DECLARE_DEMO(FilteredSurfaceVelocityDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Individually enabling/disabling surface velocity.", helpString);

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

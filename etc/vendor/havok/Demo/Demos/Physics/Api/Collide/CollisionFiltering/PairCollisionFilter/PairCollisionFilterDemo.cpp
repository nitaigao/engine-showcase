/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Utilities/Collide/Filter/pair/hkpPairCollisionFilter.h>

#include <Demos/Physics/Api/Collide/CollisionFiltering/PairCollisionFilter/PairCollisionFilterDemo.h>


PairwiseCollisionFilterDemo::PairwiseCollisionFilterDemo( hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from(10.0f, 10.0f, 15.0f);
		hkVector4 to(0.0f, 3.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register all collision agents.
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	//
	//	Create pairwise collision filter.
	//
	hkpPairCollisionFilter* filter;
	{
		filter = new hkpPairCollisionFilter();
	
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	//
	// Create the floor.
	//
	hkpRigidBody* lowerFloor;
	{
		hkVector4 fixedBoxSize(5.0f, .5f , 5.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f, -1.0f, 0.0f);
		}

		lowerFloor = new hkpRigidBody(info);
		m_world->addEntity(lowerFloor);

		lowerFloor->removeReference();
		fixedBoxShape->removeReference();
	}

	//
	// Create a second, lower floor.
	//
	hkpRigidBody* upperFloor;
	{
		hkVector4 fixedBoxSize(3.0f, .5f , 3.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f,4.0f,0.0f);
		}

		upperFloor = new hkpRigidBody(info);
		m_world->addEntity(upperFloor);

		upperFloor->removeReference();
		fixedBoxShape->removeReference();
	}

	//
	// Create a moving box.
	//
	hkpRigidBody* movingBox;
	{
		hkVector4 boxSize( .5f, .5f ,.5f );
		hkpShape* boxShape = new hkpBoxShape( boxSize , 0 );

		hkpRigidBodyCinfo boxInfo;
		{
			boxInfo.m_shape = boxShape;
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			// Compute the box inertia tensor
			hkReal boxMass = 1.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties( boxSize, boxMass, massProperties );
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			boxInfo.m_mass = boxMass;
			boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			boxInfo.m_position.set(0.0f, 7.0f, 0.0f);
		}

		movingBox = new hkpRigidBody( boxInfo );
		m_world->addEntity( movingBox );

		boxShape->removeReference();
		movingBox->removeReference();
	}

	//
	// Disable collisions between moving box and
	//
	filter->disableCollisionsBetween(upperFloor, movingBox);

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = \
	"This demo shows the use of the hkpPairCollisionFilter. Use this type of filter to disable collisions between dedicated pairs of objects.";


HK_DECLARE_DEMO( PairwiseCollisionFilterDemo, HK_DEMO_TYPE_PRIME, "A pairwise collision filter.", helpString);

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

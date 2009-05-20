/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/CollisionFiltering/CollisionFilter/CollisionFilterDemo.h>
//#include <hkdemoframework/hkDemoFramework.h>

// We create two grids of bodies one above the other, 16x16, covering all 256 pairs of possible
// group collisions (actually over half the cases are covered twice).
// When we enable collsions between grousp i and j, the (i,j)th and (j,i)th cube will collide.
// All others will fall though.
// We also have a base which catches the ones wich fall through

CollisionFilterDemo::CollisionFilterDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		this->setupDefaultCameras( env, from, to, up );

		forceShadowState(false);

	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(100.0f);
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	
	//
	//	Create the collision filter
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();

		// disable all collisions by default
		filter->disableCollisionsUsingBitfield( 0xfffffffe, 0xfffffffe );

		// Enable some others. The two arguments are bitfields, so you can set bit 'x' using eg. (1<<x)

		// Enable group 2 against groups 4 and 5
		filter->enableCollisionsUsingBitfield( 1<<2,  (1<<4) | (1<<5));	

		// Enable collision of the base with everything else
		filter->enableCollisionsUsingBitfield( 1U<<31, 0xffffffff );	

		 // Enable group 15 against groups 10,11,12,13
		filter->enableCollisionsUsingBitfield( 1<<15, (1<<10) | (1<<11) | (1<<12) | (1<<13) );  
	
	
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}


	//
	// Create the base
	//
	{
		hkVector4 baseRadii( 16.0f, 0.5f, 16.0f);
		hkpBoxShape* baseShape = new hkpBoxShape(baseRadii, 0 );
		
		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_shape = baseShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position = hkVector4( 0.0f, -2.0f, 0.0f );
		boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(31);
		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);
		m_world->addEntity( baseRigidBody );
		baseRigidBody->removeReference();
		baseShape->removeReference();
	}

	

	//
	// Create shapes needed
	//
	hkVector4 boxRadii( 0.4f, 0.4f, 0.4f);
	hkpBoxShape* cubeShape = new hkpBoxShape(boxRadii, 0 );


	//
	// Create a rigid body construction template
	//

	{	
		//
		// create an array of pairs of boxes: The top box is moveable,
		// and the top two boxes use the collision filter
		for (int x = 0; x < 16; x++ )
		{  
			for (int y = 0; y < 16; y++ )
			{  
								
				// Create the bottom static box, using the hkpGroupFilter collision filter
				{
						hkpRigidBodyCinfo boxInfo;
						boxInfo.m_shape = cubeShape;
						boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
						boxInfo.m_inertiaTensor.setDiagonal(5.0f,5.0f,5.0f);
						boxInfo.m_mass = 5.0f;
						boxInfo.m_position.set( x - 7.5f, 2.5f, y - 7.5f );
						boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( x );
						hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
						m_world->addEntity( boxRigidBody );
						boxRigidBody->removeReference();
				}

				// Create the top moving box, using the hkpGroupFilter collision filter
				{
						hkpRigidBodyCinfo boxInfo;
						boxInfo.m_shape = cubeShape;
						boxInfo.m_position.set( x - 7.5f, 5.0f, y - 7.5f );
						boxInfo.m_mass = 5.0f;
						boxInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
						boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( y );

						hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
						hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
						m_world->addEntity( boxRigidBody );
						boxRigidBody->removeReference();
				}
			}
		}
	}
	cubeShape->removeReference();

	m_world->unlock();
}





#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"We create two grids of bodies one above the other, 16x16, covering all 256 pairs of possible " \
"group collisions (actually over half the cases are covered twice). " \
"When we enable collisions between groups i and j, the (i,j)th and (j,i)th cube will collide. " \
"All others will fall though. We also have a base which catches the ones which fall through. " \
"The filter is set up to:\n" \
"- Enable collision between group 2 against groups 4 and 5\n" \
"- Enable group 15 against groups 10,11,12,13\n" \
"- Enable collision of the base with everything else.\n" \
"- Group 0 is enabled with everything by default."; 

HK_DECLARE_DEMO(CollisionFilterDemo, HK_DEMO_TYPE_PRIME, "Group Collision Filter Demo", helpString);

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

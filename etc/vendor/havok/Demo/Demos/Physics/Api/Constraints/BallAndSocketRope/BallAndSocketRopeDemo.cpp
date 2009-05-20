/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/BallAndSocketRope/BallAndSocketRopeDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>


#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

#include <Common/Visualize/hkDebugDisplay.h>



struct BallAndSocketRopeVariant
{
	const char*	 m_name;
	int m_numBodies;
	const char* m_details;
};

// suggestions for variants: motor tau + dumping,
// with(out) collision
static const BallAndSocketRopeVariant g_variants[] =
{
	{ "Length  10" ,  10, "" },
	{ "Length  20" ,  20, "" },
	{ "Length  50" ,  50, "" },
	{ "Length 100" , 100, "" },
	{ "Length 200" , 200, "" },
};


BallAndSocketRopeDemo::BallAndSocketRopeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	const BallAndSocketRopeVariant& variant = g_variants[env->m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, -10.0f, 6.0f);
		hkVector4 to  (0.0f, 0.0f, 1.0f);
		hkVector4 up  (0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 500.0f );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.set(0.0f, 0.0f, -9.81f);
		m_world = new hkpWorld( info );
		m_world->lock();
		m_world->m_wantDeactivation = false;

		setupGraphics();

		//
		// Create a group filter to avoid intra-collision for the rope
		//
		{
			hkpGroupFilter* filter = new hkpGroupFilter();
			m_world->setCollisionFilter( filter );
			filter->removeReference();
		}

	}

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );


	//
	// Create debris
	//
	
	{
		int numDebris = 40;

		hkPseudoRandomGenerator generator(0xF14);

		hkpRigidBodyCinfo info;
		hkVector4 top; top.set(0.0f, 0.0f, 0.50f);
		hkVector4 bottom; bottom.set(0.f, 0.f, -0.50f);
		info.m_shape = new hkpCapsuleShape( top, bottom, 0.3f );
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, 5.0f, info );

		for (int d = 0; d < numDebris; d++)
		{
			hkReal xPos = generator.getRandRange(-10.0f, 10.0f);
			hkReal yPos = generator.getRandRange(-10.0f, 10.0f);

			hkBool isDynamic = (generator.getRand32() % 4) != 0;
			info.m_position.set( xPos, yPos, isDynamic ? 0.8f : 0.5f );

			info.m_motionType = isDynamic ? hkpMotion::MOTION_DYNAMIC : hkpMotion::MOTION_FIXED;

			hkpRigidBody* debris = new hkpRigidBody(info);
			m_world->addEntity(debris);
			debris->removeReference();
		}
		info.m_shape->removeReference();
	}

	//
	// Create ground box
	//
	{
		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_shape = new hkpBoxShape( hkVector4(100.0f, 100.0f, 0.1f) );
		info.m_position(2) = - 0.1f;

		hkpRigidBody* ground = new hkpRigidBody(info);
		info.m_shape->removeReference();

		m_world->addEntity(ground);
		ground->removeReference();

		HK_SET_OBJECT_COLOR( hkUlong(ground->getCollidable()), 0xff339933);
	}

	//
	// Create fixed peg over the ground
	//
	{
		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_shape = new hkpCapsuleShape( hkVector4(0.0f, 1.0f, 0.0f), hkVector4( 0,-1.0f, 0), 0.3f );
		info.m_position.set(0.0f, 0.0f, 3.0f);

		hkpRigidBody* peg = new hkpRigidBody(info);
		info.m_shape->removeReference();

		m_world->addEntity(peg);
		peg->removeReference();
	}



	//
	// Create chain
	//
	{
		hkReal elemHalfSize = 0.075f;
		hkpShape* sphereShape = new hkpSphereShape(0.3f); 
		hkpShape* capsuleShape = new hkpCapsuleShape( hkVector4(elemHalfSize, 0.0f, 0.0f), hkVector4(-elemHalfSize, 0.0f, 0.0f), 0.03f );

		hkpRigidBodyCinfo info;

		info.m_linearDamping = 0.0f;
		info.m_angularDamping = 0.3f;
		info.m_friction = 0.0f;
		//info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
		info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);


		{
			hkArray<hkpRigidBody*> entities;
		
			for (int b = 0; b < variant.m_numBodies; b++)
			{
				info.m_position.set(elemHalfSize * 2.0f * (b - hkReal(variant.m_numBodies-1) / 2.0f), 0.0f, 4.0f);

				hkReal mass;
				hkReal inertiaMultiplier;
				if (0 == b || variant.m_numBodies-1 == b)
				{
					info.m_shape = sphereShape;
					mass  = 10.0f;
					inertiaMultiplier = 1.0f;
				}
				else
				{
					info.m_shape = capsuleShape;
					mass  = 1.0f;
					inertiaMultiplier = 50.0f;
				}
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, inertiaMultiplier * mass, info);
				info.m_mass = mass;


				{
					hkpRigidBody* body = new hkpRigidBody(info);
					m_world->addEntity(body);
					entities.pushBack(body);
				}
			}
	
			{
				// connect all the bodies
				hkpConstraintChainInstance* chainInstance;
				{
					hkpBallSocketChainData* chainData = new hkpBallSocketChainData();
					chainInstance = new hkpConstraintChainInstance( chainData );

					chainInstance->addEntity( entities[0] );
					for (int e = 1; e < entities.getSize(); e++)
					{
						chainData->addConstraintInfoInBodySpace( hkVector4(elemHalfSize, 0.0f, 0.0f), hkVector4( -elemHalfSize, 0.0f, 0.0f) );
						chainInstance->addEntity( entities[e] );
					}
					chainData->removeReference();
				}

				m_world->addConstraint(chainInstance);
				chainInstance->removeReference();
			}

			for (int i = 0; i < entities.getSize(); i++)
			{
				entities[i]->removeReference();
			}
		}
		sphereShape->removeReference();
		capsuleShape->removeReference();
	}

	m_world->unlock();
}

BallAndSocketRopeDemo::~BallAndSocketRopeDemo()
{
}

hkDemo::Result BallAndSocketRopeDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}




////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This shows the low stretch of the new chain constraints";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BallAndSocketRopeDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, BallAndSocketRopeVariant, g_variants, helpString ); 

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

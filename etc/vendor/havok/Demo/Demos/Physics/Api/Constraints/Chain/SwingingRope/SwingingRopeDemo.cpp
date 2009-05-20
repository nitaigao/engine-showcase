/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Chain/SwingingRope/SwingingRopeDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintData.h>

// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>

#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>
#include <Physics/Dynamics/Constraint/Chain/StiffSpring/hkpStiffSpringChainData.h>
#include <Physics/Dynamics/Constraint/Chain/Powered/hkpPoweredChainData.h>

#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyCentreOfMassViewer.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>

namespace
{
	enum ChainType
	{
		BALL_AND_SOCKET,
		STIFF_SPRING
	};
}

struct SwingingRopeVariant
{
	const char*	 m_name;
	ChainType m_type;
	hkInt32 m_numBodies;
	hkReal m_tau;
	hkReal m_gravity;
	const char* m_details;
};


static const SwingingRopeVariant g_variants[] =
{
	{ "Ball and Socket chain. Length==3",                  BALL_AND_SOCKET,   3, 0.6f, 9.81f,	"" },
	{ "Ball and Socket chain. Length==20",                 BALL_AND_SOCKET,  20, 0.6f, 9.81f,	"" },
	{ "Ball and Socket chain. Length==50",                 BALL_AND_SOCKET,  50, 0.6f, 9.81f,	"" },
	{ "Ball and Socket chain. Length==100",                BALL_AND_SOCKET, 100, 0.6f, 9.81f,	"" },
	{ "Ball and Socket chain. Length==100, triple gravity",BALL_AND_SOCKET, 100, 0.6f, 30.0f,	"" },
	{ "Ball and Socket chain. Length==200, tau==0.60",     BALL_AND_SOCKET, 200, 0.6f, 9.81f,	"" },
	{ "Ball and Socket chain. Length==200, tau==0.02",     BALL_AND_SOCKET, 200, 0.02f, 9.81f,	"" },
	{ "Stiff spring chain.    Length==100, tau==0.60",     STIFF_SPRING,    100, 0.6f, 9.81f,	"" },
	{ "Stiff spring chain.    Length==100, tau==0.10",     STIFF_SPRING,    100, 0.1f, 9.81f,	"" }
};


SwingingRopeDemo::SwingingRopeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	// Disable warnings:
	hkError::getInstance().setEnabled(0x3ad17e8a, false); //'Have you called hkpAgentRegisterUtil::registerAllAgents? Do not know how to get closest points between HK_SHAPE_BOX and HK_SHAPE_BOX types.'
	hkError::getInstance().setEnabled(0x3ad17e8b, false);
		
	const SwingingRopeVariant& variant = g_variants[env->m_variantId];

		// Horizontal distance between the attachment points of the strings.
	const hkReal offset = 50.0f;

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, -175.0f, -70.0f);
		hkVector4 to  (0.0f, 0.0f, -70.0f);
		hkVector4 up  (0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 5.0f, 10000.0f );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100000.0f );
		info.m_gravity.set(0.0f, 0.0f, -variant.m_gravity);
		info.m_solverTau = variant.m_tau;
		m_world = new hkpWorld( info );
		m_world->lock();

		m_world->m_wantDeactivation = false;

		setupGraphics();
	}


	{
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( hkVector4(0.5, 0.5f, 0.5f), 0.01f );
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, 10.0f, info);
		info.m_mass = 1.0f;
		info.m_linearVelocity(2) = -10.0f;


		//
		// Construct string of independent bilateral constraints
		//
		{
			hkpConstraintData* data;
			{
				hkpBallAndSocketConstraintData* bsData = new hkpBallAndSocketConstraintData();
				bsData->setInBodySpace(hkVector4::getZero(), hkVector4( -1.5f, 0.0f, -0.3f));
				data = bsData;
			}

			hkpRigidBody* prevBody = HK_NULL;
			for (int b = 0; b < variant.m_numBodies; b++)
			{
				info.m_position.set(1.5f * hkReal(b) - offset / 2.0f, 0.0f, 0.3f * hkReal(b));
				info.m_motionType = b ? hkpMotion::MOTION_DYNAMIC : hkpMotion::MOTION_FIXED;

				hkpRigidBody* body = new hkpRigidBody(info);
				m_world->addEntity(body);
				HK_SET_OBJECT_COLOR( hkUlong(body->getCollidable()), 0xffff0000 );

				if (prevBody)
				{
					// add constraint
					hkpConstraintInstance* instance = new hkpConstraintInstance(prevBody, body, data);
					m_world->addConstraint( instance );
					instance->removeReference();
				}

				prevBody = body;
				// we remove reference, but we know one is still kept by m_world
				body->removeReference();
			}
			data->removeReference();
		}


		//
		// Construct constraint chain
		//
		{
			hkArray<hkpEntity*> entities;

			for (int b = 0; b < variant.m_numBodies; b++)
			{
				info.m_position.set(1.5f * hkReal(b) + offset / 2.0f, 2.0f, 0.3f * hkReal(b));
				info.m_motionType = b ? hkpMotion::MOTION_DYNAMIC : hkpMotion::MOTION_FIXED;

				hkpRigidBody* body = new hkpRigidBody(info);
				m_world->addEntity(body);
				HK_SET_OBJECT_COLOR( hkUlong(body->getCollidable()), 0xff00ff00 );

				entities.pushBack(body);
				// we know, a reference is kept by the world
				body->removeReference();
			}
	
			{
				hkpConstraintChainInstance* chainInstance = HK_NULL;

				if (variant.m_type == BALL_AND_SOCKET)
				{
					hkpBallSocketChainData* chainData = new hkpBallSocketChainData();
					chainInstance = new hkpConstraintChainInstance( chainData );

					chainInstance->addEntity( entities[0] );
					for (int e = 1; e < entities.getSize(); e++)
					{
						chainData->addConstraintInfoInBodySpace( hkVector4::getZero(), hkVector4( -1.5f, 0.0f, -0.3f) );
						chainInstance->addEntity( entities[e] );
					}

					chainData->m_tau = variant.m_tau;
					chainData->removeReference();
				}
				else if(variant.m_type == STIFF_SPRING)
				{
					hkpStiffSpringChainData* chainData = new hkpStiffSpringChainData();
					chainInstance = new hkpConstraintChainInstance( chainData );

					chainInstance->addEntity( entities[0] );
					for (int e = 1; e < entities.getSize(); e++)
					{
						chainData->addConstraintInfoInBodySpace( hkVector4::getZero(), hkVector4( -1.0f, 0.0f, -0.2f), 0.55f );
						chainInstance->addEntity( entities[e] );
					}

					chainData->m_tau = variant.m_tau;
					chainData->removeReference();
				}

				m_world->addConstraint( chainInstance );
				chainInstance->removeReference();
			}

		}

		info.m_shape->removeReference();

	}

	m_world->unlock();
}

SwingingRopeDemo::~SwingingRopeDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0x3ad17e8a, true); 
	hkError::getInstance().setEnabled(0x3ad17e8b, true);
}

hkDemo::Result SwingingRopeDemo::stepDemo()
{
	const hkReal offset = 80.0f;
	m_env->m_textDisplay->outputText3D("Normal iterative constraints", -offset/2.0f - 40.0f, 0.0f, -10.0f);
	m_env->m_textDisplay->outputText3D("Constraint chain", +offset/2.0f, 0.0f, -10.0f);

	return hkDefaultPhysicsDemo::stepDemo();
}

////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates the difference in behavior of a system of bodies specified with independent constraints "\
"(opaque colored cubes) and one specified with a constraint chain (solid cubes)." \
"Note the loss of stability of the system of independent constraints for chains of the length of 20.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SwingingRopeDemo, HK_DEMO_TYPE_PRIME, SwingingRopeVariant, g_variants, helpString );

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

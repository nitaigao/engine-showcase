/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Chain/ChainCfm/ChainCfmDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>

#include <Physics/Dynamics/Constraint/Chain/Powered/hkpPoweredChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>


#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>
#include <Physics/Dynamics/Constraint/Motor/Velocity/hkpVelocityConstraintMotor.h>

#include <Physics/Dynamics/Constraint/Bilateral/Prismatic/hkpPrismaticConstraintData.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyCentreOfMassViewer.h>

#include <Common/Visualize/hkDebugDisplay.h>





//////////////////////////////////////////////////////////////////////////
//
// Demo
//
//////////////////////////////////////////////////////////////////////////



ChainCfmDemo::ChainCfmDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	m_cfmAdditive = 0.0f;
	m_cfmMultiplicative = 0.0f;

	m_selectedController = 0;


	const int numChains = 2;
	const int numBodiesInChain = 7;

	//
	// Setup the camera
	//
	{
		const hkReal length = (numBodiesInChain+1) * numChains;
		hkVector4 from(length / 2.0f, -length - 1.0f, 0.0f);
		hkVector4 to  (length / 2.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 5.0f, 10000.0f );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.set(0.0f, 0.0f, -9.81f);
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		m_world->m_wantDeactivation = false;

		setupGraphics();

	}

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	hkpRigidBody* fixedBody = HK_NULL;

	//
	// Crate a series of chains 
	//
	{
		// Create a motor to be shared by all the powered chains
		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor();
		{
			motor->m_tau = 0.9f;
			motor->m_damping = 1.0f;

			motor->m_maxForce = HK_REAL_MAX;
			motor->m_constantRecoveryVelocity = 0.5f;
			motor->m_proportionalRecoveryVelocity = 0.5f;
		}

		// Create a rigid body cinfo template
		hkpRigidBodyCinfo info;
		{
			info.m_shape = new hkpBoxShape( hkVector4(.3f, .3f, .3f) );
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, 1.0f, info);
		}

		// Create a series of chains
		for (int i = 0; i < numChains; i++)
		{
			// Create constraint chain data + instance
			hkpPoweredChainData* data = new hkpPoweredChainData();
			hkpConstraintChainInstance* instance = new hkpConstraintChainInstance(data);

			// Adjust the cfm parameters for the demo
			switch(i)
			{
				case 0: data->m_cfmAngAdd = 1.5f; break;
				case 1: data->m_cfmAngMul = 2.0f; break;
			}

			// Build a series of bodies
			for (int b = 0; b < numBodiesInChain; b++)
			{
				info.m_motionType = b ? hkpMotion::MOTION_DYNAMIC : hkpMotion::MOTION_FIXED;
				info.m_position.set( hkReal(b) + hkReal(i*(numBodiesInChain+1)), 0.0f, 0.0f );
				hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, hkReal(1<<(numBodiesInChain-b) ), info);
				//hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, hkReal(numBodiesInChain - b) * hkReal(numBodiesInChain - b) * 10.0f, info);
				//hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, /*hkReal(1+b) * hkReal(1+b)*/hkReal() * 10.0f, info);

				hkpRigidBody* body = new hkpRigidBody(info);
				instance->addEntity(body);
				m_world->addEntity(body);
				body->removeReference();

				if (!fixedBody)
				{
					fixedBody = body; 
				}

				// Add a simple constraint info into the chain data
				if (b)
				{
					data->addConstraintInfoInBodySpace( hkVector4::getZero(), hkVector4( -1.0f, 0.0f, 0.0f ), hkQuaternion::getIdentity(), motor, motor, motor);
				}
			}

			// Add constraint chain to the world, and remove references
			m_world->addConstraint( instance );
			instance->removeReference();
			data->removeReference();

		}

		// Remove references from the shared motor and template shape
		info.m_shape->removeReference();
		motor->removeReference();
	}

	// Create two heavy objects to and place them at the end of the chains. 
	for (int i = 0; i < numChains; i++)
	{
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpSphereShape(0.4f);
		info.m_position.set( hkReal(numBodiesInChain-1)  + hkReal(i*(numBodiesInChain+1)), 0.0f, 1.0f);
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, 10.0f, info );

		hkpRigidBody* body = new hkpRigidBody(info);
		m_world->addEntity(body);
		info.m_shape->removeReference();

		// Also constraint them in the world
		hkpPrismaticConstraintData* data = new hkpPrismaticConstraintData();
		const hkVector4& axis = hkTransform::getIdentity().getColumn(2);
		//const hkVector4& perp = hkTransform::getIdentity().getColumn(1);
		hkVector4 pivot; pivot.setSub4(body->getPosition(), fixedBody->getPosition());
		data->setMaxLinearLimit(100.0f);
		data->setMinLinearLimit(-100.0f);
		//data->setInBodySpace( pivot, hkVector4::getZero(), axis, axis, perp, perp );
		data->setInWorldSpace(body->getTransform(), fixedBody->getTransform(), body->getPosition(), axis);


		hkpConstraintInstance* instance = new hkpConstraintInstance(body, fixedBody, data);
		m_world->addConstraint(instance);
		instance->removeReference();
		data->removeReference();

		body->removeReference();
	}

	m_world->unlock();
}


hkDemo::Result ChainCfmDemo::stepDemo()
{
	m_world->lock();

	const int numBodiesInChain = 7;

	m_env->m_textDisplay->outputText3D("Additive CFM", 0.0f, 0.0f, 2.0f);
	m_env->m_textDisplay->outputText3D("Multiplicative CFM", hkReal(numBodiesInChain+1), 0.0f, 2.0f);

	hkpPhysicsSystem* system = m_world->getWorldAsOneSystem();

	for (int e = 0; e < system->getRigidBodies().getSize(); e++)
	{
		hkpRigidBody* body = system->getRigidBodies()[e];

		if (!body->isFixed())
		{
			hkVector4 textPos; textPos.setAdd4( hkVector4(0.0f, 0.0f, 0.5f), body->getPosition());
			hkString str; str.printf("%.1f", body->getMass());
			m_env->m_textDisplay->outputText3D(str, textPos(0), textPos(1), textPos(2));
		}
	}

	system->removeReference();

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo demonstrates effect of using additive and multiplicative cfm parameters for constraint chains linking bodies of varying masses. " \
"Each body has its mass displayed above it. ";

HK_DECLARE_DEMO(ChainCfmDemo, HK_DEMO_TYPE_PRIME, "Cfm parameters in constraint chains.", helpString);

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

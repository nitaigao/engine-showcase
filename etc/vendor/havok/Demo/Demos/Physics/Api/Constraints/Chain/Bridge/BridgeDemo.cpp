/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Chain/Bridge/BridgeDemo.h>

#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>



BridgeDemo::BridgeDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env)
{
	
	// Disable warnings:
	hkError::getInstance().setEnabled(0x3ad17e8a, false); //'Have you called hkpAgentRegisterUtil::registerAllAgents? Do not know how to get closest points between HK_SHAPE_BOX and HK_SHAPE_BOX types.'

	//
	// Setup the camera
	//
	{
		hkVector4 from(  -2.0f,-2.0f,  1.0f);
		hkVector4 to  (  50.0f,  10.0f,  0.0f);
		hkVector4 up  (  0.0f,  0.0f,  1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize(10000.0f);
		info.m_gravity.set(0.0f, 0.0f, -9.81f);
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}


	//
	// Create bridge slabs. 
	// Note that we artificially increase the inertia tensor of each slab by a factor of 10 to increase stability.
	//
	hkArray<hkpRigidBody*> bodies;
	const int bridgeLength = 200; 
	const hkVector4 slabSize(0.5f, 1.0f, 0.1f);
	{
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( slabSize );
		info.m_position.setZero4();
		const hkReal inertiaToMassRatio = 10.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, inertiaToMassRatio, info);
		info.m_mass = 1.0f;

		for (int i = 0; i < bridgeLength; i++)
		{
			if (i == 0 || i == bridgeLength-1)
			{
				info.m_motionType = hkpMotion::MOTION_FIXED;
				info.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			}
			else
			{
				info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
				info.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
			}
			
			info.m_position(2) += i%2 ? 0.0f : 0.01f;
			info.m_position(0) += slabSize(0) * 2.2f;
			hkpRigidBody* body = new hkpRigidBody(info);
			bodies.pushBack(body);
			m_world->addEntity(body);
			body->removeReference();
		}

		info.m_shape->removeReference();
	}

	//
	// Create two constraint chains 
	//
	{
		hkpBallSocketChainData* chainData0 = new hkpBallSocketChainData();
		hkpBallSocketChainData* chainData1 = new hkpBallSocketChainData();

		// The chain's subsolver uses its own tau/damping settings separate from the main Havok solver.
		// Setting tau to a lower value allows the system to remain stable even with higher forces exerted at 
		// individual links. Decreasing tau works well for relatively slow moving setups, i.e. it will work ok
		// for a slowly moving bridge, but won't work convincingly for a quick slash of a whip.
		chainData0->m_tau = 0.1f;
		chainData1->m_tau = 0.1f;

		hkVector4 tmp = slabSize;
		tmp(0) *= 1.101f;
		tmp(2) = 0.0f;
		hkVector4 a0 = tmp;
		hkVector4 b0 = tmp; b0(0) = - b0(0);
		hkVector4 a1 = tmp; a1(1) = - a1(1);
		hkVector4 b1 = tmp; b1(1) = - b1(1); b1(0) = - b1(0);


		// Populate constraint info arrays in chain datas.
		{
			for (int i = 0; i < bodies.getSize() - 1; i++)
			{
				chainData0->addConstraintInfoInBodySpace( a0, b0 );
				chainData1->addConstraintInfoInBodySpace( a1, b1 );
			}
		}

		// instances
		hkpConstraintChainInstance* chain0 = new hkpConstraintChainInstance(chainData0);
		hkpConstraintChainInstance* chain1 = new hkpConstraintChainInstance(chainData1);

		// Populate entity lists in chain instances.
		{
			for (int i = 0; i < bodies.getSize(); i++)
			{
				chain0->addEntity(bodies[i]);
				chain1->addEntity(bodies[i]);
			}
		}

		m_world->addConstraint( chain0 );
		m_world->addConstraint( chain1 );
		chain0->removeReference();
		chain1->removeReference();
		chainData0->removeReference();
		chainData1->removeReference();
	}
	
	m_world->unlock();
}

BridgeDemo::~BridgeDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0x3ad17e8a, true); 
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo, which creates a bridge build of 200 dynamic slabs connected by two chains of ball-and-socket constraints. "
"Essentially this yields a hinge constraint between each pair of slabs. Note very low streaching, and realistic "
"wave propagation behavior of the bridge when its forcefully dragged with mouse spring.";

HK_DECLARE_DEMO(BridgeDemo, HK_DEMO_TYPE_PRIME, "Constraint Chain Bridge", helpString);

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

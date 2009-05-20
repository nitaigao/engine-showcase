/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Internal/Dynamics/Constraints/hkpConstraintProjector.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Dynamics/World/Util/hkpWorldCallbackUtil.h>
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>

#include <Demos/Physics/Api/Dynamics/RigidBodies/Projection/Projections.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

ProjectionsDemo::ProjectionsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_doProjection(false)
{
	{
		hkpWorldCinfo worldinfo;
		worldinfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_SOFT);
		worldinfo.m_gravity.set( 0, 0, -9.81f );		
		worldinfo.m_enableDeactivation	=	false;
		worldinfo.m_solverTau			=	0.05f;	/* Decrease Tau on purpose	*/ 
		worldinfo.setBroadPhaseWorldSize( 200.0f );
		m_world = new hkpWorld( worldinfo );
		m_world->lock();
		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Visualize constraints
	//
	//
	// Create a group filter
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( filter );
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}
	
	//
	//	Create a ground box
	//
	{		

		hkVector4 size( 25, 25, 1 );

		hkpRigidBody* rb = GameUtils::createBox( size, 0, hkVector4::getZero() );
		rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_STATIC ) );

		m_world->addEntity( rb );
		rb->removeReference();
	}
	
	//
	// Create rag dolls
	//
	{
		m_ragdollType = GameUtils::RPT_COUNT-1;
		for(int i=0,ni=11;i<ni;i++)
		{
			static const hkReal	spacing=2;
			static const hkReal	height=2;
			hkQuaternion		rotation=hkQuaternion::getIdentity();
			hkVector4			position=hkVector4::getZero();
			position(0)	=	(hkReal)(-(ni/2)*spacing+i*spacing);
			position(2)	=	5;
			hkpPhysicsSystem*	ragdoll=GameUtils::createRagdoll(		height,
																		position,
																		rotation,
																		i+1,
																		(GameUtils::RagdollPartType)m_ragdollType);
			hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree(	ragdoll->getConstraints().begin(),
																		ragdoll->getConstraints().getSize(),
																		ragdoll->getRigidBodies()[0]);
			const hkArray<hkpRigidBody*>& rigidbodies = ragdoll->getRigidBodies();
			for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
			{
				hkpRigidBody* body = rigidbodies[iRB];
				body->setLinearDamping( 0.1f );
				body->getMaterial().setFriction(0.31f);				
			}
			m_world->addPhysicsSystem(ragdoll);
			m_ragdolls.pushBack( ragdoll );
			/* Attach to world	*/ 
			hkpRigidBody*					body=rigidbodies[i];
			hkpBallAndSocketConstraintData*	cd=new hkpBallAndSocketConstraintData();
			hkVector4						pivot=body->getPosition();
			cd->setInWorldSpace(body->getTransform(),hkTransform::getIdentity(),pivot);
			hkpConstraintInstance*			ci=new hkpConstraintInstance(body,HK_NULL,cd);
			m_world->addConstraint(ci);
			ci->removeReference();
		}
	}
	//
	// Setup the camera
	//
	{
		hkVector4 from(   0.0f, -20.0f, 5.0f );
		hkVector4 to  (   0.0f,   0.0f, 5.0f );
		hkVector4 up  (   0.0f,   0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up );
		
		float ldir[] = { 0, 0.5f, -1.0f };
		setSoleDirectionLight(m_env, ldir, 0xffffffff );

		setupGraphics();
	}

	m_world->unlock();
}

ProjectionsDemo::~ProjectionsDemo()
{
	m_world->lock();

	for( int i = 0; i < m_ragdolls.getSize(); i++ )
	{
		m_ragdolls[i]->removeReference();
	}

	m_world->unlock();
}

hkDemo::Result ProjectionsDemo::stepDemo()
{
	if(	m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2)||
		m_env->m_window->getKeyboard().wasKeyPressed('P'))
	{
		m_doProjection=!m_doProjection;
	}
	
	
	hkDemo::Result	result=hkDefaultPhysicsDemo::stepDemo();
	const char*		projectionText="Projection [OFF]";
	if(m_doProjection)
	{
		projectionText="Projection [ON]";
		m_world->lock();
		/* Take all constraints					*/ 
		hkpPhysicsSystem*						system(m_world->getWorldAsOneSystem());
		const hkArray<hkpConstraintInstance*>&	constraints(system->getConstraints());
		/* And project 							*/ 
		hkpConstraintProjector					cp(hkpConstraintProjector::HK_HIERARCHY);
		cp.project(constraints);		
		/* Update display transforms			*/ 
		hkpWorldCallbackUtil::firePostSimulationCallback(m_world);
		m_world->unlock();
	}
	
	m_env->m_textDisplay->outputText(projectionText,8,(int)m_env->m_window->getHeight()-24);

	return result;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =	"Using constraint projections to fix large errors in articulated bodies.\r\n"
									"Press 'P' to switch projection on/off\r\n"
									"Note: The solver has been configured to make projection more apparent."
									;

HK_DECLARE_DEMO(ProjectionsDemo, HK_DEMO_TYPE_PHYSICS, helpString, helpString );

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

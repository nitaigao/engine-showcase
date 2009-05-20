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

#include <Demos/Physics/Test/Performance/BroadphaseAddRemove/BroadphaseAddRemove.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

//#define SELF_BENCHMARK

#if defined(SELF_BENCHMARK)
#include <stdlib.h>
#endif

BroadphaseAddRemoveDemo::BroadphaseAddRemoveDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_timer(0)
{
	
	m_useBatch	=	true;
	m_draw		=	true;
	m_rate		=	0.1f;
	m_count		=	64;
	m_maxbodies	=	10000;
	m_removeat	=	0;
	m_rndgen	=	new hkPseudoRandomGenerator(180673);
	//
	// Create world
	//
	{
		hkpWorldCinfo worldinfo;
		worldinfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_SOFT);
		//worldinfo.m_gravity.set( 0, 0, -9.81f );		
		worldinfo.m_gravity.set( 0, 0, 0 );		
		worldinfo.m_enableDeactivation	=	false;
		worldinfo.m_autoUpdateKdTree = false;
		worldinfo.setBroadPhaseWorldSize( 200.0f );
		m_world = new hkpWorld( worldinfo );
		m_world->lock();
		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}	
	
	//
	// Create shape
	//

	m_shape=new hkpSphereShape(0.1f);	

	//
	//	Create the collision filter
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();

		// disable all collisions by default
		filter->disableCollisionsUsingBitfield( 0xfffffffe, 0xfffffffe );
		
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}
	
	//
	// Setup the camera
	//
	{
		hkVector4 from(   0.0f, -20.0f, 0.0f );
		hkVector4 to  (   0.0f,   0.0f, 0.0f );
		hkVector4 up  (   0.0f,   0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up );
		
		float ldir[] = { 0, 0.5f, -1.0f };
		setSoleDirectionLight(m_env, ldir, 0xffffffff );

		setupGraphics();
	}

	m_world->unlock();
}

BroadphaseAddRemoveDemo::~BroadphaseAddRemoveDemo()
{
	m_world->lock();
	
	m_world->unlock();
}

void			BroadphaseAddRemoveDemo::spawnFirework()
{
	hkpRigidBodyCinfo	infos;
	infos.m_shape				=	m_shape;
	infos.m_motionType			=	hkpMotion::MOTION_SPHERE_INERTIA;		
	//infos.m_collisionFilterInfo	=	hkpGroupFilter::calcFilterInfo(1);
	hkVector4			position;
	m_rndgen->getRandomVector11(position);
	position.mul4(5);
	m_bodies.reserve(m_bodies.getSize()+m_count);
	hkArray<hkpEntity*>	batch;
	batch.reserve(m_count);
	//
	for(int i=0;i<m_count;++i)
	{
		hkVector4			direction;
		m_rndgen->getRandomVector11(direction);
		infos.m_position=position;
		infos.m_position.addMul4(1.0f, direction);

		hkpRigidBody*		body=new hkpRigidBody(infos);
		body->setLinearDamping(10.0f);


		if(!m_draw) body->addProperty( HK_PROPERTY_DISPLAY_SHAPE, HK_NULL );
		if(m_useBatch)
			batch.pushBack(body);
		else
			m_world->addEntity(body);

		direction.mul4(10);
		body->applyLinearImpulse(direction);
		m_bodies.pushBack(body);
	}
	if(batch.getSize()) m_world->addEntityBatch(&batch[0],batch.getSize());
}

void			BroadphaseAddRemoveDemo::flushFirework()
{
	m_batch.clear();
	while(m_bodies.getSize()>m_maxbodies)
	{
		m_removeat=m_removeat%m_bodies.getSize();
		if(m_useBatch)
			m_batch.pushBack(m_bodies[m_removeat]);
		else
			m_world->removeEntity(m_bodies[m_removeat]);
		m_bodies[m_removeat]->removeReference();
		m_bodies.removeAt(m_removeat++);
	}
	if(m_batch.getSize()) m_world->removeEntityBatch(&m_batch[0],m_batch.getSize());
}

hkDemo::Result BroadphaseAddRemoveDemo::stepDemo()
{	
	#if defined(SELF_BENCHMARK)
	LARGE_INTEGER	counter0;
	printf("Benchmarking...\r\n");
	::QueryPerformanceCounter(&counter0);	
	for(int i=0;i<512;++i)
	{
		spawnFirework();
	}
	LARGE_INTEGER	counter1;
	LARGE_INTEGER	frequency;
	::QueryPerformanceCounter(&counter1);
	::QueryPerformanceFrequency(&frequency);
	printf("Time: %.I64d ms\r\n",((counter1.QuadPart-counter0.QuadPart)*1000)/frequency.QuadPart);
	exit(1);
	#endif
	
	if(m_env->m_window->getKeyboard().wasKeyPressed('B'))
	{
		m_useBatch=!m_useBatch;
	}
	if(m_env->m_window->getKeyboard().wasKeyPressed('D'))
	{
		m_draw=!m_draw;
	}
	
	if(m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2)) m_useBatch=!m_useBatch;
	if(m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3)) m_draw=!m_draw;

	m_world->lock();
	m_timer-=m_world->getSolverInfo()->m_deltaTime;
	if(m_timer<0)
	{
		HK_TIMER_BEGIN_LIST( "DemoAddBodies", "Add" );
		spawnFirework();
		if(m_bodies.getSize()>=m_maxbodies) m_timer+=m_rate;
		HK_TIMER_SPLIT_LIST( "Remove" );		
		flushFirework();
		HK_TIMER_END_LIST();
	}
	m_world->unlock();
	char		text[256];
	hkString::sprintf(text,"%d bodies [%d bodies per second]\r\n[B]atchs: %s\r\n[D]raw: %s",m_bodies.getSize(),(int)(m_count/m_rate),
																					m_useBatch?"ON":"OFF",
																					m_draw?"ON":"OFF");
	m_env->m_textDisplay->outputText(text,8,(int)m_env->m_window->getHeight()-96);
	
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =	"Broadphase.\r\n"
									;

HK_DECLARE_DEMO(BroadphaseAddRemoveDemo, HK_DEMO_TYPE_PHYSICS, helpString, helpString );

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

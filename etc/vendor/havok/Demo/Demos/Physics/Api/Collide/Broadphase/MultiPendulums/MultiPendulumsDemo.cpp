/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Broadphase/MultiPendulums/MultiPendulumsDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

	// Constraints
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>

	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Demos/DemoCommon/Utilities/MarbleAction/MarbleAction.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>


void MultiPendulumsDemo::createMultiPendulumField(  )
{ 
#	if defined(HK_DEBUG)
	const int shift = 1;
#else
	const int shift = 0;
#endif
	const int numBodies = hkMath::clamp( m_env->m_cpuMhz / 2, 100, 4000) >> shift;
	int size = (int)hkMath::sqrt( float(numBodies) );
	
	int row = -size/2;
	int column = -size/2;


	hkVector4 boxSize( .2f,.2f,.2f );
	hkpShape *boxShape = new hkpBoxShape( boxSize , 0 );

	hkpRigidBody *fixedBody = m_world->getFixedRigidBody();

	hkPseudoRandomGenerator generator(324);

	for(int i=1; i< numBodies; i++)
	{
		
		if ( i % size == 0)
		{
			row = -(size>>1);
			column++;
		}
		else
		{
			row++;
		}

		hkVector4 boxPos;
		boxPos.set( hkReal(row),1.0f,hkReal(column) );


		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = boxShape;
		boxInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		boxInfo.m_position = boxPos;
		boxInfo.m_mass = 1.0f;
		boxInfo.m_angularDamping = 1.0f;
				
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);

		hkpRigidBody* body = new hkpRigidBody(boxInfo);

		hkVector4 velocity(0.0f,0.0f, generator.getRandRange( 0.0f, 2.0f ) );
		body->setLinearVelocity( velocity );
		m_world->addEntity( body );
		body->removeReference(); 

		body->getMaterial().setFriction(0.0f);

		hkVector4 pivot, pivotOffset;
		pivotOffset.set( 0.0f, 1.0f, 0.0f );
		pivot.setAdd4( boxPos, pivotOffset );

		hkpBallAndSocketConstraintData* bas = new hkpBallAndSocketConstraintData();
		bas->setInWorldSpace( fixedBody->getTransform(), body->getTransform(), pivot );		  

		m_world->createAndAddConstraintInstance( fixedBody, body, bas )->removeReference();
		bas->removeReference();
	}

	boxShape->removeReference();

}


void MultiPendulumsDemo::createWorld()
{

	//
	// Create the base
	//
	// 
	// Create the base
	//
	{
		const float xdim = 25.0f;
		const float zdim = 25.0f;
		const float bW = 0.5f;	// borderWidth
		const float bH = 0.5f;	// borderHeight

		GameUtils::createStaticBox( m_world, 0.0f,     -0.5f,0.0f,		xdim,1.0f,zdim );

		GameUtils::createStaticBox( m_world, -xdim,  bH,0.0f,		   bW,bH,zdim );
		GameUtils::createStaticBox( m_world,  xdim,  bH,0.0f,		   bW,bH,zdim );
		GameUtils::createStaticBox( m_world,     0.0f,  bH, zdim,	xdim,bH,   bW );
		GameUtils::createStaticBox( m_world,     0.0f,  bH,-zdim,	xdim,bH,   bW );
	}


	createMultiPendulumField(  );

	{
		//
		//	Create the moving block 
		//
		hkVector4 characterPos(0.0f,3.0f,5.0f);
		hkVector4 bSize( .7f, .7f, .7f );
		hkpRigidBody* characterRigidBody = GameUtils::createBox(bSize, 300.0f, characterPos);
		m_world->addEntity( characterRigidBody );
		characterRigidBody->removeReference();	// Remove reference, since we no longer want to remember this

		//
		// Create the "marble" action to drive him around
		//
		hkVector4 forward(1.0f,0.0f,0.0f);
		m_marbleaction = new MarbleAction(characterRigidBody, forward, characterRigidBody->getPosition(),0.5f);
		m_world->addAction(m_marbleaction);
	}
}


MultiPendulumsDemo::MultiPendulumsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	// Disable warnings:									
	hkError::getInstance().setEnabled(0xaf55adde, false); //'No runtime block of size 637136 currently available. Allocating new block from unmanaged memory.'


	//
	// Setup the camera
	//
	{
		hkVector4 from(-15.0f, 14.0f, 24.0f);
		hkVector4 to  (  4.0f,  0.0f,  4.0f);
		hkVector4 up  (  0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );

		forceShadowState(false);

	}

	{
		hkpWorldCinfo info;

		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_gravity.set( 0.0f, -9.81f, 0.0f );

		info.m_broadPhaseWorldAabb.m_min.set( -100.0f, -100.0f, -100.0f );
		info.m_broadPhaseWorldAabb.m_max.setNeg4( info.m_broadPhaseWorldAabb.m_min );
		//info.m_minDesiredIslandSize = 0;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	createWorld();

	m_world->unlock();
}

hkDemo::Result MultiPendulumsDemo::stepDemo()
{
	const hkgPad* pad = m_env->m_gamePad;
	m_world->lock();

	// Check if action is still in the world : It should always be, but it's better to check here to make sure
	if(m_marbleaction->getWorld() != HK_NULL)
	{
		if ( pad->wasButtonPressed(0xffffffff) )
		{
			m_marbleaction->getEntity()->activate();
		}
			
		m_marbleaction->setLeftPressed((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0);
		m_marbleaction->setRightPressed((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0);
		m_marbleaction->setForwardPressed((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0);
		m_marbleaction->setBackwardPressed((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0);

		m_marbleaction->setJumpPressed(pad->wasButtonPressed(HKG_PAD_BUTTON_0));
		m_marbleaction->setBrakePressed((pad->getButtonState() & HKG_PAD_BUTTON_1) != 0);	
		if (pad->wasButtonPressed(HKG_PAD_BUTTON_3))
		{
			m_marbleaction->reset();
		}
		if (pad->wasButtonPressed(HKG_PAD_BUTTON_2))
		{
			m_marbleaction->getEntity()->deactivate();
		}
	}
	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

MultiPendulumsDemo::~MultiPendulumsDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xaf55adde, true);

	m_marbleaction->removeReference();	// Remove reference, since we no longer want to remember this
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This is an excellent broadphase stress test. As the pendulums swing up and down they traverse" \
"an entire axis in our 3Axis sweep and prune implementation. " \
"Use \224 \225 \226 \227 to control the moving block.";

HK_DECLARE_DEMO(MultiPendulumsDemo, HK_DEMO_TYPE_PRIME, "Pendulums", helpString);

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

	// constraints
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>


	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>


	// Here's our funky action
#include <Demos/DemoCommon/Utilities/MarbleAction/MarbleAction.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Demos/Physics/UseCase/RopeBridge/RopeBridgeDemo.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>

#define BRIDGE_LAYER 2


RopeBridgeDemo::~RopeBridgeDemo()
{
	m_marbleaction->removeReference();	// Remove reference, since we no longer want to remember this
}


void RopeBridgeDemo::createObstacleCourse()
{
  
	hkpRigidBody* rb = HK_NULL;

	// the floor
	hkVector4 baseSize( 20.0f, 0.5f, 30.0f);
	hkVector4 basePos(0.0f,-0.25f,0.0f);
	hkpRigidBody* baseRigidBody = GameUtils::createBox(baseSize, 0.0f, basePos);
	m_world->addEntity( baseRigidBody );
	baseRigidBody->removeReference(); 	
		
	{
		// diving platform 
		baseSize.set( 4.1f, 4.0f, 2.0f);
		basePos.set(0.0f,2.0f,0.0f);
		rb = GameUtils::createBox(baseSize, 0.0f, basePos);
		m_world->addEntity( rb );
		rb->removeReference();

		// teeter-totter
		baseSize.set( 6.5f, 0.25f, 2.5f);
		basePos.set(6.0f,1.0f,0.0f);
		rb = GameUtils::createBox(baseSize, 10.0f, basePos);
		m_world->addEntity( rb );
		rb->removeReference();

		hkVector4 pivot = basePos;
		hkVector4 axis( 0.0f,0.0f,1.0f );
		hkpHingeConstraintData* axle = new hkpHingeConstraintData(); 
		axle->setInWorldSpace(rb->getTransform(), baseRigidBody->getTransform(), pivot, axis );
		m_world->createAndAddConstraintInstance( rb, baseRigidBody, axle )->removeReference(); 
		axle->removeReference();
	}

	baseSize.set( 0.7f, 0.8f, 0.7f);
	basePos.set(8.5f,0.25f,0.0f);
	rb = GameUtils::createBox(baseSize, 0.0f, basePos);
	m_world->addEntity( rb );
	rb->removeReference();


	// blocks over teeter-totter
	baseSize.set( 0.5f, 0.5f, 0.5f);
	basePos.set(8.0f,1.3f,0.0f);



	// pile of blocks
	int i,j,k;
	int iNum = 3;
	int jNum = 3;
	int kNum = 3;

	for( i = 0; i < iNum; i++ )
	{
		for( j = 0; j < jNum; j++ )
		{
			for( k = 0; k < kNum; k++ )
			{
				hkVector4 blockPos = basePos;
				blockPos(0) += baseSize(0) * (hkReal(i) - (hkReal(iNum)/2.0f));
				blockPos(1) += baseSize(1) * hkReal(j);
				blockPos(2) += baseSize(2) * (hkReal(k) - (hkReal(kNum)/2.0f));

				//
				// NOTE: Here we are creating a new shape (Collision detection representation)
				// for each rigid body created. This is not necessary for each rigid body in the pile
				// as rigid bodies can share shape objects.
				//
				rb = GameUtils::createBox(baseSize, 1.0f, blockPos);
				m_world->addEntity( rb );
				rb->removeReference();
			}
		}
	}

	hkpRigidBody *prevRb = baseRigidBody;
	baseSize.set( 1.0f, 0.25f, 4.0f);
	basePos.set(9.6f,0.0f,0.0f);

	
	hkVector4 halfExtents(baseSize(0) * 0.5f, baseSize(1) * 0.5f, baseSize(2) * 0.5f);
	hkpBoxShape* boxShape = new hkpBoxShape( halfExtents , 0 );

	// create the bridge to nowhere
	iNum = 50;
	for( i = 0; i < iNum; i++ )
	{
		basePos(0) += baseSize(0) + 0.025f;

		if( i == iNum-1 )
		{
			rb = baseRigidBody;
		}
		else
		{
			hkpRigidBodyCinfo info;

			info.m_position = basePos;
			info.m_shape = boxShape;

			hkpInertiaTensorComputer::setShapeVolumeMassProperties( boxShape, 20.0f, info );
			info.m_inertiaTensor.mul( 1.5f );

			// this will keep the bridge from being springy
			info.m_linearDamping  = 0.5f;			
			info.m_angularDamping = 2.0f;			
			info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA; //hkpMotion::MOTION_STABILIZED_SPHERE_INERTIA;

			rb = new hkpRigidBody(info);
			rb->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo( BRIDGE_LAYER ) );

			m_world->addEntity( rb );
			rb->removeReference();
		}

		// you could also do these with hinges, which would probably be better, actually.
		// this is a simple bridge, relys on the current constraints softness to hang properly
		if(1)
		{
			hkVector4 pivot( basePos(0) - ( baseSize(0) + 0.05f )*0.5f, basePos(1), basePos(2) + baseSize(2)*0.5f );

			hkpBallAndSocketConstraintData* bas = new hkpBallAndSocketConstraintData(); 
			bas->setInWorldSpace(prevRb->getTransform(), rb->getTransform(), pivot );
			m_world->createAndAddConstraintInstance( prevRb, rb, bas )->removeReference(); 
			bas->removeReference();

			pivot(2) = -baseSize(2)*0.5f;
			bas = new hkpBallAndSocketConstraintData(); 
			bas->setInWorldSpace(prevRb->getTransform(), rb->getTransform(), pivot );
			m_world->createAndAddConstraintInstance( prevRb, rb, bas )->removeReference(); 
			bas->removeReference();
		}
		// this bridge has a nicer feel to it.  it's got a sort of scissor joint.  
		// if the constraints get stiff, this will still hang nicely
		else
		{  
			hkVector4 jointSize( 0.05f, 0.05f, 0.05f );
			hkVector4 jointPos( basePos(0) - ( baseSize(0) + 0.025f )*0.5f, basePos(1)-0.05f, basePos(2) + baseSize(2)*0.5f );

			hkpRigidBody *joint = GameUtils::createBox(jointSize, 1.0f, jointPos);
			joint->setCollisionFilterInfo(1<<2);
			m_world->addEntity( joint );
			joint->removeReference();

			hkpBallAndSocketConstraintData* bas = new hkpBallAndSocketConstraintData(); 
			bas->setInWorldSpace(prevRb->getTransform(), joint->getTransform(), jointPos);
			m_world->createAndAddConstraintInstance( prevRb, joint, bas )->removeReference(); 
			bas->removeReference();

			bas = new hkpBallAndSocketConstraintData(); 
			bas->setInWorldSpace(joint->getTransform(), rb->getTransform(), jointPos);
			m_world->createAndAddConstraintInstance( joint, rb, bas )->removeReference(); 
			bas->removeReference();

			jointPos(2) = -baseSize(2)*0.5f;
			joint = GameUtils::createBox(jointSize, 1.0f, jointPos);
			joint->setCollisionFilterInfo(1<<2);
			m_world->addEntity( joint );
			joint->removeReference();
			
			bas = new hkpBallAndSocketConstraintData(); 
			bas->setInWorldSpace(prevRb->getTransform(), joint->getTransform(), jointPos);
			m_world->createAndAddConstraintInstance( prevRb, joint, bas )->removeReference(); 
			bas->removeReference();

			bas = new hkpBallAndSocketConstraintData();
			bas->setInWorldSpace(joint->getTransform(), rb->getTransform(), jointPos);
			m_world->createAndAddConstraintInstance( joint, rb, bas )->removeReference(); 
			bas->removeReference();
			
		}
		prevRb = rb;
	}


	boxShape->removeReference();

}

RopeBridgeDemo::RopeBridgeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(-15.0f, 25.0f, 25.0f);
		hkVector4 to  (  5.0f,  0.0f,  5.0f);
		hkVector4 up  (  0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_REMOVE_ENTITY;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// disable inter-bridge collisions
	{
		hkpGroupFilter* filter = new hkpGroupFilter();

		filter->enableCollisionsUsingBitfield( 0xffffffff,0xffffffff );
		filter->disableCollisionsBetween( BRIDGE_LAYER,  BRIDGE_LAYER );

		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}



		// After we add this listener, gBuilder gets notified when enties/constraints/actions are added
		// Similar we can add collsion listeners (using eg. m_world->addCollisionListener() ) to monitor
		// new collison events.
	createObstacleCourse();

	//
	// Create the sphere/marble/character
	//
	hkVector4 characterPos(2.0f,10.0f,0.0f);
	hkVector4 bSize( .7f, .7f, .7f );
	hkpRigidBody* characterRigidBody = GameUtils::createSphere(0.35f, 50.0f, characterPos);
	m_world->addEntity( characterRigidBody );
	characterRigidBody->removeReference();	// Remove reference, since we no longer want to remember this


		// Create the "marble" action to drive him around
	hkVector4 forward(1.0f,0.0f,0.0f);
	m_marbleaction = new MarbleAction(characterRigidBody, forward, characterRigidBody->getPosition());
	m_world->addAction(m_marbleaction);

	m_world->unlock();
}

hkDemo::Result RopeBridgeDemo::stepDemo()
{
	m_world->lock();

	const hkgPad* pad = m_env->m_gamePad;

	{
	
		// Check if action is still in the world : If controlled body "falls" out of broadphase our
		// hkpBroadPhaseBorder will catch it and remove it from simulation, which will cause the action
		// to remove itself from simulation automatically, and set its "owner" to HK_NULL.

		
		if(m_marbleaction->getWorld() != HK_NULL)
		{
			m_marbleaction->getEntity()->activate();

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
		}
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A scene with constraints, collision and a controllable sphere \n" \
"Use \224 \225 \226 \227 to control the sphere.";

HK_DECLARE_DEMO(RopeBridgeDemo, HK_DEMO_TYPE_PRIME, "Rope Bridge", helpString);

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

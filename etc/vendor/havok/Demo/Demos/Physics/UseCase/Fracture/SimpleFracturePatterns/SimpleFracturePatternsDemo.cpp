/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

	// we need to access our game pad
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Common/Visualize/hkDebugDisplay.h>


#include <Demos/Physics/UseCase/Fracture/SimpleFracturePatterns/SimpleFracturePatternsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/GlassFractureCollisionListener.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/ConcreteFractureCollisionListener.h>

#include <Common/Base/Types/Color/hkColor.h>

namespace
{
	struct Vector4
	{
		float f[4];
	};

	struct SimpleFracturePatternsVariant
	{
		const char*	 m_name;
		enum FractureType
		{
			GLASS,
			CONCRETE
		};
		FractureType m_fractureType;
		int m_maxFractureRecursion;
		int m_numOfObjects;
		HK_ALIGN16(Vector4 m_objectSize);
		HK_ALIGN16(Vector4 m_objectOffset);
		hkUint32 m_objectColor;
		hkReal m_bulletSize;
		const char* m_details;
	};
}

static const char vhelpText[] = "Simple fracture. Uses arrow keys and button 1 and 2 to shoot bullets. "
"Note: We do not calculate the fracture, but simply apply pre-calculated fracture patterns. "
"This demo is intended to show how to use the plane equation utility to create simple effects. "
"We are not dealing with creating new textured display objects, so it is only a tech demo which "
"could be used as a starting point for adding fracture to a game.";

static const SimpleFracturePatternsVariant g_variants[] =
{
	{ "Glass pane",     SimpleFracturePatternsVariant::GLASS,      5,	1, {{2.0f, 2.0f, 0.05f, 0.0f}}, {{0.00f, 0.0f, 0.0f, 0.0f}}, 0x70404080,    0.05f, vhelpText},
	{ "5 glass panes",  SimpleFracturePatternsVariant::GLASS,      5,   5, {{2.0f, 2.0f, 0.05f, 0.0f}}, {{0.50f, 0.0f, 0.3f, 0.0f}}, 0x70404080,    0.05f, vhelpText},
	{ "Thick glass",	SimpleFracturePatternsVariant::CONCRETE,   1,   3, {{2.0f, 2.0f, 0.30f, 0.0f}}, {{2.01f, 0.0f, 0.0f, 0.0f}}, 0x70404080,    0.05f, vhelpText},
	{ "Concrete",		SimpleFracturePatternsVariant::CONCRETE,   1,   3, {{2.0f, 2.0f, 0.30f, 0.0f}}, {{2.01f, 0.0f, 0.0f, 0.0f}}, hkUint32(hkColor::GREY), 0.05f, vhelpText}
};


SimpleFracturePatternsDemo::SimpleFracturePatternsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_shootingDirX = 0;
	m_shootingDirY = 0;
	m_generatorSeed = 1;
	m_gunCounter = 0;

	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	//const SimpleFracturePatternsVariant& variant =  g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.2f, 1.3f, -6.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 200.0f );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;

			// reduce gravity for cooler effect
		info.m_gravity.set(0.0f, -3.8f, 0.0f);

		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_REMOVE_ENTITY;
		info.m_collisionTolerance = 0.02f;
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		//m_debugViewerNames.pushBack( hkpRigidBodyInertiaViewer::getName() );

		setupGraphics();
	}

	const SimpleFracturePatternsVariant& variant = g_variants[m_env->m_variantId];

	//
	//	Create our fracture listener
	//
	{
		// Create the listeners
		switch(variant.m_fractureType)
		{
			case SimpleFracturePatternsVariant::GLASS:    
			{
				m_collisionListener = new GlassFractureCollisionListener( &m_fracturedBodies, 5.0f ); 
				break;
			}
			case SimpleFracturePatternsVariant::CONCRETE: 
			{
				m_collisionListener  = new ConcreteFractureCollisionListener( &m_fracturedBodies, 5.0f ); 
				break;
			}
		}

	}



	//
	// Create the base out of a box grid
	//
	{
		hkVector4 baseSize( 15.0f, 15.0f, 0.5f );
		hkpShape* shape = new hkpBoxShape( baseSize, 0.01f );

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_restitution = 0.1f;
		ci.m_friction    = 0.3f;
		ci.m_position.set( 0.0f, -0.5f, 0.0f );
		ci.m_rotation.setAxisAngle( hkTransform::getIdentity().getColumn(0), HK_REAL_PI * 0.5f );
		ci.m_motionType = hkpMotion::MOTION_FIXED;

		m_world->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}	

	//
	//	Create fracture-able body
	//
	{
		for (int i = 0; i < variant.m_numOfObjects; i++)
		{
			hkVector4 halfExtent; halfExtent.setMul4(0.5f, reinterpret_cast<const hkVector4&>(variant.m_objectSize));

			hkpRigidBodyCinfo ci;

			ci.m_shape = new hkpBoxShape( halfExtent, 0.001f );
				// We use a very low restitution to avoid the expensive restitution algorithm
				// Also breaking bodies do have a very low restitution anyways
			ci.m_restitution = 0.1f;
			ci.m_friction    = 0.3f;
			ci.m_position.set(	0.0f, halfExtent(1), 0.0f  );
			ci.m_position.addMul4( i - 0.5f * variant.m_numOfObjects, reinterpret_cast<const hkVector4&>(variant.m_objectOffset) );
			ci.m_motionType = hkpMotion::MOTION_DYNAMIC;
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;

			hkpInertiaTensorComputer::setShapeVolumeMassProperties( ci.m_shape, 25.0f, ci );

			hkpRigidBody* fractureBody = new hkpRigidBody( ci );
			ci.m_shape->removeReference();

				// attach our fracture collision listener so our body gets breakable
			fractureBody->addCollisionListener( m_collisionListener );

			fractureBody->addProperty( HK_PROPERTY_DEBUG_DISPLAY_COLOR, int(variant.m_objectColor) );

			m_world->addEntity( fractureBody )->removeReference();
		}
	}

	m_world->unlock();
}

void SimpleFracturePatternsDemo::makeFakeInput()
{
	// move the "gun"
	m_env->m_gamePad->forceButtonPressed(HKG_PAD_DPAD_LEFT);

	// constantly try to fire
	m_env->m_gamePad->forceButtonPressed(HKG_PAD_BUTTON_2);
}

hkDemo::Result SimpleFracturePatternsDemo::stepDemo()
{
	const hkReal deltaTime = 0.016f;

	{
		m_world->lock();

		const hkgPad* pad = m_env->m_gamePad;


		//
		// Cannon control + drawing
		//
		///[StepGame]
		/// To step the game we check to see if the user has pressed one of the control keys:
		int x = ((pad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0)? -1:0;
		x = ((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)? 1:x;
		int y = ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)? -1:0;
		y = ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)?    1:y;


		m_shootingDirX -= x * 0.006f;
		m_shootingDirY += y * 0.006f;

		hkVector4 canonStart(0,1,-5);

		hkVector4 canonDir( m_shootingDirX, m_shootingDirY, 1.0f );
		canonDir.normalize3();

		// display the canon dir
		{
			hkpWorldRayCastInput in;
			in.m_from = canonStart;
			in.m_to.setAddMul4( in.m_from, canonDir, 100.0f );
			in.m_filterInfo = 0;
			hkpWorldRayCastOutput out;
			m_world->castRay( in , out );
			hkVector4 hit; hit.setInterpolate4( in.m_from, in.m_to, out.m_hitFraction );
			HK_DISPLAY_LINE( in.m_from, hit, 0x600000ff );
			if ( out.hasHit() )
			{
				HK_DISPLAY_ARROW( hit, out.m_normal, 0x60ff00ff );
			}
			HK_DISPLAY_ARROW( canonStart, canonDir, hkColor::CYAN );
		}

		//
		// Shooting bullets
		//
		{
			hkReal radiusOfBullet = 0.0f;

			if ( pad->wasButtonPressed(HKG_PAD_BUTTON_1)!= 0 )
			{
				radiusOfBullet = g_variants[m_env->m_variantId].m_bulletSize / 2.0f;
			}

			if (  pad->isButtonPressed(HKG_PAD_BUTTON_2)!= 0)
			{
				if ( m_gunCounter -- < 0 )
				{
					m_gunCounter = 5;
					radiusOfBullet = g_variants[m_env->m_variantId].m_bulletSize / 2.0f;
				}
			}

			if ( radiusOfBullet )
			{
				hkpRigidBodyCinfo ci;
				ci.m_shape = new hkpSphereShape( radiusOfBullet);
				ci.m_mass = 0.1f;
				ci.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;
				ci.m_restitution = 0.2f;
				ci.m_friction = 0.3f;
				ci.m_position.set( 0.0f, 1.0f, 0.0f );
				ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
				ci.m_inertiaTensor.setDiagonal( 100.0f, 100.0f, 100.0f );
				ci.m_linearVelocity.setMul4( 30.0f, canonDir );
				ci.m_position = canonStart;

				hkpRigidBody* bullet = new hkpRigidBody( ci );
				ci.m_shape->removeReference();

				m_world->addEntity( bullet );
				bullet->removeReference();
			}
		}

		m_world->unlock();
	}

	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		//
		// Re-attach listeners for recursive fracture of large pieces (large in volume or size);
		//
		// The newly added/fractured bodies are stored on the list, but a collision listener is only added to them after two frames.
		// This is an easy solution to an important problem of uncontrolled recursive fracture.
		//
		// Problem:
		// If listeners were attached immediately, then in the next simulation step new contact points would NOT result in 
		// a proper rigid body collision response and change of velocities. They would trigger subsequent fracture instead.
		//
		// Cause:
		// When a body is fractured, the new pieces copy the velocity that the original body had at the beginning of the simulation
		// step. The results of solver computations for the fractured body are therefore ignored. Upon the next call to collision 
		// detection, the newly create pieces have the same velocities as the original body, which results in repeated fracturing.
		//
		//
		{
			hkReal maxVolumeForNoRecursiveFracture = 1.0f;
			hkReal maxRadiusForNoRecursiveFracture = 1.0f;

			switch(g_variants[m_env->m_variantId].m_fractureType)
			{
			case SimpleFracturePatternsVariant::GLASS:    
				{
					maxVolumeForNoRecursiveFracture = 0.5f * 0.5f * 0.05f;
					maxRadiusForNoRecursiveFracture = 0.8f;
					break;
				}
			case SimpleFracturePatternsVariant::CONCRETE: 
				{
					maxVolumeForNoRecursiveFracture = 0.3f * 0.3f * 0.3f;
					maxRadiusForNoRecursiveFracture = 0.8f;
					break;
				}
			default:
				HK_ASSERT(0,0);
				break;
			}

			//
			// Add collision listeners to bodies that waited long enough.
			//
			{
				hkInplaceArray<hkpEntity*, 64> entitiesToGetACollisionListener;
				m_entitiesWaitingForCollisionListeners.stepDeltaTime(deltaTime, entitiesToGetACollisionListener);
				for (int i = 0; i < entitiesToGetACollisionListener.getSize(); i++)
				{
					hkpRigidBody* body = static_cast<hkpRigidBody*>(entitiesToGetACollisionListener[i]);
					body->addCollisionListener(m_collisionListener);
					body->removeReference();
				}
			}

			//
			// Queue new bodies so that they get a collision listener after a couple of frames.
			// Ignore bodies which are to small in volume and size.
			//
			for (int i = 0; i < m_fracturedBodies.getSize(); i++ )
			{
				hkpRigidBody* body = static_cast<hkpRigidBody*>(m_fracturedBodies[i]);

				hkReal volume = body->getProperty(HK_PROPERTY_RIGID_BODY_VOLUME).getReal();
				if (volume >= maxVolumeForNoRecursiveFracture || body->getRigidMotion()->getMotionState()->m_objectRadius > maxRadiusForNoRecursiveFracture)
				{
					m_entitiesWaitingForCollisionListeners.addEntity(body, deltaTime * 2.1f);
				}
				body->removeReference();
			}
			m_fracturedBodies.clear();

		}

		m_world->unlock();
	}

	return DEMO_OK;
}


SimpleFracturePatternsDemo::~SimpleFracturePatternsDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true);

	if(m_world)
	{
		if (m_jobThreadPool)
		{
			m_jobThreadPool->waitForCompletion();
			delete m_jobThreadPool;
			m_jobThreadPool = HK_NULL;
		}
		HK_ASSERT2( 0xf021345e, m_world->getReferenceCount() == 1, "The world must be deleted here, as its bodies have a listener connected, and that listener is delete here." );
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	m_collisionListener->removeReference();

	for (int i = 0; i < m_fracturedBodies.getSize(); i++)
	{
		m_fracturedBodies[i]->removeReference();
	}
	m_fracturedBodies.clear();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A small game which allows you to shoot small bullets into a piece of breakable material.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SimpleFracturePatternsDemo, HK_DEMO_TYPE_PRIME, SimpleFracturePatternsVariant, g_variants, helpString ); 

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

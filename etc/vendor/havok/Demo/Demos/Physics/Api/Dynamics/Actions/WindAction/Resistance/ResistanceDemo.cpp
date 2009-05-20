/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Utilities/Actions/Wind/hkpPrevailingWind.h>

#include <Demos/Physics/Api/Dynamics/Actions/WindAction/Resistance/ResistanceDemo.h>


#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

hkVector4 ResistanceDemo::initialPosition( int i )
{
	return hkVector4( ((i / 4) + 1) * 7.0f - 14.0f, 25.0f, ((i % 4) + 1) * 7.0f - 14.0f );
}

// A demo which shows resistance being applied to several objects.
ResistanceDemo::ResistanceDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// X and Z are the half-extents of the floor, and Y is the height of the walls.
	hkReal x = 25.0f;
	hkReal y = 30.0f;
	hkReal z = 25.0f;
	
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, 35.0f, z * 3.0f );
		hkVector4 to(0.0f, 10.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register all collision agents.
	//
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	hkVector4 areaSize( x, y, z );
	
	
	hkReal thickness = 2.0f;

	// Create the floor.
	{
		hkpRigidBody* lowerFloor;
		hkVector4 fixedBoxSize( x , thickness , z );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f, -thickness, 0.0f);
		}

		lowerFloor = new hkpRigidBody(info);
		m_world->addEntity(lowerFloor);

		lowerFloor->removeReference();
		fixedBoxShape->removeReference();
	}

	// Create the wind
	{
		m_resistanceManager.m_wind = new hkpPrevailingWind( hkVector4( 0.0f, 0.0f, 0.0f ) );
	}

	// Create a grid of objects.
	{
		hkQuaternion rot;
		hkVector4 axis( 0.0f, 0.5f, 1.0f );
		axis.normalize3();
		
		const int numBodies = 16;
		
		for (int i = 0; i < numBodies; i += 1 )
		{
			hkpRigidBody* body;
			switch ( i % 2 )
			{
			case 0:
				{
					body = GameUtils::createBox( hkVector4( 4.0f, 4.0f, 0.1f ), 1.0f, initialPosition( i ) );
					break;
				}
				
			case 1:
			default:
				{
					body = GameUtils::createCylinder( 2.0f, 0.1f, 1.0f, initialPosition( i ));
					break;
				}
			}
			rot.setAxisAngle( axis, (HK_REAL_PI / numBodies) * i );
			body->setRotation( rot );
			m_world->addEntity( body );
			m_resistanceManager.add( body );
			body->removeReference();	
		}
	}
	m_world->addWorldPostSimulationListener( &m_resistanceManager );
	m_world->unlock();
}

void ResistanceManager::add( hkpRigidBody* rb )
{
	m_bodies.pushBack( rb );
	rb->addReference();
}

void ResistanceManager::postSimulationCallback( hkpWorld* world )
{
	for ( int i = 0; i < m_bodies.getSize(); ++i )
	{
		//hkpWind::applyResistance( m_bodies[i], world->m_dynamicsStepInfo.m_stepInfo.m_deltaTime, 0.5f );
		m_wind->applyWindAndResistance( m_bodies[i], world->m_dynamicsStepInfo.m_stepInfo.m_deltaTime, 5.0f );
	}
}

ResistanceManager::~ResistanceManager()
{
	for ( int i = 0; i < m_bodies.getSize(); ++i )
	{
		m_bodies[i]->removeReference();
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows the effect of resistance due to motion on some thin rigid bodies.";

HK_DECLARE_DEMO(ResistanceDemo, HK_DEMO_TYPE_PRIME, "This demo shows the effect of resistance due to motion", helpString);

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

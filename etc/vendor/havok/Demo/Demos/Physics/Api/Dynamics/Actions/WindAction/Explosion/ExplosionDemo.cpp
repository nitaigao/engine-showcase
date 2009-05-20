/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Actions/WindAction/Explosion/ExplosionDemo.h>

#include <Physics/Utilities/Actions/Wind/hkpWindRegion.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

// We use debug lines to visualize explosions.
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayBox.h>

// ////////////////////////////////////////////////////////////////////////
// EXPLOSION
// ////////////////////////////////////////////////////////////////////////

Explosion::Explosion ( const hkVector4& center, const ExplosionInfo& info )
:	m_info( info ),
	m_center( center ),
	m_age( 0 )
{
}

hkReal Explosion::getStrengthFactor() const
{
	const hkReal relDistance = m_info.m_propagationSpeed * m_age - m_info.m_width;

	// Strength falls off with the square of the distance, but we ensure that all bodies
	// receive a full dose of peak wind by waiting until the peak has fully appeared before
	// diminishing the strength.
	hkReal factor;
	if ( relDistance > 0.0f )
	{
		factor =  1.0f / ( relDistance * relDistance );
	}
	else
	{
		factor = 1.0f;
	}

	return factor;	
}

void Explosion::getWindVector( const hkVector4& pos, hkVector4& windOut ) const
{
	hkVector4 normalToPos;
	hkReal distanceToPos;
	{
		normalToPos.setSub4( pos, m_center );
		if ( normalToPos.lengthSquared3() > 0.0f )
		{
			distanceToPos = (hkReal) normalToPos.normalizeWithLength3();
		}
		else
		{
			distanceToPos = 0.0f;
			normalToPos.set( 0.0f, 1.0f, 0.0f );
		}
	}

	hkReal distanceToWaveFront = m_info.m_propagationSpeed * m_age;
	
	if  ( ( distanceToPos < distanceToWaveFront )
		&& ( distanceToPos > hkMath::max2( distanceToWaveFront - m_info.m_width, 0.0f ) ) )
	{
		windOut.setMul4( m_info.m_peakWindStrength * getStrengthFactor(), normalToPos );
	}
	else
	{
		windOut.setZero4();
	}
}

void Explosion::update( hkReal delta )
{
	m_age += delta;

	// Visualization of explosion as a bar graph of strength versus distance.
	{
		const hkReal distanceToWaveFront = m_info.m_propagationSpeed * m_age;
		const hkReal strength = m_info.m_peakWindStrength * getStrengthFactor();
		
		const hkReal distanceToWaveBack = hkMath::max2 ( 0.0f, distanceToWaveFront - m_info.m_width );

		const hkVector4 waveFront( distanceToWaveFront, 0.0f, 0.0f );
		const hkVector4 peakStart( distanceToWaveFront, strength, 0.0f );
		const hkVector4 peakEnd( distanceToWaveBack, strength, 0.0f );
		const hkVector4 waveBack( distanceToWaveBack, 0.0f, 0.0f );

		{
			hkVector4 leftWaveFront;
			{
				leftWaveFront.setSub4( m_center, waveFront );
				leftWaveFront( 1 ) = -leftWaveFront( 1 );
			}
			hkVector4 leftPeakStart;
			{
				leftPeakStart.setSub4( m_center, peakStart );
				leftPeakStart( 1 ) = -leftPeakStart( 1 );
			}
			hkVector4 leftPeakEnd;
			{
				leftPeakEnd.setSub4( m_center, peakEnd );
				leftPeakEnd( 1 ) = -leftPeakEnd( 1 );
			}
			hkVector4 leftWaveBack;
			{
				leftWaveBack.setSub4( m_center, waveBack );
				leftWaveBack( 1 ) = -leftWaveBack( 1 );
			}

			HK_DISPLAY_LINE( leftWaveFront, leftPeakStart, hkColor::YELLOW );
			HK_DISPLAY_LINE( leftPeakStart, leftPeakEnd, hkColor::YELLOW );
			HK_DISPLAY_LINE( leftPeakEnd, leftWaveBack, hkColor::YELLOW );
		}

		{
			hkVector4 rightWaveFront;
			{
				rightWaveFront.setAdd4( m_center, waveFront );
			}
			hkVector4 rightPeakStart;
			{
				rightPeakStart.setAdd4( m_center, peakStart );
			}
			hkVector4 rightPeakEnd;
			{
				rightPeakEnd.setAdd4( m_center, peakEnd );
			}
			hkVector4 rightWaveBack;
			{
				rightWaveBack.setAdd4( m_center, waveBack );
			}
			
			HK_DISPLAY_LINE( rightWaveFront, rightPeakStart, hkColor::YELLOW );
			HK_DISPLAY_LINE( rightPeakStart, rightPeakEnd, hkColor::YELLOW );
			HK_DISPLAY_LINE( rightPeakEnd, rightWaveBack, hkColor::YELLOW );
		}
	}
}

hkBool Explosion::isActive() const
{
	// When strength factor drops below 0.5% of the original force, we can regard the explosion as finished.
	return getStrengthFactor() > 0.005;
}

// ////////////////////////////////////////////////////////////////////////
// EXPLOSION MANAGER
// ////////////////////////////////////////////////////////////////////////

ExplosionManager::ExplosionManager() {}

ExplosionManager::~ExplosionManager()
{
	const int numExplosions = m_explosions.getSize();
	for( int i = 0; i < numExplosions; ++i )
	{
		m_explosions[i]->removeReference();
	}
}

void ExplosionManager::update( hkReal delta )
{
	// Iterate over explosions, and remove those which have finished their work.
	int i = 0;
	while ( i < m_explosions.getSize() )
	{
		m_explosions[i]->update( delta );
		if ( m_explosions[i]->isActive() )
		{
			++i;
		}
		else
		{
			m_explosions[i]->removeReference();
			m_explosions.removeAt( i );
		}
	}
}

void ExplosionManager::getWindVector( const hkVector4& pos, hkVector4& windOut ) const
{
	windOut.setZero4();

	// The wind is the sum of the wind due to explosions.
	const int numExplosions = m_explosions.getSize();
	for( int i = 0; i < numExplosions; ++i )
	{
		hkVector4 explosionWind;
		{
			m_explosions[i]->getWindVector( pos, explosionWind );
		}
		windOut.add4( explosionWind );
	}
}

void ExplosionManager::addExplosion( const hkVector4& center, const ExplosionInfo& info )
{
	Explosion* explosion = new Explosion( center, info );
	m_explosions.pushBack( explosion );
}

// ////////////////////////////////////////////////////////////////////////
// DEMO
// ////////////////////////////////////////////////////////////////////////

hkVector4 ExplosionDemo::initialPosition( hkReal height, int i, int gridWidth, hkReal gap )
{
	return hkVector4( (i / gridWidth) * gap - ( (gridWidth - 1) * gap / 2.0f), height, (i % gridWidth) * gap - ( (gridWidth - 1) * gap / 2.0f));
}

// A demo which shows drag being applied to several objects.
ExplosionDemo::ExplosionDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	demoX( 25.0f ),
	demoY( 20.0f ),
	demoZ( 25.0f ),
	m_random( 2 ),
	m_exInfo( 200.0f, 150.0f, 9.0f )
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, 40.0f, demoZ * 3.0f );
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

	// Thickness of floor and walls
	hkReal thickness = 1.0f;

	// Create the floor.
	{
		hkpRigidBody* lowerFloor;
		hkVector4 fixedBoxSize( demoX , thickness , demoZ );
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

	// Create walls.
	{
		for( int i = 0; i < 4; ++i )
		{
			hkpRigidBody* wall;
			hkpBoxShape* fixedBoxShape;
			if ( i < 2 )
			{
				fixedBoxShape = new hkpBoxShape( hkVector4( thickness , demoY , demoZ ), 0 );
			}
			else
			{
				fixedBoxShape = new hkpBoxShape( hkVector4( demoX , demoY, thickness ), 0 );
			}
			
			hkpRigidBodyCinfo info;
			{
				info.m_shape = fixedBoxShape;
				info.m_motionType = hkpMotion::MOTION_FIXED;
				switch( i ) 
				{
					case 0:
						info.m_position.set( demoX + thickness, demoY, 0.0f );
						break;
					case 1:
						info.m_position.set( -demoX - thickness, demoY, 0.0f );
						break;
					case 2:
						info.m_position.set( 0.0f, demoY, demoZ + thickness );
						break;
					case 3:
					default:
						info.m_position.set( 0.0f, demoY, -demoZ - thickness );
						break;
				}
				
			}

			wall = new hkpRigidBody( info );
			
			m_world->addEntity( wall );
			HK_SET_OBJECT_COLOR((hkUlong) wall->getCollidable(), hkColor::rgbFromChars( 255, 255, 255, 30 )); 

			wall->removeReference();
			fixedBoxShape->removeReference();
		}
	}

	// Create a ceiling.
	{
		hkpRigidBody* ceiling;
		hkVector4 fixedBoxSize( demoX , thickness , demoZ );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f, (demoY * 2.0f) + thickness, 0.0f);
		}

		ceiling = new hkpRigidBody(info);
		m_world->addEntity( ceiling );
		HK_SET_OBJECT_COLOR( (hkUlong) ceiling ->getCollidable(), hkColor::rgbFromChars( 255, 255, 255, 30 ) ); 

		ceiling->removeReference();
		fixedBoxShape->removeReference();
	}

	// Create a grid of objects.
	{
		const int gridWidth = 8;
		const int numBodies = gridWidth * gridWidth;
		const hkReal gap = 2.0f * demoX / ( gridWidth + 1);

		for (int i = 0; i < numBodies; i += 1 )
		{
			hkpRigidBody* body;
			switch ( i % 3 )
			{
			case 0:
				{
					body = GameUtils::createBox( hkVector4( 0.3f, 3.5f, 1.7f ), 1.0f, initialPosition( 1.75f, i, gridWidth, gap ) );
					break;
				}

			case 1:
				{
					body = GameUtils::createBox( hkVector4( 0.3f, 1.7f, 3.5f ), 1.0f, initialPosition( 0.85f, i, gridWidth, gap ) );
					break;
				}

			default:
				{
					body = GameUtils::createBox( hkVector4( 1.7f, 3.5f, 0.3f ), 1.0f, initialPosition( 1.75f, i, gridWidth, gap ) );
					break;
				}
			}
			m_world->addEntity( body );
			//hkpWindAction* action = new hkpWindAction( body, &m_explosionManager, 0.1f );
			//m_world->addAction( action );
			//action->removeReference();
			body->removeReference();
		}
	}

	// Create a wind phantom.
	{
		hkAabb area;
		{
			area.m_min.set( -demoX, 0.0f, -demoZ );
			area.m_max.set( demoX, 2.0f * demoY, demoZ );
		}
		hkpAabbPhantom* phantom = new hkpAabbPhantom( area );
		m_world->addPhantom( phantom );
		m_region = new hkpWindRegion( phantom, &m_explosionManager, 0.1f );
		m_world->addWorldPostSimulationListener( m_region );
		phantom->removeReference();
	}

	// Ensure one central explosion.
	m_explosionManager.addExplosion( hkVector4( 0.0f, 0.0f, 0.0f ), m_exInfo );

	m_world->unlock();
}

ExplosionDemo::~ExplosionDemo()
{
	m_region->removeReference();
}

hkDemo::Result ExplosionDemo::stepDemo()
{
	// Update currently active explosions.
	m_explosionManager.update( m_world->m_dynamicsStepInfo.m_stepInfo.m_deltaTime );

	// Occasionally add a new explosion.
	if ( m_random.getRandRange( 0.0f, 1.0f ) < 0.01 )
	{
		hkReal xRange = demoX + m_exInfo.m_width;
		hkReal zRange = demoZ + m_exInfo.m_width;
		hkVector4 center( m_random.getRandRange( - xRange, xRange ), 0.0f, m_random.getRandRange( -zRange, zRange ) );
		m_explosionManager.addExplosion( center, m_exInfo );
	}

	

	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demonstrates using wind to model explosions.";

HK_DECLARE_DEMO(ExplosionDemo, HK_DEMO_TYPE_PRIME, "This demonstrates using wind to model explosions", helpString);

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

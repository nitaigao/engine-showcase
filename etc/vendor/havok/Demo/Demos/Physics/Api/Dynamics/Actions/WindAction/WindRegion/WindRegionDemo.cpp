/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Utilities/Actions/Wind/hkpWindRegion.h>

#include <Demos/Physics/Api/Dynamics/Actions/WindAction/WindRegion/WindRegionDemo.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivity.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivityUtil.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayBox.h>

// We need to create a constraint
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

// Needed for determinism checks
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>


	/// A very simple vortex-like wind centred around the origin.
class SimpleVortex : public hkpWind
{
	public:
		SimpleVortex( const hkVector4& center, const hkVector4& normal );
		virtual void getWindVector( const hkVector4& pos, hkVector4& windOut ) const;
	private:
		hkVector4 m_center;
		hkVector4 m_normal;
};

SimpleVortex::SimpleVortex( const hkVector4& center, const hkVector4& normal )
:	m_center( center ),
	m_normal( normal )
{
}

void SimpleVortex::getWindVector( const hkVector4& pos, hkVector4& windOut ) const
{
	hkVector4 relPos;
	{
		relPos.setSub4( pos, m_center );
	}
	hkVector4 h;
	{
		h.setMul4( relPos.dot3( m_normal ), m_normal );
	}
	hkVector4 ray;
	{
		ray.setSub4( h, relPos );
	}
	hkVector4 perp;
	{
		perp.setCross( m_normal, ray );
	}
	perp.normalize3IfNotZero();
	windOut.setMul4( 1.0f, perp );
	windOut.addMul4( 4.5f, m_normal );
	windOut.addMul4( 1.0f, ray );
}

hkpRigidBody* WindRegionDemo::createCompositeBody( hkReal width, hkReal mass, const hkVector4& position )
{
	hkArray<hkpShape*> shapeArray;
	hkArray<hkpMassElement> massElements;

	const hkReal widthRatio = 0.05f;

	{
		hkVector4 pointA( -width / 2.0f, 0.0f, widthRatio * width );
		hkVector4 pointB( -width / 2.0f, 0.0f, -widthRatio * width );
		hkpCylinderShape* disc0Shape = new hkpCylinderShape( pointA, pointB, width / 2.0f );
		shapeArray.pushBack( disc0Shape );

		hkpMassElement disc0MassElement;
		hkpInertiaTensorComputer::computeCylinderVolumeMassProperties( pointA, pointB, width / 2.0f, mass / 2.0f, disc0MassElement.m_properties );
		massElements.pushBack( disc0MassElement );
	}

	{
		hkVector4 pointA( width / 2.0f, widthRatio * width, 0.0f );
		hkVector4 pointB( width / 2.0f, -widthRatio * width, 0.0f );
		hkpCylinderShape* disc0Shape = new hkpCylinderShape( pointA, pointB, width / 2.0f );
		shapeArray.pushBack( disc0Shape );

		hkpMassElement disc0MassElement;
		hkpInertiaTensorComputer::computeCylinderVolumeMassProperties( pointA, pointB, width / 2.0f, mass / 2.0f, disc0MassElement.m_properties );
		massElements.pushBack( disc0MassElement );
	}

	hkpShape* shape = new hkpListShape( shapeArray.begin(), shapeArray.getSize() );

	hkpRigidBodyCinfo info;
	{
		hkpMassProperties massProperties;
		{
			hkpInertiaTensorComputer::combineMassProperties(massElements, massProperties);
		}
		info.m_shape = shape;
		info.m_motionType = hkpMotion::MOTION_DYNAMIC;
		info.m_mass = massProperties.m_mass;
		info.m_inertiaTensor = massProperties.m_inertiaTensor;
		info.m_centerOfMass = massProperties.m_centerOfMass;
		info.m_position = position;
	}

	return new hkpRigidBody( info );
}

hkVector4 WindRegionDemo::initialPosition( hkReal height, int i, int gridWidth, hkReal gap )
{
	return hkVector4( (i / gridWidth) * gap - ( (gridWidth - 1) * gap / 2.0f), height, (i % gridWidth) * gap - ( (gridWidth - 1) * gap / 2.0f));
}

//hkVector4 WindRegionDemo::initialPosition( const hkVector4& areaSize, int i )
//{//
	//return hkVector4( ((i / 4) + 1) * 7.0f - 14.0f, areaSize( 1 ) + 4.0f, ((i % 4) + 1) * 7.0f - 14.0f );
//}

// A demo which shows drag being applied to several objects.
WindRegionDemo::WindRegionDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// X and Z are the half-extents of the floor, and Y is the height of the walls.
	hkReal x = 20.0f;
	hkReal y = 30.0f;
	hkReal z = 20.0f;
	
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, 40.0f, z * 3.0f );
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

	// Create the floor.
	{
		hkReal thickness = 0.5f;

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
	}

	// Create the wind phantom.
	{
		hkAabb area;
		{
			area.m_min.set( -x, 0.0f, -z );
			area.m_max.set( x, y, z );
		}
		hkpAabbPhantom* phantom = new hkpAabbPhantom( area );
		m_world->addPhantom( phantom );
		hkpWind* wind = new SimpleVortex( hkVector4( 0.0f, 0.0f, 0.0f ), hkVector4( 0.0f, 1.0f, 0.0f ) );
		m_region = new hkpWindRegion( phantom, wind, 0.1f, 0.7f );
		m_world->addWorldPostSimulationListener( m_region );
		phantom->removeReference();
	}

	// Create a grid of objects.
	{
		hkQuaternion rot;
		hkVector4 axis( 0.0f, 0.5f, 1.0f );
		axis.normalize3();

		const int gridWidth = 4;
		const int numBodies = gridWidth * gridWidth;
		const hkReal gap = 2.0f * x / ( gridWidth + 1);

		hkPseudoRandomGenerator* generator = HK_NULL;
		HK_ON_DETERMINISM_CHECKS_ENABLED(generator = new hkPseudoRandomGenerator(234));

		for (int i = 0; i < numBodies; i += 1 )
		{
			hkpRigidBody* body;
			switch ( i % 5 )
			{
			case 0:
				{
					body = GameUtils::createBox( hkVector4( 1.5f, 2.5f, 0.2f ), 1.0f, initialPosition( 35.0f, i, gridWidth, gap ) );
					break;
				}

			case 1:
				{
					body = GameUtils::createCapsuleFromBox( hkVector4( 2.2f, 1.8f, 1.8f ), 1.0f, initialPosition( 35.0, i, gridWidth, gap ) );
					break;
				}
			case 2:
				{
					body = GameUtils::createRandomConvexGeometric( 1.1f, 1.0f, initialPosition( 35.0, i, gridWidth, gap ), 30, generator );
					break;
				}
			case 4:
				{
					body = createCompositeBody( 1.9f, 1.0f, initialPosition( 35.0f, i, gridWidth, gap ) );
					break;
				}
			default:
				{
					body = GameUtils::createSphere( 1.0f, 1.0f, initialPosition( 35.0f, i, gridWidth, gap ) );
					break;
				}
			}
			rot.setAxisAngle( axis, (HK_REAL_PI / numBodies) * i );
			body->setRotation( rot );
			m_world->addEntity( body );
			body->removeReference();	
		}

		HK_ON_DETERMINISM_CHECKS_ENABLED(delete generator);
		HK_ON_DETERMINISM_CHECKS_ENABLED(generator = HK_NULL);

	}
	
	m_world->unlock();
}

WindRegionDemo::~WindRegionDemo()
{
	m_region->removeReference();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows a wind region applying wind to objects which intersect its phantom.";

HK_DECLARE_DEMO(WindRegionDemo, HK_DEMO_TYPE_PRIME, "This demo shows a wind region", helpString);

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

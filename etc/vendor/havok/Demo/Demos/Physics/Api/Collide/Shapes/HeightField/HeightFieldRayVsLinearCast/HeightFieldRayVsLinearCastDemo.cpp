/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/HeightField/HeightFieldRayVsLinearCast/HeightFieldRayVsLinearCastDemo.h>


//
// For casting
//
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>

//
// For the linear cast phantom
//

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

//
// Need some shapes
//
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>


//
// For display purposes
//

#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


//
// A basic implementation (for demonstrative purposes) of a hkpSampledHeightFieldShape
//

//
// NOTE: this heightfield will be oriented such that the Z-axis is its up axis, i.e. heightfield 'plane' will be in (X,Y)
//

class RayTestSampledHeightFieldShape: public hkpSampledHeightFieldShape
{
	public:
		RayTestSampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci ): hkpSampledHeightFieldShape(ci)
		{
		}

		//
		// This will create a 1m spike in the centre of the heightfield
		//
		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			return (x == m_xRes/2) && (z == m_zRes/2);
		}

		// this should return true if the two triangles share the edge p00-p11
		// otherwise it should return false if the triangles share the edge p01-p10
		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{	
			return false;
		}

		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}
};

HeightFieldRayVsLinearCastDemo::HeightFieldRayVsLinearCastDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env), 	m_time(1.0f)
{

	//	
	// Setup the camera.
	//
	{
		hkVector4 from(-10.0f, 2.5f, 5.0f);
		hkVector4 to  (  0.0f, 0.0f, 0.0f);
		hkVector4 up  (  0.0f, 0.0f, 1.0f);
		setupDefaultCameras(env, from, to, up);
	}



	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//
	//	Register collision agents
	//
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	//
	//  The size of the heightfield
	//

	const int gridSize = 16;

	//
	//	Create a height field
	//
	hkpSampledHeightFieldShape* heightFieldShape;
	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = gridSize;
		ci.m_zRes = gridSize;

		heightFieldShape = new RayTestSampledHeightFieldShape( ci );
		m_shape = heightFieldShape;
	}

	//
	//	Create the landscape body
	//
	{
		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_shape = heightFieldShape;
		ci.m_position.set( -0.5f * gridSize, 0.5f * gridSize, -0.5f);
		m_position = ci.m_position;

		//
		// NOTE: this heightfield will be oriented such that the Z-axis is its up axis, i.e. heightfield 'plane' will be in (X,Y)
		//

		m_rotation = hkQuaternion(hkVector4(1.0f, 0.0f, 0.0f, 0.0f), HK_REAL_PI / 2);
		ci.m_rotation = m_rotation;

		m_world->addEntity( new hkpRigidBody( ci ) )->removeReference();
	}

	//
	// Create a simple phantom to use for the linear cast
	//
	{
		const hkReal sphereRadius = 0.5f;
		hkpSphereShape* shape = new hkpSphereShape( sphereRadius );		
		m_simpleShapePhantom = new hkpSimpleShapePhantom(shape, hkTransform::getIdentity());
		shape->removeReference();
		
		m_world->addPhantom( m_simpleShapePhantom );
		m_simpleShapePhantom->removeReference();
	}

	m_world->unlock();
}

//
//  Cleanup 
//

HeightFieldRayVsLinearCastDemo::~HeightFieldRayVsLinearCastDemo()
{
	m_shape->removeReference();
}


hkDemo::Result HeightFieldRayVsLinearCastDemo::stepDemo()
{
	m_world->lock();
	
	m_time += 0.01f;

	//
	//	Do the raycasts
	//
	{
		for (int i = 0; i < 2; i++ )
		{
			//
			//	Setup the ray coordinates in world coords
			//
			hkpShapeRayCastInput ray;
			if (i)
			{
				//
				// flat ray starting outside (tests clipping and normal walking)
				// 
				const hkReal r = 20.0f;
				const hkReal t = m_time;
				ray.m_from.set( hkMath::sin( t ) * r, hkMath::cos( t ) * r, 0.0f );
				ray.m_to.setMul4( -1.0f, ray.m_from );
			}
			else
			{
				//
				// vertical ray (test vertex intersection)
				//
				ray.m_from.set( 0.0f, 0.0f, 3.0f );
				ray.m_to.setMul4( -1.0f, ray.m_from );
			}

			hkTransform worldTransform(m_rotation, m_position);

			//
			//	Transform the raycast into shape local space - we will ray cast directly against the shape
			//
			hkpShapeRayCastInput rayLocal;
			{
				rayLocal.m_from.setTransformedInversePos( worldTransform, ray.m_from);
				rayLocal.m_to.  setTransformedInversePos( worldTransform, ray.m_to);
			}

			//
			//	Do the raycast
			//
			hkpShapeRayCastOutput output;
			{
				output.reset();
				m_shape->castRay( rayLocal, output );
			}

			//
			//	Check the hit
			//
			if (output.hasHit())
			{

				hkVector4 intersectionPointWorld;
				intersectionPointWorld.setInterpolate4( ray.m_from, ray.m_to, output.m_hitFraction );		

				//
				//	Transform the hit normal into world space
				//
				hkVector4 transformedNormal;
				transformedNormal.setRotatedDir( m_rotation, output.m_normal );
				transformedNormal.normalize3();

				//
				//	Display the raycast, hit and normal
				//
				HK_DISPLAY_LINE( ray.m_from, intersectionPointWorld, hkColor::RED );
				HK_DISPLAY_ARROW( intersectionPointWorld, transformedNormal, hkColor::RED );
			}
		}
	}

	//
	//	Do a linear sphere casts
	//
	{
		//
		//	Setup the ray coordinates in world coords
		//
		hkpShapeRayCastInput ray;
		{
			//
			// flat ray starting outside (tests clipping and normal walking)
			// 
			const hkReal r = 20.0f;
			const hkReal t = m_time + HK_REAL_PI;
			ray.m_from.set( hkMath::sin( t ) * r, hkMath::cos(t) * r, 0.3f );
			ray.m_to.setMul4( -1.0f, ray.m_from );
		}	


		// The input for the linear cast
		hkpLinearCastInput input;
		input.m_to = ray.m_to;

		// The collector for the linear caster
		hkpClosestCdPointCollector collector;
		
		// Ensure the cast collector is reset
		collector.reset();
		
		// Perform the linear cast
		m_simpleShapePhantom->setPositionAndLinearCast(ray.m_from, input, collector, HK_NULL);
		

		//
		//	Check if there is a hit and draw results
		//
		if ( collector.hasHit() )
		{
			hkReal hitFraction = collector.getHitContact().getDistance();
			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4( ray.m_from, ray.m_to, hitFraction );		

			HK_DISPLAY_LINE( ray.m_from, intersectionPointWorld, hkColor::YELLOW );
			HK_DISPLAY_ARROW( intersectionPointWorld, collector.getHitContact().getNormal(), hkColor::YELLOW );

			// reposition this purely for display - it shows where the sphere ended up
			m_simpleShapePhantom->setPosition(intersectionPointWorld);
		}
	}

	// Display the legend
	{
		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText("Red: Raycasts", 20, h-50, (hkUint32)hkColor::RED, 1);
		m_env->m_textDisplay->outputText("Yellow: Linear sphere cast", 20, h-30, (hkUint32)hkColor::YELLOW, 1);
	}
	
	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A basic sampled heightfield is created and oriented so that the Z-axis is up." \
" Two raycasts and one linear sphere cast are fired, and the intersection points and normals are drawn." \
" Note that the linear sphere cast approximation gives a penetrating result.";

HK_DECLARE_DEMO(HeightFieldRayVsLinearCastDemo, HK_DEMO_TYPE_PRIME, "Issues with heightfield raycasts and linear casts", helpString);

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

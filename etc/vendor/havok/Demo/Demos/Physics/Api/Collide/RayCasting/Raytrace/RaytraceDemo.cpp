/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/Raytrace/RaytraceDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>



#include <Graphics/Common/hkGraphics.h> 
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>

static const int MY_STACK_SIZE = 3500000;

enum RayTraceDemoType
{
	RAY_TRACE_TEAPOT,
	RAY_TRACE_SIMPLE_LANDSCAPE,
	RAY_TRACE_COMPLEX_LANDSCAPE,
};

struct RayTraceVariant
{
	const char*	 m_name;
	RayTraceDemoType m_demoType;
	const char* m_details;
};

static const RayTraceVariant g_variants[] =
{
	{ "Raytraced Teapot, 4000 Rays", RAY_TRACE_TEAPOT, "Raytrace a teapot using MOPP" },
	{ "Raytraced Landscape, 4800 Triangles, 4000 Rays", RAY_TRACE_SIMPLE_LANDSCAPE, "Raytrace a simple landscape using MOPP" },
	{ "Raytraced Landscape, 79000 Triangles, 4000 Rays", RAY_TRACE_COMPLEX_LANDSCAPE, "Raytrace a complex landscape using MOPP" },
};


RayTraceDemo::RayTraceDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env)
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, false); //'The system has requested a heap allocation on stack overflow.'

	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 10.0f, -30.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();
		setupGraphics();
	}

	// Increase the stack size.
	m_oldStack = hkThreadMemory::getInstance().getStack();
	m_stackBuffer = hkAllocate<char>( MY_STACK_SIZE, HK_MEMORY_CLASS_DEMO );
	hkThreadMemory::getInstance().setStackArea( m_stackBuffer, MY_STACK_SIZE );

	m_landscapeContainer = HK_NULL;
	const RayTraceVariant& variant =  g_variants[m_variantId];
	m_rotateCounter = 0;

	//
	// Load the teapot and create a fixed rigid body
	//
	if (variant.m_demoType == RAY_TRACE_TEAPOT)
	{
		hkVector4 tkScaleFactor(1, 1, 1);
		hkString fullname("Resources/Physics/Tk/Teapot/");
		fullname += "teapot.tk";

		m_shape = GameUtils::loadTK2MOPP(fullname.cString(),tkScaleFactor, -1.0f);
		HK_ASSERT2(0x6af24378, m_shape,"TK file failed to load to MOPP in GameUtils::loadTK2MOPP.");

		hkpRigidBodyCinfo ci;
		ci.m_shape =  m_shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;

		hkpRigidBody* entity = new hkpRigidBody(ci);
		m_world->addEntity(entity)->removeReference();
	}
	else
	{
		int landscapeIndex;
		if(variant.m_demoType == RAY_TRACE_SIMPLE_LANDSCAPE)
		{
			landscapeIndex = LandscapeRepository::getInstance().getLandscapeIndexByName("simpleFlatLandOneMetreTris");
		}
		else
		{
			landscapeIndex = LandscapeRepository::getInstance().getLandscapeIndexByName("defaultFlatLandOneMetreTris");
		}
		m_landscapeContainer = LandscapeRepository::getInstance().getLandscape(landscapeIndex);

		m_shape = m_landscapeContainer->m_shape;
		m_shape->addReference();

		hkpRigidBodyCinfo ci;
		ci.m_shape =  m_shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;

		hkpRigidBody* entity = new hkpRigidBody(ci);

		m_world->addEntity(entity)->removeReference();
				
		setupDefaultCameras( env, m_landscapeContainer->m_cameraFrom, m_landscapeContainer->m_cameraTo, hkVector4(0,1,0) );
	}

	//
	// Allocate the canvas 
	//
	m_canvasData = hkAlignedAllocate<unsigned char> (64, CANVAS_HEIGHT*CANVAS_WIDTH*4, HK_MEMORY_CLASS_DEMO);

	//
	//	Create the texture
	//
	{
		m_context = env->m_window->getContext();
		m_texture = hkgTexture::create( m_context );
		m_texture->allocateSurface( CANVAS_WIDTH, CANVAS_HEIGHT, true, false, m_canvasData );
		m_textureRealized = false;
	}
	m_texture->realize(true);
	m_world->unlock();
}

void RayTraceDemo::drawTexture( hkgTexture* texture )
{
	hkgViewport* curView = m_env->m_window->getCurrentViewport();
	hkgViewport* orthoView = m_env->m_window->getWindowOrthoView();  //2D ortho, width by height
	float windowHeight = (float)m_env->m_window->getHeight();

	orthoView->setAsCurrent(m_context); // 2D

	m_context->lock();

	m_context->setDepthReadState(false);
	m_context->setDepthWriteState(true);
	m_context->setLightingState(false);
    m_context->setCurrentSoleTexture( texture, HKG_TEXTURE_DECAL );	
	m_context->setTexture2DState( true ); // turn on textures for this
	m_context->setBlendState( false );

	float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
	m_context->setCurrentColor4( white );
	
	float p[3];
	float uv[2];
	p[2] = -0.01f; //depth (0..-2)

	float tl[3];
	float lr[3];

	hkReal offset = 20;
	tl[0] = 0.0f + offset;
	tl[1] = windowHeight  - offset;

	lr[0] = float( CANVAS_WIDTH * TEXTURE_SCALE ) + offset;
	lr[1] = windowHeight - float( CANVAS_HEIGHT * TEXTURE_SCALE ) - offset;

	m_context->beginGroup( HKG_IMM_TRIANGLE_LIST );

		p[0] = tl[0]; p[1] = tl[1]; 
		uv[0] = 0.0f; uv[1] = 0.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );
		
		p[0] = tl[0]; p[1] = lr[1]; 
		uv[0] = 0.0f; uv[1] = 1.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = tl[1]; 
		uv[0] = 1.0f; uv[1] = 0.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );

		p[0] = tl[0]; p[1] = lr[1];
		uv[0] = 0.0f; uv[1] = 1.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = tl[1]; 
		uv[0] = 1.0f; uv[1] = 0.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );

		p[0] = lr[0]; p[1] = lr[1]; 
		uv[0] = 1.0f; uv[1] = 1.0f;
		m_context->setCurrentTextureCoord( uv );
		m_context->setCurrentPosition( p );

	m_context->endGroup();
	m_context->setCurrentSoleTexture( HK_NULL, HKG_TEXTURE_DECAL );	

	curView->setAsCurrent(m_context); // reset

	m_context->unlock();
}

RayTraceDemo::~RayTraceDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, true); 

	hkReferencedObject::lockAll();
	hkAlignedDeallocate<unsigned char>(m_canvasData);
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;
	m_shape->removeReference();
	m_context->lock();
	m_texture->free();
	m_texture->release();
	m_context->unlock();

	hkReferencedObject::unlockAll();
	hkDeallocate( m_stackBuffer );
	hkThreadMemory::getInstance().setStackArea( m_oldStack.m_current, m_oldStack.getFreeBytes() );
}


hkDemo::Result RayTraceDemo::stepDemo()
{

	if ( m_env->m_window->getMouse().getButtonState() != 0 )
	{
		m_rotateCounter = 60;
	}
	m_rotateCounter--;


	// rotate the camera viewpoint
	if (m_rotateCounter <= 0)
	{

		hkgWindow* w = m_env->m_window;
		hkgCamera* c = w->getViewport(0)->getCamera();

		hkVector4 from; c->getFrom( &from(0) );
		hkVector4 to; c->getTo(&to(0));
		hkVector4 up; c->getUp(&up(0));
		up.set(0,1,0);


		hkVector4 d; d.setSub4(to, from);
		hkQuaternion q( up, .03f);
		d.setRotatedDir(q, d);
		from.setSub4(to, d);

		setupDefaultCameras(m_env, from, to, up );
	}


	//
	//	Do our raytrace
	//
	{
		//
		//	Get our camera information
		//
		hkVector4 from;
		hkVector4 zOffset;
		hkVector4 upOffset;
		hkVector4 rightOffset;
		{
			from.setZero4();
			zOffset.setZero4();
			upOffset.setZero4();
			rightOffset.setZero4();

			hkgWindow* w = m_env->m_window;
			hkgCamera* c = w->getViewport(0)->getCamera();

			c->getFrom( &from(0) );
			c->getDir( &zOffset(0) );
			c->getUp( &upOffset(0) );

			rightOffset.setCross( upOffset, zOffset );
		}

		//
		// global info
		//
		hkVector4 lightPos; lightPos.setAddMul4( from, upOffset, 10 );
		hkReal lightStrength = 150.0f;

		// reflect the light in the normal graphics too 
		{
			hkVector4 lightDir; 
			hkVector4 lightTo; 
			m_env->m_window->getViewport(0)->getCamera()->getDir( &lightTo(0) );
			lightTo.add4( from ); 
			lightDir.setSub4( lightTo, lightPos);
			setSoleDirectionLight(m_env, &lightDir(0), 0xffffffff);
		}


		//
		//	Prepare the data for iterations
		//
		{
			hkgWindow* w = m_env->m_window;
			hkgCamera* c = w->getViewport(0)->getCamera();
			hkReal fa = c->getFar();
			zOffset.mul4( fa );
			upOffset.mul4( fa * c->getFOV() / 45 * CANVAS_HEIGHT/ CANVAS_WIDTH);
			rightOffset.mul4( fa * c->getFOV() / 45 );
		}

		hkpShapeRayCastInput ray;
		ray.m_from = from;
		ray.m_to.setAdd4( from, zOffset );
		ray.m_to.addMul4( 0.5f, upOffset );
		ray.m_to.addMul4( 0.5f, rightOffset );
		upOffset.mul4( 1.0f / CANVAS_HEIGHT );
		rightOffset.mul4( 1.0f / CANVAS_WIDTH );

		m_numPrimaryRays = 0;
		m_numShadowRays = 0;

		hkpShapeRayCastInput lightRay;
		lightRay.m_from = lightPos;

		//
		//	Do the raytracer
		//
		HK_TIMER_BEGIN("Raytrace Frame", HK_NULL);
		m_stopwatch.start();
		{
			hkpShapeRayCastOutput output;
			hkpShapeRayCastOutput output2;
			int* data = reinterpret_cast<int*>(m_texture->getDataPointer());
			hkVector4 yStart = ray.m_to;

			for (int y = 0; y < CANVAS_HEIGHT; y++)
			{
				ray.m_to = yStart;
				for (int x = 0; x < CANVAS_WIDTH; x++ )
				{
				#if (HK_ENDIAN_BIG==1) 
					data[0] = 0x3f3f7fFF; // background (rgba in that byte order)
				#else
					data[0] = 0xFF7f3f3f; // background
				#endif
				
					output.reset();
					m_numPrimaryRays++;
					m_shape->castRay( ray, output );
					if ( output.hasHit() )
					{
						hkReal brightness = .4f;

						hkVector4 hitPos; hitPos.setInterpolate4( ray.m_from, ray.m_to, output.m_hitFraction );
						hkVector4 lightDir; lightDir.setSub4( lightPos, hitPos );

						// get the direct light info
						{
							hkReal lightDot = output.m_normal.dot3( lightDir );
							if ( lightDot > 0.0f )
							{
								//
								//	Do shadows
								//
								output2.reset();
								lightRay.m_to.setInterpolate4( lightRay.m_from,hitPos, 0.999f);

								m_numShadowRays++;
								if (!m_shape->castRay( lightRay, output2 ))
								{
									hkReal lightDirInvLen = lightDir.lengthInverse3();
									brightness += lightDot * lightDirInvLen * lightStrength;

									// add some glare
									{
										hkVector4 reflectedDir; reflectedDir.setAddMul4( lightDir, output.m_normal, -2.0f * lightDot );
										hkVector4 rayDirection; rayDirection.setSub4( ray.m_to, ray.m_from );
										hkReal dot2 = reflectedDir.dot3( rayDirection );
										if ( dot2 > 0 )
										{
											dot2 *= lightDirInvLen;
											dot2 *= dot2;
											dot2 /= rayDirection.lengthSquared3();
											dot2 *= dot2;
											dot2 *= dot2;
											dot2 *= dot2;
											dot2 *= dot2;
											brightness += dot2 * lightStrength * 2;
										}
									}

								}
							}

						}


						unsigned s = (unsigned)hkMath::hkToIntFast( brightness );
						if ( s > 255 ) s = 255;

						#if (HK_ENDIAN_BIG==1) 
							data[0] = 0x000000ff | (s<<24) | (s<<16) | (s<<8); // rgba in that byte order
						#else
							data[0] = 0xff000000 | (s<<16) | (s<<8) | s;
						#endif
					}
					data++;
					ray.m_to.sub4( rightOffset );
				}
				yStart.sub4( upOffset );
			}
		}
		m_stopwatch.stop();
		HK_TIMER_END();
	}

	//
	// Output statistics
	//
	{
		hkReal timePerRayuSecs = m_stopwatch.getSplitSeconds() * 1e6f / (m_numPrimaryRays + m_numShadowRays);
		m_stopwatch.reset();
		char buf[512];
		hkString::snprintf(buf, 512, "Primary Rays:%d\nShadow Rays:%d\nAverage time per ray:%f microsecs", m_numPrimaryRays, m_numShadowRays, timePerRayuSecs);
		m_env->m_textDisplay->outputText(buf, 20, 350);
	}

	//
	//	Draw the texture (really slow as we don't have a quicker reinit for the textures / direct write yet in the HKG)
	//
	m_env->m_window->getContext()->lock();
	if (m_textureRealized)
	{
		m_texture->free();
	}
	m_texture->realize(true);
	m_env->m_window->getContext()->unlock();

	m_textureRealized = true;
		
	return DEMO_OK;
}

void RayTraceDemo::postRenderDisplayWorld(hkgViewport* v)
{
	drawTexture( m_texture );
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"We use our MOPP technology to quickly pull out appropriate triangles from the mesh " \
"The result is a real time raytracer.";

// Landscape variants get run for stats, teapot doesn't
HK_DECLARE_DEMO_VARIANT( RayTraceDemo, HK_DEMO_TYPE_PRIME                     , g_variants[0].m_name, 0, helpString, g_variants[0].m_details);
HK_DECLARE_DEMO_VARIANT( RayTraceDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_STATS, g_variants[1].m_name, 1, helpString, g_variants[1].m_details);
HK_DECLARE_DEMO_VARIANT( RayTraceDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_STATS, g_variants[2].m_name, 2, helpString, g_variants[2].m_details);

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

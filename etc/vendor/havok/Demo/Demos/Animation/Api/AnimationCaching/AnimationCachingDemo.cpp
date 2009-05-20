/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/AnimationCaching/AnimationCachingDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene data
#include <Common/SceneData/Scene/hkxScene.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>
#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>

// For debug drawing and batch registration
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>


struct CacheDemoVariant
{
	int m_buckets[2]; /// Number of buckets to use for this variant (two cache pools are used)
	const char* m_name;
	const char* m_details;
	hkaAnimation::AnimationType m_compressionType;
};

static const CacheDemoVariant g_variants[] =
{
	{ { 0, 0 }, "Wavelet No Cache", "Wavelet compression without using a cache", hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION },
	{ { 2, 2 }, "Wavelet Small Cache", "Wavelet compression using a small cache", hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION },
	{ { 3, 5 }, "Wavelet Medium Cache", "Wavelet compression using a medium cache", hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION },
	{ { 10,6 }, "Wavelet Large Cache", "Wavelet compression using a large cache", hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION },
	{ { 0, 0 }, "Delta No Cache", "Delta compression without using a cache", hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION },
	{ { 2, 2 }, "Delta Small Cache", "Delta compression using a small cache", hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION },
	{ { 3, 5 }, "Delta Medium Cache", "Delta compression using a medium cache", hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION },
	{ { 10,6 }, "Delta Large Cache", "Delta compression using a large cache", hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION },
	{ { 0, 0 }, "Spline No Cache", "Spline compression without using a cache", hkaAnimation::HK_SPLINE_COMPRESSED_ANIMATION },
	{ { 0, 0 }, "Uncompressed No Cache", "Uncompressed without using a cache", hkaAnimation::HK_INTERLEAVED_ANIMATION }
};

// Put any animations to test in this list
// NUM_ANIMS will need to be updated if changed
static const char* animationAssets[ NUM_ANIMS ] =
{
	"Resources/Animation/HavokGirl/hkWalkLoop.hkx",
	"Resources/Animation/HavokGirl/hkRunLoop.hkx",
	"Resources/Animation/HavokGirl/hkIdle.hkx",
	"Resources/Animation/HavokGirl/hkJumpLandLoop.hkx"
};

static const char* rigAsset = "Resources/Animation/HavokGirl/hkRig.hkx";

AnimationCachingDemo::AnimationCachingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	// Disable reports: 
	if(m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x36118e94, false); 		
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(  -15.0f, -15.0f, 4.0f);
		hkVector4 to  (  0.0f,  0.0f,   -4.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	// create a cache to handle the caching of animation data
	{
		hkaChunkPoolCinfo cpCinfo[2];

		cpCinfo[0].m_buckets = g_variants[ env->m_variantId ].m_buckets[ 0 ];
		cpCinfo[0].m_slots = 3;
		cpCinfo[0].m_chunkSize = 6 * 1024;

		cpCinfo[1].m_buckets = g_variants[ env->m_variantId ].m_buckets[ 1 ];
		cpCinfo[1].m_slots = 2;
		cpCinfo[1].m_chunkSize = 12 * 1024;

		// The cache size in bytes is equal to m_buckets * m_slots * m_chunkSize

		hkaDefaultChunkCacheCinfo cacheCinfo;
		cacheCinfo.m_numberOfCachePools = 2;
		cacheCinfo.m_cachePools.setSize( 2 );
		cacheCinfo.m_cachePools[0] = cpCinfo[0];
		cacheCinfo.m_cachePools[1] = cpCinfo[1];

		// Create a cache only if the number of buckets is nonzero
		// Some variants of this demo do not use a cache.
		// N.B. This is the default chunk cache which is not threadsafe (but invokes no critical sections
		// so is fastest when used in a single threaded environment). For sharing a cache across
		// multiple threads, the Animation/Animation/Playback/Cache/Multithreaded/hkaMultithreadedChunkCache
		// class is provided - see also the Demo\StandAloneDemos\ConsoleExampleAnimationMT for an example use.
		m_cache = ( cpCinfo[0].m_buckets ) ? new hkaDefaultChunkCache( cacheCinfo ) : HK_NULL;
	}

	m_loader = new hkLoader();

	// load the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( rigAsset );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2( 0x27343435, ac && ( ac->m_numSkeletons > 0 ), "No skeleton loaded" );
		m_skeleton = ac->m_skeletons[0];
	}

	// load the animations and bindings
	for ( int i = 0; i < NUM_ANIMS; i++ )
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( animationAssets[ i ] );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2( 0x27343435, ac && ( ac->m_numAnimations > 0 ), "No animation loaded" );
		{
			// create the delta compressed animation
			m_animation[i] = createCompressedAnimation( ac->m_animations[0], g_variants[ m_env->m_variantId ].m_compressionType );

			// binding
			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			m_binding[i] = ac->m_bindings[0];
			m_binding[i]->m_animation = m_animation[i];
		}
	}

	for ( int j = 0; j < NUM_SKELETONS; ++j )
	{
		// Create an animation control
		hkaDefaultAnimationControl* dac = new hkaDefaultAnimationControl( m_binding[ j % NUM_ANIMS ] );
		dac->setLocalTime( j * .1f );

		// Create a new animated skeleton
		hkaAnimatedSkeleton* skeleton = new hkaAnimatedSkeleton( m_skeleton );
		skeleton->setReferencePoseWeightThreshold( 0.01f );
		m_activeSkeletons.pushBack( skeleton );

		// Bind the control to the skeleton
		skeleton->addAnimationControl( dac );

		// The animated skeleton now owns the control
		dac->removeReference();
	}

	// Set the timers to 0
	m_totalSampleTime = 0;
	m_totalSampleCount = 0;
	m_useCaching = (m_cache != HK_NULL);

	setupGraphics( );
}

AnimationCachingDemo::~AnimationCachingDemo()
{
	// Re-enable report
	hkError::getInstance().setEnabled(0x36118e94, true);

	// will remove all sections
	delete m_loader;

	// Delete the active skeletons
	for (int s=0; s< m_activeSkeletons.getSize(); s++)
	{
		m_activeSkeletons[s]->removeReference();
	}

	// For safety, we 'unload' each animation from the cache since we are about to delete them.
	// This is not necessary in this demo as we are also about to delete the cache, but it is good practice.
	if(m_cache)
	{
		for (int a=0; a < NUM_ANIMS; a++)
		{
			m_animation[a]->clearAllCacheKeys(m_cache);
		}
	}

	for (int a=0; a < NUM_ANIMS; a++)
	{
		delete m_animation[a];
	}

	// free cache
	if(m_cache)
	{
		m_cache->removeReference();
	}
}


hkaAnimation* AnimationCachingDemo::createCompressedAnimation( hkaAnimation* raw, hkEnum< hkaAnimation::AnimationType, hkInt32 > type )
{
	if ( type == hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION )
	{
		// delta compress the raw animation
		hkaDeltaCompressedAnimation::CompressionParams params;
		params.m_blockSize = 16;
		params.m_quantizationBits = 8;

		// create the animation
		return new hkaDeltaCompressedAnimation( *(hkaInterleavedUncompressedAnimation*)raw, params );
	}

	if ( type == hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION )
	{
		// delta compress the raw animation
		hkaWaveletCompressedAnimation::CompressionParams params;
		params.m_blockSize = 16;
		params.m_quantizationBits = 8;

		// create the animation
		return new hkaWaveletCompressedAnimation( *(hkaInterleavedUncompressedAnimation*)raw, params );
	}

	if ( type == hkaAnimation::HK_SPLINE_COMPRESSED_ANIMATION )
	{
		hkaSplineCompressedAnimation::TrackCompressionParams tparams;
		hkaSplineCompressedAnimation::AnimationCompressionParams aparams;

		// create the animation with default settings
		return new hkaSplineCompressedAnimation( *(hkaInterleavedUncompressedAnimation*)raw, tparams, aparams ); 
	}

	if ( type == hkaAnimation::HK_INTERLEAVED_ANIMATION )
	{
		return new hkaInterleavedUncompressedAnimation( *(hkaInterleavedUncompressedAnimation*)raw );
	}

	return 0;
}


hkDemo::Result AnimationCachingDemo::stepDemo()
{
	if(m_cache)
	{
		hkString s;
		s += "Press \221 to turn cache off/on\n\n";
		s += "Press \222 to flush the cache's pools\n\n";
		s += "Press \223 to print cache statistics to the file \"cacheStats.txt\"\n  [Debug build only]";
		m_env->m_textDisplay->outputText(s.cString(), 20, 60);
	}

	// turn on off cache
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) && m_cache )
	{
		m_useCaching = !m_useCaching;

		// flush the cache pools
		m_cache->flushCachePool( 0 );
		m_cache->flushCachePool( 1 );

		m_totalSampleTime = 0;
		m_totalSampleCount = 0;
	}

	hkaDefaultChunkCache* cache = (m_useCaching?m_cache:HK_NULL);

	// update flushing cache pools with user input
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2 ) && cache )
	{
		// flush the cache pools
		cache->flushCachePool( 0 );
		cache->flushCachePool( 1 );

		// display message to notify user of cache pool flush
		char buf[64];
		hkString::sprintf( buf, "The cache's pools have been flushed!" );
		m_env->m_textDisplay->outputText( buf, 20, 200, 0xffffffff, 140);

	}

	// print cache statistics with user input
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_3 ) && cache )
	{
		// display message to notify user of cache pool flush
		char buf[64];
		hkString::sprintf( buf, "The cache's statistics have been printed." );
		m_env->m_textDisplay->outputText( buf, 20, 200, 0xffffffff, 140);

		// print cache statistics
		hkOstream file("cacheStats.txt");
		cache->printCacheStats( &file );
	}

	hkPseudoRandomGenerator rand(12345);

	hkStopwatch stopwatch;

	for (int i = 0; i < m_activeSkeletons.getSize(); ++i )
	{
		hkaAnimatedSkeleton* inst = m_activeSkeletons[i];

		// Advance the animation
		inst->stepDeltaTime( m_timestep );

		hkaPose pose (inst->getSkeleton());

		// Time the sampling of the animations
		stopwatch.start();
		inst->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin(), cache );	// using our cache
		stopwatch.stop();

		// Draw
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set( (i % 6) * 2.0f - 12, (i / 6) * 2.0f - 12, 0 );
		AnimationUtils::drawPose( pose, worldFromModel, rand.getRand32() | 0xffc0c0c0 );
	}

	// Accumulate the total sampling times for all animations this frame
	m_totalSampleTime += stopwatch.getElapsedTicks();
	m_totalSampleCount++;

	// Print the timing information
	{
		hkReal averageTimeMS = hkReal( m_totalSampleTime ) / hkReal( hkStopwatch::getTicksPerSecond() ) / hkReal( m_totalSampleCount ) * 1000.0f;
		char buf[ 1024 ];
		hkString::sprintf( buf, "Mean Sampling time (%d anims): %.2fms", m_activeSkeletons.getSize(), averageTimeMS );

		m_env->m_textDisplay->outputText( buf, 20, 380, 0xffffffff, 1 );
		if(m_cache)
		{
			m_env->m_textDisplay->outputText( m_useCaching?"Cache ON":"Cache OFF", 20, 400, 0xffffffff, 1 );
		}
	}

	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Demo showing how to setup and use a hkaChunkCache to improve animation decompression performance.\n";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( AnimationCachingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, CacheDemoVariant, g_variants, helpString );

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

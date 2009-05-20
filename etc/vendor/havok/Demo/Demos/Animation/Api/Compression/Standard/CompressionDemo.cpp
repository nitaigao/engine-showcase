/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Compression/Standard/CompressionDemo.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Reflection/hkClass.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>

// For debug drawing and batch registration
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>


#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>

#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.h>

static hkVector4 OFFSET( 0.3f, 0, 0, 0 );

static hkUint32 hkCalcAnimSize(hkaWaveletCompressedAnimation& anim)
{
	return
			sizeof(hkaWaveletCompressedAnimation) +
			anim.m_numDataBuffer;
}

static hkUint32 hkCalcAnimSize(hkaDeltaCompressedAnimation& anim)
{
	return
			sizeof(hkaDeltaCompressedAnimation) +
			anim.m_numDataBuffer;
}

static hkUint32 hkCalcAnimSize(hkaSplineCompressedAnimation& anim)
{
	return anim.getSizeInBytes();
}

class TestRecord
{
public:

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, TestRecord);

	hkaAnimationBinding* m_binding;
	hkaAnimatedSkeleton* m_animatedSkeleton;
	const char* m_name;
	int m_bytes;
	hkVector4 m_offset;
	hkStopwatch m_stopwatch;
};

void CompressionDemo::AddAnimation(const char* name, hkaAnimation* anim, hkaAnimationBinding* binding, int skipOffset )
{
	TestRecord* rec = new TestRecord;

	rec->m_name = name;
	rec->m_bytes = 0;

	// Set up the binding
	rec->m_binding = new hkaAnimationBinding();
	hkString::memCpy(rec->m_binding, binding, sizeof(hkaAnimationBinding) );
	rec->m_binding->m_animation = anim;

	// Create an animation control
	hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl(rec->m_binding);

	// Create a new animated skeleton
	rec->m_animatedSkeleton = new hkaAnimatedSkeleton( m_skeleton );
	rec->m_animatedSkeleton->setReferencePoseWeightThreshold(0.001f);

	// Bind the control to the skeleton
	rec->m_animatedSkeleton->addAnimationControl( ac );

	// The animated skeleton now owns the control
	ac->removeReference();

	rec->m_offset = OFFSET;
	rec->m_offset.mul4( hkReal(m_animationRecords.getSize() + skipOffset) );

	m_animationRecords.pushBack(rec);
}

static const int NUM_SPLINE = 5;
static const char* splineNames[NUM_SPLINE];
static hkaSplineCompressedAnimation::TrackCompressionParams splineParams[NUM_SPLINE];

static const int NUM_DELTA = 5;
static const char* deltaNames[NUM_DELTA];
static hkaDeltaCompressedAnimation::CompressionParams deltaParams[NUM_DELTA];

static const int NUM_WAVELET = 5;
static const char* waveletNames[NUM_WAVELET];
static hkaWaveletCompressedAnimation::CompressionParams waveletParams[NUM_WAVELET];

CompressionDemo::CompressionDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env), m_currentAnimation(0)
{
	
	// Disable reports: 									
	if( m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x36118e94, false); 
		hkError::getInstance().setEnabled(0x432434a4, false); 
		hkError::getInstance().setEnabled(0x432434a5, false); 
	}

	m_useCache = false;

	// Rotation tolerance for track analysis
	const hkReal rTol = 0.01f;

	//Spline compression
	{
		splineNames[0] = "Spline Tolerance 0.001, 40 Bit Quaternions";
		hkaSplineCompressedAnimation::TrackCompressionParams p;
		p.m_rotationTolerance = 0.001f;
		p.m_rotationQuantizationType = hkaSplineCompressedAnimation::TrackCompressionParams::THREECOMP40;
		splineParams[0] = p;
	}

	{
		splineNames[1] = "Spline Tolerance 0.005, 40 Bit Quaternions";
		hkaSplineCompressedAnimation::TrackCompressionParams p;
		p.m_rotationTolerance = 0.005f;
		p.m_rotationQuantizationType = hkaSplineCompressedAnimation::TrackCompressionParams::POLAR32;
		splineParams[1] = p;
	}

	{
		splineNames[2] = "Spline Tolerance 0.050, 40 Bit Quaternions";
		hkaSplineCompressedAnimation::TrackCompressionParams p;
		p.m_rotationTolerance = 0.050f;
		p.m_rotationQuantizationType = hkaSplineCompressedAnimation::TrackCompressionParams::THREECOMP40;
		splineParams[2] = p;
	}

	{
		splineNames[3] = "Spline Tolerance 0.001, 48 Bit Quaternions";
		hkaSplineCompressedAnimation::TrackCompressionParams p;
		p.m_rotationTolerance = 0.001f;
		p.m_rotationQuantizationType = hkaSplineCompressedAnimation::TrackCompressionParams::THREECOMP48;
		splineParams[3] = p;
	}

	{
		splineNames[4] = "Spline Tolerance 0.001, 32 Bit Quaternions";
		hkaSplineCompressedAnimation::TrackCompressionParams p;
		p.m_rotationTolerance = 0.001f;
		p.m_rotationQuantizationType = hkaSplineCompressedAnimation::TrackCompressionParams::POLAR32;
		splineParams[4] = p;
	}

	// Delta compression
	{
		deltaNames[0] = "Delta 16 bit, Blocksize 8";
		hkaDeltaCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 16;
		p.m_blockSize = 8;
		p.m_rotationTolerance = rTol;
		deltaParams[0] = p;
	}

	{
		deltaNames[1] = "Delta 10 bit, Blocksize 8";
		hkaDeltaCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 10;
		p.m_blockSize = 8;
		p.m_rotationTolerance = rTol;
		deltaParams[1] = p;
	}

	{
		deltaNames[2] = "Delta 8 bit, Blocksize 8";
		hkaDeltaCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_rotationTolerance = rTol;
		deltaParams[2] = p;
	}

	{
		deltaNames[3] = "Delta 6 bit, Blocksize 8";
		hkaDeltaCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 6;
		p.m_blockSize = 8;
		p.m_rotationTolerance = rTol;
		deltaParams[3] = p;
	}

	{
		deltaNames[4] = "Delta 4 bit, Blocksize 8";
		hkaDeltaCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 4;
		p.m_blockSize = 8;
		p.m_rotationTolerance = rTol;
		deltaParams[4] = p;
	}



	//Wavelet compression
	{
		waveletNames[0] = "Wavelet 8 bit, Blocksize 8, Tolerance 0.0";
		hkaWaveletCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_preserve = 0;
		p.m_rotationTolerance = 0.0;
		waveletParams[0] = p;
	}

	{
		waveletNames[1] = "Wavelet 8 bit, Blocksize 8, Tolerance 0.0001";
		hkaWaveletCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_preserve = 0;
		p.m_rotationTolerance = 0.0001f;
		waveletParams[1] = p;
	}

	{
		waveletNames[2] = "Wavelet 8 bit, Blocksize 8, Tolerance 0.0005";
		hkaWaveletCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_preserve = 0;
		p.m_rotationTolerance = 0.0005f;
		waveletParams[2] = p;
	}

	{
		waveletNames[3] = "Wavelet 8 bit, Blocksize 8, Tolerance 0.001";
		hkaWaveletCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_preserve = 0;
		p.m_rotationTolerance = 0.001f;
		waveletParams[3] = p;
	}

	{
		waveletNames[4] = "Wavelet 8 bit, Blocksize 8, Tolerance 0.005";
		hkaWaveletCompressedAnimation::CompressionParams p;
		p.m_quantizationBits = 8;
		p.m_blockSize = 8;
		p.m_preserve = 0;
		p.m_rotationTolerance = 0.005f;
		waveletParams[4] = p;
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from( 0, -6, 1);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	// create a cache to handle the caching of animation data
	{
		hkaChunkPoolCinfo cpCinfo[1];		// total cache memory :

		// For all of the run animations : 4k per line 20 lines = 80k
		cpCinfo[0].m_buckets = 1;
		cpCinfo[0].m_slots = 20;
		cpCinfo[0].m_chunkSize = 4 * 1024;

		hkaDefaultChunkCacheCinfo cacheCinfo;
		cacheCinfo.m_numberOfCachePools = 1;
		cacheCinfo.m_cachePools.setSize( 1 );
		cacheCinfo.m_cachePools[0] = cpCinfo[0];

		m_cache = new hkaDefaultChunkCache( cacheCinfo );
	}

	m_loader = new hkLoader();

	// Get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get the animation and the binding
	hkaInterleavedUncompressedAnimation* rawAnimation;
	hkaAnimationBinding* binding;
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		rawAnimation = static_cast<hkaInterleavedUncompressedAnimation*>(ac->m_animations[0]);

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		binding = ac->m_bindings[0];
	}

	// Raw
	AddAnimation("Original Raw", rawAnimation, binding, 0);
	m_originalSize = rawAnimation->m_numTransforms * sizeof(hkQsTransform);
	m_animationRecords.back()->m_bytes = m_originalSize;

	// Spline compress
	for (int d=0; d < NUM_SPLINE ; d++)
	{
		hkaSplineCompressedAnimation* anim =  new hkaSplineCompressedAnimation(*rawAnimation, splineParams[d], hkaSplineCompressedAnimation::AnimationCompressionParams() );
		AddAnimation(splineNames[d], anim, binding, 1);
		m_animationRecords.back()->m_bytes = hkCalcAnimSize(*anim);
	}

	// Wavelet compress
	for (int w=0; w < NUM_WAVELET ; w++)
	{
		hkaWaveletCompressedAnimation* anim =  new hkaWaveletCompressedAnimation(*rawAnimation, waveletParams[w]);
		AddAnimation(waveletNames[w], anim, binding, 2);
		m_animationRecords.back()->m_bytes = hkCalcAnimSize(*anim);
	}

	// Delta compress
	for (int d=0; d < NUM_DELTA ; d++)
	{
		hkaDeltaCompressedAnimation* anim =  new hkaDeltaCompressedAnimation(*rawAnimation, deltaParams[d]);
		AddAnimation(deltaNames[d], anim, binding, 3);
		m_animationRecords.back()->m_bytes = hkCalcAnimSize(*anim);
	}

	setupGraphics( );

}

CompressionDemo::~CompressionDemo()
{
	// Re-enable reports
	hkError::getInstance().setEnabled(0x36118e94, true); 
	hkError::getInstance().setEnabled(0x432434a4, true);
	hkError::getInstance().setEnabled(0x432434a5, true);

	m_cache->removeReference();

	// Delete the active skeletons
	for (int s=0; s< m_animationRecords.getSize(); s++)
	{
		if (s > 0)
		{
			delete m_animationRecords[s]->m_binding->m_animation;
		}
		delete m_animationRecords[s]->m_binding;
		m_animationRecords[s]->m_animatedSkeleton->removeReference();

		delete m_animationRecords[s];
	}

	delete m_loader;
}

hkDemo::Result CompressionDemo::stepDemo()
{
	// Point local arrays on the scratchpad

	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) )
	{
		m_currentAnimation = (m_currentAnimation-1+m_animationRecords.getSize())%(m_animationRecords.getSize());
		hkVector4 at = m_animationRecords[m_currentAnimation]->m_offset;
		lookAt( at );
	}

	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) )
	{
		m_currentAnimation = (m_currentAnimation+1)%(m_animationRecords.getSize());
		hkVector4 at = m_animationRecords[m_currentAnimation]->m_offset;
		lookAt( at );
	}

	// Turn caching on / off
	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) )
	{
		m_useCache = !m_useCache;
	}

	// print cache statistics with user input
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_3 ) )
	{
		// display message to notify user of cache pool flush
		char buf[64];
		hkString::sprintf( buf, "The cache's statistics have been saved." );
		m_env->m_textDisplay->outputText( buf, 20, 180, 0xffffffff, 140);

		// print cache statistics
		hkOstream file("cacheStats.txt");
		m_cache->printCacheStats( &file );
	}

	hkaPose pose (m_skeleton);

	for (int i = 0; i < m_animationRecords.getSize(); ++i )
	{
		TestRecord* rec = m_animationRecords[i];
		hkaAnimatedSkeleton* inst = rec->m_animatedSkeleton;

		// Advance the animation
		rec->m_stopwatch.reset();
		rec->m_stopwatch.start();

		inst->stepDeltaTime( m_timestep );

		// Sample
		if ( m_useCache )
		{
			inst->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() , m_cache );	// using our cache
		}
		else
		{
			inst->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );
		}

		rec->m_stopwatch.stop();

		// Draw
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation = rec->m_offset;
		hkUint32 color = (i == m_currentAnimation) ? 0xffff0000 : 0xffffffff;
		AnimationUtils::drawPose( pose, worldFromModel, color );

		const hkReal heightOffset = -1.0f;
		const hkReal widthOffset = -0.20f;
		hkString label( rec->m_name );
		label.setAsSubstr( 0, label.indexOf( ' ', 0 ) );
		hkVector4& textPos = m_animationRecords[i]->m_offset;
		m_env->m_textDisplay->outputText3D( label, textPos(0) + widthOffset, textPos(1), textPos(2) + heightOffset, 0xffffffff );
	}

	TestRecord* rec = m_animationRecords[m_currentAnimation];

	char msg[2048];

	hkUint64 ticks = rec->m_stopwatch.getElapsedTicks();
	double tpf = double(ticks/rec->m_stopwatch.getNumTimings());
	double tpms = double(rec->m_stopwatch.getTicksPerSecond()/1000000);
	double mspf = tpf / tpms;
	hkString::sprintf(msg, "%s\nbytes:%d\nratio:%.2f:1\nmicrosecs/f:%f\ncaching:%s", rec->m_name, rec->m_bytes, (hkReal)m_originalSize / rec->m_bytes, mspf, ((m_useCache) ? "enabled" : "disabled") );
	const int h = m_env->m_window->getHeight();
	m_env->m_textDisplay->outputText( msg, 20, h-100, 0xffffffff);

	return DEMO_OK;
}

void CompressionDemo::lookAt( const hkVector4& to )
{
	hkgWindow* w = m_env->m_window;
	hkVector4 offset, from, oldTo;

	for(int i = 0; i < w->getNumViewports(); ++i)
	{
		hkgCamera* c = w->getViewport(i)->getCamera();

		c->getFrom( &from(0) );
		c->getTo( &oldTo(0) );
		offset.setSub4(to, oldTo);
		from.add4(offset);

		c->setFrom(&from(0));
		c->setTo(&to(0));
		c->orthogonalize();
		c->computeModelView();
		c->computeProjection();
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo takes a single raw animation and compresses it with different "
"compression settings. The compression ratio and decompression performance is shown "
"for each of the settings.\n"
" Press \221 or \220 to toggle between these.\n"
" Press \222 to toggle chunk caching on/off.\n"
" Press \223 to print the cache statistics to the file [Debug build only] \"cacheStats.txt\".\n"
"Simply export any of your own assets to estimate final game performance.";


HK_DECLARE_DEMO(CompressionDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Animation compression testbed.", helpString);

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

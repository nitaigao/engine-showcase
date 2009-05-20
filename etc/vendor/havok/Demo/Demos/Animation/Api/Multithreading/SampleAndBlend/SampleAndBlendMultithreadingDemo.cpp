/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Multithreading/SampleAndBlend/SampleAndBlendMultithreadingDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

#include <Common/Base/Monitor/Spu/hkSpuMonitorCache.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset management
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Skeletal Animation
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/Mirrored/hkaMirroredAnimation.h>
#include <Animation/Animation/Animation/Mirrored/hkaMirroredSkeleton.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>

#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Playback/Multithreaded/hkaMultithreadedAnimationUtils.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>

// For debug drawing
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>
#include <Demos/DemoCommon/Utilities/WindowedAverage/WindowedAverage.h>

#include <Common/Base/Spu/Util/hkSpuUtil.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobs.h>
#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobQueueUtils.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>

#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.h>
#include <Animation/Animation/Playback/Cache/Multithreaded/hkaMultithreadedChunkCache.h>

// Change the following values to add or change assets
#define NUM_SKELETONS 50
#define NUM_ANIMATIONS 2
#define NUM_BONES 50

static const char* RIG_FILE_NAME = "Resources/Animation/HavokGirl/hkRig.hkx";

static const char* ANIMATION_FILE_NAMES[ NUM_ANIMATIONS ] =
{
	"Resources/Animation/HavokGirl/hkRunLoop.hkx",
	"Resources/Animation/HavokGirl/hkRunTurnLLoop.hkx"
};

#define NUM_ANIMATION_TYPES 5	// Change this if you add another type of animation (as two variants, CPU and SPU)

#define WINDOWED_AVERAGE_SIZE 256


SampleAndBlendMultithreadingDemo::SampleAndBlendMultithreadingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env),
	m_useMt(m_variantId < NUM_ANIMATION_TYPES),
	m_convertLocalToModelInAnimationJob(true),
	m_asyncSampling(false),
	m_simpleStatistics(true),
	m_useCache(false),
	m_cache(HK_NULL),
	m_windowedAverage(HK_NULL),
	m_time(0.0f)
{
	// Disable reports: 
	if(m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x36118e94, false); 		
	}

	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	//
	// Setup the camera
	//
	{
		hkVector4 from(  -1.0f, -7.0f,  1.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	m_loader = new hkLoader();

	//
	// Get the rig
	//
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( RIG_FILE_NAME );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];

		HK_ASSERT2( 0x0, m_skeleton->m_numBones == NUM_BONES, "Loaded skeleton has " << m_skeleton->m_numBones << "bones. Please #define NUM_BONES " << m_skeleton->m_numBones );
	}

	//
	// Get the animations and the bindings
	//
	for ( int i = 0; i < NUM_ANIMATIONS; i++ )
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( ANIMATION_FILE_NAMES[ i ] );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animations.pushBack( ac->m_animations[0] );

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_bindings.pushBack( ac->m_bindings[0] );
	}

	//
	// Make new (compressed or mirrored) versions depending on the variant.
	//
	{
		switch(m_variantId % NUM_ANIMATION_TYPES)
		{
			case 0: // Uncompressed
			{
				// No need to do anything
				break;
			}
			case 1: // Delta
			{
				hkaDeltaCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;

				for (int i=0; i < m_animations.getSize(); i++)
				{
					hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>(m_animations[i]);
					m_animations[i] = new hkaDeltaCompressedAnimation(*uncompressedAnimation, p);
					m_bindings[i]->m_animation = m_animations[i];
				}
				break;
			}
			case 2: // Wavelet
			{
				hkaWaveletCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;
				p.m_truncProp = .25;

				for (int i=0; i < m_animations.getSize(); i++)
				{
					hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>(m_animations[i]);
					m_animations[i] = new hkaWaveletCompressedAnimation(*uncompressedAnimation, p);
					m_bindings[i]->m_animation = m_animations[i];
				}
				break;
			}
			case 3: // Mirrored
			{
				hkaWaveletCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;
				p.m_truncProp = .25;				
				
				for (int i=0; i < m_animations.getSize(); i++)
				{
					hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>(m_animations[i]);
					hkaWaveletCompressedAnimation* w = new hkaWaveletCompressedAnimation(*uncompressedAnimation, p);
					makeMirroredAnimation( w, i );
					w->removeReference();
				}
				
				break;
			}
			case 4: // Spline
			{
				hkaSplineCompressedAnimation::TrackCompressionParams p;
				hkaSplineCompressedAnimation::AnimationCompressionParams a;

				for (int i=0; i < m_animations.getSize(); i++)
				{
					hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>(m_animations[i]);
					m_animations[i] = new hkaSplineCompressedAnimation( *uncompressedAnimation, p, a );
					m_bindings[i]->m_animation = m_animations[i];
				}

				break;
			}
		}
	}

	//
	// Create animated skeletons
	//
	for ( int i = 0; i < NUM_SKELETONS; ++i )
	{
		// Create a new animated skeleton
		hkaAnimatedSkeleton* skeleton = new hkaAnimatedSkeleton( m_skeleton );
		m_activeSkeletons.pushBack( skeleton );

		for (int j=0; j < m_animations.getSize(); j++)
		{
			// Create an animation control
			hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl(m_bindings[j]);
			ac->setLocalTime( i * .2f );

			// Bind the control to the skeleton
			skeleton->addAnimationControl( ac );

			// The animated skeleton now owns the control
			ac->removeReference();
		}
	}

	setupGraphics( );

	//
	// Allocate output buffers for pose sampling
	//
	m_poses.setSize( NUM_SKELETONS );
	for (int p=0; p < NUM_SKELETONS; p++)
	{
		m_poses[p] = new hkaPose(m_activeSkeletons[p]->getSkeleton());
		m_poses[p]->setToReferencePose();
	}

	hkaAnimationJobQueueUtils::registerWithJobQueue(m_jobQueue);

	//
	// Make helper which will allow use to average time, see windowed average etc.
	//
	m_windowedAverage = new WindowedAverage( WINDOWED_AVERAGE_SIZE );

	//
	// Add a cache if it can be used by this platform and variant.
	//
	addCacheIfUsable();
}

void SampleAndBlendMultithreadingDemo::addCacheIfUsable()
{
	// Set up cache if usable:
	// 1. Not used on spu, so no point in creating one for that
	// 2. Also only useful for the animation types which use a cache: wavelet and delta (and this demo uses a mirrored animation wrapping a wavelet)
	hkBool canUseCache = true;

	canUseCache = canUseCache &&
		(	(m_animations[0]->getType() == hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION) ||
			(m_animations[0]->getType() == hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION) ||
			(m_animations[0]->getType() == hkaAnimation::HK_MIRRORED_ANIMATION)	);

	if( canUseCache ) 
	{
		// We create a cache - not tuned to the animations in this demo, but with example 
		// settings so that we can confirm it improves performance.
		hkaChunkPoolCinfo cpCinfo[2];

		cpCinfo[0].m_buckets = 3;
		cpCinfo[0].m_slots = 3;
		cpCinfo[0].m_chunkSize = 6 * 1024;

		cpCinfo[1].m_buckets = 5;
		cpCinfo[1].m_slots = 2;
		cpCinfo[1].m_chunkSize = 12 * 1024;

		// The cache size in bytes is equal to m_buckets * m_slots * m_chunkSize

		hkaDefaultChunkCacheCinfo cacheCinfo;
		cacheCinfo.m_numberOfCachePools = 2;
		cacheCinfo.m_cachePools.setSize( 2 );
		cacheCinfo.m_cachePools[0] = cpCinfo[0];
		cacheCinfo.m_cachePools[1] = cpCinfo[1];

		// Which cache to use depends on whether we are single or multithreaded
		if( m_useMt )
		{
			m_cache = new hkaMultithreadedChunkCache( cacheCinfo );
		}
		else
		{
			m_cache = new hkaDefaultChunkCache( cacheCinfo );
		}
	}
}

void SampleAndBlendMultithreadingDemo::makeMirroredAnimation(hkaAnimation* original, int index )
{
	
	hkObjectArray< hkString > ltag;
	hkObjectArray< hkString > rtag;

	ltag.pushBack( " L " );
	ltag.pushBack( "EyeL" );

	rtag.pushBack( " R " );
	rtag.pushBack( "EyeR" );

	hkaMirroredSkeleton *mirroredSkeleton = new hkaMirroredSkeleton( m_skeleton );

	mirroredSkeleton->computeBonePairingFromNames( ltag, rtag );

	hkQuaternion v_mir( 1.0f, 0.0f, 0.0f, 0.0f );
	mirroredSkeleton->setAllBoneInvariantsFromReferencePose( v_mir, 0.0f );

	hkaMirroredAnimation *mirroredAnimation = new hkaMirroredAnimation( original, m_bindings[index], mirroredSkeleton );
	m_bindings[index] = mirroredAnimation->createMirroredBinding();
	m_animations[index] = mirroredAnimation;

	mirroredSkeleton->removeReference();
}

SampleAndBlendMultithreadingDemo::~SampleAndBlendMultithreadingDemo()
{
	// Re-enable report
	hkError::getInstance().setEnabled(0x36118e94, true);

	// If we created a mirrored animation, clean up binding
	if (m_animations[0]->getType() == hkaAnimation::HK_MIRRORED_ANIMATION)
	{
		for ( int i = 0; i < m_bindings.getSize(); i++ )
		{
			hkaMirroredAnimation::destroyMirroredBinding( m_bindings[i] );
		}
	}

	// If we created compressed animations, clean them up.
	if (m_animations[0]->getType() != hkaAnimation::HK_INTERLEAVED_ANIMATION)
	{
		for ( int i = 0; i < m_animations.getSize(); i++ )
		{
			m_animations[i]->removeReference();
		}
	}

	// Delete the active skeletons
	for (int s=0; s< m_activeSkeletons.getSize(); s++)
	{
		m_activeSkeletons[s]->removeReference();
		delete m_poses[s];
	}

	// Delete the windowed averager
	delete m_windowedAverage;

	// Delete animation cache if used
	if( m_cache )
	{
		m_cache->removeReference();
	}

	// Finally delete the loader (all serialized data)
	delete m_loader;
}

hkDemo::Result SampleAndBlendMultithreadingDemo::stepDemo()
{
	//
	// Handle input
	//
	{
		if ( m_useMt && m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) )
		{
			m_convertLocalToModelInAnimationJob = !m_convertLocalToModelInAnimationJob;
		}

		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) )
		{
			m_asyncSampling = !m_asyncSampling;
		}

		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) )
		{
			m_simpleStatistics = !m_simpleStatistics;
		}

		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_DPAD_DOWN) )
		{
			if( m_cache != HK_NULL )
			{
				m_useCache = !m_useCache;
			}
		}
	}

	HK_TIMER_BEGIN("StepAnimations", HK_NULL);

	const int numSkeletons = m_activeSkeletons.getSize();

	//
	// Step the skeletons - this happens in single threaded mode
	//
	{
		for (int i = 0; i < numSkeletons; ++i )
		{
			hkaAnimatedSkeleton* inst = m_activeSkeletons[i];

			const int numControls = inst->getNumAnimationControls();

			for ( int j = 0; j < numControls; j++ )
			{
				// Distribute the animation weights based on the current time, skeleton and control indices
				hkReal weight = .5f*(1.0f + hkMath::sin( m_time * hkReal( i + 1 ) / hkReal( numSkeletons ) * hkReal( j + 1 ) / hkReal( numControls ) ) );
				hkaDefaultAnimationControl* ac = static_cast< hkaDefaultAnimationControl* >( inst->getAnimationControl( j ) ); // We know these are Default controls
				ac->setMasterWeight( weight );
			}

			// Advance the animation
			inst->stepDeltaTime( m_timestep );
		}
	}

	HK_TIMER_END();


	//
	// Do the sampling and blending
	//
	hkStopwatch timer;
	{
		
		timer.start();

		if(m_useMt)
		{
			if(m_asyncSampling)
			{
				doMultithreadedSamplingAsynchronously();
			}
			else
			{
				doMultithreadedSamplingSynchronously();
			}	
		}
		else
		{
			// Run animation on the PPU in single threaded mode
			for (int i = 0; i < numSkeletons; ++i )
			{
				m_activeSkeletons[i]->sampleAndCombineAnimations(	m_poses[i]->accessUnsyncedPoseLocalSpace().begin(), 
					m_poses[i]->getFloatSlotValues().begin(), m_useCache ? m_cache : HK_NULL );
			}
		}

		timer.stop();
	}

	
	hkReal sampleTime = ( timer.getSplitSeconds() * 1000000 );
	m_windowedAverage->pushNewValue( sampleTime );

	//
	// Display only if not being run with NULL renderer (e.g. bootstrapping for stats generation)
	//
	if( hkString::strNcmp( m_env->m_options->m_renderer, "n", 1) != 0)
	{
		if (!m_convertLocalToModelInAnimationJob)		// To compare timings, make sure that we explicitly do conversion if not already done on SPU
		{
			HK_TIMER_BEGIN("syncModelSpace", HK_NULL);
			for (int i = 0; i < numSkeletons; ++i )
			{
				m_poses[i]->syncModelSpace();
			}
			HK_TIMER_END();
		}

		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		for (int i = 0; i < numSkeletons; ++i )
		{
			worldFromModel.m_translation.set( hkReal(i - 1 - (NUM_SKELETONS>>2)), 0, 0);
			AnimationUtils::drawPose( *m_poses[i], worldFromModel );
		}
	
		const int h = m_env->m_window->getHeight();
		if ( m_useMt )
		{
			m_env->m_textDisplay->outputText( (m_asyncSampling?"Asynchronous processing (Hit \222 to change)":"Synchronous processing (Hit \222 to change)"), 20, h-60 );
			m_env->m_textDisplay->outputText( (m_convertLocalToModelInAnimationJob?"LocalToModel in Animation Job (Hit \223 to change)":"LocalToModel Single Threaded On CPU  (Hit \223 to change)"), 20, h-40 );		
		}
		if( m_cache )
		{
			m_env->m_textDisplay->outputText( (m_useCache?"Using Example CACHE (Hit DPad 'Down' to change)":"No Cache Used (Hit DPad 'Down' to change)"), 20, h-20 );
		}

		printTimingStatistics();

	}

	m_time += m_timestep;

	return DEMO_OK;
}

void SampleAndBlendMultithreadingDemo::doMultithreadedSamplingSynchronously()
{
	const int numSkeletons = m_activeSkeletons.getSize();

	// Set up the jobs
	hkObjectArray<hkaAnimationSampleAndCombineJob> sampleAndCombineJobs;
	sampleAndCombineJobs.setSize(numSkeletons);

	HK_TIMER_BEGIN("JobSetup", HK_NULL);

	// Make jobs
	for (int i = 0; i < numSkeletons; ++i )
	{
		// We gather data for each active control and place it in a buffer to be used by the SPU
		// The buffer must persist in memory as long as the job is active.

		// Allocate space for the buffer (typically 50 bytes per active control)
		hkaMultithreadedAnimationUtils::allocateSampleAndCombineJob( m_activeSkeletons[i], sampleAndCombineJobs[i] );

		{
			// Get output - local or model
			hkQsTransform* outputTransforms = HK_NULL; 
			hkInt16* parentIndices = HK_NULL;	// Used as flag for job to do local-to-model conversion
			if (m_convertLocalToModelInAnimationJob)
			{
				outputTransforms = m_poses[i]->accessUnsyncedPoseModelSpace().begin();
				parentIndices = m_skeleton->m_parentIndices;
			}
			else
			{
				outputTransforms = m_poses[i]->accessUnsyncedPoseLocalSpace().begin();
				parentIndices = HK_NULL;
			}

			// Populate the buffer with data from the controls.
			// Warning - if you are *not* using a hkaPose class to allocate your output array for
			// float slots you must ensure that your float slots array is a multiple of 16 bytes in size.
			// See hkaMultithreadedAnimationUtils::allocateFloatSlotsArrayRoundedUpToMultipleOf16() for details.
			hkaMultithreadedAnimationUtils::createSampleAndCombineJob( m_activeSkeletons[i], 
				m_activeSkeletons[i]->getSkeleton()->m_numBones, outputTransforms, 
				m_activeSkeletons[i]->getSkeleton()->m_numFloatSlots, m_poses[i]->getFloatSlotValues().begin(),
				parentIndices,
				sampleAndCombineJobs[i] );

			// Use cache if selected.
			if( m_useCache )
			{
				// Note that in general the cache parameter *will not* be used if the mulithreaded job processing 
				// takes place on spu, but will be used on cpu (other multithreaded job processing) and also
				// that it is up to the user to ensure that it is a multithreaded-safe cache 
				// (e.g. the hkaMultithreadedChunkCache).
				sampleAndCombineJobs[i].m_cache = m_cache;
			}

		}

		// This semaphore is released when the job is complete (if it's non-null)
		sampleAndCombineJobs[i].m_jobDoneSemaphore = HK_NULL;

		// Add the job - execution begins immediately on the SPU. 
		// Since however this uses a critical section, and the job set up code above is relatively fast, 
		// we defer adding until all jobs are set up, and then use an addJobBatch - this will be faster.
		// HK_TIMER_BEGIN("AddJob", HK_NULL);
		// m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&sampleAndCombineJobs[i]), hkJobQueue::JOB_HIGH_PRIORITY);
		// HK_TIMER_END();
	}

	// Add jobs as a batch
	{
		HK_TIMER_BEGIN("AddJobBatch", HK_NULL);
		hkArray<hkJobQueue::JobQueueEntry*> jobQueueEntries;
		jobQueueEntries.setSize(numSkeletons);
		for (int i = 0; i < numSkeletons; ++i )
		{
			jobQueueEntries[i] = reinterpret_cast<hkJobQueue::JobQueueEntry*>(&sampleAndCombineJobs[i]);
		}

		// Add the job - execution begins immediately on the SPU
		m_jobQueue->addJobBatch( jobQueueEntries, hkJobQueue::JOB_HIGH_PRIORITY );
		HK_TIMER_END();
	}

	HK_TIMER_END();

	m_jobThreadPool->processAllJobs( m_jobQueue );

	// On platforms with multiple CPUs (e.g. Windows, Xbox360), have the main thread take jobs too.
	// This could be done on the PLAYSTATION(R)3 too, which would force the PPU to take jobs, but probably isn't worth it.
	m_jobQueue->processAllJobs();

	// Wait for all threads to finish

	// There's no need to wait on the hkaAnimationSampleAndCombineJob's semaphore here, since we're going to end up waiting for all the jobs to finish.
	// However, if each job had its own semaphore and we wanted to wait on an specific job, this would be the place to do it.

	// Wait for the actual tasks to finish. This makes sure all timer information will have finished DMAing to main memory
	m_jobThreadPool->waitForCompletion();

	// Clean up the control buffers
	for (int i = 0; i < numSkeletons; ++i )
	{
		hkaMultithreadedAnimationUtils::deallocateSampleAndCombineJob( sampleAndCombineJobs[i] );
	}
}


void SampleAndBlendMultithreadingDemo::doMultithreadedSamplingAsynchronously()
{
	const int numSkeletons = m_activeSkeletons.getSize();

	// Set up the jobs
	hkObjectArray<hkaAnimationSampleAndCombineJob> sampleAndCombineJobs;
	sampleAndCombineJobs.setSize(numSkeletons);
	int numJobsAdded = 0;

	// Allocate an array that will contain the "job finished" flags for each  animation job.  This needs to be allocated on the heap since these
	// addresses are written to from the SPU.  The array should be as large as the number of jobs you'll ever process in one frame. 
	hkLocalArray<hkUint32> animationJobDoneFlags( sampleAndCombineJobs.getSize() );
	animationJobDoneFlags.setSize( sampleAndCombineJobs.getSize(), 0);

	m_jobQueue->setWaitPolicy(hkJobQueue::WAIT_INDEFINITELY);

	HK_TIMER_BEGIN("JobSetup", HK_NULL);

	// Make jobs
	for (int i = 0; i < numSkeletons; ++i )
	{
		// We gather data for each active control and place it in a buffer to be used by the SPU
		// The buffer must persist in memory as long as the job is active.

		// Allocate space for the buffer (typically 50 bytes per active control)
		hkaMultithreadedAnimationUtils::allocateSampleAndCombineJob( m_activeSkeletons[i], sampleAndCombineJobs[i] );

		{
			// Get output - local or model
			hkQsTransform* outputTransforms = HK_NULL; 
			hkInt16* parentIndices = HK_NULL;	// Used as flag for job to do local-to-model conversion
			if (m_convertLocalToModelInAnimationJob)
			{
				outputTransforms = m_poses[i]->accessUnsyncedPoseModelSpace().begin();
				parentIndices = m_skeleton->m_parentIndices;
			}
			else
			{
				outputTransforms = m_poses[i]->accessUnsyncedPoseLocalSpace().begin();
				parentIndices = HK_NULL;
			}

			// Populate the buffer with data from the controls.
			// Warning - if you are *not* using a hkaPose class to allocate your output array for
			// float slots you must ensure that your float slots array is a multiple of 16 bytes in size.
			// See hkaMultithreadedAnimationUtils::allocateFloatSlotsArrayRoundedUpToMultipleOf16() for details.
			hkaMultithreadedAnimationUtils::createSampleAndCombineJob( m_activeSkeletons[i], 
				m_activeSkeletons[i]->getSkeleton()->m_numBones, outputTransforms, 
				m_activeSkeletons[i]->getSkeleton()->m_numFloatSlots, m_poses[i]->getFloatSlotValues().begin(),
				parentIndices,
				sampleAndCombineJobs[i] );

			// Use cache if selected 
			if( m_useCache )
			{
				// Note that in general the cache parameter *will not* be used if the mulithreaded job processing 
				// takes place on spu, but will be used on cpu (other multithreaded job processing) and also
				// that it is up to the user to ensure that it is a multithreaded-safe cache 
				// (e.g. the hkaMultithreadedChunkCache).
				sampleAndCombineJobs[i].m_cache = m_cache;
			}

		}


		// This semaphore is released when the job is complete (if it's non-null)
		sampleAndCombineJobs[i].m_jobDoneSemaphore = HK_NULL;
		// Here we're going to use a flag instead of a semaphore to detect job completion
		sampleAndCombineJobs[i].m_jobDoneFlag = &(animationJobDoneFlags[i]);
	}

	HK_TIMER_END();

	// Start processing
	m_jobThreadPool->processAllJobs( m_jobQueue );

	// On platforms with multiple CPUs (e.g. Windows, Xbox360), have the main thread take jobs too
	// This could be done on the PLAYSTATION(R)3 too, which would force the PPU to take jobs, but probably isn't worth it.
	m_jobQueue->processAllJobs();

	hkArray<hkJobQueue::JobQueueEntry*> jobQueueEntriesForBatch;

	// Next block on the PPU until all jobs are marked as done
	hkBool allJobsComplete = false;
	while( !allJobsComplete )
	{

		// To fake the asynchronous nature of job creation, just for this demo, every so often add in some more jobs	
		if( hkMath::rand01() < 0.01f)
		{
			int numToAdd = (int)(hkMath::rand01() * 7);
			int maxCanAdd = numSkeletons - numJobsAdded;
			numToAdd = (numToAdd <= maxCanAdd)? numToAdd: 0;
			
			jobQueueEntriesForBatch.setSize(numToAdd);

			for(int i = 0; i < numToAdd; i++)
			{
				jobQueueEntriesForBatch[i] = reinterpret_cast<hkJobQueue::JobQueueEntry*>(&sampleAndCombineJobs[numJobsAdded]);
				numJobsAdded++;
			}

			// Add the jobs - execution begins immediately on the SPU
			if(numToAdd > 0)
			{
				m_jobQueue->addJobBatch( jobQueueEntriesForBatch, hkJobQueue::JOB_HIGH_PRIORITY );
			}
		}

		// Deal with jobs which are done here
		//
		// If animationJobDoneFlags[i] == 1, then animation job 'i' is complete
		//
				

		// Check for job completion
		allJobsComplete = true;
		for( hkInt32 i = 0; i < numSkeletons; ++i )
		{
			if( animationJobDoneFlags[i] == 0 )
			{
				// One job not yet finished so can't exit
				allJobsComplete = false;
			}
		}

	}

	// Let job queue know that we are are done.
	m_jobQueue->setWaitPolicy(hkJobQueue::WAIT_UNTIL_ALL_WORK_COMPLETE);

	// Wait for the actual tasks to finish. This makes sure all timer information will have finished DMAing to main memory
	m_jobThreadPool->waitForCompletion();

	// Clean up the control buffers
	for (int i = 0; i < numSkeletons; ++i )
	{
		hkaMultithreadedAnimationUtils::deallocateSampleAndCombineJob( sampleAndCombineJobs[i] );
	}
}

void SampleAndBlendMultithreadingDemo::printTimingStatistics()
{
	char buf[1024];

	const int h = m_env->m_window->getHeight();

	int x = 20;
	int y = 80;
	int d = 20;

	if ( m_simpleStatistics )
	{
		hkString::sprintf(buf, "Average sample Time (last %3d Samples): %8.0f (uSecs)", m_windowedAverage->getWindowSize(), m_windowedAverage->getWindowedMean() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Current Sample Time:                    %8.0f (uSecs)", m_windowedAverage->getLastTime() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Sample time for %d characters, %d blends, %d bones", NUM_SKELETONS, NUM_ANIMATIONS, NUM_BONES );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, " " );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Simple Statistics (Hit \221 to change)" );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );
	}
	else
	{
		hkString::sprintf(buf, "     Total Samples: %8d", m_windowedAverage->getNumSamples() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "       Window Size: %8d", m_windowedAverage->getWindowSize() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "   Widowed Std Dev: %8.0f (uSecs)", m_windowedAverage->getWindowedStandardDeviation() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "  Windowed Maximum: %8.0f (uSecs)", m_windowedAverage->getWindowedMax() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "  Windowed Minimum: %8.0f (uSecs)", m_windowedAverage->getWindowedMin() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "   Windowed Median: %8.0f (uSecs)", m_windowedAverage->getWindowedMedian() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "     Windowed Mean: %8.0f (uSecs)", m_windowedAverage->getWindowedMean() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "        Total Mean: %8.0f (uSecs)", m_windowedAverage->getTotalMean() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "           Current: %8.0f (uSecs)", m_windowedAverage->getLastTime() );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Sample time for %d characters, %d blends, %d bones", NUM_SKELETONS, NUM_ANIMATIONS, NUM_BONES );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, " " );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Detailed Statistics (Hit \221 to change)" );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );
	}

}

void SampleAndBlendMultithreadingDemo::makeFakeInput()
{
	// Toggle m_convertLocalToModelOnSPU every frame
	m_env->m_gamePad->forceButtonPressed( HKG_PAD_BUTTON_3 );
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Shows animation decompression and blending on the SPU" ;


#	define ST_STRING "CPU, Single-threaded"
#	define MT_STRING "CPU, Multi-threaded "

static const int demoType = HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL | HK_DEMO_TYPE_STATS;

#define TO_STRING1( x ) # x
#define TO_STRING2( x ) TO_STRING1( x )

HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, MT_STRING " - Uncompressed,     " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 0,       "Sampling on " MT_STRING,              "Sampling on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, MT_STRING " - Delta,            " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 1,   "Delta decompression on " MT_STRING,   "Delta decompression on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, MT_STRING " - Wavelet,          " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 2, "Wavelet decompression on " MT_STRING, "Wavelet on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, MT_STRING " - Mirrored Wavelet, " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 3, "Mirrored on " MT_STRING, "Mirrored on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, MT_STRING " - Spline,           " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 4, "Spline compression on " MT_STRING, "Spline compression on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, ST_STRING " - Uncompressed,     " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 5,       "Sampling on " ST_STRING,              "Sampling on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, ST_STRING " - Delta,            " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 6,   "Delta decompression on " ST_STRING,   "Delta decompression on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, ST_STRING " - Wavelet,          " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 7, "Wavelet decompression on " ST_STRING, "Wavelet decompression on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, ST_STRING " - Mirrored Wavelet, " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 8, "Mirrored on " ST_STRING, "Mirrored on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleAndBlendMultithreadingDemo, demoType, ST_STRING " - Spline,           " TO_STRING2( NUM_SKELETONS ) " Skeletons of " TO_STRING2( NUM_BONES ) " Bones, " TO_STRING2( NUM_ANIMATIONS ) " Blended Animations, No Cache", 9, "Spline compression on " ST_STRING, "Spline compression on " ST_STRING);

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

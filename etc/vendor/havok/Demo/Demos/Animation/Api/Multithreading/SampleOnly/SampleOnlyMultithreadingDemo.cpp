/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Multithreading/SampleOnly/SampleOnlyMultithreadingDemo.h>

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
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>

#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobs.h>
#include <Animation/Animation/Playback/Multithreaded/hkaAnimationJobQueueUtils.h>

#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>

#include <Animation/Animation/Animation/Mirrored/hkaMirroredAnimation.h>
#include <Animation/Animation/Animation/Mirrored/hkaMirroredSkeleton.h>

#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>

#include <Animation/Animation/Playback/Cache/Default/hkaDefaultChunkCache.h>
#include <Animation/Animation/Playback/Cache/Multithreaded/hkaMultithreadedChunkCache.h>

#define NUM_ANIMATIONS 50
#define NUM_BONES 50

// Uncomment to test individual track sampling (currently Spline and Interleaved only)
//#define USE_INDIVIDUAL_TRACK_SAMPLING

static const char* RIG_FILE_NAME = "Resources/Animation/HavokGirl/hkRig.hkx";
static const char* ANIMATION_FILE_NAME = "Resources/Animation/HavokGirl/hkRunTurnLLoop.hkx";


#define NUM_ANIMATION_TYPES 5	// Change this if you add another type of animation (as two variants, CPU and SPU)

#define WINDOWED_AVERAGE_SIZE 256

SampleOnlyMultithreadingDemo::SampleOnlyMultithreadingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env),
	m_useMt(m_variantId < NUM_ANIMATION_TYPES),
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
		hkVector4 from(  -1.0f, -25.0f,  1.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	m_loader = new hkLoader();


	//
	// Get the rig
	//
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath(RIG_FILE_NAME);
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];

		HK_ASSERT2( 0x0, m_skeleton->m_numBones == NUM_BONES, "Loaded skeleton has " << m_skeleton->m_numBones << "bones. Please #define NUM_BONES " << m_skeleton->m_numBones );
	}

	hkaInterleavedUncompressedAnimation* uncompressedAnimation;
	
	//
	// Get the animation and the binding
	//
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath(ANIMATION_FILE_NAME);
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*> (ac->m_animations[0]);

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
	}

	//
	// Make new (compressed or mirrored) versions depending on the variant.
	//
	{
		switch(m_variantId % NUM_ANIMATION_TYPES)
		{
			case 0:
			{
				m_animation = uncompressedAnimation;
				break;
			}
			case 1: // Delta
			{
				hkaDeltaCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;

				m_animation = new hkaDeltaCompressedAnimation(*uncompressedAnimation, p);
				m_binding->m_animation = m_animation;
				break;
			}
			case 2: // Wavelet
			{
				hkaWaveletCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;
				p.m_truncProp = .25;

				m_animation = new hkaWaveletCompressedAnimation(*uncompressedAnimation, p);
				m_binding->m_animation = m_animation;
				break;
			}
			case 3: // Mirrored
			{
				hkaWaveletCompressedAnimation* w = HK_NULL;
				{
					hkaWaveletCompressedAnimation::CompressionParams p;
					p.m_quantizationBits = 16;
					p.m_blockSize = 8;
					p.m_truncProp = .25;

					w = new hkaWaveletCompressedAnimation(*uncompressedAnimation, p);
				}
				makeMirroredAnimation( w );	
				w->removeReference();
				break;
			}
			case 4: // Spline
			{
				hkaSplineCompressedAnimation::TrackCompressionParams p;
				hkaSplineCompressedAnimation::AnimationCompressionParams a;

#ifdef USE_INDIVIDUAL_TRACK_SAMPLING
				a.m_enableSampleSingleTracks = true;
#endif

				m_animation = new hkaSplineCompressedAnimation( *uncompressedAnimation, p, a );
				m_binding->m_animation = m_animation;

				break;
			}

		}
	}

	//
	// Create controls (to help step the animations, handling looping, speed etc)
	//
	for (int i=0; i< NUM_ANIMATIONS; i++)
	{
		hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl(m_binding);
		control->setLocalTime(i * 0.05f);
		m_controls.pushBack( control );
	}

	setupGraphics( );

	hkaAnimationJobQueueUtils::registerWithJobQueue(m_jobQueue);

	//
	// Make helper which will allow use to average time, see windowed average etc.
	//
	m_windowedAverage = new WindowedAverage( WINDOWED_AVERAGE_SIZE );

	//
	// Add a cache if it can be used by this platform and variant
	//
	addCacheIfUsable();

}

void SampleOnlyMultithreadingDemo::addCacheIfUsable()
{
	// Set up cache if usable:
	// 1. Not used on spu, so no point in creating one for that
	// 2. Also only useful for the animation types which use a cache: wavelet and delta (and this demo uses a mirrored animation wrapping a wavelet)
	hkBool canUseCache = true;

	canUseCache = canUseCache &&
		(	(m_animation->getType() == hkaAnimation::HK_DELTA_COMPRESSED_ANIMATION) ||
			(m_animation->getType() == hkaAnimation::HK_WAVELET_COMPRESSED_ANIMATION) ||
			(m_animation->getType() == hkaAnimation::HK_MIRRORED_ANIMATION)	);
	
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

void SampleOnlyMultithreadingDemo::makeMirroredAnimation(hkaAnimation* original)
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

	hkaMirroredAnimation *mirroredAnimation = new hkaMirroredAnimation( original, m_binding, mirroredSkeleton );
	m_binding = mirroredAnimation->createMirroredBinding();
	m_animation= mirroredAnimation;

	mirroredSkeleton->removeReference();
}

SampleOnlyMultithreadingDemo::~SampleOnlyMultithreadingDemo()
{
	// Re-enable report
	hkError::getInstance().setEnabled(0x36118e94, true);

	// If we created a mirrored animation, clean up binding
	if (m_animation->getType() == hkaAnimation::HK_MIRRORED_ANIMATION)
	{
		hkaMirroredAnimation::destroyMirroredBinding( m_binding );
	}

	// If we created a compressed animation, clean it up.
	if (m_animation->getType() != hkaAnimation::HK_INTERLEAVED_ANIMATION)
	{
		m_animation->removeReference();
	}

	for (int i=0; i< NUM_ANIMATIONS; i++)
	{
		m_controls[i]->removeReference();
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

hkDemo::Result SampleOnlyMultithreadingDemo::stepDemo()
{
	//
	// Handle input
	//
	{
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
	
	//
	// Advance the animation - this happens in single threaded mode
	//
	for (int i=0; i< NUM_ANIMATIONS; i++)
	{
		m_controls[i]->update( m_timestep );
	}

	//
	// Create output pose buffers - it must persist while the job is executing
	//
	hkLocalArray<hkaPose*> pose( NUM_ANIMATIONS );
	pose.setSize( NUM_ANIMATIONS );
	for (int p=0; p < NUM_ANIMATIONS; p++)
	{
		pose[p] = new hkaPose(m_skeleton);
	}

	HK_TIMER_END();


	//
	// Do the sampling
	//
	hkStopwatch timer;
	{
		timer.start();

		if( m_useMt )
		{
			if(m_asyncSampling)
			{
				doMultithreadedSamplingAsynchronously(pose);
			}
			else
			{
				doMultithreadedSamplingSynchronously(pose);
			}	
		}
		else
		{
			// Sample animation on the PPU is single threaded mode
			for (int i = 0; i < NUM_ANIMATIONS; ++i )
			{
				// N.B. We are putting track data directly into a pose, which necessarily assumes that the binding is the identity.
				// In general of course this will not be true, but for the purpose of this demo it is simplest to do this rather
				// than sample into a temporary array and track data and copy over into the bone (pose) data.

#ifdef USE_INDIVIDUAL_TRACK_SAMPLING
				const int numTracks = m_controls[i]->getAnimationBinding()->m_animation->m_numberOfTransformTracks;
				const hkInt16* const tracks = m_controls[i]->getAnimationBinding()->m_transformTrackToBoneIndices;
				m_animation->sampleIndividualTransformTracks( m_controls[i]->getLocalTime(), tracks, numTracks, pose[i]->accessUnsyncedPoseLocalSpace().begin() );
#else
				m_animation->sampleTracks( m_controls[i]->getLocalTime(), pose[i]->accessUnsyncedPoseLocalSpace().begin(), HK_NULL, m_useCache ? m_cache : HK_NULL  );
#endif
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

		HK_TIMER_BEGIN("syncModelSpace", HK_NULL);
		for (int i = 0; i < NUM_ANIMATIONS; ++i )
		{
			pose[i]->syncModelSpace();
		}
		HK_TIMER_END();

		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		for (int i = 0; i < NUM_ANIMATIONS; ++i )
		{
			worldFromModel.m_translation.set( hkReal(i - 1 - (NUM_ANIMATIONS>>2)), 0, 0);
			AnimationUtils::drawPose( *pose[i], worldFromModel );
			delete pose[i];
		}

		const int h = m_env->m_window->getHeight();
		if ( m_useMt )
		{
			m_env->m_textDisplay->outputText( (m_asyncSampling?"Asynchronous processing (Hit \222 to change)":"Synchronous processing (Hit \222 to change)"), 20, h - 40 );
		}
		if( m_cache )
		{
			m_env->m_textDisplay->outputText( (m_useCache?"Using Example CACHE (Hit DPad 'Down' to change)":"No Cache Used (Hit DPad 'Down' to change)"), 20, h-20 );
		}

		printTimingStatistics();

		m_time += m_timestep;
	}

	return DEMO_OK;
}

void SampleOnlyMultithreadingDemo::doMultithreadedSamplingSynchronously(hkLocalArray<hkaPose*>& pose)
{
	HK_TIMER_BEGIN("JobSetup", HK_NULL);

	// This data must persist while the spu job is executing
	hkaSampleAnimationJob::AnimationData* animData = new hkaSampleAnimationJob::AnimationData[ NUM_ANIMATIONS ];

	int maxBufferSize = 0;
	for (int i=0 ; i < NUM_ANIMATIONS; i++)
	{
		hkReal localTime = m_controls[i]->getLocalTime();
		m_animation->getFrameAndDelta( localTime, animData[i].m_frameIndex, animData[i].m_frameDelta );
		animData[i].m_maxTransformTrack = m_animation->m_numberOfTransformTracks;
		animData[i].m_maxFloatTrack = m_animation->m_numberOfFloatTracks;
		animData[i].m_animationOnCPU = m_animation;
		animData[i].m_poseOut = pose[i]->accessUnsyncedPoseLocalSpace().begin();

		// Warning - if you are *not* using a hkaPose class to allocate your output array for
		// float slots you must ensure that your float slots array is a multiple of 16 bytes in size.
		// See hkaMultithreadedAnimationUtils::allocateFloatSlotsArrayRoundedUpToMultipleOf16() for details.
		animData[i].m_floatSlotsOut = pose[i]->getFloatSlotValues().begin();

		// Here we can (optionally) try and minimise the buffer size to avoid stack overflow for large animations.
	}

	HK_TIMER_END();

	// We'll distribute the work evenly over the spus.
	int numThreads = m_jobThreadPool->getNumThreads();
	numThreads++; // count the main thread too

	hkObjectArray<hkaSampleAnimationJob> sampleJobs( numThreads );
	sampleJobs.setSize( numThreads );


	hkaSampleAnimationJob::AnimationData* currentAnimData = animData;
	for (int s=0; s < numThreads; s++)
	{
		const int animsForJob = NUM_ANIMATIONS / numThreads + int(s < NUM_ANIMATIONS % numThreads);	// handles any remainders

		// Initialize the job to sample all the animations
		sampleJobs[s].m_animData = currentAnimData;
		sampleJobs[s].m_numAnims = animsForJob;
		sampleJobs[s].m_jobDoneSemaphore = HK_NULL;

		// Here we can (optionally) try and minimise the buffer size to avoid stack overflow for large animations.
		sampleJobs[s].m_bufferSize = maxBufferSize;

		// Use cache if selected 
		if( m_useCache )
		{
			// Note that in general the cache parameter *will not* be used if the mulithreaded job processing 
			// takes place on spu, but will be used on cpu (other multithreaded job processing) and also
			// that it is up to the user to ensure that it is a multithreaded-safe cache 
			// (e.g. the hkaMultithreadedChunkCache).
			sampleJobs[s].m_cache = m_cache;
		}

		// Add the job - execution begins immediately on the SPU
		// Since however this uses a critical section, and the job set up code above is relatively fast, 
		// we defer adding until all jobs are set up, and then use an addJobBatch - this will be faster.
		//HK_TIMER_BEGIN("AddJob", HK_NULL);
		//m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&job[s]), hkJobQueue::JOB_HIGH_PRIORITY, hkJobQueue::JOB_TYPE_HINT_SPU );
		//HK_TIMER_END();

		HK_ASSERT2(0x0, sampleJobs[s].isValid(), "Job is invalid");

		currentAnimData += animsForJob;
	}

	// Add jobs as a batch
	{
		HK_TIMER_BEGIN("AddJobBatch", HK_NULL);
		hkArray<hkJobQueue::JobQueueEntry*> jobQueueEntries;
		jobQueueEntries.setSize(numThreads);
		for (int i = 0; i < numThreads; ++i )
		{
			jobQueueEntries[i] = reinterpret_cast<hkJobQueue::JobQueueEntry*>(&sampleJobs[i]);
		}
		m_jobQueue->addJobBatch( jobQueueEntries, hkJobQueue::JOB_HIGH_PRIORITY );
		HK_TIMER_END();
	}

	m_jobThreadPool->processAllJobs( m_jobQueue );

	// On platforms with multiple CPUs (e.g. Windows, Xbox360), have the main thread take jobs too.
	// This could be done on the PLAYSTATION(R)3 too, which would force the PPU to take jobs, but probably isn't worth it.
	m_jobQueue->processAllJobs();

	// Wait for all threads to finish
	// There's no need to wait on the hkaAnimationSampleAndCombineJob's semaphore here, since we're going to end up waiting for all the jobs to finish.
	// However, if each job had its own semaphore and we wanted to wait on an specific job, this would be the place to do it.

	// Wait for the actual tasks to finish. This makes sure all timer information will have finished DMAing to main memory
	m_jobThreadPool->waitForCompletion();
	
	delete [] animData;

}

void SampleOnlyMultithreadingDemo::doMultithreadedSamplingAsynchronously(hkLocalArray<hkaPose*>& pose)
{
	HK_TIMER_BEGIN("JobSetup", HK_NULL);

	// This data must persist while the spu job is executing
	hkaSampleAnimationJob::AnimationData* animData = new hkaSampleAnimationJob::AnimationData[ NUM_ANIMATIONS ];

	m_jobQueue->setWaitPolicy(hkJobQueue::WAIT_INDEFINITELY);

	int maxBufferSize = 0;
	for (int i=0 ; i < NUM_ANIMATIONS; i++)
	{
		hkReal localTime = m_controls[i]->getLocalTime();
		m_animation->getFrameAndDelta( localTime, animData[i].m_frameIndex, animData[i].m_frameDelta );
		animData[i].m_maxTransformTrack = m_animation->m_numberOfTransformTracks;
		animData[i].m_maxFloatTrack = m_animation->m_numberOfFloatTracks;
		animData[i].m_animationOnCPU = m_animation;
		animData[i].m_poseOut = pose[i]->accessUnsyncedPoseLocalSpace().begin();

		// Populate the buffer with data from the controls.
		// Warning - if you are *not* using a hkaPose class to allocate your output array for
		// float slots you must ensure that your float slots array is a multiple of 16 bytes in size.
		// See hkaMultithreadedAnimationUtils::allocateFloatSlotsArrayRoundedUpToMultipleOf16() for details.
		animData[i].m_floatSlotsOut = pose[i]->getFloatSlotValues().begin();

	}

	HK_TIMER_END();

	// We'll distribute the work evenly over the spus.
	int numThreads = m_jobThreadPool->getNumThreads();
	numThreads++; // count the main thread too

	const int numJobs = numThreads;
	hkObjectArray<hkaSampleAnimationJob> sampleJobs( numJobs );
	sampleJobs.setSize( numJobs );

	// Allocate an array that will contain the "job finished" flags for each  animation job.  This needs to be allocated on the heap since these
	// addresses are written to from the SPU.  
	hkLocalArray<hkUint32> animationJobDoneFlags(numJobs);
	animationJobDoneFlags.setSize(numJobs, 0);

	hkaSampleAnimationJob::AnimationData* currentAnimData = animData;
	for (int s=0; s < numJobs; s++)
	{
		const int animsForJob = NUM_ANIMATIONS / numThreads + int(s < NUM_ANIMATIONS % numThreads);	// handles any remainders

		// Initialize the job to sample all the animations
		sampleJobs[s].m_animData = currentAnimData;
		sampleJobs[s].m_numAnims = animsForJob;
		sampleJobs[s].m_jobDoneSemaphore = HK_NULL;
		sampleJobs[s].m_jobDoneFlag = &(animationJobDoneFlags[s]);

		// Here we can (optionally) try and minimise the buffer size to avoid stack overflow for large animations.
		sampleJobs[s].m_bufferSize = maxBufferSize;

		// Use cache if selected
		if( m_useCache )
		{
			// Note that in general the cache parameter *will not* be used if the mulithreaded job processing 
			// takes place on spu, but will be used on cpu (other multithreaded job processing) and also
			// that it is up to the user to ensure that it is a multithreaded-safe cache 
			// (e.g. the hkaMultithreadedChunkCache).
			sampleJobs[s].m_cache = m_cache;
		}

		HK_ASSERT2(0x19e27e5b, sampleJobs[s].isValid(), "Data is invalid!");	// Sanity check before we hand off the jobs

		currentAnimData += animsForJob;
	}


	int numJobsAdded = 0;

	m_jobThreadPool->processAllJobs( m_jobQueue );

	// On platforms with multiple CPUs (e.g. Windows, Xbox360), have the main thread take jobs too
	// This could be done on the PLAYSTATION(R)3 too, which would force the PPU to take jobs, but probably isn't worth it.
	m_jobQueue->processAllJobs();

	// Next block on the PPU until all jobs are marked as done
	hkBool allJobsComplete = false;
	while( !allJobsComplete )
	{
		// To fake the asynchronous nature of job creation, just for this demo, every so often add in some more jobs.		
		if( hkMath::rand01() < 0.01f)
		{
			if(numJobsAdded < numJobs)
			{
				m_jobQueue->addJob( *reinterpret_cast<hkJobQueue::JobQueueEntry*>(&sampleJobs[numJobsAdded]), hkJobQueue::JOB_HIGH_PRIORITY);
				numJobsAdded++;
			}
		}

		// Deal with jobs which are done here
		//
		// If animationJobDoneFlags[i] == 1, then animation job 'i' is complete
		//

		// Check for completion
		allJobsComplete = true;
		for( hkInt32 i = 0; i < numJobs; ++i )
		{
			if( animationJobDoneFlags[i] == 0 )
			{
				allJobsComplete = false;
			}
		}

	}

	// Let job queue know that we are are done.
	m_jobQueue->setWaitPolicy(hkJobQueue::WAIT_UNTIL_ALL_WORK_COMPLETE);

	// Wait for the actual tasks to finish. This makes sure all timer information will have finished DMAing to main memory
	m_jobThreadPool->waitForCompletion();

	delete [] animData;

}

void SampleOnlyMultithreadingDemo::printTimingStatistics()
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

		hkString::sprintf(buf, "Sample time for %d animations, %d bones", NUM_ANIMATIONS, NUM_BONES );
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

		hkString::sprintf(buf, "Sample time for %d animations, %d bones", NUM_ANIMATIONS, NUM_BONES );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, " " );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );

		hkString::sprintf(buf, "Detailed Statistics (Hit \221 to change)" );
		m_env->m_textDisplay->outputText( buf, x, h - (y+=d) );
	}

}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Shows animation decompression and blending on the SPU" ;

#	define ST_STRING "CPU, Single-threaded"
#	define MT_STRING "CPU, Multi-threaded "

static const int demoType = HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE;

#define TO_STRING1( x ) # x
#define TO_STRING2( x ) TO_STRING1( x )

HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, MT_STRING " - Uncompressed" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 0,       "Sampling on " MT_STRING,              "Sampling on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, MT_STRING " - Delta" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 1,   "Delta decompression on " MT_STRING,   "Delta decompression on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, MT_STRING " - Wavelet" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 2, "Wavelet decompression on " MT_STRING, "Wavelet on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, MT_STRING " - Mirrored Wavelet" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 3, "Mirrored on " MT_STRING, "Mirrored on " MT_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, MT_STRING " - Spline" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 4, "Spline compression on " MT_STRING, "Spline compression on " MT_STRING);

HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, ST_STRING " - Uncompressed" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 5,       "Sampling on " ST_STRING,              "Sampling on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, ST_STRING " - Delta" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 6,   "Delta decompression on " ST_STRING,   "Delta decompression on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, ST_STRING " - Wavelet" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 7, "Wavelet decompression on " ST_STRING, "Wavelet decompression on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, ST_STRING " - Mirrored Wavelet" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 8, "Mirrored on " ST_STRING, "Mirrored on " ST_STRING);
HK_DECLARE_DEMO_VARIANT(SampleOnlyMultithreadingDemo, demoType, ST_STRING " - Spline" TO_STRING2( NUM_ANIMATIONS ) " Animations of " TO_STRING2( NUM_BONES ) " Bones " , 9, "Spline compression on " ST_STRING, "Spline compression on " ST_STRING);

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

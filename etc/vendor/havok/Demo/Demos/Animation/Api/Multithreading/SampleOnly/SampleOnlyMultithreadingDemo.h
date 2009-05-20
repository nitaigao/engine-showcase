/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef SAMPLE_ANIMATION_MULTITHREADING_DEMO_H
#define SAMPLE_ANIMATION_MULTITHREADING_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// This demo shows how to use the Animation Jobs (hkaSampleAnimationJob) to sample an animation.
// The demo does not use an animation cache by default (though it can be turned on).
//
// At runtime, the optimal sampling strategy is chosen depending on where the job is being run (i.e. depending 
// on the architecture on which the code is being run):
//  If the (multithreaded) job processing takes place on PPU/CPU then the m_animationOnCPU memeber
// of the data is accessed directly and sampling takes place through the hkaCpuSampleAnimationJob()
// method which calls animData.m_animationOnCPU->samplePartialTracks() directly.
//  Otherwise, the (multithreaded) job processing takes place on SPU then the hkaSpuSampleAnimationJob() method 
// is called, the animation data (chunks) is DMAd over, and the samplePartialWithDataChunks() method is used.
class SampleOnlyMultithreadingDemo : public hkDefaultAnimationDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		SampleOnlyMultithreadingDemo( hkDemoEnvironment* env );

		~SampleOnlyMultithreadingDemo();

		Result stepDemo();

		virtual void postConstruct() {}

	private:

		// An animation cache will be added (but not enabled until the user turns it on) only if it can be used.
		void addCacheIfUsable();

		// 'Synchronous' sampling adds all jobs to the queue at once and waits for all jobs to finish before processing results.
		void doMultithreadedSamplingSynchronously(hkLocalArray<class hkaPose*>& pose);

		// 'Asynchronous' sampling add jobs asynchronously to the queue and can process results of each job as soon as it is done.
		void doMultithreadedSamplingAsynchronously(hkLocalArray<class hkaPose*>& pose);

		void printTimingStatistics();

		void makeMirroredAnimation(class hkaAnimation* original);

		class hkLoader* m_loader;

		class hkaSkeleton* m_skeleton;

		class hkaAnimation* m_animation;

		class hkaAnimationBinding* m_binding;

		hkArray<class hkaDefaultAnimationControl*> m_controls;

		// Running in 'multithreaded' mode == using animation Jobs instead of direct sampling.
		const hkBool m_useMt;

		hkBool m_asyncSampling;
		hkBool m_simpleStatistics;

		hkBool m_useCache;
		class hkaChunkCache* m_cache;

		class WindowedAverage* m_windowedAverage;
		hkReal m_time;
};

#endif //SAMPLE_ANIMATION_MULTITHREADING_DEMO_H

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

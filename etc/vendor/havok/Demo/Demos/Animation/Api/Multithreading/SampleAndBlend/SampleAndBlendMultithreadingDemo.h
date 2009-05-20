/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef SAMPLE_AND_BLEND_ANIMATION_MULTITHREADING_DEMO_H
#define SAMPLE_AND_BLEND_ANIMATION_MULTITHREADING_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

// This demo shows how to use the Animation Jobs (hkaAnimationSampleAndCombineJob) to sample and
// blend an animated skeleton. 
// The demo does not use an animation cache by default (though it can be turned on).
//
// At runtime, the optimal sampling strategy is chosen depending on where the job is being run (i.e. depending 
// on the architecture on which the code is being run):
//  If the (multithreaded) job processing takes place on PPU/CPU then the m_animatedSkeletonOnCPU memeber
// of the data is accessed directly and sampling and blending takes place through the hkaCpuSampleAndCombineJob()
// method which calls sampleCombineJob.m_animatedSkeletonOnCPU->sampleAndCombinePartialAnimations() directly.
//  Otherwise, the (multithreaded) job processing takes place on SPU then the hkaSpuSampleAndCombineJob() method 
// is called, the animation data (chunks) is DMAd over, and the samplePartialWithDataChunks() method is used.
class SampleAndBlendMultithreadingDemo : public hkDefaultAnimationDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		SampleAndBlendMultithreadingDemo( hkDemoEnvironment* env );

		~SampleAndBlendMultithreadingDemo();

		Result stepDemo();

		virtual void postConstruct() {}

		void makeFakeInput();


	private:

		// An animation cache will be added (but not enabled until the user turns it on) only if it can be used.
		void addCacheIfUsable();

		// 'Synchronous' sampling adds all jobs to the queue at once and waits for all jobs to finish before processing results.
		void doMultithreadedSamplingSynchronously();

		// 'Asynchronous' sampling add jobs asynchronously to the queue and can process results of each job as soon as it is done.
		void doMultithreadedSamplingAsynchronously();

		void printTimingStatistics();

		void makeMirroredAnimation(class hkaAnimation* original, int index);

		class hkLoader* m_loader;

		class hkaSkeleton* m_skeleton;

		hkArray< class hkaAnimation* > m_animations;

		hkArray< class hkaAnimationBinding* > m_bindings;

		hkArray<class hkaAnimatedSkeleton*> m_activeSkeletons;

		hkArray<class hkaPose*> m_poses;

		// Running in 'multithreaded' mode == using animation Jobs instead of direct sampling.
		const hkBool m_useMt;

		hkBool m_convertLocalToModelInAnimationJob;
		hkBool m_asyncSampling;
		hkBool m_simpleStatistics;

		hkBool m_useCache;
		class hkaChunkCache* m_cache;

		class WindowedAverage* m_windowedAverage;
		hkReal m_time;
};

#endif //SAMPLE_AND_BLEND_ANIMATION_MULTITHREADING_DEMO_H

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

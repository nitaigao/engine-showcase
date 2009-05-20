/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef SAMPLE_ANIMATION_MULTITHREADING_DEMO_H
#define SAMPLE_ANIMATION_MULTITHREADING_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

class BuildKdTreeDistributedDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		BuildKdTreeDistributedDemo( hkDemoEnvironment* env );

		~BuildKdTreeDistributedDemo();

		Result stepDemo();

		void buildTreeMTDistributed();
		void buildTreeSTDistributed();

		void buildTreeMT();

		void buildTreeST();

	private:

		hkAabb m_bounds;
		class hkKdTree* m_tree;

		hkArray<const hkpCollidable*> m_collidables;

		HK_ALIGN128( hkSemaphoreBusyWait m_semaphore ); // We use this semaphore to indicate when the jobs have completed

		hkReal m_worldSizeX;
		hkReal m_worldSizeY;
		hkReal m_worldSizeZ;

		int m_numBodies;

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

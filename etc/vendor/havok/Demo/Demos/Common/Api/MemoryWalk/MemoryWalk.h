/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_GARBAGE_COLLECTION_H
#define HK_GARBAGE_COLLECTION_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/World/Memory/hkpWorldMemoryAvailableWatchDog.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Common/Base/Memory/Memory/FreeList/FixedMemoryBlockServer/hkFixedMemoryBlockServer.h>
#include <Common/Base/Memory/Memory/FreeList/hkFreeListMemory.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

class MemoryWalkDemo : public hkDefaultPhysicsDemo
{
	typedef hkDefaultPhysicsDemo Parent;
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	MemoryWalkDemo(hkDemoEnvironment* environment);
	~MemoryWalkDemo();

	Result stepDemo();

private:

	/// hkReferencedObject
	virtual void calcStatistics( hkStatisticsCollector* collector) const;

	struct Allocation
	{
		int m_start;
		int m_size;
		hkBool m_allocated;
		int m_pool;
	};

	struct Block
	{
		void* m_start;
		hk_size_t m_size;
	};

	void _drawAllocations();

	void _drawRowBlock(int row,int startX,int endX);

	static void HK_CALL _addBlock(void* start, hk_size_t size,hkBool allocated,int pool,void* param)
	{
		MemoryWalkDemo* self = (MemoryWalkDemo*)param;
		Allocation alloc;
		alloc.m_start = int(((char*)start) - self->m_memoryStart);
		alloc.m_size = int(size);
		alloc.m_allocated = allocated;
		alloc.m_pool = pool;
		self->m_allocs.pushBack(alloc);
	}
	void _updateAllocations();
	void _alloc(hk_size_t size);
	void _free(int index);

	hkArray<Allocation> m_allocs;
	hkArray<Block> m_blocks;

    int m_numRows;
	int m_width,m_height;
	int m_rowSize;
    
    hkPseudoRandomGenerator m_rand;

    char* m_memoryStart;
    hk_size_t m_memorySize;
    hkFixedMemoryBlockServer* m_server; 
    hkFreeListMemory* m_memory; 
};

#endif

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

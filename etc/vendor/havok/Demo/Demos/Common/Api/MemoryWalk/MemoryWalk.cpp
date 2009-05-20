/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/MemoryWalk/MemoryWalk.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/DebugUtil/StatisticsCollector/hkStatisticsCollector.h>

#include <Common/Base/Memory/Memory/FreeList/hkFreeListMemory.h>
#include <Common/Base/Memory/Memory/FreeList/FixedMemoryBlockServer/hkFixedMemoryBlockServer.h>
#include <Common/Base/Memory/Memory/FreeList/SystemMemoryBlockServer/hkSystemMemoryBlockServer.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

static const char* detail =
"This demo shows what happens using memory allocation: "	\
"Hopefully garbage collection saves the day!";

MemoryWalkDemo::MemoryWalkDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env ),
	m_rand(1000)
{
    m_numRows = 23;

    m_memorySize = 1024*1024;
    m_memoryStart = (char*)hkThreadMemory::getInstance().allocateChunk(int(m_memorySize),HK_MEMORY_CLASS_DEMO);

    m_server = new hkFixedMemoryBlockServer(m_memoryStart,m_memorySize);
    m_memory = new hkFreeListMemory(m_server);

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 0.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );
	}

	//	Create the world
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, 0.0f, -10.0f );
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	//
	// Create objects
	//

	m_world->unlock();

	m_width = m_env->m_window->getWidth();
	m_height = m_env->m_window->getHeight();
	m_rowSize = int(m_memorySize/m_numRows);
}


MemoryWalkDemo::~MemoryWalkDemo()
{
    delete m_memory;
    delete m_server;

    hkThreadMemory::getInstance().deallocateChunk(m_memoryStart,int(m_memorySize),HK_MEMORY_CLASS_DEMO);

	m_world->markForWrite();
	delete m_world;
	m_world = HK_NULL;
}

void MemoryWalkDemo::calcStatistics( hkStatisticsCollector* collector) const
{
	collector->addArray("allocs", m_allocs);
	collector->addArray("blocks", m_blocks);

	if (m_memoryStart)
	{
		collector->addNormalChunk("memoryChunk", m_memoryStart, int(m_memorySize));
	}

	Parent::calcContentStatistics(collector, HK_NULL);
}

void MemoryWalkDemo::_alloc(hk_size_t size)
{
    void* alloc = m_memory->allocateChunk(int(size),HK_MEMORY_CLASS_DEMO);
    if (alloc)
    {
        Block block;
        block.m_start = alloc;
        block.m_size = size;

        m_blocks.pushBack(block);

        /// zero the alloc
        hkString::memSet(alloc,0,(int)size);
    }
    else
    {
        /// Failed to alloc
    }
}

void MemoryWalkDemo::_free(int index)
{
    // We need to remove an allocation
    Block& block = m_blocks[index];
    m_memory->deallocateChunk(block.m_start,int(block.m_size),HK_MEMORY_CLASS_DEMO);
    m_blocks.removeAt(index);
}

void MemoryWalkDemo::_drawRowBlock(int row,int startX,int endX)
{
	hkgDisplayContext* context = m_env->m_displayHandler->getContext();

	float v0[4];
	float v1[4];
	float v2[4];
	float v3[4];

	hkReal width = m_width-10.0f;

	//if (startX == endX) endX++;

	v0[2] = v1[2] = v2[2] = v3[2] = -0.01f; //magic z value

	float height = (m_height-10.0f)/m_numRows;

	float y0 = (row*height) + 5.0f;
	float y1 = y0 + (height*0.6f);

	float x0 = (((float)startX)*width/m_rowSize) + 5.0f;
	float x1 = (((float)endX)*width/m_rowSize) + 5.0f;

	v0[0] = x0;
	v0[1] = y0;

	v1[0] = x1;
	v1[1] = y0;

	v2[0] = x1;
	v2[1] = y1;

	v3[0] = x0;
	v3[1] = y1;

	context->setCurrentPosition( v0);
	context->setCurrentPosition( v1);
	context->setCurrentPosition( v2);

	context->setCurrentPosition( v0);
	context->setCurrentPosition( v2);
	context->setCurrentPosition( v3);
}

void MemoryWalkDemo::_drawAllocations()
{
	hkgDisplayContext* context = m_env->m_displayHandler->getContext();

	context->lock();
	context->beginGroup( HKG_IMM_TRIANGLE_LIST );

	for (int i=0;i<m_allocs.getSize();i++)
	{
		Allocation& alloc = m_allocs[i];

		hkUint32 separateBlocks = (alloc.m_start>>4)&0x7f;
		if (!alloc.m_allocated)
		{
			separateBlocks/=4;
		}

        if (alloc.m_pool ==0)
        {
			if (alloc.m_allocated)
				context->setCurrentColorPacked( 0xffff0000 | separateBlocks|(separateBlocks<<8));
			else
				context->setCurrentColorPacked( 0xff400000 | separateBlocks|(separateBlocks<<8));
        }
        else
        {
			if (alloc.m_pool <0)
			{
				if (alloc.m_allocated)
					context->setCurrentColorPacked( 0xff0000ff | (separateBlocks<<8)|(separateBlocks<<16));
				else
					context->setCurrentColorPacked( 0xff000040 | (separateBlocks<<8)|(separateBlocks<<16));
			}
			else
			{
				if (alloc.m_allocated)
					context->setCurrentColorPacked( 0xff00ff00 | separateBlocks|(separateBlocks<<16));
				else
					context->setCurrentColorPacked( 0xff004000 | separateBlocks|(separateBlocks<<16));
			}
			
        }

		int rowStart = alloc.m_start/m_rowSize;
		int startOffset = alloc.m_start%m_rowSize;
		int rowEnd = (alloc.m_start+alloc.m_size)/m_rowSize;
		int endOffset = (alloc.m_start+alloc.m_size)%m_rowSize;

		if (rowStart == rowEnd)
		{
			_drawRowBlock(rowStart,startOffset,endOffset);
		}
		else
		{
			_drawRowBlock(rowStart,startOffset,m_rowSize);
			// Middle bits
			for (int j=rowStart+1;j<=rowEnd-1;j++)
			{
				_drawRowBlock(j,0,m_rowSize);
			}
			_drawRowBlock(rowEnd,0,endOffset);
		}
	}
	context->endGroup();
	context->unlock();

}

void MemoryWalkDemo::_updateAllocations()
{
    int num = m_rand.getRand32();

    if ((num&15)>4)
    {
        // Add an allocation
        hk_size_t size = m_rand.getRand32()%50000;
        // do the allocation
        _alloc(size);
    }

    if (((num>>8)&15)>8)
    {
        // Add an allocation
		hk_size_t size = m_rand.getRand32()%( hkThreadMemory::MEMORY_MAX_SIZE_SMALL_BLOCK + 1 );
        // do the allocation
        _alloc(size);
    }


    if ((num&0x100)&&m_blocks.getSize()>0)
    {
        _free(m_rand.getRand32()%m_blocks.getSize());
    }

    num = (m_rand.getRand32())&0xffff;
    if (num>4000 && num<4002)
    {
        /// Free all
        while (m_blocks.getSize()) { _free(0); }
    }

    if (num>405 && num<500)
    {
        m_memory->garbageCollect();
    }

    if (num>20 && num<125)
    {
        m_memory->optimize();
    }
}


hkDemo::Result MemoryWalkDemo::stepDemo()
{
	//hkReal physicsDeltaTime = 1.0f / 60.0f;

	for (int i=0;i<100;i++)
	{
		_updateAllocations();
	}

    m_allocs.clear();
    m_memory->walkMemory(_addBlock,this);

	_drawAllocations();

	hkDefaultPhysicsDemo::stepDemo();

	m_world->lock();
	m_world->unlock();

	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Drawing memory allocations.\nRed - large blocks\nGreen - Small Blocks";

HK_DECLARE_DEMO( MemoryWalkDemo, HK_DEMO_TYPE_PRIME, "Memory walk", helpString);

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

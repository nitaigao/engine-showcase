/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// The demoframework will use hkStlDebugMemory if the demos project properties
// is given the "-c" (mem Check) option in 'Debugging->Command Arguments'.
// hkStlDebugMemory will not operate in debug configuration as defined by Havok.
// Debug configuration is an optimized build with line numbers enabled.
// If there are memory leaks, their location and call stack
// are printed to the debug window.

#ifndef HK_STL_DEBUG_MEMORY_H
#define HK_STL_DEBUG_MEMORY_H

#include <Common/Base/Memory/Memory/hkMemory.h>
#include <Common/Base/Memory/StackTracer/hkStackTracer.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>

#include <Common/Base/Memory/Memory/Debug/hkDebugMemory.h>
#include <Common/Base/Memory/hkDebugMemorySnapshot.h>

// use STL here so we avoid memory recursion.
#include <map>
#if defined(HK_PLATFORM_PS3_PPU) && (_HAS_EXCEPTIONS==0) && HK_CELL_SDK_VERSION < 0x190002
	void std::exception::_Raise() const { HK_BREAKPOINT(0); }
#endif

#		define PTR_CLASS _FARQ

template <typename T>
class hkAllocator : public std::allocator<T>
{
public:

	hkAllocator() {}

	template <typename U> hkAllocator(const hkAllocator<U>&) {};

	char PTR_CLASS * _Charalloc(size_t size)
	{
		return (char PTR_CLASS *)malloc( size );
	}

	T PTR_CLASS * allocate(size_t count)
	{
		return (T PTR_CLASS *)malloc( count * sizeof(T) );
	}

	void deallocate(void PTR_CLASS * ptr, size_t )
	{
		free(ptr);
	}

	template <typename U>
	struct rebind { typedef hkAllocator<U> other; };
};

class hkDebugMemorySnapshot;

/// Memory implementation to find common memory problems.
/// hkStlDebugMemory finds leaks, multiple frees and underrun/overruns.
/// hkStlDebugMemory always scrubs allocations in contrast to hkPoolMemory
/// which only scrubs when HK_DEBUG is defined.
/// See the userguide for more details.<p>

class hkStlDebugMemory : public hkDebugMemory
{
	public:
		typedef void (HK_CALL *OutputStringFunc)(const char* s, void* arg);

		static void HK_CALL defaultOutputStringFunc(const char* s, void* a);

		virtual void allocateChunkBatch(void** blocksOut, int nblocks, int nbytes,HK_MEMORY_CLASS cl )
		{
			for( int i = 0; i < nblocks; ++i )
			{
				blocksOut[i] = allocateChunk(nbytes, cl);
			}
		}
		virtual void deallocateChunkBatch(void** blocks, int nblocks, int nbytes,HK_MEMORY_CLASS cl )
		{
			for( int i = 0; i < nblocks; ++i )
			{
				deallocateChunk(blocks[i], nbytes, cl);
			}
		}

        hkStlDebugMemory( OutputStringFunc outFunc = defaultOutputStringFunc, void* funcArg = HK_NULL, hkUint32 maxStackTrace = MAX_STACKTRACE);

        ~hkStlDebugMemory();

		virtual void* allocateChunk(int nbytes, HK_MEMORY_CLASS cl);

		virtual void deallocateChunk(void*, int nbytes, HK_MEMORY_CLASS );

		virtual void preAllocateRuntimeBlock(int nbytes, HK_MEMORY_CLASS cl);

		virtual void* allocateRuntimeBlock(int nbytes, HK_MEMORY_CLASS cl);

		virtual void deallocateRuntimeBlock(void*, int nbytes, HK_MEMORY_CLASS cl);

		virtual void provideRuntimeBlock(void*, int nbytes, HK_MEMORY_CLASS cl);

		virtual void registerStaticAddress(void*, int nbytes);

		virtual void freeRuntimeBlocks();

		virtual void printStatistics(hkOstream* c);

		virtual hkBool isOk() const;

		virtual void* findBaseAddress(void* p, int nbytes);

		virtual void lockBlock(void* p);

		virtual void unlockBlock(void* p);

        virtual void calculateStatistics(hkMemoryStatistics& stats);

        virtual hkBool hasMemoryAvailable(hk_size_t size);

		virtual void setMaxStackTraceDepth(int maxDepth);

        virtual void setLimitedMemoryListener(hkLimitedMemoryListener* listener)
        {
            hkCriticalSectionLock lock( &m_section );
            m_listener = listener;
        }

        virtual hkLimitedMemoryListener* getLimitedMemoryListener() { return m_listener; }

        virtual void setMemoryHardLimit(hk_size_t maxMemory)
        {
            hkCriticalSectionLock lock( &m_section );
            if (maxMemory==0)
            {
                m_hardLimit = maxMemory;
                return;
            }
            HK_ASSERT(0x3423434,!(maxMemory < hk_size_t(m_sysAllocsSize)));
            if (maxMemory < hk_size_t(m_sysAllocsSize)) { return; }
            HK_ASSERT(0x3423434,!(m_softLimit!=0&&maxMemory<m_softLimit));
            if (m_softLimit!=0&&maxMemory<m_softLimit) { return; }
            m_hardLimit = maxMemory;
        }
        virtual hk_size_t getMemoryHardLimit() { return m_hardLimit; }

        virtual void setMemorySoftLimit(hk_size_t maxMemory)
        {
            hkCriticalSectionLock lock( &m_section );
            if (maxMemory==0)
            {
                m_softLimit = 0;
                return;
            }
            HK_ASSERT(0x3424344,!(m_hardLimit && maxMemory > m_hardLimit));
            if (m_hardLimit && maxMemory > m_hardLimit) { return; }
            m_softLimit = maxMemory;
        }
        virtual hk_size_t getMemorySoftLimit() { return m_softLimit; }

	protected:

		void printMemoryReport();

		virtual void* internalAllocate(int nbytes, HK_MEMORY_CLASS, int flags, int alignment=16);

		virtual void internalFree(void* p, int bytesToFree, int flags);

		void outputString(const char* s)
		{
			(*m_outputStringFunc)(s, m_outputStringArg);
		}

	public:

            /// Overriding hkDebugMemory
        virtual void setAllocationMark(int mark);
        virtual int getAllocationMark() { return m_mark; }
        virtual void getSnapshot(int mark,hkBool mask,hkDebugMemorySnapshot& snapshot);
        virtual hkBool getPointerInfo(const void* ptr,PointerInfo& out);

#if defined ( HK_PLATFORM_WIN32 ) && (HK_COMPILER_MSVC_VERSION >= 1400)
		typedef std::map<void*, PointerInfo, std::less<void*>, hkAllocator< std::pair <const void*, PointerInfo> > > Map;
#else
		typedef std::map<void*, PointerInfo, std::less<void*> > Map;
#endif

	protected:

		Map m_activePointers;
		OutputStringFunc m_outputStringFunc;
		void* m_outputStringArg;
		hkUint32 m_maxStackTrace;

		hkStackTracer m_tracer;
		mutable hkCriticalSection m_section;

        int m_mark;

        /// All the statistics of the same style as the old stats collector

        int m_numSysAllocs;
            /// The total number of bytes allocated by the system memory
        int m_sysAllocsSize;
            /// The current maximum of the total number of bytes allocated by the system memory
        int m_sysAllocsHighMark;

            /// The limited memory listener
        hkLimitedMemoryListener* m_listener;

            /// The total amount of memory we're allowing allocation on
        hk_size_t m_softLimit;
        hk_size_t m_hardLimit;
};

/// The implementation.
/// The reason this is not inside of a .cpp file is so that there isn't a dependency in the lib with STL, which is
/// what the implementation uses for block tracking.

hkStlDebugMemory::hkStlDebugMemory( OutputStringFunc func, void* arg, hkUint32 maxStackTrace) :
	m_outputStringFunc(func),
	m_outputStringArg(arg),
	m_maxStackTrace(maxStackTrace),
    m_section(0),
    m_mark(0),
    m_listener(HK_NULL),
    m_softLimit(0),
    m_hardLimit(0)
{
    m_numSysAllocs =0 ;
    m_sysAllocsSize = 0;
    m_sysAllocsHighMark = 0;
}

hkStlDebugMemory::~hkStlDebugMemory()
{
	printMemoryReport();
}

void hkStlDebugMemory::setMaxStackTraceDepth(int maxDepth)
{
	m_maxStackTrace = maxDepth;
}

hkBool
hkStlDebugMemory::hasMemoryAvailable(hk_size_t size)
{
    hk_size_t limit = m_softLimit?m_softLimit:m_hardLimit;
    if (limit == 0) return true;

        /// Consider 4k as the minimum memory workspace
    if (size ==0) size = 4*1024;
    hk_size_t remaining = limit - m_sysAllocsSize;

	return size<remaining;
}

void
hkStlDebugMemory::calculateStatistics(hkMemoryStatistics& stats)
{
    stats.m_allocated = m_sysAllocsSize;
    stats.m_used = m_sysAllocsSize;
	// <js.todo.a use :: stylee
    stats.m_available = stats.INFINITE_SIZE;
    stats.m_totalAvailable = stats.INFINITE_SIZE;
	stats.m_largestBlock = stats.INFINITE_SIZE;

    if (m_hardLimit != 0)
	{
        stats.m_available = m_hardLimit - m_sysAllocsSize;
        stats.m_totalAvailable = m_hardLimit - m_sysAllocsSize;
        stats.m_largestBlock = m_hardLimit - m_sysAllocsSize;
	}
}

void	// <js.todo.aaa change mask into enum and make it first parameter
hkStlDebugMemory::getSnapshot( int mark, hkBool mask, hkDebugMemorySnapshot& snapshot )
{
	hkCriticalSectionLock lock( &m_section );

    snapshot.reset();

    int numAllocs = 0;

	// Determine how many pointers pass the mark/mask filter.
	const hkStlDebugMemory::Map::iterator m_activePointersEnd = m_activePointers.end();
	if( mask && ( mark == 0 ) )
	{
		// If the mark is a mask with no bits set (0x0000), then all pointers pass the filter.
		numAllocs = int(m_activePointers.size());
	}
	else
	{
		// Gotta count them manually.
		for( hkStlDebugMemory::Map::iterator i = m_activePointers.begin(); i != m_activePointersEnd; ++i )
		{
			PointerInfo& inInfo = i->second;
			if (mask)
			{
				if (mark == 0 || (mark & inInfo.mark) !=0 ) numAllocs++;
			}
			else
			{
				if (inInfo.mark == mark) numAllocs++;
			}
		}
	}

	// If we have no objects to report, we're done
	if( numAllocs == 0 )
	{
		return;
	}

    // Allocate the memory
    snapshot.m_size = numAllocs;
    snapshot.m_info = (PointerInfo*)hkSystemMalloc(sizeof(PointerInfo)*numAllocs,16);
    snapshot.m_pointers = (void**)hkSystemMalloc(sizeof(void*)*numAllocs,16);

	// Fill up the records
	PointerInfo* outInfo = snapshot.m_info;
	void** ptr = snapshot.m_pointers;
    for( hkStlDebugMemory::Map::iterator i = m_activePointers.begin(); i != m_activePointersEnd; ++i )
    {
		PointerInfo& inInfo = i->second;
        if (mask)
        {
            if (mark != 0 && (mark & inInfo.mark)==0) 
			{
				continue;
			}
        }
        else
        {
            if (inInfo.mark != mark) 
			{
				continue;
			}
        }

        *ptr = i->first;
        *outInfo = i->second;

		outInfo++;
		ptr++;
    }

	HK_ASSERT2( 0xdafec31b, outInfo == ( snapshot.m_info + snapshot.m_size ), "Allocated the wrong number of pointer info records." );
}

hkBool
hkStlDebugMemory::getPointerInfo(const void* ptrIn,PointerInfo& out)
{
	hkCriticalSectionLock lock( &m_section );
	void* ptr = const_cast<void*>(ptrIn);
    hkStlDebugMemory::Map::iterator i = m_activePointers.find(ptr);
    if ( i != m_activePointers.end())
    {
        out = i->second;
        return true;
    }
    return false;
}

void hkStlDebugMemory::printMemoryReport()
{
	hkCriticalSectionLock lock( &m_section );
	if( m_activePointers.size() != 0)
	{
        hkStlDebugMemory::Map::iterator i = m_activePointers.begin();
        hkStlDebugMemory::Map::iterator e = m_activePointers.end();

		outputString("**************************************************************\n");
		outputString("* BEGIN MEMORY LEAK REPORT                                   *\n");
		outputString("**************************************************************\n");

		for( ; i!=e; ++i)
		{
			const PointerInfo& pinfo = i->second;
			char buf[256];
			hkString::snprintf(buf, 256, "\n%i bytes leaked. Data at 0x%p. Stack trace follows:\n", pinfo.numBytes, i->first );
			outputString(buf);
			// skip first two frames - they are always allocateX/internalAllocate
			m_tracer.dumpStackTrace( pinfo.stackTraces + 2, pinfo.numStackTrace - 2, m_outputStringFunc, m_outputStringArg );
		}

		outputString("**************************************************************\n");
		outputString("* END MEMORY LEAK REPORT                                     *\n");
		outputString("**************************************************************\n");
	}
	else
	{
		outputString("**************************************************************\n");
		outputString("* NO HAVOK MEMORY LEAKS FOUND                                *\n");
		outputString("**************************************************************\n");
	}
}

void hkStlDebugMemory::printStatistics(hkOstream* c)
{
	hkCriticalSectionLock lock( &m_section );
	if(c)
	{
		c->printf("\n");
		c->printf("************************************************************************************** *\n");
		c->printf("* Debug memory doesn't support memory classes. Therefore no statistics available here. *\n");
		c->printf("************************************************************************************** *\n");
		c->printf("\n");
	}
}

static void HK_CALL checkUnderOverrun(void* pfree, const hkDebugMemory::PointerInfo& info)
{
	hkUint8* check = static_cast<hkUint8*>(info.realMem);
	int prepadBytes = (int)hkUlong( static_cast<char*>(pfree) - static_cast<char*>(info.realMem) );

	int i;
	for( i = 0; i < prepadBytes; ++i )
	{
		HK_ASSERT(0x6af0c498, check[i] == hkMemory::s_fillGuardArea);
	}

	check += prepadBytes + info.numBytes;

    for( i = 0; i < hkStlDebugMemory::MEMORY_PADDING; ++i )
	{
		HK_ASSERT(0x431a98ae, check[i] == hkMemory::s_fillGuardArea);
	}
}

void hkStlDebugMemory::setAllocationMark(int mark)
{
    hkCriticalSectionLock lock( &m_section );
    m_mark = mark;
}

void* hkStlDebugMemory::internalAllocate(int nbytes, HK_MEMORY_CLASS, int flags, int alignment )
{
	if( nbytes )
	{
		hkCriticalSectionLock lock( &m_section );

        if (m_hardLimit&&m_sysAllocsSize + hk_size_t(nbytes) > m_hardLimit)
        {
                /// Say we can't allocate
            if (m_listener)
            {
                m_listener->cannotAllocate(this,nbytes);
                /// See if the callback helped... if not we fail
                if (m_sysAllocsSize + hk_size_t(nbytes) > m_hardLimit)
                {
                    /// Inform we failed
                    m_listener->allocationFailure(this,nbytes);
                    /// Return NULL
                    return HK_NULL;
                }
            }
            else
            {
                // We failed
                return HK_NULL;
            }
        }

		PointerInfo pointerinfo;
		pointerinfo.numStackTrace = m_tracer.getStackTrace(pointerinfo.stackTraces, m_maxStackTrace);
		pointerinfo.numBytes = nbytes;
		pointerinfo.flags = flags;
        pointerinfo.mark = m_mark;

		HK_ASSERT( 0x1be63280, MEMORY_PADDING >= 4 );
		HK_ASSERT( 0x1be63281, (MEMORY_PADDING % sizeof(int)) == 0 );
		HK_ASSERT( 0x1be63282, (alignment % sizeof(int)) == 0 );

		// allocate a little more (MEMORY_PADDING) at each end
		hk_size_t postpad = MEMORY_PADDING;
		// if alignment greater than padding is requested we need to be careful
		hk_size_t prepad = (MEMORY_PADDING >= alignment) ? MEMORY_PADDING : alignment;
		pointerinfo.realMem = hkSystemMalloc( int(nbytes + prepad + postpad), alignment);
		char* realMem = static_cast<char*>(pointerinfo.realMem);

		// scrub memory
		hkString::memSet( realMem, s_fillGuardArea, int(prepad) );
		hkString::memSet( realMem + prepad, s_fillReturnedToUser, nbytes );
		hkString::memSet( realMem + prepad + nbytes, s_fillGuardArea, int(postpad) );

		//
		/////
		//char* start = realMem + prepad;
		//char* end   = start + nbytes;

		//char* start4 = (char*)HK_NEXT_MULTIPLE_OF(4, hkUlong(start));
		//char* end4   = (char*)HK_NEXT_MULTIPLE_OF(4, hkUlong(end+1)) - 4;

		//start4 = hkMath::min2(start4, end);
		//end4   = hkMath::max2(end4, start);

		//HK_ASSERT2(0xad8750dd, (start4 <= end4) ^ ((hkUlong(start4) & 0x3) || (hkUlong(end4) & 0x3)), "Hmm");

		//if (start4 <= end4)
		//{
		//	hkString::memSet(start, s_fillReturnedToUser, start4-start);
		//	hkReal max = HK_REAL_MAX; or use nans
		//	hkString::memSet4(start4, (int&)(max), (end4-start4)>>2);
		//	hkString::memSet(end4, s_fillReturnedToUser, end-end4);
		//}
		//else
		//{
		//	hkString::memSet( realMem + prepad, s_fillReturnedToUser, nbytes );
		//}
		/////

		void* memory = realMem + prepad;
		m_activePointers[memory] = pointerinfo;

		m_numSysAllocs += 1;
		m_sysAllocsSize += nbytes;
		if( m_sysAllocsSize > m_sysAllocsHighMark )
		{
			m_sysAllocsHighMark = m_sysAllocsSize;
		}

		return memory;
	}
	else
	{
		return HK_NULL;
	}
}

void hkStlDebugMemory::internalFree(void* pfree, int bytesToFree, int flags)
{
	hkCriticalSectionLock lock( &m_section );


    hkStlDebugMemory::Map::iterator i = m_activePointers.find(pfree);
	HK_ASSERT2( 0x356976c1, i != m_activePointers.end(), "Freeing junk pointer");
	const PointerInfo& info = i->second;

	HK_ASSERT2( 0xaf53fe12, info.m_lockCount <= 0, "Freeing locked memory block.");
	HK_ASSERT2( 0x5861b912, (info.flags & MEM_ALIGNED) == (flags & MEM_ALIGNED), "Mismatched aligned methods");
	HK_ASSERT2( 0x5861b913, (info.flags & MEM_CHUNK) == (flags & MEM_CHUNK), "Mismatched chunk methods");
	if(bytesToFree == -1)
	{
		HK_ASSERT2( 0x5861b911, (flags & MEM_CHUNK) == 0, "Calling deallocate on a chunk");
	}
	else
	{
		HK_ASSERT2( 0x743ce000, info.numBytes == bytesToFree, "Freeing chunk of wrong size");
	}

	checkUnderOverrun(pfree, info);
	{ // scrub area
		int prepadBytes = (int)hkUlong( static_cast<char*>(pfree) - static_cast<char*>(info.realMem) );
		hkString::memSet( info.realMem, s_fillFreeSpace, prepadBytes + info.numBytes + MEMORY_PADDING );
	}

	m_numSysAllocs -= 1;
	m_sysAllocsSize -= bytesToFree;

	hkSystemFree( info.realMem );
	m_activePointers.erase(i);
}

void* hkStlDebugMemory::allocateChunk(int nbytes, HK_MEMORY_CLASS c)
{
	return internalAllocate(nbytes, c, MEM_CHUNK);
}

void hkStlDebugMemory::deallocateChunk(void* p, int nbytes, HK_MEMORY_CLASS cl)
{
	if(p)
	{
		internalFree(p, nbytes, MEM_CHUNK);
	}
}

hkBool hkStlDebugMemory::isOk() const
{
	hkCriticalSectionLock lock( &m_section );
    hkStlDebugMemory::Map::const_iterator i = m_activePointers.begin();
    hkStlDebugMemory::Map::const_iterator e = m_activePointers.end();

	for( ; i!=e; ++i)
	{
		checkUnderOverrun( i->first, i->second );
	}
	return true;
}

#	include <Common/Base/Fwd/hkwindows.h>
void HK_CALL hkStlDebugMemory::defaultOutputStringFunc( const char* s, void* context)
{
	OutputDebugStringA(s);
	printf("%s",s); // Also output to console for automated testing
}

void hkStlDebugMemory::preAllocateRuntimeBlock(int nbytes, HK_MEMORY_CLASS cl)
{
}

void* hkStlDebugMemory::allocateRuntimeBlock(int nbytes, HK_MEMORY_CLASS cl)
{
	void* memory = allocateChunk(nbytes, cl);
	return memory;
}

void hkStlDebugMemory::deallocateRuntimeBlock(void* p, int nbytes, HK_MEMORY_CLASS cl)
{
	deallocateChunk(p, nbytes, cl);
}

void hkStlDebugMemory::provideRuntimeBlock(void*, int nbytes, HK_MEMORY_CLASS cl)
{
}

void hkStlDebugMemory::registerStaticAddress(void* address, int nbytes)
{
	PointerInfo pointerinfo;
	{
		pointerinfo.numBytes      = nbytes;
		pointerinfo.flags         = MEM_CHUNK;
		pointerinfo.mark          = m_mark;
		pointerinfo.realMem       = address;
	}

	m_activePointers[address] = pointerinfo;
}

void hkStlDebugMemory::freeRuntimeBlocks()
{
}

void* hkStlDebugMemory::findBaseAddress(void* p, int nbytes)
{
	// try to use p as a base address
	{
        hkStlDebugMemory::Map::iterator i = m_activePointers.find(p);
		if ( i != m_activePointers.end())
		{
			HK_ON_DEBUG( const PointerInfo& pinfo = i->second; )
			HK_ASSERT2( 0xf0457ef5, hkAddByteOffset(p, nbytes) <= hkAddByteOffset(i->first, pinfo.numBytes), "Invalid Block" );
			return i->first;
		}
	}

	// try to walk backward from p
	{
		void* q = (void*)(HK_NEXT_MULTIPLE_OF(16, hkUlong(p)));
		for (int n = 0; n < 50; n++)
		{
			q = hkAddByteOffset(q, hkUlong(-16));

            hkStlDebugMemory::Map::iterator i = m_activePointers.find(q);
			if ( i != m_activePointers.end())
			{
				HK_ON_DEBUG( const PointerInfo& pinfo = i->second; )
				HK_ASSERT2( 0xf0457ef5, hkAddByteOffset(p, nbytes) <= hkAddByteOffset(i->first, pinfo.numBytes), "Invalid Block" );
				return i->first;
			}
		}
	}



	// search the full list
	{
        hkStlDebugMemory::Map::iterator i = m_activePointers.begin();
        hkStlDebugMemory::Map::iterator e = m_activePointers.end();

		for( ; i!=e; ++i)
		{
			const PointerInfo& pinfo = i->second;
			if ( i->first <= p && hkAddByteOffset(p, nbytes) <= hkAddByteOffset(i->first, pinfo.numBytes) )
			{
				return i->first;
			}
		}
	}

	HK_ASSERT2(0xaf543fe2, false, "Invalid memory block.");
	return HK_NULL;
}

void hkStlDebugMemory::lockBlock(void* p)
{
    hkStlDebugMemory::Map::iterator i = m_activePointers.find(p);
	PointerInfo& pinfo = i->second;
	pinfo.m_lockCount++;
}

void hkStlDebugMemory::unlockBlock(void* p)
{
    hkStlDebugMemory::Map::iterator i = m_activePointers.find(p);
	PointerInfo& pinfo = i->second;
	pinfo.m_lockCount--;
}

#endif // HK_STL_DEBUG_MEMORY_H


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

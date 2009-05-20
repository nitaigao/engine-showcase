/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DEBUG_MEMORY_H
#define HK_DEBUG_MEMORY_H

#include <Common/Base/Memory/Memory/hkMemory.h>

class hkDebugMemorySnapshot;

/// The following is an interface that 'debugging' versions of the memory manager can derive from
/// A implementation which does derive and work as the interface intends is the hkStlDebugManager

class hkDebugMemory : public hkMemory
{
	public:

		hkDebugMemory();

			/// Returns either HK_NULL if no debug memory is available (e.g. on PlayStation(R)2 using GCC) or a valid pointer to the debug memory.
		static hkDebugMemory* HK_CALL create();

            /// It is debug memory
		virtual bool isDebugMemory(){ return true; }

            /// Definition of debug memory block tracking structure
        enum { MAX_STACKTRACE = 15 };
		enum { MEMORY_PADDING = 16 };
		enum MemFlags
		{
			MEM_DEFAULT = 0,
			MEM_CHUNK = 1,
			MEM_ALIGNED = 2
		};

		struct PointerInfo
		{
			PointerInfo() : realMem(HK_NULL), numStackTrace(0), numBytes(-1), flags(MEM_DEFAULT), m_lockCount(0) { }
			void* realMem;
			hkUlong stackTraces[MAX_STACKTRACE];
			int numStackTrace;
			int numBytes;
			int flags;
			int m_lockCount;
            int mark;
		};

            /// All subsequent allocations will have this mark associated with them
        virtual void setAllocationMark(int mark) =0;
            /// Get the mark
        virtual int getAllocationMark() =0;

            /// Fills in the snapshot structure, with allocations which 'match' mark
            /// if mask is false, mark value must be equal
            /// if mask is true then the mark is anded,and if the result is non zero then
            /// if mask is true and mark is 0 then all allocations will be returned
        virtual void getSnapshot(int mark,hkBool mask,hkDebugMemorySnapshot& snapshot)=0;

            /// For a given pointer will return true if there is a block starting at this address
            /// in out
        virtual hkBool getPointerInfo(const void* ptr,PointerInfo& out) =0;

			/// In certain cases it might be necessary to manually register e.g. static memory blocks
			/// with the debug memory. Use this function to do this.
		virtual void registerStaticAddress(void*, int nbytes) = 0;

		virtual void setMaxStackTraceDepth(int maxDepth) = 0;
};

#endif // HK_DEBUG_MEMORY_H


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

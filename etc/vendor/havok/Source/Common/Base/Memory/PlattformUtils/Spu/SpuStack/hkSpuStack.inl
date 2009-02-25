/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



hkSpuStack::hkSpuStack()
{
	m_next = HK_NULL;
	m_end = 0;
}

HK_FORCE_INLINE hkSpuStack::~hkSpuStack()
{
}

void hkSpuStack::initMemory(void* p, int size)
{
	m_next = p;
	m_end = hkAddByteOffset(p, size);

}


void* hkSpuStack::allocateStackRoundSizeTo128WithCriticalAssert(int numBytes, const char* what)
{
	int allocationSize = HK_NEXT_MULTIPLE_OF(128, numBytes);
#if defined HK_DEBUG_SPU || defined HK_DEBUG
	int freeStackSize = getFreeStackSize(); 
#endif
	HK_CRITICAL_ASSERT2(0xaf8365df, freeStackSize >= numBytes, "Out of stack memory.");

	return allocateStack( allocationSize, what );
}

void* hkSpuStack::allocateStackRoundSizeTo128(int numBytes, const char* what)
{
	int allocationSize = HK_NEXT_MULTIPLE_OF(128, numBytes);
	return allocateStack( allocationSize, what );
}

void* hkSpuStack::allocateStack(int numBytes, const char* what)
{
	HK_ASSERT2(0xaf8365de, !(numBytes & 0x7f) , "Allocation-size should be a multiple of 128.");
	HK_ON_DEBUG( int freeStackSize = getFreeStackSize(); )
	HK_ASSERT2(0xaf8365df, freeStackSize >= numBytes, "Out of stack memory.");

	void* current = m_next;

	m_next = hkAddByteOffset(current, numBytes);


	return current;
}



void hkSpuStack::deallocateStack(void* p)
{

	m_next = p;
}

void hkSpuStack::shrinkAllocatedStack(void* p, int newSize )
{
	m_next = hkAddByteOffset(p, newSize);
}



void hkSpuStack::deallocateStack(int numBytes)
{

	m_next = hkAddByteOffset(m_next.val(), -numBytes);
}




/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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

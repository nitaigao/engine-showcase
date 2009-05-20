/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <class T, class CmpT>
hkHeapArray<T,CmpT>::hkHeapArray()
	: m_heap()
	, m_finder()
	, m_locked(false) 
{ 
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::clear() 
{ 
	m_heap.clear(); 
	m_finder.clear(); 
	m_locked = false; 
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::reserve(hkUint32 s) 
{ 
	m_heap.reserve(s); 
	m_finder.reserve(s); 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkUint32 
hkHeapArray<T,CmpT>::getSize() const 
{ 
	return m_heap.getSize(); 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkBool 
hkHeapArray<T,CmpT>::isEmpty() const 
{ 
	return m_heap.isEmpty(); 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkBool 
hkHeapArray<T,CmpT>::locked() const 
{ 
	return m_locked; 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkBool 
hkHeapArray<T,CmpT>::removed(hkUint32 i) const 
{ 
	HK_ASSERT(0x8033f400,valid(i)); 
	return (int(m_finder[i]) >= m_heap.getSize()); 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkBool 
hkHeapArray<T,CmpT>::valid(hkUint32 i) const 
{ 
	return (i < (hkUint32)m_finder.getSize()); 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkUint32 
hkHeapArray<T,CmpT>::position(hkUint32 i) const 
{ 
	HK_ASSERT(0x8033f401,valid(i)); 
	return (m_heap[i].m_index); 
}

template <class T, class CmpT>
HK_FORCE_INLINE const T & 
hkHeapArray<T,CmpT>::top() const 
{ 
	HK_ASSERT(0x8033f402,! isEmpty()); 
	return m_heap[0].m_elem; 
}

template <class T, class CmpT>
HK_FORCE_INLINE const T & 
hkHeapArray<T,CmpT>::peek(hkUint32 i) const 
{ 
	HK_ASSERT(0x8033f403,! removed(i)); 
	return (m_heap[m_finder[i]].m_elem); 
}

template <class T, class CmpT>
HK_FORCE_INLINE const T & 
hkHeapArray<T,CmpT>::operator [] (hkUint32 i) const 
{ 
	return peek(i); 
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::lock() 
{ 
	HK_ASSERT(0x8033f404,! locked()); 
	m_locked =true; 
}

template <class T, class CmpT>
HK_FORCE_INLINE hkUint32 
hkHeapArray<T,CmpT>::push(const T & e) 
{
	HK_ASSERT(0x8033f405,! locked());
	const hkUint32 id = getSize();
	m_finder.pushBack(id);
	m_heap.pushBack(Linker(e, id));
	adjust(id);
	return id;
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::pop() 
{
	HK_ASSERT(0x8033f406,locked());
	HK_ASSERT(0x8033f407,! isEmpty());
	swap(0, getSize() - 1);
	m_heap.popBack();
	if (! isEmpty()) adjust(0);
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::erase(hkUint32 i) 
{
	HK_ASSERT(0x8033f408,locked());
	HK_ASSERT(0x8033f409,! removed(i));
	const hkUint32 j = m_finder[i];
	swap(j, getSize() - 1);
	m_heap.popBack();
	if (j != getSize()) adjust(j);
}

template <class T, class CmpT>
void 
hkHeapArray<T,CmpT>::update(hkUint32 i, const T & e) 
{
	HK_ASSERT(0x8033f40a,locked());
	HK_ASSERT(0x8033f40b,! removed(i));
	const hkUint32 j = m_finder[i];
	m_heap[j].m_elem = e;
	adjust(j);
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::adjust(hkUint32 i) 
{
	HK_ASSERT(0x8033f40c,i < (hkUint32)m_heap.getSize());
	hkUint32 j;

	// Check the upper part of the heap
	for (j = i; (j > 0) && (less(m_heap[(j - 1) / 2], m_heap[j])); j = ((j - 1) / 2))
		swap(j, (j - 1) / 2);

	// Check the lower part of the heap
	for (i = j; (j = 2 * i + 1) < getSize(); i = j) 
	{
		if ((j + 1 < getSize()) && (less(m_heap[j], m_heap[j + 1])))
			++j;

		if (less(m_heap[j], m_heap[i]))
			return;

		swap(i, j);
	}
}

template <class T, class CmpT>
HK_FORCE_INLINE void 
hkHeapArray<T,CmpT>::swap(hkUint32 a, hkUint32 b) 
{
	Linker tmp(m_heap[a].m_elem,m_heap[a].m_index); 
	m_heap[a].m_elem  = m_heap[b].m_elem;
	m_heap[a].m_index = m_heap[b].m_index;
	m_heap[b].m_elem  = tmp.m_elem;
	m_heap[b].m_index = tmp.m_index;
	m_finder[(m_heap[a].m_index)] = a;
	m_finder[(m_heap[b].m_index)] = b;
}

template <class T, class CmpT>
HK_FORCE_INLINE hkBool 
hkHeapArray<T,CmpT>::less(const Linker & a, const Linker & b) 
{ 
	return m_compare(a.m_elem, b.m_elem); 
}

template <class T, class CmpT>
HK_FORCE_INLINE 
hkHeapArray<T,CmpT>::Linker::Linker(const T & e, hkUint32 i) 
	: m_elem(e)
	, m_index(i) 
{ 
}

template <class T, class CmpT>
HK_FORCE_INLINE 
hkHeapArray<T,CmpT>::Linker::~Linker() 
{ 
}


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

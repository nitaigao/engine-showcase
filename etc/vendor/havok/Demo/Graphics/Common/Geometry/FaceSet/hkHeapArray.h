/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HKBASE_HKHEAPARRAY_H
#define HKBASE_HKHEAPARRAY_H

#include <Common/Base/Container/Array/hkObjectArray.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

// mutable heap
// can be interfaced pretty much like an array
template <class T, class CmpT = hkAlgorithm::less<T> >
class hkHeapArray
{
    public:

    hkHeapArray();

    HK_FORCE_INLINE void clear();

    HK_FORCE_INLINE void reserve(hkUint32 s);
    HK_FORCE_INLINE hkUint32 getSize() const;

    HK_FORCE_INLINE hkBool isEmpty() const;
    HK_FORCE_INLINE hkBool locked() const;
    HK_FORCE_INLINE hkBool removed(hkUint32 i) const;
    HK_FORCE_INLINE hkBool valid(hkUint32 i) const;

    HK_FORCE_INLINE hkUint32 position(hkUint32 i) const;

    HK_FORCE_INLINE const T & top() const;
    HK_FORCE_INLINE const T & peek(hkUint32 i) const;
    HK_FORCE_INLINE const T & operator [] (hkUint32 i) const;

    HK_FORCE_INLINE void lock();
    HK_FORCE_INLINE hkUint32 push(const T & e);
    HK_FORCE_INLINE void pop();
    HK_FORCE_INLINE void erase(hkUint32 i);
    void update(hkUint32 i, const T & e);

    protected:

    hkHeapArray(const hkHeapArray &);
    hkHeapArray & operator = (const hkHeapArray &);

    class Linker
    {
    public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, Linker);
		HK_FORCE_INLINE Linker(const T & e, hkUint32 i);
		~Linker();

		T        m_elem;
		hkUint32 m_index;
    };

    typedef hkObjectArray<Linker>   LinkedHeap;
    typedef hkArray<hkUint32> Finder;

    HK_FORCE_INLINE void adjust(hkUint32 i);
    HK_FORCE_INLINE void swap(hkUint32 a, hkUint32 b);
    HK_FORCE_INLINE hkBool less(const Linker & a, const Linker & b);

    LinkedHeap m_heap;
    Finder     m_finder;
    CmpT       m_compare;
    hkBool     m_locked;
};

#include <Graphics/Common/Geometry/FaceSet/hkHeapArray.inl>

#endif // HKBASE_HKHEAPARRAY_H

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

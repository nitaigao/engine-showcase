/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEXMAP_H
#define HK_GRAPHICS_VERTEXMAP_H

#include <Common/Base/Container/PointerMap/hkPointerMapBase.h>
#include <Common/Base/Container/Tree/hkAnderssonTree.h>
#include <Common/Base/Container/Array/hkArray.h>

/// A class to map between vertices and indices. In particular, this can be a one-to-many mapping
class hkgVertexMap
{
	// map idx -> arrayentry(array<vtx>)
	// array<vtx> saves vtxpos in vtxarray

	public:

		typedef hkAATree::Iterator Iterator;

		hkgVertexMap(hkAATree::compareFunc cmp=hkAATree::defaultCompare, 
					 hkAATree::cloneFunc dup=hkAATree::defaultClone, 
					 hkAATree::destroyFunc rel=hkAATree::defaultDestroy);

		~hkgVertexMap();

			///	Insert key with associated value val. Keys are unique and non null.
			/// If key already exists, the new value will be appended to the array of the key.
		void insert( hkUlong key, hkUlong val );

			/// If key present, return a pointer to the index array, or NULL if not present
		const hkArray<hkUlong>* get( hkUlong key ) const;

			/// Return the number of unique keys in this map.
		HK_FORCE_INLINE int getSize() const
		{
			return m_tree.getSize();
		}

			/// clear everything
		void clear();

			/// Estimates and sets the appropriate sizes for a given number of keys.
		HK_FORCE_INLINE void reserve( int numElements )
		{
			m_array.reserve(numElements);
			m_map.reserve(numElements);
			HK_ASSERT(0x74563de4,hkMemory::getInstance().isOk());
		}

			/// Get an iterator over the keys of this map. User is responsible for deleting it.
		HK_FORCE_INLINE Iterator* createIterator()
		{
			Iterator* iter = new Iterator;
			iter->first(&m_tree);
			return iter;
		}

			/// Get the key at iterator i.
		HK_FORCE_INLINE hkUlong getKey( Iterator* i ) const
		{
			return (hkUlong)(i->m_it->m_data);
		}

			/// Get the value at iterator i.
		HK_FORCE_INLINE const hkArray<hkUlong>* getValue( Iterator* i ) const
		{
			hkUlong id = (hkUlong)(i->m_it->m_data);
			hkUlong slot;
			HK_ON_DEBUG(hkResult ok =) m_map.get( id, &slot );
			HK_ASSERT2(0x74563de3,ok == HK_SUCCESS,"inconsistency: key in search tree but not in map\n" );
			return m_array[int(slot)];
		}

			/// Get the next iterator after i.
		HK_FORCE_INLINE void next( Iterator* i ) const
		{
			i->next();
		} 

			/// Return if the iterator has not reached the end.
		HK_FORCE_INLINE hkBool isValid( Iterator* i ) const
		{
			hkUlong id = (hkUlong)(i->m_it->m_data);
			if (id == 0) 
				return false;

			hkUlong slot;
			hkResult ok = m_map.get( id, &slot );
			if (ok == HK_FAILURE) 
				return false;

			return true;
		}

	public:

#if !defined(HK_PLATFORM_SPU)
	protected:
#endif
		hkPointerMapBase<hkUlong>   m_map;
		hkAATree                    m_tree;
		hkArray<hkArray<hkUlong>* > m_array;
};

#endif // HK_GRAPHICS_VERTEXMAP_H

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

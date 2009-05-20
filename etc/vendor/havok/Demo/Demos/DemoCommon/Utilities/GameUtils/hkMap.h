/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKMAP_H
#define HKBASE_HKMAP_H

#include <Common/Base/hkBase.h>

/// A struct to hold a pair of elements.
template <typename F, typename S>
struct hkPair
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ARRAY, hkPair );

	hkPair()
	{
	}
	
	hkPair( const F& f, const S& s)
		: first(f), second(s)
	{
	}
	
	F first;
	S second;
};


/// This class is deprecated. Use hkPointerMap instead.<br>
///	<br>
/// This is <i>not</i> a complete replacement for the STL map,
/// and should be used only with plain old data types
/// (or objects that have a trivial constructor, destructor,
/// copy constructor and assignment operator that are equivalent 
/// to memcpy), as constructors, destructors, etc. are not called.
/// If you want to create a map of objects, you should use hkObjectMap instead.
template <typename KEY, typename VALUE, typename STORAGE=hkArray< hkPair<KEY,VALUE> > >
class hkMap
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(HK_MEMORY_CLASS_MAP, sizeof(hkMap<KEY,VALUE,STORAGE>));

		typedef typename STORAGE::iterator iterator;
		typedef typename STORAGE::const_iterator const_iterator;

			///
		hkMap() { }

			/// Get the value with the key k.
			/// If the key is not in the map, it is added and the returned reference
			/// can be used to set a corresponding value.
		VALUE& operator[] (const KEY& k);

			/// Finds the value with the key k.
		const VALUE* find(const KEY& k) const;

			/// Return the value with the key k or the default value d if not found.
		const VALUE& get(const KEY& k, const VALUE& d) const;

			/// Erases the key-value pair with the key k.
		hkResult erase(const KEY& k);

			/// Erase all key/value pairs.
		void clear();

			/// Returns an STL-like iterator to the first element.
		HK_FORCE_INLINE iterator begin();

			/// Returns an STL-like iterator to the 'one past the last' element.
		HK_FORCE_INLINE iterator end();

			/// Returns an STL-like const iterator to the first element.
		HK_FORCE_INLINE const_iterator begin() const;

			/// Returns an STL-like const iterator to the 'one past the last' element.
		HK_FORCE_INLINE const_iterator end() const;

	public:

			///The map's key-value pairs.
		STORAGE m_storage;
};

#include <Demos/DemoCommon/Utilities/GameUtils/hkMap.inl>

#endif // HKBASE_HKMAP_H

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

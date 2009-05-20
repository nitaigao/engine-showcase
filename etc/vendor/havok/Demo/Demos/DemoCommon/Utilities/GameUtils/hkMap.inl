/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template <typename K, typename V, typename S>
V& hkMap<K,V,S>::operator[] (const K& k)
{
	for(int i=0; i<m_storage.getSize(); ++i)
	{
		if( m_storage[i].first == k )
		{
			return m_storage[i].second;
		}
	}
	hkPair<K,V>& p = *m_storage.expandBy(1);
	p.first = k;
	return p.second;
}

template <typename K, typename V, typename S>
const V* hkMap<K,V,S>::find(const K& k) const
{
	for(int i=0; i<m_storage.getSize(); ++i)
	{
		if( m_storage[i].first == k )
		{
			return &m_storage[i].second;
		}
	}
	return HK_NULL;
}

template <typename K, typename V, typename S>
const V& hkMap<K,V,S>::get(const K& k, const V& defval) const
{
	if( const V* v = find(k) )
	{
		return *v;
	}
	else
	{
		return defval;
	}
}

template <typename K, typename V, typename S>
hkResult hkMap<K,V,S>::erase(const K& k)
{
	for(int i = m_storage.getSize() - 1; i >= 0; --i)
	{
		if( m_storage[i].first == k )
		{
			m_storage.removeAt(i);
			return HK_SUCCESS;
		}
	}
	return HK_FAILURE;
}

template <typename K, typename V, typename S>
void hkMap<K,V,S>::clear()
{
	m_storage.clear();
}

template <typename K, typename V, typename S>
typename hkMap<K,V,S>::iterator hkMap<K,V,S>::begin() 
{
	return m_storage.begin();
}


template <typename K, typename V, typename S>
typename hkMap<K,V,S>::iterator hkMap<K,V,S>::end()
{
	return m_storage.end();
}


template <typename K, typename V, typename S>
typename hkMap<K,V,S>::const_iterator hkMap<K,V,S>::begin() const
{
	return m_storage.begin();
}


template <typename K, typename V, typename S>
typename hkMap<K,V,S>::const_iterator hkMap<K,V,S>::end() const
{
	return m_storage.end();
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

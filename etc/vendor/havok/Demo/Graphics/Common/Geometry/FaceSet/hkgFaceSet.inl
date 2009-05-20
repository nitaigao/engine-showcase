/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline const hkgVertexSet*	hkgFaceSet::getVertexSet() const 
{ 
	return m_vertices; 
}

inline hkgVertexSet* hkgFaceSet::getVertexSet() 
{ 
	return m_vertices; 
} 

inline void hkgFaceSet::addPrimitive(hkgFaceSetPrimitive* p)
{
	HK_ASSERT(0x1814bd44,  p->hasData() );
	
	if (p->getType() == HKG_TRI_LIST)
		m_flags = HKG_FACE_SET_UNOPTIMIZED; // status back open again 
	
	m_flags &= ~(HKG_FACE_SET_REORDERED | HKG_FACE_SET_PLATFORM_OPT);

	if (p->getBlendMatrices() && (m_maxPrimitiveBones < p->getBlendMatrices()->getNumMatrices()) )
	{
		m_maxPrimitiveBones = p->getBlendMatrices()->getNumMatrices(); // keep it up to date.
	}

	m_primitives.pushBack(p);
}

inline int hkgFaceSet::getNumPrimitives() const
{
	return m_primitives.getSize();
}

inline const hkgFaceSetPrimitive* hkgFaceSet::getPrimitive(int i) const
{
	HK_ASSERT(0x353e9261,  i>=0 && i < m_primitives.getSize());
	return m_primitives[i];
}

inline hkgFaceSetPrimitive* hkgFaceSet::getPrimitive(int i) 
{
	HK_ASSERT(0x353e9261,  i>=0 && i < m_primitives.getSize());
	return m_primitives[i];
}

inline int hkgFaceSet::getMaxPrimitiveBones() const
{
	return m_maxPrimitiveBones;
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

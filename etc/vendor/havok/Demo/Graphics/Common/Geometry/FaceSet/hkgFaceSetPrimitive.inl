/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgFaceSetPrimitive::hkgFaceSetPrimitive( HKG_FACE_RENDER_TYPE t )
: m_type(t), m_indices(HK_NULL), m_vertBufStartIndex(0), m_len(0), m_blendMatrices(HK_NULL)
{
}



inline hkgFaceSetPrimitive::~hkgFaceSetPrimitive()
{
	setLength(0);

	if (m_blendMatrices)
		m_blendMatrices->release();
}

inline int hkgFaceSetPrimitive::getLength() const
{
	return m_len;
}

inline HKG_FACE_RENDER_TYPE hkgFaceSetPrimitive::getType() const
{
	return m_type;
}

inline void hkgFaceSetPrimitive::setVertexBufferStartIndex( int n )
{
	m_vertBufStartIndex = n;
}

inline int hkgFaceSetPrimitive::getVertexBufferStartIndex() const
{
	return m_vertBufStartIndex;
}

inline const unsigned short* hkgFaceSetPrimitive::getIndices() const 
{ 
	return m_indices; 
}

inline unsigned short*	hkgFaceSetPrimitive::getIndices() 
{
	return m_indices; 
}

inline void hkgFaceSetPrimitive::setIndices(unsigned short* conn, int len)
{
	if (m_indices)
		setLength(0); // deallocate previous

	m_len = len;
	m_indices = conn;
}

inline bool hkgFaceSetPrimitive::hasData() const
{
	return (m_len > 0);
}

inline hkgBlendMatrixSet* hkgFaceSetPrimitive::getBlendMatrices() const
{	
	return m_blendMatrices;
}

inline void hkgFaceSetPrimitive::setBlendMatrices(hkgBlendMatrixSet* bm)
{
	if (bm)
		bm->reference();
	if (m_blendMatrices)
		m_blendMatrices->release();
	m_blendMatrices = bm;
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

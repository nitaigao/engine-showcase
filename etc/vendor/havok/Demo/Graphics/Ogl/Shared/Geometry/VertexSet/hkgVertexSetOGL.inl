/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkgVertexSetOGL::hkgVertexSetOGL(hkgDisplayContext* context) 
: hkgVertexSet( context),
#ifdef HK_OGL_USE_INTERLEAVED_ARRAYS
	m_buffer(HK_NULL),
	m_oglFormat(0), 
	m_vboBuffer(0)
{

}
#else
	m_vboPos(0), 
	m_vboNormal(0), 
	m_vboWeights(0), 
	m_vboIndices(0),
	m_vboCol(0)
{
	m_vboTex[0] = 0;
	m_vboTex[1] = 0;
	m_vboTex[2] = 0;
	m_vboTex[3] = 0;
	m_vboTangents[0] = 0;
	m_vboTangents[1] = 0;

	m_interleaved = false;
	hkString::memSet(&m_buffers, 0, sizeof(Buffers));
}
#endif		

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET_DX10_H
#define HK_GRAPHICS_VERTEX_SET_DX10_H

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

class hkgVertexSetDX10 : public hkgVertexSet
{
public:

	static hkgVertexSet* createVertexSetDX10(hkgDisplayContext* context)
	{
		return new hkgVertexSetDX10(context);
	}	

	virtual ~hkgVertexSetDX10();

	virtual void setNumVerts(int nv, HKG_VERTEX_FORMAT format, bool dynamic = false);

	virtual unsigned char* lock(HKG_LOCK_FLAG f, int offset = 0, int len = -1);  
	virtual void unlock(int offset = 0, int len = -1);

		/// Set the client pointers / stream pointers to this data. Passed the current 
		/// enabled streams so that it can match what it needs.
	virtual void setStreams(); 

protected:

	hkgVertexSetDX10(hkgDisplayContext* context);

	ID3D10Buffer*						m_vertexBuffer;		// interleaved vertex buffer
	hkArray<D3D10_INPUT_ELEMENT_DESC>	m_vertexDescArray;		// Vertex declaration for shaders
	class hkgDisplayContextDX10*		m_context;
	
	hkArray<unsigned char>				m_localVerts;
	int									m_curLockOffset;
	int									m_curLockLen;

	int m_maxHardwareBlendMatrices;
};

#include <Graphics/Dx10/Shared/Geometry/VertexSet/hkgVertexSetDX10.inl>

#endif //HK_GRAPHICS_VERTEX_SET_DX10_H

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

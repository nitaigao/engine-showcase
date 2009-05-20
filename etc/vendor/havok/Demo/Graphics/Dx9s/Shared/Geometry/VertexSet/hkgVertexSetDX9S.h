/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET_DX9S_H
#define HK_GRAPHICS_VERTEX_SET_DX9S_H

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

#define HKG_DX9S_ENABLE_LOCAL_BUFFER_CACHE 1

class hkgVertexSetDX9S : public hkgVertexSet
{
public:

	static hkgVertexSet* createVertexSetDX9S( hkgDisplayContext* context)
	{
		return new hkgVertexSetDX9S( context);
	}	

	virtual ~hkgVertexSetDX9S();

	virtual void setNumVerts(int nv, HKG_VERTEX_FORMAT format, bool dynamic = false);

	virtual unsigned char* lock(HKG_LOCK_FLAG f,int offset = 0, int len = -1);  
	virtual void unlock(int offset = 0, int len = -1);

		/// Set the client pointers / stream pointers to this data. Passed the current 
		/// enabled streams so that it can match what it needs.
	virtual void setStreams(); 
	virtual void clearStreams(); 

	inline bool isBufferInHardware() const; 

	void invalidateVB();
	void restoreVB();

protected:

	hkgVertexSetDX9S( hkgDisplayContext* context);

	LPDIRECT3DVERTEXBUFFER9			m_pVB;		// interleaved vertex buffer
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;      // Vertex declaration for shaders
	LPDIRECT3DVERTEXDECLARATION9	m_pInstancedVertexDeclaration;      // Vertex declaration for shaders
	
	int m_maxHardwareBlendMatrices;
	bool m_hardwareVB;

#if HKG_DX9S_ENABLE_LOCAL_BUFFER_CACHE
	unsigned char* m_localBuffer;
#endif

};

#include <Graphics/Dx9s/Shared/Geometry/VertexSet/hkgVertexSetDX9S.inl>

#endif //HK_GRAPHICS_VERTEX_SET_DX9S_H

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

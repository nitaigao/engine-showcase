/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET_OGLS_H
#define HK_GRAPHICS_VERTEX_SET_OGLS_H

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>


#ifdef HK_PLATFORM_PS3_PPU
#define HKG_OGLS_ENABLE_SYNC_FENCES 0	// PS3 only, but will alter the performance (really badly) of read access etc as all buffers will be mapped READ_WRITE	
#define HKG_OGLS_ENABLE_LOCAL_BUFFER_CACHE 1  // keep a cpu side copy. Fast read, all perf goes into unlock() as that will dfo the buf update on write. Try to use partial locks too as they will improve perf as the buffers are non interleaved.
#else
#define HKG_OGLS_ENABLE_SYNC_FENCES 0 // do not turn on on PC etc
#define HKG_OGLS_ENABLE_LOCAL_BUFFER_CACHE 1
#endif

class hkgVertexSetOGLS : public hkgVertexSet
{
public:

	static hkgVertexSet* createVertexSetOGLS( hkgDisplayContext* context)
	{
		return new hkgVertexSetOGLS( context);
	}

	virtual ~hkgVertexSetOGLS();

	virtual void setNumVerts(int nv, HKG_VERTEX_FORMAT format, bool dynamic = false);

	virtual unsigned char* lock(HKG_LOCK_FLAG f, int offset = 0, int len = -1);  
	virtual unsigned char* partialLock(HKG_LOCK_FLAG f, HKG_VERTEX_FORMAT vertexFormatPartsToLock, int offset = 0, int len = -1);

	virtual void unlock(int offset = 0, int len = -1);

	virtual void setStreams(); 
	virtual void clearStreams();

protected:

	inline hkgVertexSetOGLS( hkgDisplayContext* context);

	struct Buffers 
	{
		unsigned char*	m_pos;		// sys mem buffer
		unsigned char*	m_normal;	// sys mem buffer
		unsigned char*	m_weights; // sys mem buffer
		unsigned char*	m_indices; // sys mem buffer
		unsigned char*	m_tex[4];		// sys mem buffer
		unsigned char*	m_tangents[2]; // sys mem buffer		
		unsigned char*	m_col;		// sys mem buffer
	};

	Buffers			m_buffers;
	GLuint			m_fence;
	HKG_VERTEX_FORMAT m_currentLockFormat;

	bool	m_vboPosInit;
	bool	m_vboNormalInit;
	bool	m_vboWeightsInit; 
	bool	m_vboIndicesInit; 
	bool	m_vboTexInit[4];	
	bool	m_vboTangentsInit[2];
	bool	m_vboColInit;		

	GLuint			m_vboPos;		// vid mem buffer, if supported
	GLuint			m_vboNormal;	// vid mem buffer, if supported
	GLuint			m_vboWeights; // vid mem buffer, if supported
	GLuint			m_vboIndices; // vid mem buffer, if supported
	GLuint			m_vboTex[4];		// vid mem buffer, if supported
	GLuint			m_vboTangents[2]; // vid mem buffer, if supported	
	GLuint			m_vboCol;		// vid mem buffer, if supported
};

#include <Graphics/Ogls/Shared/Geometry/VertexSet/hkgVertexSetOGLS.inl>

#endif //HK_GRAPHICS_VERTEX_SET_OGLS_H

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

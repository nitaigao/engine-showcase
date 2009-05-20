/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET_OGL_H
#define HK_GRAPHICS_VERTEX_SET_OGL_H

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

//#define HK_OGL_USE_INTERLEAVED_ARRAYS

class hkgVertexSetOGL : public hkgVertexSet
{
public:

	static hkgVertexSet* createVertexSetOGL(hkgDisplayContext* context)
	{
		return new hkgVertexSetOGL(context);
	}

	virtual ~hkgVertexSetOGL();

	virtual void setNumVerts(int nv, HKG_VERTEX_FORMAT format, bool dynamic = false);

	virtual unsigned char* lock(HKG_LOCK_FLAG f, int offset = 0, int len = -1);  
	virtual void unlock(int offset = 0, int len = -1);

	virtual void setStreams(); 
	virtual void clearStreams();

protected:

	inline hkgVertexSetOGL(hkgDisplayContext* context);

	//An interleaved version (contigious in mem, with every vertex following each other)
#ifdef HK_OGL_USE_INTERLEAVED_ARRAYS

	unsigned char*	m_buffer; // sys mem buffer
	GLenum			m_oglFormat;
	GLuint			m_vboBuffer; // vid mem buffer, if supported

#else // non interleaved

	struct Buffers 
	{
		unsigned char*	m_pos; // sys mem buffer
		unsigned char*	m_normal; // sys mem buffer
		unsigned char*	m_weights; // sys mem buffer
		unsigned char*	m_indices; // sys mem buffer
		unsigned char*	m_tex[4]; // sys mem buffer
		unsigned char*	m_tangents[2]; // sys mem buffer
		unsigned char*	m_col; // sys mem buffer
	};

	Buffers			m_buffers;

	GLuint			m_vboPos; // vid mem buffer, if supported
	GLuint			m_vboNormal; // vid mem buffer, if supported
	GLuint			m_vboWeights; // vid mem buffer, if supported
	GLuint			m_vboIndices; // vid mem buffer, if supported
	GLuint			m_vboTex[4]; // vid mem buffer, if supported
	GLuint			m_vboTangents[2]; // vid mem buffer, if supported
	GLuint			m_vboCol; // vid mem buffer, if supported

#endif 
};

#include <Graphics/Ogl/Shared/Geometry/VertexSet/hkgVertexSetOGL.inl>

#endif //HK_GRAPHICS_VERTEX_SET_OGL_H

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

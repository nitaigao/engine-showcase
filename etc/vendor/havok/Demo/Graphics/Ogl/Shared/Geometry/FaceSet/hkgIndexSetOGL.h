/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_INDEXSET__H
#define HKG_INDEXSET__H

// index set striding
typedef hkUint8 HKG_INDEX_FORMAT;

#define HKG_INDEX_FORMAT_NONE	0  
#define HKG_INDEX_FORMAT_UB		1  // 1 unsigned byte
#define HKG_INDEX_FORMAT_SHORT	2  // 2 unsigned bytes ( short )
#define HKG_INDEX_FORMAT_INT	3  // 4 unsigend bytes ( int )

class hkgIndexSetOGL : public hkReferencedObject
{
	public:

		HKG_DECLARE_CLASS_ALLOCATOR();

		hkgIndexSetOGL();
		~hkgIndexSetOGL();

			// creates the index buffer and stores the relevant component information
		void setNumIndices( int numIndices, HKG_INDEX_FORMAT indexFormat );

			// get the number of indices represented by the index set
		int getNumIndices() const;

			// locks the index set so that you can alter it's values
		void lock();

			// unlocks the index set
		void unlock(); 

			// get the stride between indices in bytes
		int getStrideInBytes();

			// set the index data at the specified offset ( buffer MUST be first locked ) returning 'true' == success
		bool setIndexData( int offset, const void* data );

			// get the index data at the specified offset ( buffer MUST be first locked )
		const void* getIndexData( int offset );

			// get index format for the index set
		HKG_INDEX_FORMAT getIndexFormat() const;

			// get the OpenGL index format for the index set
		GLenum getOpenGLIndexFormat() const;

			// get a pointer to the index buffer
		const unsigned char* getIndexBuffer() const;

            // get vertex buffer object
        const GLuint getVBO() const { return m_vbo; }

	private:

			// the format of the index buffer
		HKG_INDEX_FORMAT m_indexFormat;

			// the stride between adjacent indices in bytes
		int m_stride;

			// the number of indices in this index set
		int m_numIndices;

			// locked buffer indicator
		bool m_locked;

			// an interleaved index buffer
		unsigned char* m_indexBuffer;

			// OpenGL index format
		GLenum m_oglIndexFormat;

            // vertex buffer for indices
        GLuint m_vbo;
};

#endif // HKG_INDEXSET__H


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

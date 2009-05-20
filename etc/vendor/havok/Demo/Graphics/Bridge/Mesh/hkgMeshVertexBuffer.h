/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKG_MESH_VERTEX_BUFFER_H
#define HKG_MESH_VERTEX_BUFFER_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

extern const hkClass hkgMeshVertexBufferClass;

class hkMemoryMeshVertexBuffer;
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

	/// An implementation of hkMeshVertexBuffer which uses the hkg graphics engine
class hkgMeshVertexBuffer: public hkMeshVertexBuffer
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Copy constructor
		hkgMeshVertexBuffer(const hkgMeshVertexBuffer& rhs);

			/// Constructor with existing vertex set
		hkgMeshVertexBuffer(hkgDisplayContext* context, hkgVertexSet* vertices);

			/// Sets up the buffer with the given vertex format and buffer sizes
		hkgMeshVertexBuffer(const hkVertexFormat& format, int numVertices, hkgDisplayContext* context);

			/// Dtor
		virtual ~hkgMeshVertexBuffer();

	        // hkMeshVertexBuffer implementation
        virtual hkMeshVertexBuffer* clone();

	        // hkMeshVertexBuffer implementation
        virtual bool isSharable() { return m_isSharable; }

	        // hkMeshVertexBuffer implementation
        void getVertexFormat( hkVertexFormat& formatOut );

	        // hkMeshVertexBuffer implementation
        virtual int getNumVertices();

	        // hkMeshVertexBuffer implementation
        virtual LockResult lock( const LockInput& input, LockedVertices& lockedVerticesOut );

	        // hkMeshVertexBuffer implementation
        virtual LockResult partialLock( const LockInput& input, const PartialLockInput& partialInput, LockedVertices& lockedOut);

	        // hkMeshVertexBuffer implementation
        virtual void getElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, hkVector4* compData);

	        // hkMeshVertexBuffer implementation
        virtual void setElementVectorArray(const LockedVertices& lockedVertices, int elementIndex, const hkVector4* compData);

	        // hkMeshVertexBuffer implementation
        virtual void getElementIntArray(const LockedVertices& lockedVertices, int elementIndex, int* compData);

	        // hkMeshVertexBuffer implementation
        virtual void setElementIntArray(const LockedVertices& lockedVertices, int elementIndex, const int* compData);

	        // hkMeshVertexBuffer implementation
        virtual void unlock( const LockedVertices& lockedVertices );

            /// Converts the vertexSets format into a hkdVertexFormat
        static hkResult HK_CALL getVertexSetFormatAsVertexFormat(const hkgVertexSet* vertexSet, hkVertexFormat& formatOut);

            /// Returns true if the vertex buffer is valid
        HK_FORCE_INLINE bool isValid() const { return m_vertexSet != HK_NULL; }


			/// Copys content of vertexSet (must be locked)to buffer (using the getComponent calls - ie slow)
        static void HK_CALL copySetToBuffer(const hkgVertexSet* vertexSet, int start, LockedVertices& lockedOut);

			/// Copys data from the buffer to the vertex set (must be locked) using the setComponent calls - ie slow
		static void HK_CALL copyBufferToSet(const hkMeshVertexBuffer::LockedVertices& lockedVertices, int start, hkgVertexSet* vertexSet);

			// hkReferencedObjectImplementation
        virtual const hkClass* getClassType() const { return &hkgMeshVertexBufferClass; }

			// hkReferencedObjectImplementation
		virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

	private:

			// Do not construct an empty buffer wrapper
		hkgMeshVertexBuffer();


	public:

		// Members
        hkgVertexSet* m_vertexSet;

        hkgDisplayContext* m_context;

	protected:

		hkBool m_locked;
		LockInput m_lockInput;
		hkVertexFormat m_format;
		int m_numVertices;

        // All of the following stuff is needed because the Havok graphics system, can have vertices stored, interleaved/
        // or not. Additionally the data it has can be non noncontiguous on certain platforms and vertex formats.
        // The following structures are used to make the data externally accessible in the same way - down to emulating
        // if the data layout cannot be supported via a hkdVertexLayout

        hkBool m_isLayoutable;
        hkBool m_isSharable;
        hkMemoryMeshVertexBuffer* m_lockBuffer;            ///

};

#endif // HKG_MESH_VERTEX_BUFFER_H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VERTEX_BUFFER_UTIL_H
#define HK_VERTEX_BUFFER_UTIL_H

#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>

/// A utility to help in the processing of vertices and vertex buffers

class hkMeshVertexBufferUtil
{
    public:

            /// Extract the buffer as vector4s
        static hkResult HK_CALL getElementVectorArray(const hkMeshVertexBuffer::LockedVertices::Buffer& buffer, hkVector4* out, int numVertices);

            /// Get components and store in a hkVector4 array (helper function - calls getElementVectorArray with the appropriate buffer)
        static hkResult HK_CALL getElementVectorArray(const hkMeshVertexBuffer::LockedVertices& lockedVertices, int bufferIndex, hkVector4* dst);

            /// Set components held in buffer from an array of hkVector4s
        static hkResult HK_CALL setElementVectorArray(const hkMeshVertexBuffer::LockedVertices::Buffer& buffer, const hkVector4* src, int numVertices);

            /// Set components from a hkVector4 array (helper fucntion - calls setElementVectorArray with the appropritate buffer)
        static hkResult HK_CALL setElementVectorArray(const hkMeshVertexBuffer::LockedVertices& lockedVertices, int bufferIndex, const hkVector4* src);

			/// The elements being extracted must have been previously been locked with lock/partialLock.
		static hkResult HK_CALL getElementIntArray(const hkMeshVertexBuffer::LockedVertices& lockedVertices, int elementIndex, int* dst);

			/// Sets the elements of an array doing conversions from ints as needed. Converts only integral types.
			/// The elements being set must have previously have been locked with lock/partialLock.
		static hkResult HK_CALL setElementIntArray(const hkMeshVertexBuffer::LockedVertices& lockedVertices, int elementIndex, const int* src);

            /// Copy the elements of src to dst of elementSize with the appropriate striding
        static void HK_CALL stridedCopy(const void* srcIn, int srcStride, void* dstIn, int dstStride, int elementSize, int numVertices);

            /// Zero the strided destination data
        static void HK_CALL stridedZero(void* dstIn, int dstStride, int elementSize, int numVertices);

            /// Copy vertex data from srcVertices to dstVertices
        static void HK_CALL copy(const hkMeshVertexBuffer::LockedVertices& srcVertices, const hkMeshVertexBuffer::LockedVertices& dstVertices);

            /// Copy the data from one buffer to the other - they must be the same type (m_type + m_numValues the same)
        static void HK_CALL copy(const hkMeshVertexBuffer::LockedVertices::Buffer& srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer& dstBuffer, int numVertices);

            /// Copy/convert all of the src buffers into the destination buffers.
        static void HK_CALL convert(const hkMeshVertexBuffer::LockedVertices& srcVertices, const hkMeshVertexBuffer::LockedVertices& dstVertices);

			/// Copys/converts all members of src into dst
		static void HK_CALL convert(hkMeshVertexBuffer* src, hkMeshVertexBuffer* dst);

            /// Copy and convert as necessary
        static void HK_CALL convert(const hkMeshVertexBuffer::LockedVertices::Buffer& srcBuffer, const hkMeshVertexBuffer::LockedVertices::Buffer& dstBuffer, int numVertices);

            /// Finds out if the locked vertex data is contiguous
        static hkBool HK_CALL isContiguous(const hkMeshVertexBuffer::LockedVertices& srcVertices, void** start, int& dataSize);

            /// Partition the format into shared and non shared (instanced) elements
        static void HK_CALL partitionVertexFormat(const hkVertexFormat& format, hkVertexFormat& sharedFormat, hkVertexFormat& instanceFormat);

            /// Get array component
        static hkResult HK_CALL getElementVectorArray(hkMeshVertexBuffer* vertexBuffer, hkVertexFormat::DataUsage usage, int subUsage, hkArray<hkVector4>& vectorsOut);

};

#endif // HK_VERTEX_BUFFER_UTIL_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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

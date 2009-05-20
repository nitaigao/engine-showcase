/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FACE_SET_PRIM_H
#define HK_GRAPHICS_FACE_SET_PRIM_H

#include <Graphics/Common/Geometry/BlendMatrixSet/hkgBlendMatrixSet.h>
#include <Graphics/Common/Geometry/FaceSet/hkgFaceSetPrimitiveDefines.h>

/// A primitive is a single collection of vertex connections that form a shape. There are
/// only two primitive types supported at the moment, triangle lists and strips. Triangle
/// lists are specified so that every 3 vertices define a separate triangle (not necessarily 
/// connected to any other). Triangle strips are such that every triangle is made from the 
/// current vertex and the previous two, starting at the third vertex on. A primitive can have 
/// indices for each vertex and so will have a index buffer, or it can just specify a start 
/// vertex index and let the vertices come in order in the vertex array for a given number
/// of vertices. It would then not need a index buffer and so the indices in this
/// class can be null. Note that the class is not reference counted yet. Indices are
/// stored as unsigned shorts so has an inherent limitation similar to most hardware
/// of 65535 _vertices_ per primitive, not indices, so if you reuse vertices a lot the
/// primitives can be quite big.
class hkgFaceSetPrimitive
{
	public:

		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( hkgFaceSetPrimitive );

			/// Construct a primitive the same as the give primitive 'p', and with a index offset of 'connectOffset'.
			/// This is primarily used internally to clone primitives.
		hkgFaceSetPrimitive( const hkgFaceSetPrimitive& p, int connectOffset = 0 );

			/// Construct an empty primitive of given render type, 't'.
			/// The render type is either HKG_TRI_LIST or HKG_STRIP.
		inline hkgFaceSetPrimitive( HKG_FACE_RENDER_TYPE t );

			/// Destructor for the primitive. Will deallocate the held indices if any by setting the length to zero (setLength(0)).
		inline ~hkgFaceSetPrimitive();

			/// Get the render type of this primitive. Can be HKG_TRI_LIST or HKG_STRIP, for a simple triangle list 
			/// where each triangle is three separate indices, and where a strip is defined as the current index 
			/// along with the two previous indices.
		inline HKG_FACE_RENDER_TYPE getType() const;
			
			/// Set the length of the index array. Will allocate the index array accordingly and data is not copied
			/// between resizes. Set the length to zero if no index array (and use the vertex buffer start index instead).
		void setLength(int n, bool allocateIndexBuffer = true ); 

			/// Set the offset into the vertex buffer where the vertex list presides. This is used
			/// if the indices are not specified.
		inline void setVertexBufferStartIndex( int n ); 

			/// Get the current index buffer length. This is zero if there is no index buffer.
		inline int getLength() const;

			/// Get the offset into the vertex buffer where the vertex list starts. Use this value
			/// as the start point in the vertex array for the vertex list if no index array is
			/// present.
		inline int getVertexBufferStartIndex() const; 
			
			/// Get the const pointer to the index buffer. Note that it may be HK_NULL if it is not 
			/// specified. It is a unsigned short array of getLength() length.
		inline const unsigned short* getIndices() const;

			/// Get the non-const pointer to the index buffer. Note that it may be HK_NULL if it is not 
			/// specified. It is a unsigned short array of getLength() length.
		inline unsigned short*	getIndices();

			/// Set the index array pointer explicitly. Use this function instead of setLength if	
			/// you can as will have quicker setup. You can pass HK_NULL as the connectivity and give 
			/// just a length if you want to use the vertex array start position instead.
			/// This method will take ownership of the given array (if one) and so will
			/// try to hkDeallocate it when it no longer needs the array (after resize or destruction).
		inline void setIndices(unsigned short* conn, int len); 

			/// A test function to query for proper data. By proper, it can only check for ranges as it does not know 
			/// which vertex array it will be used with.
		inline bool hasData() const; 

		inline hkgBlendMatrixSet* getBlendMatrices() const; 
		inline void setBlendMatrices(hkgBlendMatrixSet* bm);

	protected:

		HKG_FACE_RENDER_TYPE	m_type;

		unsigned short*		m_indices; 
		int		m_vertBufStartIndex; 
		int		m_len;

		hkgBlendMatrixSet*  m_blendMatrices; // per primitive (usually null or the same ptr for a few prims)
};


#include <Graphics/Common/Geometry/FaceSet/hkgFaceSetPrimitive.inl>

#endif //HK_GRAPHICS_FACE_SET_PRIM_H

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

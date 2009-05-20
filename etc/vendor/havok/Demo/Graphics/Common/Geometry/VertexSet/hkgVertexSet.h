/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_VERTEX_SET
#define HK_GRAPHICS_VERTEX_SET

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSetDefines.h>

class hkgFaceSet;

extern const hkClass hkgVertexSetClass;

/// A vertex set is basically a wrapper for a pure float vertex array. It will
/// be stored internally in an interleaved format, but the exact layout of that
/// format is left to the implementation. A vertex set is not reference counted
/// as it is assumed that a face set is the only user.
class hkgVertexSet : public hkgReferencedObject
{
	public:

			/// Create a platform specific vertex set for the given face set owner and in a given context
		static hkgVertexSet* (HK_CALL *create)(hkgDisplayContext* context);


			/// Set the number of vertices and which optional components to have.
			/// Normals are assumed, but this will be changed to be an option in future too.
			/// This will reallocate any held buffers and does not preserve data. If you set the
			/// numVerts to 0 then it will deallocate all held resources.
		virtual void setNumVerts(int nv, HKG_VERTEX_FORMAT vertexFormat, bool dynamic = false) = 0;

			/// Get the number of vertices represented by the vertex set.
		inline int getNumVerts() const;

		/// Lock the vertex set so that you can alter its values to read
		/// consistent data from the buffers. It locks from 'offset' len vertices.
		/// If len is -1, the lock will be to the end of the buffers and negative offset is
		/// not allowed.
		virtual unsigned char* lock(HKG_LOCK_FLAG f, int offset = 0, int len = -1) = 0;

		/// Same as lock, but you can specify the components you will edit. Most implementations
		/// are all interleaved, so partial locking will have no effect (same as lock()), but any that split the 
		/// storage to optimize skinning updates for instance, will benefit by knowing what will
		/// become dirty.
		virtual unsigned char* partialLock(HKG_LOCK_FLAG f, HKG_VERTEX_FORMAT vertexFormatPartsToLock, int offset = 0, int len = -1);

			/// Normally we use interleaved vertex formats
			/// such that all vertex information is
		inline bool isInterleaved() const;

			/// Normally we assume the vertex data is fixed (ot at least change very infrequently)
			/// This will say if the vb was created so that it is optimized for frequent writes.
		inline bool isDynamic() const;

		/// Unlock the given range, from 'offset' of 'len' vertices. This should
		/// match what you previously locked. Call this once you no longer require
		/// write access or consistent read access.
		virtual void unlock( int offset = 0, int len = -1) = 0;

			/// Get the distance, or stride, between each vertex component. There are
			/// 4 bytes in a float, so if stride is 12 then the vertex array is
			/// not interleaved, but all components are split internally so lock()
			/// returns the pure positional vertex array. In DirectX for example, a full format such as:
			///
			/// vert{
			///    float x,y,z;
			///    float nx,ny,nz;
			///	   float u,v;
			///    unsigned color;
			/// }
			///    which would have a stride of 8*float + 4 = 36 bytes.
			/// In this case, lock() returns the interleaved buffer of all components
			/// such that buf[i*stride] is the ith vert as above (take out u.v / color as required).
		inline int getStrideInBytes() const;

			/// Set the client pointers / stream pointers to this data.
		    /// This will effectively get the hardware to use this vertex set as the current vertex buffers.
		virtual void setStreams() = 0;

			/// This is called when the current stream is being set to NULL, so make sure that
		/// device etc is set to not use those arrays anymore (eg: OpneGL client state disable etc.)
		virtual void clearStreams() { }

			/// Set the vertex component at index i. You must lock the buffer first. Default just sets using the offsets given.
		virtual void setVertexComponentData(HKG_VERTEX_COMPONENT component, int index, const void* data);

			/// Make a new vertex set, as a copy of the this one
			/// Virtual so that impls can make fast clones (memcpy versions etc) if they like
			/// as this one has to do a per vertex copy.
		virtual hkgVertexSet* clone(hkgDisplayContext* ctx);

		// The PlayStation(R)2 uses a densely packed vertex format with the blending
		// weights and indices packed across multiple components.
		// These components are returned into a pre-allocated 4 byte buffer pointed to by 'data'.

			/// Get the vertex component at index i. You must lock the buffer with read access first.
		inline void* getVertexComponentData( HKG_VERTEX_COMPONENT component, int index, hkUint8* data = HK_NULL );

			/// Get the vertex component at index i. You must lock the buffer with read access first.
		inline const void* getVertexComponentData( HKG_VERTEX_COMPONENT component, int index, hkUint8* data = HK_NULL ) const;

		// same as getVertexComponentData( comp, 0 ), just allowed on write only buffers as you are not requesting the data itself
		// You must lock the buffer (in any mode) for this to work. Will return NULL if the data is not contiguous (some comps on PlayStation(R)2 for instance). Safe on PC etc though.
		inline void* getVertexComponentStartPointer( HKG_VERTEX_COMPONENT component );

		// same as getVertexComponentData( comp, 0 ), just allowed on write only buffers as you are not requesting the data itself
		// You must lock the buffer (in any mode) for this to work. Will return NULL if the data is not contiguous (some comps on PlayStation(R)2 for instance). Safe on PC etc though.
		inline const void* getVertexComponentStartPointer( HKG_VERTEX_COMPONENT component ) const;

        /// Returns true if all of the components making up the vertex set are contiguous (ie getVertexComponentStart would return non null for all components)
        /// If it is contiguous the data could still be either strided or interleaved
        inline bool isContiguous() const;

		// same as (&getVertexComponentData( comp, 1 ) - &getVertexComponentData( comp, 0 )).
		// Allowed on write only buffers. Stride is in bytes, add to  getVertexComponentStartPointer for each component.
		// If this func returns 0 then that component has no contiguous access, such as weights on the PlayStation(R)2 which are interleaved in spare data (w comps) in the per vertex. Safe on PC etc though.
		// You do not need to lock the buffer for this to work (stride is constant across locks, unlike the start ptrs and possibly alignment)
		inline int getVertexComponentStride( HKG_VERTEX_COMPONENT component ) const;

		// Returns if aligned to given num bytes. If 16 byte aligned then good for SIMD etc
		// You must lock the buffer (in any mode) for this to work.
		inline bool isVertexComponentAligned( HKG_VERTEX_COMPONENT component, int alignment ) const;


			/// Get the offset from the vertex start addr to the given component;
			/// ulong so that it can encode non interleaved buffer offsets
		hkUlong getVertexComponentOffset(HKG_VERTEX_COMPONENT component) const;

			/// Get vertex format for the vertex set. The return type is
			/// a bit field of enabled components.
			/// The components are at the top of this file.
		inline HKG_VERTEX_FORMAT getVertexFormat() const;

			/// Get the component storage format
		inline HKG_VERTEX_STORAGE getVertexComponentFormat(HKG_VERTEX_COMPONENT component) const;

		inline int getComponentSize( HKG_VERTEX_COMPONENT component ) const;

		void copyExistingVertex( int toThisIndex, int otherIndex, hkgVertexSet* otherSet );

		inline hkgDisplayContext* getContext() const;

			/// As we can have vertex specific data (vertex colors, tangents, etc)
			/// by default the material may have chosen the wrong shader (most of the
			/// time it will have been correct though). This is called before drawing the
			/// geom, enabling it to chose the correct shader et
		inline HKG_MATERIAL_VERTEX_HINT getMaterialHints() const;

		/// This will count all texcoord slots used plus the tangent and binormals if present (as they are really just tcoords too)
		inline int getNumTextureCoordsUsed() const;


        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgVertexSetClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:

		inline hkgVertexSet( hkgDisplayContext* context);

		// only allowed remove ref now
		virtual ~hkgVertexSet();

		hkgDisplayContext* m_context;

		// counters
		int	m_numVerts;
		int	m_stride;
		bool m_interleaved; // normal true
		bool m_dynamic; // expected to change often (be locked often, so cloth, skins, etc)

		HKG_VERTEX_FORMAT	m_vertexFormat; // which componets are used
		hkLong				m_storageOffsets[HKG_VERTEX_COMPONENT_NUM]; // offsets to the components relative to lockedBuf
		HKG_VERTEX_STORAGE	m_storageFormats[HKG_VERTEX_COMPONENT_NUM]; // the format of each
		HKG_LOCK_FLAG		m_currentLockType;
		static hkUint8	    g_componentSizes[HKG_VERTEX_STORAGE_NUM]; // use HKG_VERTEX_STORAGE to index into this to get the size in bytes
		unsigned char*	m_lockedBuf; // interleaved, default locked buf
};

#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.inl>

#endif //HK_GRAPHICS_VERTEX_SET

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

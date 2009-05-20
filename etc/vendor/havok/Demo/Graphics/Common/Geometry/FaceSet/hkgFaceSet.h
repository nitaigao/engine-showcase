/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FACE_SET_H
#define HK_GRAPHICS_FACE_SET_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/hkgObject.h>
#include <Graphics/Common/Geometry/FaceSet/hkgFaceSetPrimitive.h>

class hkgVertexSet;
class hkgRay;
struct hkgFaceSetPickData;

typedef int HKG_FACE_SET_STATUS_FLAGS;
#define HKG_FACE_SET_UNOPTIMIZED	0
#define HKG_FACE_SET_STRIPPED		1<<1
#define HKG_FACE_SET_REORDERED		1<<2
#define HKG_FACE_SET_PLATFORM_OPT	1<<3

typedef unsigned short HKG_FACE_SET_OPTIONS;
#define HKG_FACE_SET_DEFAULT 0 // normal, no post T&L cache optimization
#define	HKG_FACE_SET_CACHE   1 // strip, assuming there is a (small) post T&L cache
#define	HKG_FACE_SET_CACHE24 3 // strip, assuming there is a 24 entry post T&L cache (GeForce3 or better)

extern const hkClass hkgFaceSetClass;

/// The face set is a object that holds a set of vertices and a corresponding collection of primitives
/// that use those vertices. A primitive is a single rendering primitive, such as a triangle strip or list.
/// The vertices get placed in a vertex array on most platforms and the primitives index into that array.
/// The face set exposes methods to optimize (such as NvTriStrip) the underlying triangle data.
class hkgFaceSet : public hkgReferencedObject
{
	public:
			/// Create a platform specific holder for the given context. The context is
			/// needed as some platforms require the context to create the internal arrays.
		static hkgFaceSet* (HK_CALL *create)(hkgDisplayContext* ctx);

		static hkgFaceSet* HK_CALL createFaceSet(hkgDisplayContext* ctx) { return new hkgFaceSet(); }

			/// Get the const pointer to the vertex set. The vertex set can be though of as a vertex array.
			/// Note that the name is now explicit, as the vertex set is referenced and not owned anymore.
			/// will be Null at start
		inline const hkgVertexSet* getVertexSet() const;

			/// Get the non-const pointer to the vertex set. The vertex set can be though of as a vertex array.
		inline hkgVertexSet* getVertexSet();

			/// Set the vertex set to use. This can be shared between multiple face sets.
			/// Use the offset in the primitives to reference the start point.
		void setVertexSet( hkgVertexSet* vs );

			/// Add a new primitive to the face set. The face set will claim owner ship of the primitive and
			/// so you should not delete the primitive explicitly yourself. This will be changed in future to
			/// be referenced like most of the other parts of the graphics library, It also means that you must
			/// create a new primitive object for every primitive you add.
			/// A primitive represents a triangle connectivity information for a single renderable list or strip.
		inline void addPrimitive(hkgFaceSetPrimitive* p);

			/// Get the number of primitives in the set.
		inline int getNumPrimitives() const;

			/// Get the const pointer to the i-th primitive in the set.
		inline hkgFaceSetPrimitive* getPrimitive(int i);

		inline const hkgFaceSetPrimitive* getPrimitive(int i) const;

		virtual hkgFaceSet* clone( hkgDisplayContext* ctx, bool shareVertexSet ) const;

			/// This strip method will recreate new primitives after computing stripping
			/// information. Note that this will only split Tri List prims into one or
			/// more strips and possibly leave a tri list with
			/// leftovers. It will leave stripped prims as is. This will alter the
			/// order and number of primitives. This method can be quite slow, it has
			/// potentially exponential complexity in the number of vertices, although it
			/// tries to do shortcuts.
		void strip(HKG_FACE_SET_OPTIONS opts, bool stitch = true, int minStripLen = 0);

			/// Reordering the vertex set will mean better locality for the indices
			/// It will depends that the current prim order and will alter the index
			/// arrays in each prim to match the new order.
		void reorderVertexSet();

			/// This method tries to reorder each contained primitive such that the indices reference
			/// only vertices within a certain set size (= cache size) as modern graphics hardware
			/// has a post-T&L vertex cache. It turns strips into triangle lists. This method is pretty
			/// fast. It has complexity that is linear in the number of vertices.
		void optimizeForVertexCache(HKG_FACE_SET_OPTIONS opts);

		void findSharedVertices();

			/// Tessellate the triangles 'iteration' times. Each iteration will add 4 triangles per face.
			/// Not implemented in this release.
		void tesselate( unsigned int iterations );

			/// Given the current vertices and prims, use the texture coord s and v
			/// to compute the tangent and bitangent (binormal) for each vertex. Will fail if
			/// the vertices do not have tangents space allocated already in the format. Will fail
			/// if the vertices have no texture coordinates at the given stage to use.
		void computeTangents(int stage = 0);

			/// Weld the underlying vertex array so that there are no duplicates
			/// within the given tolerance. This will force the object to have
			/// an index array to start with, and then it will reindex the index
			/// arrays so that they match the new vertex array layout.
		void weld( float tolerance );

			/// Ray cast intersection with the object. Returns the number of triangles hit (will be zero if no hot).
			/// Only check the data if the hits are greater than one. The ray should be in the same space as the face vertices.
			/// See the hkgFaceSetPickData struct for more information on the hit data.
		unsigned int intersect( const hkgRay& ray, bool ignoreBackfaces, hkgFaceSetPickData& data) const;

			/// MinE and MaxE will be assumed to be initialized to values already
			/// to optimize group AABB computation. If this is for a single
			/// query, minE should be FLT_MAX, and maxE should be -FLT_MAX
		void computeAABB(float minE[3], float maxE[3]) const;

			/// Initialize all internal data required after setting up the vertex information and primitives.
			/// On some platforms this may convert the underling vertex representation to get rid of indexing for
			/// example in the case of PS2GL that does not support indexed vertex arrays for example. So do not
			/// expect the data to be exactly the same after initialization. If you provide a remap array, it
			/// will return an array of where the original vertices moved too (a one to many mapping on PlayStation(R)2 for instance)
			/// It will set the size of the array to none if it did not reorder.
		virtual void initialize(hkArray<hkUint32>* reorderedVertices = HK_NULL);

			/// On most platforms, once the blend matrices are set in the primitives
			/// the renderer can use them as is. On platforms such as PlayStation(R)2, the actual
			/// packets must be altered to point to the newly added blend matrices.
		virtual void finalizeBlendMatrices() { }

			/// Render the primitives to the current viewport in the context. Will set the vertex arrays
			/// to be current and render all primitive parts.
		virtual void render(hkgDisplayContext* context);

			/// Construct a 2D grid. It will assume an empty face set.
			/// A grid is a planar collection of triangles, represented as one large triangle strip. It will
			/// extend from the 'corner' with 'sideA' as one vector and 'sideB' as the other vector representing
			/// the two edges that start at 'corner'. The tessellation is specified as 'resA' along A and 'resB' along B.
			/// If you want vertex colors or texture coordinates in the arrays, specify as such in the last two args.
		void makeGrid( const float corner[3], const float sideA[3], const float sideB[3], int resA = 1, int resB = 1, int numTexCoords = 1, bool vcolor = false, bool tangents = false);

			/// Construct a 3D box. It will assume an empty face set.
			/// A box will be made with one corner as 'corner', with each of the three 'side' vectors as the three
			/// edges that connect that point. The winding will be affected by the order of the 3 sides. The tessellation
			/// along the three edges is specified with the three 'res' arguments. If you want vertex colors or texture
			/// coordinates, specify so with the last 2 args.
		void makeBox( const float corner[3], const float sideA[3], const float sideB[3], const float sideC[3], int resA = 1, int resB = 1, int resC = 1, int numTexCoords = 1, bool vcolor = false, bool tangents = false);

			/// Construct a Sphere. It will assume an empty face set.
			/// A sphere centred at 'center' with given 'radius', and tessellation in along the major and minor directions are specified.
			/// If you want vertex colors or texture coordinates, specify so with the last 2 args.
		void makeSphere( const float center[3], float radius, int numMajor = 8, int numMinor = 8, int numTexCoords = 1, bool vcolor = false, bool tangents = false);

			/// Construct a Cone (or Cylinder if the radius both ends). It will assume an empty face set.
			/// Starting at 'from', the vector to 'to' specifies the axis of the shape, which will have 'radius0' radius at the 'from' end
			/// and 'radius1' radius at the to end. Radius can be negative. The major and minor tessellation parameters are specified next and also
			/// whether to build caps at each end (if radius is not zero of course). If you want vertex colors or texture coordinates, specify so
			/// with the last 2 args. The main body of the cone/cylinder is one triangle strip and the caps (if any) are tri lists.
		void makeCone( const float from[3], const float to[3], float radius0, float radius1, int numMajor = 8, int numMinor = 8, bool cap = true, int numTexCoords = 1, bool vcolor = false, bool tangents = false);

			/// Print general information about this face set to the give stream. It will assume
			/// that the stream is text based.
		void print(hkOstream& stream) const;

			/// Collapse will reduce all primitives with the same type to a single primitive.
			/// After this call you will have at most two primitives: one strip and one tri list.
		void collapse();

			/// Transform all the vertices held in this face set by a 4x4 matrix
			/// It will lock the verts, transform, and then unlock. This is used to bake transforms
			/// down to static face sets so that they do not need to be transformed the same
			/// each frame.
		void transform( const float t[16] );

			/// The most bone that any one primitive has a matrix stack of
			/// This is require to see if the the buffers need to be setup specially or not (too big for hw etc)
		inline int getMaxPrimitiveBones() const;

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgFaceSetClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:

		hkgFaceSet();
		virtual ~hkgFaceSet();

		void removeIndexArrays(hkArray<hkUint32>* reorderedVertices = HK_NULL); // as a necessity on non indexed modess such as our PS2 renderer
		void createIndexArrays(); // as a precursor to vertex welding on non indexed vert arrays
		void removeStripsAndIndices(hkArray<hkUint32>* reorderedVertices = HK_NULL); // as a necessity on non indexed modess such as our PS2 renderer

		// methods for implementing optimizeForVertexCache()
		void tipsify(int i, int TLcacheSize);
		int getNextVertex(const hkgArray<unsigned short>& neighbors,
						  const hkgArray<int>& liveCount,
						  const hkgArray<unsigned short>& deadEnd, int& deadEndCount,
						  int& cursor, int numVertices,
						  const hkgArray<int>& cachingTimestamp, int timeStamp,
						  int TLcacheSize);
		int skipDeadEnd(const hkgArray<unsigned short>& deadEnd, int& deadEndCount,
						const hkgArray<int>& liveCount,
						int& cursor, int numVertices);

		// vertices not now owned, just referenced:
		hkgVertexSet*		m_vertices;

		// primitives
		hkgArray<hkgFaceSetPrimitive*> m_primitives;

		HKG_FACE_SET_STATUS_FLAGS    m_flags;

		int					m_maxPrimitiveBones;

};

#include <Graphics/Common/Geometry/FaceSet/hkgFaceSet.inl>

#endif //HK_GRAPHICS_FACE_SET_H

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

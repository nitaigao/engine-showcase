/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DISPLAY_OBJECT_H
#define HK_GRAPHICS_DISPLAY_OBJECT_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Geometry/hkgGeometry.h>
#include <Graphics/Common/Input/Mouse/hkgPickData.h>
#include <Graphics/Common/Camera/hkgCamera.h>

class hkgRay;

#include <Graphics/Common/DisplayObject/hkgDisplayObjectDefines.h>

extern const hkClass hkgDisplayObjectClass;

class hkgDisplayObject : public hkgReferencedObject
{
	public:

			/// Creates a platform specific display object.
		static hkgDisplayObject* (HK_CALL *create)();

			/// Default create function, use ::create instead.
		inline static hkgDisplayObject* defaultCreateInternal();

			/// Get the number of geometries added to the display object
		inline int getNumGeometry() const;

			/// Add a geometry to the object. Geometries can be added to multiple
			/// display objects for instancing purposes if desired.
		inline void addGeometry(hkgGeometry* g);

		virtual hkgDisplayObject* copy( HKG_DISPLAY_OBJECT_COPY_FLAGS f, hkgDisplayContext* ctx /*only required if doing deeper copies (facesets and/or verts)*/ ) const;

			/// Set the local to world transform for the object. The transform
			/// is a 4x4 column major matrix (see hkgMath).
			/// This _must_ be a orthonormal basis and translation only
			/// so only transforms with rotation and translation allowed
			/// to make the math quicker for inverses. This graphics lib does
			/// not support scale in this iteration. If the transform is the
			/// identity, then say so with the isIdentity flag as the code
			/// will then skip the matrix modelview multiply and the inverts.
			/// The transform will have the local offset applied to it as well if
			/// required / specified by setGeometryOffset().
		inline void setTransform( const float* t, bool isIdentity = false);


		inline void setTransformAligned( hkTransform& t );

			/// Get the local to world transform for the object. It will return
			/// a pointer to the 16 floats that represent the 4x4, column major, matrix.
			/// It will have the local translation (geometry offset as described in setGeometryOffset)
			/// already applied so is the real transform for the object.
		inline const float*	getTransform() const;

			/// In Havok 1.x and some other situations, it is desirable to offset the
			/// object from its local transform space. In Havok 1.x this was due to
			/// center of mass shifts. It is a simple translation that will be added to
			/// transform in local space (so is rotated by the transform as given to setTransform
			/// and then added to the transform). Set to (0,0,0) to disable.
		inline void setGeometryOffset( const float o[3] );

			/// Get the current geometry offset (if any). The default is (0,0,0) which
			/// disables the offset.
		inline const float* getGeometryOffset() const;

			/// Get the current geometry offset (if any). The default is (0,0,0) which
			/// disables the offset.
		inline float* getGeometryOffset();

			/// Get the flag that indicates whether to treat the transform like the identity
			/// (which it may not be, but speeds up the rendering).
		inline bool getIdentityFlag() const;

			/// Set the transform to act like the identity (which it may not be, but speeds
			/// up the rendering).
		inline void setIdentityFlag(bool t);

			/// Special state, for objects always in wireframe or no z objects (such as skyboxes)
		inline void setSpecialState( bool forceNoZRead, bool forceNoZWrite,	bool forceWireframe, bool forceBothSides);
		inline void getSpecialState( bool& forceNoZRead, bool& forceNoZWrite, bool& forceWireframe, bool& forceBothSides) const;

			/// The status of a display object is flag to indicate specially properties or hints
			/// to the renderer for optimization. At the moment, the only status is whether the object
			/// A static display object will be split up or merged with other display objects
			/// so will not retain its form. You can use this for all display objects you know
			/// you will not want to reference directly again. EG: Static scenery that does not change.
			/// A display world may choose to merge multiple static display objects as an optimization
			/// and so once you set a body to be static, it may not exist as this object in the display
			/// world anymore.
		inline void setStatusFlags( HKG_DISPLAY_OBJECT_STATUS f);

			/// See the description for setStatusFlags().
		inline HKG_DISPLAY_OBJECT_STATUS getStatusFlags() const;

			/// Set the transform matrix to the identity. It will also set the identity flag.
		inline void setTransToIdentity();

			/// Set the translation offset to zero and so disable the extra offset calc.
		inline void setOffsetToZero();

			/// Get the i-th geometry pointer held by this object. Const access.
		inline const hkgGeometry* getGeometry(int i) const;

			/// Get the i-th geometry pointer held by this object.
		inline hkgGeometry* getGeometry(int i);

			/// Remove the pointer to the i-th object. A quick remove just takes the ith entry out and
			/// replaces with the (size-1)th entry, but this does not preserve order. If you want to
			/// preserve order, specify so with the preserveOrder flag. This method will not decrement
			/// the ref count of the geometry. You must do so if you don't want the geom, or are
			/// adding it somewhere else.
		inline hkgGeometry* removeGeometry(int i, bool preserveOrder = false);

			/// The same as removeGeometry(int i ..) except it will search for the name first. The search is case sensitive.
			/// See removeGeometry(int i ..) for explanation on preserveOrder.
		inline hkgGeometry* removeGeometry(const char* name, bool preserveOrder = false);

			/// Set the name of the display object for future reference. Will copy the string. Null
			/// terminated and stored in a hkString.
		inline void setName( const char * s);

			/// Get the name of the object. May be zero length if no name.
		inline const hkString& getName() const;

			/// Get the transparency information for this object.
			/// It can be a mixture, for instance HKG_GEOMETRY_TRANS_OPAQUE | HKG_GEOMETRY_TRANS_TRANSPARENT
			/// is valid as a type as it represents a mixed mode geometry ( HKG_GEOMETRY_TRANS_MIXED )
			/// The transparency for each geometry depends on material and per vertex alpha.
		inline HKG_GEOMETRY_TRANSPARENCY getTransparencyType() const;

			/// Will set all geoms to use this material. Just iterates through the geometries and
			/// sets the material.
		void setMaterialOnAll( hkgMaterial* mat );

			/// This will try and optimize the geometries so that they render faster, Just iterates through the
			/// geometries at the moment and calls optimize() on them. If you want to do cross geometry optimization
			/// by collapsing several together, then consider using bake(), but only of you do not want to
			/// remove or reference any given geometries again. This optimize function is less severe.
		void optimize();

			/// Perform a ray-object intersection test.
			/// It returns the number of hits and the data contains the info on the closest hit, in the form of
			/// the geometry index which was hit then the geometry hit information of the form hkgGeometryPickData
			/// Will return 0 for no hit, and you can ignore backfacing triangles by specifying their ignoreBackfaces flag.
			/// The ray should be in world space.
		virtual int intersect( const hkgRay& ray, bool ignoreBackfaces, hkgDisplayObjectPickData& data) const;

		void getAABB(float minE[3], float maxE[3]) const;

			/// For objects with HKG_DISPLAY_OBJECT_USER_AABB status, set the local AABB
		void setAABBLocal( const float minE[3], const float maxE[3] );

		inline const float* getAABBCentPtr() const;

		inline const float* getAABBExtPtr() const;

		inline bool hasAABB() const;

		void computeAABB();

			/// Given a camera, is the bounding sphere visible in the current frustum. This is an inline
			/// test, simply calling the sphere test for the world bound against the camera clip frustum.
		inline bool boundsVisibleInFrustum( const hkgCamera* camera );

			/// Render this object to the current viewport in the context. This will push (and pop) the
			/// current world transform on the context and render each geometry set (solid, then mixed, the alpha)
			/// in turn. If you call this directly (as it is usually called by the hkgDisplayWorld)
			/// and the object is either fully solid or alpha, then make sure that the context reflects
			/// the desired render mode as it will assume that is being batch rendered and the state is set.
		virtual void render(hkgDisplayContext* context) const;

			/// Retrieve the simple texture stats for this object in terms of the number of material
			/// face set (numMatFaces) and the number of those that are textured. Also the number of
			/// unique textures found.
		void getTextureStats(int& numMatFaces, int& numTexMatFaceSets, int& numTextures ) const;

			/// Recompute the transparency grouping (solid-mixed-alpha) based on all available
			/// material and vertex information. You need to call this for instance if the
			/// transparency state changes for a material used.
		void computeTransparency();

			/// Print the general stats and recurse to all geometries for this object. The
			/// print will assume the stream is text based.
		void print(hkOstream& stream) const;

			/// Baking will result in the minimal number of sub geometries. It will not preserve rendering
			/// order or vertex sets as they will all be placed together as much as possible, but general
			/// order will be kept as much as possible. Do no all this if you want to reference or change
			/// subobject parts again, but you can alter sub parts after this as long as you re-query for
			/// access to them. If you change data again after this that affects this object, you can call
			/// this function again. This function may be slow, so is not meant to be used with data
			/// that changes every frame. It may create low level API lists such as display lists or DMA
			/// chains etc for the object.
		void bake(hkgDisplayContext* context);

			/// Handy debugging
		inline void setDrawLocalFrame( bool on );

			/// Handy debugging
		inline bool getDrawLocalFrame() const;

		enum UserPointerType
		{
			USER_POINTER_UNKNOWN = 0,
			USER_POINTER_RIGID_BODY,
			USER_POINTER_BREAKABLE_BODY,
		};

		inline void setUserPointer(int type, void* d);

		inline void* getUserPointer() const;

		inline int getUserPointerType() const;

		inline void setGeometry(int index, hkgGeometry* g);

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgDisplayObjectClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:


		inline hkgDisplayObject();
		inline virtual ~hkgDisplayObject();

		void renderGeomsInOrder(hkgDisplayContext* context) const;

		inline hkgArray<hkgGeometry*>*	remapIndex( int& index ) const;
		inline hkgGeometry*				searchAndRemoveByName( hkgArray<hkgGeometry*>* geomArray, const char* name, bool preserveOrder);

	public:

		// A full 4x4 model trans matrix, col major (ogl style) but must
		// be a rigid transform.
		HK_ALIGN16( mutable float m_trans[16] );

		// A post offset so that you can update the trans direct from rbs
		// etc. and have a const offset applied locally on update.
		float						m_offset[3];

		// Todo: reduce mem and share more here.
		mutable bool						m_identityTrans;
		bool						m_zeroOffset;
		bool						m_drawLocalFrame;
		bool						m_drawAABB; // in DEBUG only
		bool						m_forceNoZRead;   // always no z reads for this object
		bool						m_forceNoZWrite;  // always no z writes for this object
		bool						m_forceWireframe; // always wire this object
		bool                        m_forceDrawBothSides; // always draw both inside and outside faces

		hkgArray<hkgGeometry*>		m_solidGeoms;
		hkgArray<hkgGeometry*>		m_alphaGeoms;
		hkgArray<hkgGeometry*>		m_mixedGeoms;

		HKG_GEOMETRY_TRANSPARENCY	m_geomTransp;

		// The bound in local space.
		float						m_aabbCent[3]; // middle point
		float						m_aabbExtent[3]; // half extents
		float						m_computedRadius; // len of half extents, <0 for no extents

		hkString					m_name;
		HKG_DISPLAY_OBJECT_STATUS	m_status;

		hkUint32					m_userPtrType; /// the type of the user pointer, needed for picking: 0 means collidable, 1 means breakable body
		void*						m_userPtr;	   /// Currently only used for breakable bodies
};

#include <Graphics/Common/DisplayObject/hkgDisplayObject.inl>

#endif // HK_GRAPHICS_DISPLAY_OBJECT_H

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

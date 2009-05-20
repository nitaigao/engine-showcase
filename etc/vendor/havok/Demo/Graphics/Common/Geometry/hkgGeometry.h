/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_GEOMETRY
#define HK_GRAPHICS_GEOMETRY

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Geometry/hkgMaterialFaceSet.h>
#include <Graphics/Common/Math/hkgRay.h>
#include <Graphics/Common/Input/Mouse/hkgPickData.h>
#include <Graphics/Common/Geometry/hkgGeometryDefines.h>

extern const hkClass hkgGeometryClass;

/// A geometry in the graphics lib is an ordered collection of face sets organized by material.
/// It is very similar to a hkgDisplayObject except it has no transform. This may be consolidated
/// in future.
class hkgGeometry : public hkgReferencedObject
{
	public:
			/// Create a platform specific geometry representation.
			/// In fact, at the moment, this class is cross platform as
			/// is just a container for material FaceSets and so needs
			/// no platform knowledge. This will thus return the same object
			/// on all platforms.
		static hkgGeometry* (HK_CALL *create)();
			/// Default create function, use ::create instead.
		inline static hkgGeometry* defaultCreateInternal();

			/// Get the number of face sets held in the geometry.
		inline int getNumMaterialFaceSets() const;

			/// Get the const-pointer to the i-th material face set.
		inline const hkgMaterialFaceSet*	getMaterialFaceSet(int i) const;

			/// Get the pointer to the i-th material face set.
		inline hkgMaterialFaceSet*			getMaterialFaceSet(int i);

			/// Add a MaterialFaceSet to the geometry. A material face set
			/// can be added to multiple different geometries as need be
			/// to achieve instancing. It will be reference counted.
		inline int addMaterialFaceSet( hkgMaterialFaceSet* ms );

			/// Remove the i-th material face set from the geometry. Will return
			/// the pointer if successful and will not dereference it as you may
			/// require the object. If you no longer need the material face set,
			/// release the returned pointer. The index i must be within range.
		inline hkgMaterialFaceSet* removeMaterialFaceSet( int i);

			/// Search for and remove the face set that matches the given pointer.
			/// Will return success boolean. This function WILL dereference the pointer
			/// as it assumes that since you have the pointer and are using it as
			/// a query that you have a reference to it so this object can remove its
			/// reference safely. This may not be true in all cases and may change in future.
		bool removeMaterialFaceSet( hkgMaterialFaceSet* ms );

			/// Set the name of the geometry with a null terminated string.
		inline void setName( const char* name );

			/// Get the name of the object. This name may be of zero length.
		inline const hkString& getName() const;

			/// Get he transparency type of the geometry. As it may contain both
			/// alpha and solid parts, this may be a mixed type, ie: HKG_GEOMETRY_TRANS_OPAQUE | HKG_GEOMETRY_TRANS_TRANSPARENT
			/// or rather the type HKG_GEOMETRY_TRANS_MIXED which is a same value.
		inline HKG_GEOMETRY_TRANSPARENCY getTransparencyType() const;

			/// Set the material on all contained material face sets to be this material.
			/// The material will be referenced by each material face set that will use it.
		void setMaterialOnAll( hkgMaterial* mat );

			/// A utility function that will create a single face set and add it to this geom
			/// in a material face set. Returns the index of this new face set (usually 0 as
			/// we are creating a simple geometry most of the time).
			/// Call getMaterialFaceSet() with this returned index to get the object if desired.
			/// The context is required as the face set requires it in it's ctor.
		int makeSingleFaceSet(hkgDisplayContext* context);

			/// Will strip the face sets and reorder for best performance. This will assume
			/// certain optimization properties for the face sets that may not be valid, such as
			/// cache sizes for the nvidia strip algorithm. It may be more optimal to do this on
			/// a per face set basis if you have more knowledge.
		void optimize();

			/// Ray cast intersection with the geometry. The ray should be in the same space as
			/// the geometry. Returns num hits (0 for none). Check the given data only if the method
			/// returns more than 0 hits. See the hkgGeometryPickData struct for more information.
		int intersect( const hkgRay& ray, bool ignoreBackfaces, hkgGeometryPickData& data) const;

		inline void getAABB(float minE[3], float maxE[3]) const;

		inline const float* getAABBMaxPtr() const;

		inline const float* getAABBMinPtr() const;

		inline bool hasAABB() const;

		void computeAABB();

			/// Render the geometry to the current viewport of the given context. This will assume all state
			/// is correct and just call render on each material face set in turn if pure alpha or pure solid geometry.
			/// If mixed mode, it will render solid and alpha separately (solid first) and where set state
			/// appropriately to perform the blending.
		inline void render(hkgDisplayContext* context) const;

			/// Print the geometry information and recurse to all held material face sets to the
			/// given stream. It will assume a text based stream.
		void print(hkOstream& stream) const;

			/// Bake as much of the contained information together.
			/// It will result in the minimal number of face sets given material similarities etc.
			/// It may not preserve rendering order or vertex sets as they will all be placed
			/// together as much as possible.
		void bake(hkgDisplayContext* context);

			/// Transform all facesets by the given 4x4 transform 't'. This will not check for
			/// instancing information. If any of the face sets are used anywhere else then
			/// it will lead to undesirable effects as the face sets could be transformed more than
			/// once and so be corrupt.
		void transform( const float t[16]);

		void recomputeAlphaSets();

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgGeometryClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:

		inline hkgGeometry();
		virtual ~hkgGeometry();

		void mixedModeRender(hkgDisplayContext* context) const;

		void computeBoundCenter();
		void computeBoundRadius();

		hkgArray<hkgMaterialFaceSet*>	m_matSolidFaceSets;		// solid (no alpha)
		hkgArray<hkgMaterialFaceSet*>	m_matAlphaFaceSets;		// transparent

		bool							m_computedAABB;
		float							m_aabbMax[3];
		float							m_aabbMin[3];

		hkString						m_name;
		HKG_GEOMETRY_TRANSPARENCY		m_trans;

};

#include <Graphics/Common/Geometry/hkgGeometry.inl>

#endif //  HK_GRAPHICS_GEOMETRY

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_MAT_FACE_SET_H
#define HK_GRAPHICS_MAT_FACE_SET_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Geometry/FaceSet/hkgFaceSet.h>
#include <Graphics/Common/Material/hkgMaterial.h>

extern const hkClass hkgMaterialFaceSetClass;

/// A Material FaceSet is a collection of FaceSets that share the same material
/// and is just a simple container class with no platform specific parts.
class hkgMaterialFaceSet : public hkgReferencedObject
{
	public:

			/// Create a hkgMaterialFaceSet. This will be the same object on all platforms.
		static hkgMaterialFaceSet* (HK_CALL *create)();
			/// Default create function, use ::create instead.
		inline static hkgMaterialFaceSet* defaultCreateInternal();

			/// Add a face set to this collection. It will reference count the face set.
			/// A face set can be added to multiple different material face sets.
		inline void addFaceSet(hkgFaceSet* faceSet);

			/// Reserve enough array space for the given estimate. It will set the capacity
			/// of the internal array to be this value.
		inline void reserve( int estimateNum );

			/// Get the number of face sets in this object.
		inline int getNumFaceSets() const;

			/// Get the const pointer to the i-th face set.
		inline const hkgFaceSet* getFaceSet(int i) const;

			/// Get the pointer to the i-th face set.
		inline hkgFaceSet* getFaceSet(int i);

			/// Get the const pointer to the material for this collection of face sets. Can be HK_NULL.
		inline const hkgMaterial* getMaterial() const;

			/// Get the material for this collection of face sets. Can be HK_NULL.
		inline hkgMaterial* getMaterial();

			/// Set the material for this collection of face sets. Can be HK_NULL. It will be reference counted.
		inline void setMaterial( hkgMaterial* m );

			/// Optimize all the face sets in this collection. At the moment this will call strip() on
			/// each face set and then call reorderVertexSet to match the strip order. It may do more
			/// optimizations in future.
		void optimize();

			/// Set the material as current and render all face sets in order to the current viewport in the
			/// given context.
		void render(hkgDisplayContext* context) const;

			/// Print the material fact set information to the given stream, It will assume the stream is text based.
			/// It will recurse down to all contained face sets.
		void print(hkOstream& stream) const;

			/// Bake the face sets together. Will effectively bake all vertex sets that have the same vertex format (color,uv)
			/// into a single face sets, so depending on how alike the contained vertex information is this
			/// call could result in just on face set with all the triangles in it.
		void bake(hkgDisplayContext* context);

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgMaterialFaceSetClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:

		inline hkgMaterialFaceSet();
		virtual ~hkgMaterialFaceSet();

		hkgMaterial*		  m_material;
		hkgArray<hkgFaceSet*>  m_faceSets;
};

#include <Graphics/Common/Geometry/hkgMaterialFaceSet.inl>

#endif // HK_GRAPHICS_MAT_FACE_SET_H

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

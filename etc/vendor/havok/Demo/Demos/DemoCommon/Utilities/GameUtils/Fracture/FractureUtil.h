/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FRACTURE_UTIL_H
#define HK_FRACTURE_UTIL_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>


struct hkGeometry;
class hkpConvexShape;
class hkpConvexVerticesShape;
class hkpRigidBodyCinfo;
class hkpRigidBody;
class hkpCollisionListener;
class hkPseudoRandomGenerator;

	/// A set of helper functions to do fracturing of rigid bodies
class FractureUtil
{
	public:
			/// A shape with mass properties
		struct ShapeInfo
		{
				/// the input shape
				/// only a hkpConvexVerticesShape or hkpBoxShape is allowed
			hkpConvexShape* m_shape;

				/// The shapes volume
			hkReal m_volume;

				/// Center of mass in local space
			hkVector4 m_centerOfMass;

				/// on optional geometry (not always available)
			hkGeometry* m_geometry;

				/// Inertia tensor
			hkMatrix3 m_intertiaTensor;

			ShapeInfo();
		};

			/// Base fracture input structure.
		struct FractureInput
		{
				/// The shape which should be fractured
				/// only hkpConvexVerticesShape and hkpBoxShape are allowed
			const hkpConvexShape* m_shape;

				/// The center point of fracture in shape local space
			hkContactPoint m_fractureOrigin;

				/// The minimum volume for one generated object.
				/// If the volume is less, the piece will not be generated at all
			hkReal m_minVolumeOfGeneratedShapes;

				//
				// Graphics display objects
				//	
			class hkgDisplayObject*  m_displayObject;
			class hkgDisplayContext* m_displayContext;

			hkBool m_generateDisplayObjects;
			hkBool m_generateDisplayObjectsForDebris;

			FractureInput() 
			{
				m_displayObject = HK_NULL;
				m_displayContext = HK_NULL;
				m_generateDisplayObjects = false;
				m_generateDisplayObjectsForDebris = false;
			}
		};


			/// The input structure for fracturing glass
			/// This specific fracture pattern creates a number of
			/// 'concentric' rings around the impact hole.
			/// Outside the last concentric ring, the glass is split
			/// into cake pie slices.
		struct GlassFractureInput : public FractureInput
		{
				/// A centre hole which will be removed completle.
				/// Typically lots of particles should be spawn at this point
			hkReal m_holeRadius; 

				/// the radius of the biggest ring
			hkReal m_concentricFractureMaxRadius;

				/// The number of pieces in each ring
			int m_numPieSlices;

				/// The number of rings
			int m_numConcentricPieces;

				/// A maximum angle the input normal will be randomized by
			hkReal m_randomizeNormalAngle;

				/// A maximum angle the line from the center to the outer rim
				/// will be randomized at each ring (note that this is likely to cause 
				/// unphysical results (e.g. duplication of shape's volume) if 
				/// randomization is not restrictive enough.)
			hkReal m_pieSliceCurvature;

				/// A random number generator
			hkPseudoRandomGenerator* m_generator;
		};


			/// The output of fractureGlass()
			/// This structure holds references to the shapes,
			/// however it does not remove this references in its (non existent)
			/// destructor. Call removeReferencesToShapes() to remove the references
		struct GlassFractureOutput
		{
				/// The inner shapes are the ones from the concentric rings
			hkInplaceArray<ShapeInfo,64> m_innerShapes;

				/// The outer shapes are the ones outside the last concentric ring
			hkInplaceArray<ShapeInfo,16> m_outerShapes;

				/// Remove all references to shapes referenced by this function.
			void removeReferencesToShapes();

				/// Destructor ensures that references to stored shapes are removed.
			~GlassFractureOutput() { removeReferencesToShapes(); }

			// Graphics display objects
			hkInplaceArray<class hkgDisplayObject*, 64> m_innerShapesDisplayObjects;
			hkInplaceArray<class hkgDisplayObject*, 64> m_outerShapesDisplayObjects;
		};

			/// Concrete (crater-like) fracture.
		struct ConcreteFractureInput : public FractureInput
		{
				//
				// Basic parameters of the generated crater.
				//
			hkReal  m_outerRadius;
			hkReal  m_innerRadius;
			hkReal  m_depth;
			hkUint8 m_numSideFacets;
			hkBool  m_generateCenterPiece;
			hkBool  m_generateBulletHoleDebris;

				//
				// Randomization of crater parameters.
				//
			hkPseudoRandomGenerator* m_generator;
			hkBool m_randomizeOrientationAroundNormal;

		};

		struct ConcreteFractureOutput
		{
			hkInplaceArray<ShapeInfo,64> m_shapes;
			hkInplaceArray<ShapeInfo,64> m_bulletHoleDebrisShapes;
			void removeReferencesToShapes();
			~ConcreteFractureOutput() { removeReferencesToShapes(); }

			// Graphics display objects
			hkInplaceArray<class hkgDisplayObject*, 64> m_displayObjects;
			hkInplaceArray<class hkgDisplayObject*, 64> m_bulletHoleDebrisDisplayObjects;
		};


	public:

			/// Fracture a piece of glass. See GlassFractureInput for details
		static void HK_CALL fractureGlass(const GlassFractureInput& in, GlassFractureOutput& out); // no gluePoints for glass..

			/// Fracture of a very hard material like concrete, see ConcreteFractureInput for details
		static void HK_CALL fractureConcrete(const ConcreteFractureInput& in, ConcreteFractureOutput& out);


			//
			// Internal helper functions
			//

			/// Rotate the vertices and planes in a convex vertices shape so that
			/// the final inertia tensor is a diagonal inertia tensor
		static void HK_CALL convertConvexVerticesShapeToPrincipalAxesFrame(ShapeInfo& shapeInfo, hkTransform& oldToPrincipalAxesTransform);

		enum ExportGeometryAsProperty
		{
			EXPORT_GEOMETRY_IN_PROPERTY,
			DELETE_INTERNAL_GEOMETRY
		};

			/// Creates a series of bodies. Each of the input shapes is converted into their
			/// principle axis frames.
			/// Note:
			///  - This function modifies the input shapes
			///  - This function can append an intermediate geometry as a property to the rigid body.
			///    This speeds up the havok internal debug graphics engine, but is pretty useless
			///    otherwise
		static void HK_CALL createRigidBodiesFromShapes( const hkpRigidBodyCinfo& templateCinfo, hkReal originalVolume, ExportGeometryAsProperty exportGeometry, 
														 hkArray<ShapeInfo>& shapeInfos, hkArray<hkpRigidBody*>& bodiesOut, hkArray<hkTransform>* toPrincipalAxesTransformInOldLocalRefernceFrame);

			/// This converts a set of planes into
			/// a hkpConvexVerticesShape and calculate its inertia tensor as well as its volume.
		static hkResult HK_CALL createShapeFromPlanes( const hkArray<hkVector4>& planes, hkReal minVolume, FractureUtil::ShapeInfo& shapeInfo);

			/// Calculate the volume of a hkpConvexVerticesShape. This function is rather expensive
			/// and only works for boxes or convex vertices shapes
		static hkReal HK_CALL calcShapeVolume( const hkpConvexShape* shape );

};




#endif // HK_FRACTURE_UTIL_H


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

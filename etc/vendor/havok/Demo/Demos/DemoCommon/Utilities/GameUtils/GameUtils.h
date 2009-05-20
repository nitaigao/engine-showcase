/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMO_UTILS_H
#define DEMO_UTILS_H

class hkSphere;
class hkpRigidBody;
class hkpPhysicsSystem;
class hkpShape;
class hkIstream;
class hkpSimpleMeshShape;
class hkRagdoll;
class hkpMoppBvTreeShape;

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>

#include <Common/Serialize/Packfile/hkPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>

class hkPseudoRandomGenerator;
class hkpConvexVerticesShape;

/// These utils allow you to create rigid bodies with various shape types in a single call.
/// There is also a utility function for creating a biped ragdoll system based on standard 
/// proportions for a specified height.
///
/// One point to note, all methods allocate a new hkpShape:
/// -   These methods don't facilitate "sharing" hkShapes amongst bodies. For example,
///		if you wish to create 100 cubes of the same size, it is much better to create a single 
///		shape yourself, and create 100 rigidbodies using this shape, rather than using the 
///		createBox method below.
class GameUtils
{
	public:

		enum RagdollPartType
		{
			RPT_BOX,
			RPT_CONVEX,
			RPT_SPHERE,
			RPT_CAPSULE,
			RPT_COUNT
		};
		enum RagdollWantHandsAndFeet
		{
			NO_HANDS_AND_FEET,
			WANT_HANDS_AND_FEET
		};
		enum RagdollInterBoneCollisions
		{
			DISABLE_ALL_BONE_COLLISIONS,
			DISABLE_ONLY_ADJOINING_BONE_COLLISIONS
		};


		struct RagdollCinfo
		{
			RagdollCinfo();

			hkReal m_height;
			hkVector4 m_position;
			hkQuaternion m_rotation; 
			int m_systemNumber; 
			int m_numVertebrae;
			bool m_wantOptimizeInertias;
			RagdollPartType m_boneShapeType;
			RagdollWantHandsAndFeet m_wantHandsAndFeet;
			RagdollInterBoneCollisions m_ragdollInterBoneCollisions;

		};

			/// This function creates a collection of constrained rigid bodies 
			/// representing a humanoid biped. From the specified height, limb 
			/// lengths are calculated using standard proportions measured in heads.
			/// The specified biped height equates to 8 heads.
		static hkpPhysicsSystem*	HK_CALL createRagdoll( hkReal height, const hkVector4& position, const hkQuaternion& rotation, int systemNumber = 1, RagdollPartType type = RPT_CAPSULE );

		static hkpPhysicsSystem*	HK_CALL createRagdoll( RagdollCinfo& info );

		/// This function creates a rigid body based on the shape passed in.
		static hkpRigidBody* HK_CALL createRigidBody(hkpShape* shape, const hkReal mass, const hkVector4 &position);

		/// Creates a rectangular tower using the following assumptions
		///   - y is up
		///   - halfExtents(0) == halfExtents(2) * 2.0f
		static void HK_CALL createTower( hkpWorld* world, const hkVector4& position, const hkVector4& halfExtents, hkReal gapWidth, int size, int height );

		/// Creates a rigid body made up from an hkpBoxShape of the specific size.
		static hkpRigidBody* HK_CALL createBox(const hkVector4& size, const hkReal mass, const hkVector4& position, hkReal radius = 0.f);

		/// Create a simple mesh shape with a Mopp wrapped around it. Suitable for SPUs
		static hkpMoppBvTreeShape* createMoppShape(int side = 5, const hkVector4& position = hkVector4::getZero(), hkReal scale = 1.0f);

		/// Creates a hkpConvexVerticesShape in the form of a box.
		static hkpConvexVerticesShape* HK_CALL createConvexVerticesBoxShape( const hkVector4& center, const hkVector4& halfExtents, hkReal radius = 0.0f);

		/// Creates a rigid body made up from an hkpConvexVerticesShape in the form of a box.
		static hkpRigidBody* HK_CALL createConvexVerticesBox( const hkVector4& size, const hkReal mass, const hkVector4& position, hkReal radius = 0.0f );

		/// Creates a rigid body made up from an hkpSphereShape of the specific radius.
		static hkpRigidBody* HK_CALL createSphere(const hkReal radius, const hkReal mass, const hkVector4& position);

		/// Creates a rigid body made with a hkpCapsuleShape.
		/// The capsule will be fitted to a box of the specified size.
		/// The radius of the capsule will equate to half the length of the second largest extent.
		/// The length will equate to the length of the largest extent minus twice the radius.
		static hkpRigidBody* HK_CALL createCapsuleFromBox(const hkVector4& size, const hkReal mass, const hkVector4& position);

		/// Creates a rigid body with a hkpCylinderShape.
		static hkpRigidBody* HK_CALL createCylinder( hkReal radius, hkReal height, hkReal mass, const hkVector4& position );

		/// Creates a rigid body made with random convex hull.
		/// The vertices making up the hull are randomly create within the space 
		/// defined by a sphere of the specified radius.
		static hkpRigidBody* HK_CALL createRandomConvexGeometric(const hkReal radius, const hkReal mass, const hkVector4& position, const int numVertices, hkPseudoRandomGenerator* generator = HK_NULL);

		/// Creates a rigid body made with random convex hull.
		/// The vertices making up the hull are randomly created within the space 
		/// defined by a box of the specified size.
		static hkpRigidBody* HK_CALL createRandomConvexGeometricFromBox(const hkVector4& size, const hkReal mass, const hkVector4& position, const int numVertices, hkPseudoRandomGenerator* generator = HK_NULL);

		/// Creates a rigid body which is an extruded 2D polygon with vertices 
		// verts[0], verts[1], ... verts[0] + extrusionVector, verts[1] + extrusionVector....
		static hkpRigidBody* HK_CALL createConvexGeometricPrism(const hkArray<hkVector4>& verts, const hkVector4& extrusionDirection, hkReal radius, const hkReal mass, const hkVector4& position);

		/// Creates a rigid body made with the specified geometry.
		static hkpRigidBody* HK_CALL createConvexGeometric( hkGeometry* geometry, const hkReal mass, const hkVector4& position );

		/// Returns a vector within the space defined by a given sphere.
		static hkVector4 HK_CALL createRandomVertex(const hkSphere &sphere, hkPseudoRandomGenerator* generator = HK_NULL);

		/// Load geometry information from a .TK file, scaling the resultant geometry by
		/// the amounts specified along each axis.
        static hkGeometry* HK_CALL loadTKGeometry(const hkString fname, const hkVector4& scale);
 
		/// Load geometry information from stream representing a .TKL file.
		/// Scales the resultant geometry by the amounts specified along each axis.
		static hkGeometry* HK_CALL readFromTKLStream(hkIstream& fin, const hkVector4& scale);

		/// Load geometry information from stream representing a .TK file.
		/// Scales the resultant geometry by the amounts specified along each axis.
		static hkGeometry* HK_CALL readFromTKStream(hkIstream& fin, const hkVector4& scale);

		/// Create a fixed rigid body made with a hkpBoxShape.
		static void HK_CALL createStaticBox( hkpWorld* world, float centerX, float centerY, float centerZ, float radiusX, float radiusY, float radiusZ );

		/// Load geometry information from a .TK file, scaling the resultant geometry by
		/// the amounts specified along each axis. This geometry is then used to create
		/// an hkpShape holding a fixed concave MOPP landscape.
		static hkpMoppBvTreeShape* HK_CALL loadTK2MOPP(const hkString& file,const hkVector4& scale, hkReal radius);
		static hkpRigidBody* HK_CALL loadTK2MOPPRB( const hkString& file,const hkVector4& scale, hkReal radius, const hkTransform& rotation);
		static hkpRigidBody* HK_CALL loadTTK2MOPPRBtransform( const hkString& file,const hkVector4& scale, hkReal radius, const hkTransform& globalTransform);

		//
		// Packfile serialization
		//

		/// Load from a packfile. Assumes you have saved it with a hkWorldContainer (in hctUtilities) as
		/// the top level structure in the file. The sections given back must be deallocated when
		/// you are finished using the data. See docs on how to use the packfiles, and you can
		/// use hkpHavokSnapshot::deallocate( sections ) to free the sections (in hctUtilities).
		static hkpWorld* HK_CALL loadWorldFromPackfile( const hkString& path, hkPackfileReader::AllocatedData** allocatedData );

		/// Save the world (and its contents) to a binary packfile (using the host binary layout)
		/// It will have a hkWorldContainer (in hctUtilities) as the top level structure in the file.
		/// If you don't provide a target layout then the current host layout is assumed.
		static void HK_CALL saveWorldToPackfile( const class hkpWorld* world, const hkString& path, bool binary = true, const hkStructureLayout::LayoutRules* targetLayout = HK_NULL );


};

#endif

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

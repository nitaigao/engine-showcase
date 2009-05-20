/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Landscape/MoppInstancing/MoppInstancingDemo.h>

#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

// For drawing
#include <Common/Visualize/hkDebugDisplay.h>


MoppInstancingDemo::MoppInstancingDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{
	// Disable face culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);

	//
	// Setup the camera
	//
	{
		hkVector4 from(20.0f, 20.0f, -60.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.5f, 0.0f);
		info.setBroadPhaseWorldSize(150.0f);
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	//  create the ground mopps
	//  See the comments on createMoppShape() below.
	//

	m_originalMopp = createMoppShape();
	m_smallMopp = createScaledMopp(m_originalMopp, .75f);
	m_bigMopp = createScaledMopp(m_originalMopp, 1.5f);

	hkpShape* shapes[3] = { m_originalMopp, m_smallMopp, m_bigMopp};
	hkReal offsets[3] = {0.0f, -20.0f, 30.0f};
	
	//
	//	Create the fixed rigid bodies and add them to the world
	//
	for (int i=0; i<3; i++)
	{
		hkpRigidBodyCinfo groundInfo;
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;

		groundInfo.m_shape = shapes[i];
		groundInfo.m_position.set(offsets[i],-2.6f,0.0f);
		
		m_fixedBodies[i] = new hkpRigidBody(groundInfo);
		m_world->addEntity( m_fixedBodies[i] );

		// Drop some boxes on the mesh
		hkVector4 center(offsets[i] - 1.0f, 0, 2.5f);
		addBoxPile(10, center);
	}

	m_world->unlock();
}

void MoppInstancingDemo::addBoxPile(int numBoxes, hkVector4Parameter offset)
{
	hkVector4 boxRadii( 0.5f, 0.5f, 0.5f);
	hkpBoxShape* cubeShape = new hkpBoxShape(boxRadii, 0 );

	hkpRigidBodyCinfo boxInfo;

	boxInfo.m_mass = 1.0f;
	boxInfo.m_shape = cubeShape;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
	boxInfo.m_rotation.setIdentity();
	boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	for (int i = 0; i < numBoxes; i++ )
	{
		//
		// create a rigid body (using the template above) and add to the m_world
		//
		boxInfo.m_position.set(0, i * 1.4f, -.15f*i);
		boxInfo.m_position.add4(offset);
		boxInfo.m_restitution = .2f;

		hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
		m_world->addEntity( boxRigidBody );
		boxRigidBody->removeReference();
	}
	cubeShape->removeReference();
}

MoppInstancingDemo::~MoppInstancingDemo()
{
	m_originalMopp->removeReference();
	m_smallMopp->removeReference();
	m_bigMopp->removeReference();

	m_world->markForWrite();

	m_fixedBodies[0]->removeReference();
	m_fixedBodies[1]->removeReference();
	m_fixedBodies[2]->removeReference();
	
	m_world->removeReference();
	m_world = HK_NULL;
}

//
//	Creates a scaled version of the given hkpMoppBvTreeShape
//	Both the triangle data and hkpMoppCode are shared between the instances, resulting in very little memory overhead.
//  Also, since we use existing Havok classes, the resulting shapes can be simulated on the SPU without any additional work.
//	There are a few important things to note:
//	1) If the world is serialized out after the scaled instance is created (e.g. with hkpHavokSnapshot), multiple copies of
//		the code will be stored.
//	2) The scaled instance must not be used after the original is removed from memory. This is because the hkpMoppCode
//		isn't reference counted.
//  3) Scaling is currently only supported for hkpExtendedMeshShape. If the hkpExtendedMeshShape has ShapesSubpart, these
//		will NOT be scaled correctly.
//
hkpMoppBvTreeShape* MoppInstancingDemo::createScaledMopp(const hkpMoppBvTreeShape* originalMopp, hkReal relativeScale)
{
	const hkpShape* oldContainer = originalMopp->getShapeCollection();
	HK_ASSERT2(0x530b238a, oldContainer->getType() == HK_SHAPE_EXTENDED_MESH, "This function only scales hkpExtendedMeshShapes");

	const hkpExtendedMeshShape* oldMesh = static_cast<const hkpExtendedMeshShape*>(oldContainer);

	// Create an new mesh, and copy the mesh data
	// Note that the underlying vertex data isn't duplicated, only the pointer information, etc.
	hkpExtendedMeshShape* newMesh;
	{
		newMesh= new hkpExtendedMeshShape(oldMesh->getRadius(), oldMesh->getNumBitsForSubpartIndex());
		
		// Adjust the mesh scaling for the copy.
		// We have to do this BEFORE we add the triangle subparts, or else the AABB won't be computed correctly
		hkVector4 scaleMult; scaleMult.setAll3(relativeScale);
		hkVector4 newMeshScale; newMeshScale.setMul4( scaleMult, oldMesh->getScaling() );
		newMesh->setScaling(newMeshScale);

		for (int i=0; i<oldMesh->getNumTrianglesSubparts(); i++)
		{
			newMesh->addTrianglesSubpart(oldMesh->getTrianglesSubpartAt(i));
		}
		HK_ASSERT2(0x401d4c68, oldMesh->getNumShapesSubparts() == 0, "Can't scale shape subparts!");
	
	}

	// Create a new hkpMoppCode. The internal data points to the original data, so the memory overhead is negligible.
	const hkpMoppCode* oldCode = originalMopp->getMoppCode();
	hkpMoppCode* newcode;
	{
		hkpMoppCode::CodeInfo newCodeInfo = oldCode->m_info;

		// This is the tricky part. Adjusting the offset in this way will ensure that the collisions are handled properly.
		hkVector4 codeMult; codeMult.set( relativeScale, relativeScale, relativeScale, 1.0f / relativeScale );
		newCodeInfo.m_offset.mul4(codeMult);

		// Use the newly scaled hkpMoppCode::CodeInfo, and point to the old data
		newcode = new hkpMoppCode(newCodeInfo, oldCode->m_data.begin(), oldCode->m_data.getSize());
	}

	hkpMoppBvTreeShape* newMopp = new hkpMoppBvTreeShape(newMesh, newcode);
	newMesh->removeReference();
	newcode->removeReference();
	
	return newMopp;
}



	/// create a mopp shape:\n
	/// The idea of this particular instance is to use a strip:\n
	///	\code
	///		v0----v1
	///			/
	///		v2----v3
	///			/
	///		v4----v5
	///	and so on.
	///	\code
	/// So we just have to create an alternating vertexArray (triples of float)
	/// and a triangle index array. As we are using strips, the index array can
	/// actually use overlapping triangles: Array = [ 0,1,2,3,4,5...]. So the first triangle
	/// get's the indices 0,1,2 the next triangle the indices 1,2,3 and so on.\n\n

hkpMoppBvTreeShape* MoppInstancingDemo::createMoppShape()
{

	for (short i = 0; i < NUM_VERTICES; i++ )
	{
		m_indices[i] = i;
		m_vertices[i*4 + 0] = (i&1) * 16.0f - 8.0f;
		m_vertices[i*4 + 1] = (i&3) * 0.3f;
		m_vertices[i*4 + 2] = i * 2.0f - NUM_VERTICES;
		m_vertices[i*4 + 3] = 0.0f;
	}
	
	hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape;
	hkVector4 scale(.75f, 2.5f, 1.0f);
	mesh->setScaling(scale);
	mesh->setRadius( 0.05f );

	{
		hkpExtendedMeshShape::TrianglesSubpart part;		

		part.m_vertexBase = m_vertices;
		part.m_vertexStriding = sizeof(float) * 4;
		part.m_numVertices = NUM_VERTICES;

		part.m_indexBase = m_indices;
		part.m_indexStriding = sizeof( hkUint16);
		part.m_numTriangleShapes = NUM_TRIANGLES;
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

		mesh->addTrianglesSubpart( part );
	}

	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = true;
	hkpMoppCode* code = hkpMoppUtility::buildCode( mesh ,mci);

	hkpMoppBvTreeShape* moppShape;
	moppShape = new hkpMoppBvTreeShape( mesh, code );

	mesh->removeReference();
	code->removeReference();

	return moppShape;
}

// Raycast from the bottom (Y is up) to make sure that raycasting against the scaled mopp works
void MoppInstancingDemo::checkRayCasts(const hkpMoppBvTreeShape* mopp, const hkTransform& t)
{
	hkAabb aabb; mopp->getAabb(hkTransform::getIdentity(), 1.0f, aabb);
	hkVector4 extents; extents.setSub4(aabb.m_max, aabb.m_min);

	const int numX=10, numZ = 10;
	const hkReal deltaX = extents(0) / hkReal(numX-1);
	const hkReal deltaZ = extents(2) / hkReal(numZ-1);

	for (int x=0; x<numX; x++)
	{
		for (int z=0; z<numZ; z++)
		{
			hkpShapeRayCastInput input;
			hkpShapeRayCastOutput output;
			input.m_from(0) = aabb.m_min(0) + hkReal(x)*deltaX;
			input.m_from(1) = aabb.m_min(1)                   ;
			input.m_from(2) = aabb.m_min(2) + hkReal(z)*deltaZ;
				
			input.m_to = input.m_from;
			input.m_to(1) = aabb.m_max(1);
			input.m_rayShapeCollectionFilter = HK_NULL;

			mopp->castRay(input, output);

			hkVector4 worldFrom, worldTo;
			worldFrom.setTransformedPos(t, input.m_from);
			worldTo.setTransformedPos(t, input.m_to);

			if (output.hasHit())
			{
				hkVector4 hitpoint; hitpoint.setInterpolate4(worldFrom, worldTo, output.m_hitFraction);
				HK_DISPLAY_LINE(worldFrom, hitpoint, hkColor::GREEN);
			}

			// Check that the naive raycast gives the same results
			hkpShapeRayCastOutput testOutput;
			mopp->getShapeCollection()->castRay(input, testOutput);

			HK_ASSERT(0x793171a3, hkMath::equal(testOutput.m_hitFraction, output.m_hitFraction) );
			if ( !hkMath::equal(testOutput.m_hitFraction, 1.0f) )
			{
				HK_ASSERT(0x793171a3, testOutput.m_normal.equals3(output.m_normal) );
			}
		}
	}

}

hkDemo::Result MoppInstancingDemo::stepDemo()
{
	m_world->markForRead();

	for (int i=0; i<3; i++)
	{
		HK_ASSERT(0x79151c48, m_fixedBodies[i]->getCollidable()->getShape()->getType() == HK_SHAPE_MOPP);
		const hkpMoppBvTreeShape* mopp = static_cast<const hkpMoppBvTreeShape*> (m_fixedBodies[i]->getCollidable()->getShape());
		checkRayCasts(mopp, m_fixedBodies[i]->getTransform() );
	}
	m_world->unmarkForRead();

	return hkDefaultPhysicsDemo::stepDemo();
	
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Demonstrates how to create scaled instances of meshes without duplicating the collision data.";

HK_DECLARE_DEMO(MoppInstancingDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "How to create scaled mesh instances.", helpString);

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Collide/Shape/Compound/Collection/Mesh/hkpMeshMaterial.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Common/Visualize/hkDebugDisplay.h>

struct MeshMaterialDemo : public hkDefaultPhysicsDemo
{
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		// Add material properties as a subclass of hkpMeshMaterial
	struct MeshMaterial : public hkpMeshMaterial
	{
		hkReal m_scaleMovement; // Scale how strongly the ray is attracted to the origin.
		int m_colour; // Set the colour of the text.
		int m_index; // A real example would probably use pointer arithmetic on Subpart.m_materialBase.
	};

	struct MeshData
	{
		MeshData();
		hkArray<hkVector4> vertices;
		hkArray<hkUint16> indices;
		hkArray<hkUint8> materialIndices;
	};

	enum { X_RES=20, Y_RES=20, NUM_MATERIALS=7, NUM_TRI = ((X_RES-1)*(Y_RES-1)*2) };

	void setupMesh(hkpExtendedMeshShape& mesh)
	{
		hkpExtendedMeshShape::TrianglesSubpart part;
		part.m_vertexBase = reinterpret_cast<hkReal*>(m_meshData.vertices.begin());
		part.m_vertexStriding = sizeof(hkVector4);
		part.m_numVertices = X_RES * Y_RES;

		part.m_indexBase = m_meshData.indices.begin();
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;
		part.m_indexStriding = 3 * sizeof(hkInt16);
		part.m_numTriangleShapes = NUM_TRI;

		part.m_materialIndexStridingType = hkpExtendedMeshShape::MATERIAL_INDICES_INT8;
		part.m_materialIndexBase = m_meshData.materialIndices.begin();
		part.m_materialIndexStriding = sizeof(hkUint8);

		// Mesh material setup.
		part.m_materialBase = &m_meshMaterials[0];
		part.m_materialStriding = sizeof(MeshMaterial);
		part.m_numMaterials = NUM_MATERIALS;

		mesh.addTrianglesSubpart(part);
	}

	MeshMaterialDemo(hkDemoEnvironment* env) : hkDefaultPhysicsDemo(env)
	{
		// set up shared materials table
		hkString::memSet( m_meshMaterials, 0, sizeof(MeshMaterial) * NUM_MATERIALS );
		for( int i = 0; i < NUM_MATERIALS; ++i )
		{
			m_meshMaterials[i].m_scaleMovement = hkMath::fabs(i-3.0f);
			m_meshMaterials[i].m_colour = hkColor::getRandomColor();
			m_meshMaterials[i].m_index = i;
		}

		// new world
		hkpWorldCinfo wCi;
		m_world = new hkpWorld(wCi);
		m_world->lock();

		// add bodies for each of the meshes
		{
			setupMesh(m_mesh); 
			hkpRigidBodyCinfo rbCi;
			rbCi.m_shape = &m_mesh;
			rbCi.m_motionType = hkpMotion::MOTION_FIXED;

			hkpRigidBody* rb = new hkpRigidBody(rbCi);
			m_world->addEntity(rb);
			rb->removeReference();
		}

		m_castPos.set(0,5,0);

		{
			hkVector4 from(0,30,-10);
			hkVector4 to(0,0,0);
			hkVector4 up(0,1,0);
			setupDefaultCameras(env, from, to, up );
			setupGraphics();
		}
		
		m_world->unlock();
	}

	Result stepDemo()
	{
		m_world->lock();

		hkpWorldRayCastInput input;
		input.m_from = m_castPos;
		input.m_to.setAdd4( m_castPos, m_world->getGravity() );
		hkpWorldRayCastOutput output;
		HK_DISPLAY_LINE( input.m_from, input.m_to, 0xffffffff);
		m_world->castRay(input, output);

		m_world->unlock();

		hkVector4 delta; delta.setZero4();
		if( output.hasHit() )
		{
			// Show a slightly flattened normal
			{
				hkVector4 dir; dir.setSub4( input.m_to, input.m_from );
				hkVector4 hit; hit.setAddMul4( input.m_from, dir, output.m_hitFraction );
				output.m_normal(1) = 0.1f;
				hkVector4 norm; norm.setAddMul4( hit, output.m_normal, 5 );
				HK_DISPLAY_LINE( hit, norm, 0xff00ffff);
			}
			
			// extract and use the data from the mesh material
			const MeshMaterial* material = static_cast<const MeshMaterial*>(m_mesh.getMeshMaterial( output.m_shapeKeys[0] ) );
			if( material )
			{
				hkString s;
				s.printf("ShapeKey(%i)\nMaterial(%i)\nScale(%f)\n", output.m_shapeKeys[0], material->m_index, material->m_scaleMovement);
				hkQuadRealUnion p; p.q = input.m_from.getQuad();
				m_env->m_textDisplay->outputText3D( s, p.r[0], p.r[1]+1, p.r[2], material->m_colour );
				
				output.m_normal(1) = 0;
				delta.setMul4(material->m_scaleMovement, output.m_normal);
			}
		}
		
		const hkgPad* pad = m_env->m_gamePad;
		delta(0) += (pad->getButtonState() & HKG_PAD_DPAD_LEFT) ? 1 : 0;
		delta(0) -= (pad->getButtonState() & HKG_PAD_DPAD_RIGHT) ? 1 : 0;
		delta(2) += (pad->getButtonState() & HKG_PAD_DPAD_UP) ? 1 : 0;
		delta(2) -= (pad->getButtonState() & HKG_PAD_DPAD_DOWN) ? 1 : 0;
		m_castPos.addMul4(0.1f, delta);

		return hkDefaultPhysicsDemo::stepDemo();
	}


		// Using a mesh without a bounding volume can be very slow.
		// In this demo we don't care.
	MeshData m_meshData;
	hkpExtendedMeshShape m_mesh;
	
		// Material library shared by all meshes.
	MeshMaterial m_meshMaterials[NUM_MATERIALS];

		// Ray cast origin.
	hkVector4 m_castPos;
};

MeshMaterialDemo::MeshData::MeshData()
{
	int index = 0;
	vertices.setSize(X_RES*Y_RES);
	for( int yi = 0; yi < Y_RES; ++yi )
	{
		for( int xi = 0; xi < X_RES; ++xi )	
		{
			int xm = xi - X_RES/2;
			int ym = yi - Y_RES/2;
			vertices[index++].set( hkReal(xm), (8.0f/(X_RES*Y_RES)) * (xm*xm+ym*ym), hkReal(ym) );
		}
	}
	index = 0;
	indices.setSize(NUM_TRI*3);
	materialIndices.reserveExactly(NUM_TRI);
	for( int yi = 0; yi < Y_RES-1; ++yi )
	{
		for( int xi = 0; xi < X_RES-1; ++xi )	
		{
			indices[index++] = hkUint16( (xi+0)*Y_RES + (yi+0) );
			indices[index++] = hkUint16( (xi+1)*Y_RES + (yi+0) );
			indices[index++] = hkUint16( (xi+0)*Y_RES + (yi+1) );
			materialIndices.pushBackUnchecked( hkUint8( (yi*NUM_MATERIALS) / Y_RES ) );

			indices[index++] = hkUint16( (xi+1)*Y_RES + (yi+0) );
			indices[index++] = hkUint16( (xi+1)*Y_RES + (yi+1) );
			indices[index++] = hkUint16( (xi+0)*Y_RES + (yi+1) );
			materialIndices.pushBackUnchecked( hkUint8( (yi*NUM_MATERIALS) / Y_RES ) );
		}
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO( MeshMaterialDemo, HK_DEMO_TYPE_PRIME, "Extend hkpMeshMaterial with your own data", 
	"This example shows how to extend hkpMeshMaterial to include your own data and how "\
	"to extract the material from a raycast.\n"\
	"Use the left and right arrow keys to move the raycast. The shapekey, material index "\
	"and the material->m_scaleMovement value are shown.");

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

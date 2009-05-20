/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/RigidBodies/InitialPenetration/InitialPenetrationDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Collection/StorageExtendedMesh/hkpStorageExtendedMeshShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics/Collide/Util/hkpTriangleCompressor.h>

#include <Physics/Utilities/Actions/EasePenetration/hkpEasePenetrationAction.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Memory/Memory/hkMemory.h>





enum VariantType
{
	DOING_NOTHING,
	FADEIN_ALLOWED_PENETRATION_DEPTH,
	FADEIN_PENETRATION_DISTANCE_TOO,
};

struct BenchmarkSuiteVariant
{
	const char*	 m_name;
	VariantType  m_type;
	const char* m_details;
};


static const BenchmarkSuiteVariant g_variants[] =
{
	{ "No correction, very high CPU usage", DOING_NOTHING, "This variant hows CPU usage when a batch of bodies is added in penetrating state, and no correction is done. Note very high number of TOIs generated." },
	{ "Scaling allowed penetration depth", FADEIN_ALLOWED_PENETRATION_DEPTH, "This is the same scene as the one above. Each body has an EasePenetrationAction attached which scales it's allowed penetration depth over the period of 1 second." },
	{ "Scaling allowed penetration depth and contact depth", FADEIN_PENETRATION_DISTANCE_TOO, "This is the same scene as the one above. Additionally contact depth is also scaled, which results with less jitter and softer penetration recovery." },
};

static hkpShape* createMoppShape(hkpShapeCollection* meshShape)
{
	hkpMoppCompilerInput moppInput;
	moppInput.m_enableChunkSubdivision = true;

	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape, moppInput );
	hkpShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);

	code->removeReference();

	return moppShape;
}

static void CreatePhysicsTerrain( hkpWorld* world, const int side, float deltaHeight = 0.0f )
{
	const float triangleEdgeLen = 0.5f;
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape( 0.05f /*radius*/);

	{
		meshShape->m_vertices.setSize( side * side );
		for(int x = 0; x < side; x++)
		{
			for (int y = 0; y < side; y++ )
			{
				float height = 0.0f;
				if ( (x&1) && (y&1) )
				{
					height = -deltaHeight;
				}

				hkVector4 vertex (
					triangleEdgeLen * (x * 1.0f - (side-1) * 0.5f),
					triangleEdgeLen * height,
					triangleEdgeLen * (y * 1.0f - (side-1) * 0.5f));
				meshShape->m_vertices[x*side + y] = vertex ;
			}
		}
	}

	{
		meshShape->m_triangles.setSize( (side-1) * (side-1) * 2);
		int corner = 0;
		int curTri = 0;
		for(int i = 0; i < side - 1; i++)
		{
			for (int j = 0; j < side - 1; j++ )
			{
				meshShape->m_triangles[curTri].m_a = corner+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner;
				curTri++;

				meshShape->m_triangles[curTri].m_a = corner+side+1;
				meshShape->m_triangles[curTri].m_b = corner+side;
				meshShape->m_triangles[curTri].m_c = corner+1;
				curTri++;
				corner++;
			}
			corner++;
		}
	}

	hkpStorageExtendedMeshShape* extendedMesh = new hkpStorageExtendedMeshShape();

	hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_numTriangleShapes = meshShape->m_triangles.getSize();
	part.m_numVertices = meshShape->m_vertices.getSize();
	part.m_vertexBase = (float*)meshShape->m_vertices.begin();
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;
	part.m_vertexStriding = sizeof(hkVector4);
	part.m_indexBase = meshShape->m_triangles.begin();
	part.m_indexStriding = sizeof(hkpSimpleMeshShape::Triangle);
	extendedMesh->addTrianglesSubpart( part );

	// No longer need the simple mesh
	meshShape->removeReference();

	hkpRigidBodyCinfo ci;

	hkpShape* moppShape = createMoppShape( extendedMesh );
	extendedMesh->removeReference();

	ci.m_shape = moppShape;
	ci.m_restitution = 0.5f;
	ci.m_friction = 0.3f;
	ci.m_position.set( 0.0f, 0.0f, 0.0f );
	ci.m_motionType = hkpMotion::MOTION_FIXED;

	world->addEntity( new hkpRigidBody( ci ) )->removeReference();
	moppShape->removeReference();
}



InitialPenetrationDemo::InitialPenetrationDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{
	const BenchmarkSuiteVariant& variant =  g_variants[m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(55.0f, 50.0f, 55.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras(env, from, to, up, 0.1f, 20000.0f);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo worldInfo;
		m_world = new hkpWorld(worldInfo);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(15.0f, 15.0f, 15.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	{
		int side = 32;
		CreatePhysicsTerrain(m_world, side, 1.0f);
		
		hkPseudoRandomGenerator gen(234);
		for (int i = 0; i < 50; i++)
		{
			hkVector4 size(0.5f, 0.5f, 0.5f);
			hkVector4 pos;
			gen.getRandomVector11(pos);
			pos.mul4(hkVector4(5.0f, 0.1f, 5.0f));
			pos(1) += 0.1f;
			hkpRigidBody* body = GameUtils::createBox(size, 1.0f, pos);
			body->setQualityType(HK_COLLIDABLE_QUALITY_CRITICAL);
			m_world->addEntity(body);
			body->removeReference();

			switch(variant.m_type)
			{
			case DOING_NOTHING:
				{
					// nothing				
				}
				break;
			case FADEIN_ALLOWED_PENETRATION_DEPTH:
				{
					// Attach hkpEasePenetrationAction, disable contact penetration depth correction
					hkpEasePenetrationAction* action = new hkpEasePenetrationAction(body, 1.0f);
					action->m_reducePenetrationDistance = false;
					m_world->addAction(action);
					action->removeReference();
				}
				break;
			case FADEIN_PENETRATION_DISTANCE_TOO:
				{
					// Attach hkpEasePenetraitonAction, use the default option of correcting contact point penetration depth.
					hkpAction* action = new hkpEasePenetrationAction(body, 1.0f);
					m_world->addAction(action);
					action->removeReference();
				}
				break;
			}
		}
	}

	//
	//	Some values
	//

	m_world->unlock();
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "When adding bodies to the world, it's not uncommon that they're placed so that they penetrate the landscape. "\
"This demo shows how this results in enourmous CPU hits, and how this can be counteracted with the use of hkpEasePenetrationAction.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( InitialPenetrationDemo, HK_DEMO_TYPE_PRIME, BenchmarkSuiteVariant, g_variants, helpString );


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

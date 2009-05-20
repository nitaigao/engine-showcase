/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Common/Serialize/Packfile/hkPackfileReader.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Util/hkpTriangleUtil.h>

#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/UseCase/CustomCollectors/BackfacesCulledRayHitCollector/BackfacesCulledRayHitCollector.h>

//#include <Common/Geometry/Utilities/>

#include <Physics/Utilities/Collide/Filter/constraint/hkpConstraintCollisionFilter.h>

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpMoppBvTreeShape;

struct BackfacesCulledRayHitCollectorDemo : public hkDefaultPhysicsDemo
{
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	BackfacesCulledRayHitCollectorDemo(hkDemoEnvironment* env);
	~BackfacesCulledRayHitCollectorDemo();
	void showRaycastResults(const hkpWorldRayCastOutput& culledOutput, int triangleColor);
	Result stepDemo(); 

	hkpRigidBody* m_hemisphereRb;
	hkpRigidBody* m_boxRb;

	hkpPhysicsData* m_physicsData;
	hkPackfileReader::AllocatedData* m_loadedData;

	void doRaycast(hkpRigidBody* rb1, hkpRigidBody* rb2);
};


BackfacesCulledRayHitCollectorDemo::BackfacesCulledRayHitCollectorDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from( -9.0f, 6.0f, -6.0f);
		hkVector4 to  ( 0.0f, 0.0f, 0.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkString filename; // We have a different binary file depending on the compiler and platform
		filename.printf("Resources/Physics/Objects/hemisphere_L%d%d%d%d.hkx",
			hkStructureLayout::HostLayoutRules.m_bytesInPointer,
			hkStructureLayout::HostLayoutRules.m_littleEndian? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_reusePaddingOptimization? 1 : 0,
			hkStructureLayout::HostLayoutRules.m_emptyBaseClassOptimization? 1 : 0);
		hkIstream infile( filename.cString() );
		HK_ASSERT( 0x215d080c, infile.isOk() );
		m_physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &m_loadedData);

		m_hemisphereRb = m_physicsData->findRigidBodyByName("hemisphere");
		m_boxRb = m_physicsData->findRigidBodyByName("box");

		hkpWorldCinfo *info = m_physicsData->getWorldCinfo();
 		info->setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
 		info->m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;

		// Set gravity to zero so body floats.
		info->m_gravity.set(0.0f, 0.0f, 0.0f);	
		info->setBroadPhaseWorldSize( 1000.0f );
		info->m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING;

		m_world = m_physicsData->createWorld();
		m_world->lock();

		hkpConstraintCollisionFilter* collisionFilter = new hkpConstraintCollisionFilter();
		m_world->setCollisionFilter(collisionFilter);
		collisionFilter->removeReference();
		
		// for drawing purposes
		hkpBroadPhaseBorder* border = new hkpBroadPhaseBorder( m_world, hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		setupGraphics();

		m_world->unlock();
	}	
}

BackfacesCulledRayHitCollectorDemo::~BackfacesCulledRayHitCollectorDemo()
{
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_physicsData->removeReference();
	m_loadedData->removeReference();
}

void BackfacesCulledRayHitCollectorDemo::showRaycastResults(const hkpWorldRayCastOutput& output, int triangleColor)
{
		// Find the leaf shape
		hkpShapeContainer::ShapeBuffer buffer;
		const hkpShape* shape = output.m_rootCollidable->getShape();
		for(int keyIndex = 0; shape != HK_NULL; ++keyIndex )
		{
			if(shape->getType() == HK_SHAPE_MOPP)
			{
				keyIndex++;
			}

			// go to the next level
			const hkpShapeContainer* container = shape->getContainer();
			if (container)
			{
				shape = container->getChildShape(output.m_shapeKeys[keyIndex], buffer);
			}
			else
			{
				break;
			}
		}

		// Draw the leaf shape's outline if it's a triangle.
		if (shape->getType() == HK_SHAPE_TRIANGLE)
		{
			const hkpTriangleShape* triangle = static_cast<const hkpTriangleShape*>( shape );
			hkArray<hkVector4> transformedVertices(3); 
			transformedVertices[0].setTransformedPos(hkGetRigidBody(output.m_rootCollidable)->getTransform(), triangle->getVertex(0));
			transformedVertices[1].setTransformedPos(hkGetRigidBody(output.m_rootCollidable)->getTransform(), triangle->getVertex(1));
			transformedVertices[2].setTransformedPos(hkGetRigidBody(output.m_rootCollidable)->getTransform(), triangle->getVertex(2));

			HK_DISPLAY_LINE(transformedVertices[0], transformedVertices[1], triangleColor);
			HK_DISPLAY_LINE(transformedVertices[2], transformedVertices[1], triangleColor);
			HK_DISPLAY_LINE(transformedVertices[0], transformedVertices[2], triangleColor);

// 			hkVector4 triangleNormal;
// 			hkpTriangleUtil::calcNormal(triangleNormal,
// 				transformedVertices[0], transformedVertices[1], transformedVertices[2]);
// 			triangleNormal.normalize3();
// 
// 			hkVector4 trianglePos;
// 			triangle->getCentre(trianglePos);
// 			trianglePos.setTransformedPos(hkGetRigidBody(output.m_rootCollidable)->getTransform(), trianglePos);
// 
// 			HK_DISPLAY_ARROW(trianglePos, triangleNormal, hkColor::ORANGE);
		}	
}

hkDemo::Result BackfacesCulledRayHitCollectorDemo::stepDemo()
{
	m_world->lock();
	m_world->markForRead();

	// Cast a ray using the BackfacesCulledRayHitCollector
	{
		hkpWorldRayCastInput culledInput;
		culledInput.m_from =	hkVector4(-4.0f, 0.5f, 1.0f);
		culledInput.m_to =		hkVector4(4.0f, 0.5f, 1.0f);
		hkVector4 culledRayDirection; culledRayDirection.setSub4(culledInput.m_to, culledInput.m_from);

		HK_DISPLAY_ARROW(culledInput.m_from, culledRayDirection, hkColor::YELLOW);

		BackfacesCulledRayHitCollector culledOutput(culledRayDirection);
		m_world->castRay(culledInput, culledOutput);
		for (int i=0; i < culledOutput.getHits().getSize(); i++)
		{
			showRaycastResults(culledOutput.getHits()[i], hkColor::ORANGE);

			hkVector4 normalPosition;
			normalPosition.setInterpolate4(culledInput.m_from, culledInput.m_to, culledOutput.getHits()[i].m_hitFraction);
			HK_DISPLAY_ARROW(normalPosition, culledOutput.getHits()[i].m_normal, hkColor::RED);
		}
	}

	// Cast a ray using the regular hkpAllRayHitCollector
	{
		hkpWorldRayCastInput normalInput;
		normalInput.m_from =	hkVector4(-4.0f, -0.6f, 1.0f);
		normalInput.m_to =		hkVector4(4.0f, -0.6f, 1.0f);
		hkVector4 normalRayDirection; normalRayDirection.setSub4(normalInput.m_to, normalInput.m_from);

		HK_DISPLAY_ARROW(normalInput.m_from, normalRayDirection, hkColor::BLUE);

		hkpAllRayHitCollector normalOutput;
		m_world->castRay(normalInput, normalOutput);
		for (int i=0; i < normalOutput.getHits().getSize(); i++)
		{
			showRaycastResults(normalOutput.getHits()[i], hkColor::CYAN);

			hkVector4 normalPosition;
			normalPosition.setInterpolate4(normalInput.m_from, normalInput.m_to, normalOutput.getHits()[i].m_hitFraction);
			HK_DISPLAY_ARROW(normalPosition, normalOutput.getHits()[i].m_normal, hkColor::GREEN);
		}
	}

	m_world->unmarkForRead();
	m_world->markForWrite();
	{
// 		const hkpShapeContainer* c = m_hemisphereRb->getCollidable()->getShape()->getContainer();
// 		for (hkpShapeKey k = c->getFirstKey(); k != HK_INVALID_SHAPE_KEY; k = c->getNextKey(k))
// 		{
// 			hkpShapeContainer::ShapeBuffer buffer;
// //			hkcout << hkGetShapeTypeName(m_hemisphereRb->getCollidable()->getShape()->getType()) << hkendl; //buffer;
// 			const hkpTriangleShape* triangle = reinterpret_cast<const hkpTriangleShape*>(c->getChildShape(k, buffer));
// 
// 			hkVector4 triangleNormal;
// 			hkpTriangleUtil::calcNormal(triangleNormal,
// 				triangle->getVertex(0), triangle->getVertex(1), triangle->getVertex(2));
// 			triangleNormal.normalize3();
// 
// 			hkVector4 trianglePos;
// 			triangle->getCentre(trianglePos);
// 			trianglePos.setTransformedPos(m_hemisphereRb->getTransform(), trianglePos);
// 
// 			HK_DISPLAY_ARROW(trianglePos, triangleNormal, hkColor::BLUE);
// 		}
	}

	m_world->unmarkForWrite();
	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(BackfacesCulledRayHitCollectorDemo, HK_DEMO_TYPE_PRIME, "Shows a custom backface-culling raycast collector.", "Yellow arrow = BackfacesCulledRayHitCollector\nGreen arrow = hkpAllRayHitCollector\nHighlighted triangles have been accepted by the collectors.");

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

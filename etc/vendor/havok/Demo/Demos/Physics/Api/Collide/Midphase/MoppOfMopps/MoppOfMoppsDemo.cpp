/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Midphase/MoppOfMopps/MoppOfMoppsDemo.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Material/hkgMaterial.h>

#include <Common/Serialize/Util/hkLoader.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

MoppOfMoppsDemo::~MoppOfMoppsDemo()
{
	m_world->markForWrite();

	m_world->removeReference();
	m_world = HK_NULL;

	m_loader->removeReference();
}

static hkpRigidBody* generateLandscape(const hkpShape* treeShape)
{
	hkPseudoRandomGenerator generator(1432);

	// Randomly generate 50 trees. Put them on a flat ground
	//
	hkArray<hkpShape*> shapes; shapes.setSize(15);
	hkTransform t; t.setIdentity();
	hkVector4& p = t.getTranslation();
	for (int i = 0; i < shapes.getSize(); i++)
	{
		generator.getRandomVector11(p);
		p.mul4(15.0f);
		p(2) = 10.0f;
		shapes[i] = new hkpTransformShape(treeShape, t);
	}

	hkVector4 halfExtents(40.0f, 40.0f, 0.05f);
	shapes.pushBack( new hkpBoxShape(halfExtents, 0.0f) );

	hkpListShape* listShape = new hkpListShape(shapes.begin(), shapes.getSize()); 
	hkReferencedObject::removeReferences(shapes.begin(), shapes.getSize());

	// Create chunk mopp around the list shape
	hkpShape* listMopp;
	{
		hkpMoppCompilerInput mci;
		mci.m_enableChunkSubdivision = true;
		hkpMoppCode* listCode = hkpMoppUtility::buildCode(listShape, mci);
		listMopp = new hkpMoppBvTreeShape(listShape, listCode); 
		listCode->removeReference();
	}


	// Create rigid body
	//
	hkpRigidBodyCinfo info;
	info.m_shape = listMopp;
	info.m_mass = 0.0f;
	info.m_motionType = hkpMotion::MOTION_FIXED;

	hkpRigidBody* rb = new hkpRigidBody(info);
	listShape->removeReference();
	listMopp->removeReference();

	return rb;
}

static void generateFallingBodies(hkpWorld* world, hkpShape* shape, int numBodies)
{
	hkPseudoRandomGenerator generator(1432);

	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(shape, 1.0f, info);
	

	for (int i = 0; i < numBodies; i++)
	{
		generator.getRandomVector11(info.m_position);
		info.m_position.mul4(15.0f);
		info.m_position(2) *= 1.0f / 15.0f;
		info.m_position(2) += 20.0f;

		hkpRigidBody* body = new hkpRigidBody(info);
		world->addEntity(body);
		body->removeReference();
	}
}

MoppOfMoppsDemo::MoppOfMoppsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from( 20.0f, 40.0f, 20.0f);
		hkVector4 to  (  0.0f,  0.0f,  8.0f);
		hkVector4 up  (  0.0f,  0.0f,  1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, 0.0f, -9.81f );
		info.setBroadPhaseWorldSize(1000.0f);
		m_world = new hkpWorld( info );
		m_world->lock();
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	//
	// Load an asset
	//

	hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Physics/Objects/Tree.hkx");
	m_loader = new hkLoader();
	hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
	HK_ASSERT2(0xaefe9356, container != HK_NULL , "Could not load asset");

	hkpPhysicsData* physics = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0x245982ae, physics != HK_NULL, "Could not find physics data in root level object" );

	hkpRigidBody* palmBody = physics->getPhysicsSystems()[0]->getRigidBodies()[0];

	//
	// Create landscape body
	//
	hkpRigidBody* landscape = generateLandscape(palmBody->getCollidable()->getShape());
	m_world->addEntity(landscape);
	landscape->removeReference();

	//
	// Create falling debris, with simple & list shapes.
	//

	hkpShape* fallingShape = new hkpBoxShape(hkVector4(0.1f, 0.1f, 0.1f), 0.05f);
	generateFallingBodies(m_world, fallingShape, 200);
	fallingShape->removeReference();

	m_world->unlock();
}


hkDemo::Result MoppOfMoppsDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO( MoppOfMoppsDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "", "");

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

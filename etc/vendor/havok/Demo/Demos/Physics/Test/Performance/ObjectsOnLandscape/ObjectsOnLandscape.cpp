/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Demos/Physics/Test/Performance/ObjectsOnLandscape/ObjectsOnLandscape.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/LandscapeRepository.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

static const int MY_STACK_SIZE = 3500000;

enum AddObjectType
{
	ADD_CONVEX,
	ADD_COMPOUND,
	ADD_RAGDOLL
};

struct ObjectsOnLandscapeVariant
{
	const char*	 m_name;
	AddObjectType m_addObjectType;
	const char*	m_landscapeType;
	int m_numObjects;
	int m_pileSize;
	const char* m_details;
};

static const ObjectsOnLandscapeVariant g_variants[] =
{
	{ "500 Convex Objects on 4800-Triangle Landscape", ADD_CONVEX, "simpleFlatLandOneMetreTris", 500, 2, "500 convex pieces." },
	{ "500 Convex Objects on 79000-Triangle Landscape", ADD_CONVEX, "defaultFlatLandOneMetreTris", 500, 2, "500 convex pieces." },
	{ "100 Compound Objects on 4800-Triangle Landscape", ADD_COMPOUND, "simpleFlatLandOneMetreTris", 100, 2, "100 compound objects" },
	{ "100 Compound Objects on 79000-Triangle Landscape", ADD_COMPOUND, "defaultFlatLandOneMetreTris", 100, 2, "100 compound objects" },
	{ "30 Ragdolls on 4800-Triangle Landscape", ADD_RAGDOLL, "simpleFlatLandOneMetreTris", 30, 2, "30 ragdolls" },
	{ "100 Ragdolls on 79000-Triangle Landscape", ADD_RAGDOLL, "defaultFlatLandOneMetreTris", 100, 2, "100 ragdolls" },
};

ObjectsOnLandscapeDemo::ObjectsOnLandscapeDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	m_bootstrapIterations = 200;

	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	const ObjectsOnLandscapeVariant& variant =  g_variants[m_variantId];

	// Increase the stack size.
	m_oldStack = hkThreadMemory::getInstance().getStack();
	m_stackBuffer = hkAllocate<char>( MY_STACK_SIZE, HK_MEMORY_CLASS_DEMO );
	hkThreadMemory::getInstance().setStackArea( m_stackBuffer, MY_STACK_SIZE );

	//
	// Create the ground using the landscape repository
	//
	{
		int landscapeIndex = LandscapeRepository::getInstance().getLandscapeIndexByName(variant.m_landscapeType);
		m_landscapeContainer = LandscapeRepository::getInstance().getLandscape(landscapeIndex);

		hkpRigidBodyCinfo groundInfo;
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_shape = m_landscapeContainer->m_shape;

		hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);

		m_world->addEntity(groundbody)->removeReference();
	}

	m_packfileData = HK_NULL;

		// Create a utility for generating positions for objects
	AabbSpawnUtil spawnUtil( m_landscapeContainer->m_spawnVolumes );

	if (variant.m_numObjects > 100)
	{
		spawnUtil.m_allowOverlaps = true;
	}
//	m_env->m_window->getViewport(0)->toggleState(HKG_ENABLED_WIREFRAME);


	switch (variant.m_addObjectType)
	{
	case ADD_CONVEX:
		{
			for ( int i = 0; i < variant.m_numObjects; ++i )
			{
				hkVector4 objectSize(	spawnUtil.m_pseudoRandomGenerator.getRandRange(.5f, 2),
										spawnUtil.m_pseudoRandomGenerator.getRandRange(.5f, 2),
										spawnUtil.m_pseudoRandomGenerator.getRandRange(.5f, 2));

				hkVector4 position; 
				spawnUtil.getNewSpawnPosition( objectSize, position );
				hkReal density = 1000;

				hkpRigidBody* rb = GameUtils::createRandomConvexGeometricFromBox(objectSize, 
																				objectSize(0) * objectSize(1) * objectSize(2) * density, // mass
																				position, 
																				30, // num verts
																				&spawnUtil.m_pseudoRandomGenerator);
				m_world->addEntity(rb);
				rb->removeReference();
			}
			break;
		}
	case ADD_COMPOUND:
		{
			hkString fileName = hkAssetManagementUtil::getFilePath("Resources/Physics/Compoundbodies/compoundbodies.hkx");
			hkIstream infile( fileName.cString() );
			HK_ASSERT( 0x215d080c, infile.isOk() );
			hkPackfileReader* reader = new hkBinaryPackfileReader();
			reader->loadEntireFile(infile.getStreamReader());

			if( hkVersionUtil::updateToCurrentVersion( *reader, hkVersionRegistry::getInstance() ) != HK_SUCCESS )
			{
				HK_ASSERT2(0, 0, "Couldn't update version.");
			}
			m_packfileData = reader->getPackfileData();
			m_packfileData->addReference();

			hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>( reader->getContents( hkRootLevelContainerClass.getName() ) );
			HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );

			hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
			HK_ASSERT2(0xa6451544, physicsData != HK_NULL, "Could not find physics data in root level object" );

			delete reader;

			const hkArray<hkpRigidBody*>& rigidBodies = physicsData->getPhysicsSystems()[0]->getRigidBodies();

			for ( int i = 0; i < variant.m_numObjects; ++i )
			{
				hkpRigidBody* newBody = rigidBodies[i % rigidBodies.getSize()]->clone();
				hkQuaternion rotation; spawnUtil.m_pseudoRandomGenerator.getRandomRotation( rotation );
				newBody->setRotation( rotation );

				hkAabb aabb;  newBody->getCollidable()->getShape()->getAabb(newBody->getTransform(), .05f, aabb );
				hkVector4 objectSize; objectSize.setSub4( aabb.m_max, aabb.m_min );
				hkVector4 position; spawnUtil.getNewSpawnPosition( objectSize, position );
				
				newBody->setPosition( position );

				m_world->addEntity( newBody );
				newBody->removeReference();
			}

			break;
		}
	case ADD_RAGDOLL:
		{

			//
			// Create a group filter to remove collisions between ragdoll bones
			//
			{
				hkpGroupFilter* filter = new hkpGroupFilter();
				hkpGroupFilterSetup::setupGroupFilter( filter );
				m_world->setCollisionFilter( filter );
				filter->removeReference();
			}



			const hkReal height = 2.0f;
			hkVector4 objectSize(height, height, height);

			for ( int i = 0; i < variant.m_numObjects; ++i )
			{
				hkVector4 position; 
				spawnUtil.getNewSpawnPosition( objectSize, position );

				hkQuaternion rotation; rotation.setIdentity();

				GameUtils::RagdollCinfo ragdollInfo;

				ragdollInfo.m_position = position;
				ragdollInfo.m_height = height;
				spawnUtil.m_pseudoRandomGenerator.getRandomRotation(ragdollInfo.m_rotation);
				ragdollInfo.m_systemNumber = i + 1;
				ragdollInfo.m_boneShapeType = GameUtils::RPT_CAPSULE;
				ragdollInfo.m_numVertebrae = 1;
				ragdollInfo.m_ragdollInterBoneCollisions = GameUtils::DISABLE_ONLY_ADJOINING_BONE_COLLISIONS;
				ragdollInfo.m_wantHandsAndFeet = GameUtils::WANT_HANDS_AND_FEET;

				hkpPhysicsSystem* ragdoll = GameUtils::createRagdoll( ragdollInfo );

				const hkArray<hkpRigidBody*>& rigidbodies = ragdoll->getRigidBodies();
				for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
				{
					hkpRigidBody* body = rigidbodies[iRB];
					body->setLinearDamping( 0.1f );
					body->getMaterial().setFriction(0.31f);				
				}

				m_world->addPhysicsSystem(ragdoll);
				ragdoll->removeReference();
				
			}
			break;
		}
	}

	//
	// Setup the camera
	//
	{
		hkVector4 up  (   0.0f,   1.0f, 0.0f );
		setupDefaultCameras( env, m_landscapeContainer->m_cameraFrom, m_landscapeContainer->m_cameraTo, up );
		
		setupGraphics();
	}

	m_world->unlock();
}


ObjectsOnLandscapeDemo::~ObjectsOnLandscapeDemo()
{
	// Delete the world before removing reference to the packfile data
	if(m_world)
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	if (m_packfileData)
		m_packfileData->removeReference();

	hkDeallocate( m_stackBuffer );
	hkThreadMemory::getInstance().setStackArea( m_oldStack.m_current, m_oldStack.getFreeBytes() );
}

hkDemo::Result ObjectsOnLandscapeDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Objects falling on a landscape";
HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ObjectsOnLandscapeDemo, HK_DEMO_TYPE_PHYSICS|HK_DEMO_TYPE_SERIALIZE|HK_DEMO_TYPE_STATS, ObjectsOnLandscapeVariant, g_variants, helpString ); 

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

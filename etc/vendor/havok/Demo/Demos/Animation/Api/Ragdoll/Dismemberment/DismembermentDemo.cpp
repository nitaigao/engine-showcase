/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/Animation/Api/Ragdoll/Dismemberment/DismembermentDemo.h>

// Common
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Rig
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

// Animation
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Skin/hkxSkinBinding.h>

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaRagdollRigidBodyController.h>
#include <Animation/Ragdoll/Controller/PoweredConstraint/hkaRagdollPoweredConstraintController.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>

// Graphics & Window Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Light/hkgLightManager.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayConvex.h>



struct DismembermentDemoVariant
{
	bool m_powerDisembodiedLimbs;
	const char* m_name;
	const char* m_details;
};

static const char helpString[] =
"\220 Dismember a random limb\n"
"\221 Toggle draw mode";

static const DismembermentDemoVariant g_variants[] =
{
	{ false, "Dismemberment - Lifeless Limbs", helpString },
	{ true, "Dismemberment - Powered Limbs", helpString }
};


DismembermentDemo::DismembermentDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env) 
{
	m_drawRagdoll = false;
	m_drawSkin = true;
	m_drawSkeleton = false;
	m_randomizer = new hkPseudoRandomGenerator( 10 );

	// Setup the camera
	{
		hkVector4 from( 1.0f, 5.0f, 1.0f );
		hkVector4 to  ( 0.0f, 0.0f, -1.0f );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	// Create physical world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_gravity.set( 0.0f, 0.0f, -10.0f, 0.0f );
		info.m_collisionTolerance = 0.05f;
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_DO_NOTHING;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	// Create the floor
	for ( int i = 0; i < 4; i++ )
	{
		// Data specific to this shape.
		hkVector4 halfExtents( 5.0f, 5.0f, 0.1f );

		hkpBoxShape* shape = new hkpBoxShape( halfExtents, 0 );

		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set( 0.0f, 0.0f, -0.92f );
		rigidBodyInfo.m_rotation.setIdentity();
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;

		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		shape->removeReference();
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}

	// Load the data
	m_loader = new hkLoader();

	// setup layer collision
	{
		// Replace filter
		hkpGroupFilter* groupFilter = new hkpGroupFilter();

		// We disable collisions between different layers to determine what behaviour we want
		groupFilter->disableCollisionsBetween(LAYER_RAGDOLL_KEYFRAMED, LAYER_LANDSCAPE);

		m_world->setCollisionFilter( groupFilter, true);
		groupFilter->removeReference();
	}

	// load ragdoll rig, create ragdoll instance
	{
		// penetration_rig.hkx - highRes and lowRes  skeletons, ragdoll, mappers
		hkString assetFile = hkAssetManagementUtil::getFilePath( "Resources/Animation/Ragdoll/dismemberment_ragdoll.hkx" );
		hkRootLevelContainer* dataContainerRig = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, dataContainerRig != HK_NULL , "Could not load data asset");

		m_originalRagdoll = reinterpret_cast<hkaRagdollInstance*>( dataContainerRig->findObjectByType( hkaRagdollInstanceClass.getName() ));
		HK_ASSERT2(0, m_originalRagdoll, "Couldn't load ragdoll setup");

		// Assign the ragdoll skeleton
		m_ragdollSkeleton = m_originalRagdoll->getSkeleton();

		// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
		// This makes both ragdoll controllers lees sensitive to angular effects and hence more effective
		const hkArray<hkpConstraintInstance*>& constraints = m_originalRagdoll->getConstraintArray();
		hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), m_originalRagdoll->getRigidBodyOfBone(0) );

		// Add the ragdoll to the world
		m_originalRagdoll->addToWorld( m_world, true );

		// Find the two mappings
		{
			// Iterate over all hkaSkeletonMapper objects
			void* objectFound = dataContainerRig->findObjectByType( hkaSkeletonMapperClass.getName() );
			while ( objectFound )
			{
				hkaSkeletonMapper* mapper = reinterpret_cast<hkaSkeletonMapper*>( objectFound );

				// Use the skeleton to determine which mapper is which
				// Assign the animation skeleton

				if ( mapper->m_mapping.m_skeletonA == m_ragdollSkeleton )
				{
					m_animationFromRagdoll = mapper;
					m_animationSkeleton = mapper->m_mapping.m_skeletonB;
				}

				if ( mapper->m_mapping.m_skeletonB == m_ragdollSkeleton )
				{
					m_ragdollFromAnimation = mapper;
					m_animationSkeleton = mapper->m_mapping.m_skeletonA;
				}


				// Find the next object of this type
				objectFound = dataContainerRig->findObjectByType( hkaSkeletonMapperClass.getName(), objectFound );
			}
			
			HK_ASSERT2( 0, m_animationFromRagdoll, "Couldn't load high-to-ragdoll mapping" );
			HK_ASSERT2( 0, m_ragdollFromAnimation, "Couldn't load ragdoll-to-high mapping" );
		}
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRunLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");

		m_binding = ac->m_bindings[0];
	}

	// Create the animated character
	{
		m_character = new hkaAnimatedSkeleton( m_animationSkeleton );
		m_control = new hkaDefaultAnimationControl( m_binding, true );
		
		m_character->addAnimationControl( m_control );
	}

	// setup the graphics
	setupGraphics();

	// init pose as necessary
	{
		m_worldFromModel.setIdentity();
		m_worldFromModel.m_rotation.set( 0,0,1,0 );
	}
	
	// display ragdoll transparent
	for ( int i = 0; i < m_originalRagdoll->getNumBones(); i++)
	{
		hkpRigidBody* rb = m_originalRagdoll->getRigidBodyOfBone(i);

		if ( rb != HK_NULL )
		{
			HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars( 0, 128, 0, 128 ));
		}
	}

	// Sample the animation to find the starting pose
	{
		hkaPose animationPose( m_animationSkeleton );
		hkaPose ragdollPose( m_ragdollSkeleton );

		// Sample the character's current animated pose
		m_character->sampleAndCombineAnimations( animationPose.accessUnsyncedPoseLocalSpace().begin(), HK_NULL );

		// Convert the animation pose to a ragdoll pose
		m_ragdollFromAnimation->mapPose( animationPose.getSyncedPoseModelSpace().begin(), m_ragdollSkeleton->m_referencePose, ragdollPose.accessUnsyncedPoseModelSpace().begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Set the pose of the ragdoll
		m_originalRagdoll->setPoseModelSpace( ragdollPose.getSyncedPoseModelSpace().begin(), m_worldFromModel );
	}

	// Shallow copy the ragdoll for convenience and create the controller
	{
		hkaRagdollInstance* ragdoll = hkaRagdollUtils::copyRagdollInstanceShallow( m_originalRagdoll );

		DismembermentDemoRagdollInstanceAndController* riac = new DismembermentDemoRagdollInstanceAndController( ragdoll, m_originalRagdoll, DismembermentDemoRagdollInstanceAndController::KEYFRAMED );

		m_ragdolls.pushBack( riac );

		ragdoll->removeReference();
	}

	// Convert the skins
	{
		const char* skinFile = "Resources/Animation/Ragdoll/dismemberment_skin.hkx";

		hkString assetFile = hkAssetManagementUtil::getFilePath( skinFile );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
		HK_ASSERT2(0x27343435, scene , "No scene loaded");

		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkins > 0), "No skins loaded");

		m_numSkinBindings = ac->m_numSkins;
		m_skinBindings = ac->m_skins;

		// Make graphics output buffers for the skins
		env->m_sceneConverter->convert( scene );

		// Map skins with ragdoll bones.  Since meshes do not have names, we look to the scene.
		for ( int boneIndex = 0; boneIndex < m_ragdollSkeleton->m_numBones; boneIndex++ )
		{
			// Find the rigid body corresponding to this bone
			hkpRigidBody* body = m_ragdolls.back()->getRagdollInstance()->getRigidBodyOfBone( boneIndex );

			if ( body == HK_NULL )
			{
				continue;
			}

			// Find the name of the bone
			hkString boneName = m_ragdollSkeleton->m_bones[ boneIndex ]->m_name;

			// Remove the prefix of the bone name
			hkString meshName = boneName.replace( "Ragdoll_HavokBipedRig ", "" );

			// Find a node in the scene with this name
			hkxNode* meshNode = scene->findNodeByName( meshName.cString() );

			HK_ASSERT2( 0, meshNode != HK_NULL, "Mesh not found" );
			HK_ASSERT2( 0, hkString::strCmp( meshNode->m_object.m_class->getName(), hkxSkinBindingClass.getName() ) == 0, "Mesh not found" );

			// Cast the node to a skin binding
			hkxSkinBinding* skinBinding = static_cast< hkxSkinBinding* >( meshNode->m_object.m_object );

			// Find the mesh binding that corresponds to this mesh
			int bindingIndex = 0;
			while ( bindingIndex < m_numSkinBindings )
			{
				// Test for a match
				if ( m_skinBindings[ bindingIndex ]->m_mesh == skinBinding->m_mesh )
				{
					break;
				}

				// Try the next binding
				bindingIndex++;
			}

			HK_ASSERT2( 0, bindingIndex < m_numSkinBindings, "Binding not found" );

			// Associate this skin binding with the corresponding ragdoll bone
			m_skinFromRigidBody.insert( body, m_skinBindings[ bindingIndex ] );
		}
	}

	m_world->unlock();

	// Disable warning about reposing the ragdoll.
	hkError::getInstance().setEnabled(0x71C72FE7, false);
}

DismembermentDemo::~DismembermentDemo()
{
	// Delete and remaining DismembermentDemoRagdollInstanceAndController instances
	while ( m_ragdolls.getSize() )
	{
		delete m_ragdolls.back();
		m_ragdolls.popBack();
	}

	m_world->lock();

	// remove ragdoll from world
	if ( m_originalRagdoll->getWorld() )
	{
		m_originalRagdoll->removeFromWorld();
	}

	// The ragdoll bodies are loaded from a packfile will not have their destructors called (the memory is just dumped)
	// Because we change from keyframed to dynamic we need to make sure the dtor is called to destroy the motion
	for ( int i = 0; i < m_originalRagdoll->m_rigidBodies.getSize(); i++ )
	{
		hkpRigidBody* rb = m_originalRagdoll->m_rigidBodies[ i ];

		// Explictly call to the destructor
		if ( rb != HK_NULL )
		{
			rb->~hkpRigidBody();
		}

	}

	// before we wipe the loader
	m_world->unlock();
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	delete m_randomizer;

	delete m_character;
	delete m_control;

	delete m_loader;
}

hkDemo::Result DismembermentDemo::stepDemo()
{
	hkgAabb shadowAabb;
	m_env->m_displayWorld->getShadowWorldBounds(shadowAabb, *m_env->m_window->getCurrentViewport()->getCamera() );
    hkDefaultDemo::setupFixedShadowFrustum(m_env, *m_env->m_displayWorld->getLightManager()->getLight(0), shadowAabb );

	// Sample the animation and drive the ragdolls to the desired pose
	hkaPose animationPose( m_animationSkeleton );
	hkaPose ragdollPose( m_ragdollSkeleton );
	{

		// Sample the character's current animated pose
		m_character->sampleAndCombineAnimations( animationPose.accessUnsyncedPoseLocalSpace().begin(), HK_NULL );

		// Convert the animation pose to a ragdoll pose
		m_ragdollFromAnimation->mapPose( animationPose.getSyncedPoseModelSpace().begin(), m_ragdollSkeleton->m_referencePose, ragdollPose.accessUnsyncedPoseModelSpace().begin(), hkaSkeletonMapper::CURRENT_POSE );

		m_world->lock();

		// Drive each ragdoll to the desired pose
		for ( int i = 0; i < m_ragdolls.getSize(); i++ )
		{
			m_ragdolls[ i ]->driveToPose( ragdollPose.getSyncedPoseLocalSpace().begin(), m_worldFromModel, m_timestep );
		}

		m_world->unlock();
	}


	// Handle button presses...

	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		if ( m_drawSkin )
		{
			m_drawSkin = false;
			m_drawRagdoll = true;
			m_drawSkeleton = false;
		}
		else if ( m_drawRagdoll )
		{
			m_drawSkin = false;
			m_drawRagdoll = false;
			m_drawSkeleton = true;
		}
		else
		{
			m_drawSkin = true;
			m_drawRagdoll = false;
			m_drawSkeleton = false;
		}
	}

	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2 ) )
	{
	}

	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_3 ) )
	{
	}

	// Test if the user has pressed the dismember button
	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_0 ) )
	{
		// Test if the ragdoll is to be split or reassembled
		if ( m_ragdolls.back()->getType() == DismembermentDemoRagdollInstanceAndController::KEYFRAMED )
		{
			// Split the ragdoll

			m_world->lock();
			
			int splitBone = 0;

			// Find the split bone
			{
				int randval = static_cast< unsigned int >( m_randomizer->getRand32() ) % m_ragdolls.back()->getRagdollInstance()->m_rigidBodies.getSize();

				// Find the bone corresponding to the requested rigid body
				splitBone = m_ragdolls.back()->getRagdollInstance()->getBoneIndexOfRigidBody( m_ragdolls.back()->getRagdollInstance()->m_rigidBodies[ randval ] );
			}

			// Split the ragdoll into the trunk and a branch
			if ( splitBone != 0 )
			{			
				hkaRagdollInstance* trunk;
				hkaRagdollInstance* branch;

				hkaRagdollUtils::splitRagdollInstanceShallow( m_ragdolls.back()->getRagdollInstance(), splitBone, trunk, branch );

				m_ragdolls.back()->getRagdollInstance()->getConstraintOfBone( splitBone )->disable();

				// Remvove the unsplit DismembermentDemoRagdollInstanceAndController
				delete m_ragdolls.back();
				m_ragdolls.popBack();

				// Determine the type of controller to use based on the demo variant
				DismembermentDemoRagdollInstanceAndController::ControllerType branchType = g_variants[ m_variantId ].m_powerDisembodiedLimbs ? DismembermentDemoRagdollInstanceAndController::POWERED : DismembermentDemoRagdollInstanceAndController::NONE;

				// Add the split DismembermentDemoRagdollInstanceAndController's to the list
				m_ragdolls.pushBack( new DismembermentDemoRagdollInstanceAndController( branch, m_originalRagdoll, branchType ) );
				m_ragdolls.pushBack( new DismembermentDemoRagdollInstanceAndController( trunk, m_originalRagdoll, DismembermentDemoRagdollInstanceAndController::KEYFRAMED ) );

				// These are now owned by the DismembermentDemoRagdollInstanceAndController
				branch->removeReference();
				trunk->removeReference();
			}
			else
			{
				// Splitting on the zeroth bone is interpreted as transitioning from keyframed to dynamic for this demo

				hkaRagdollInstance* copy = hkaRagdollUtils::copyRagdollInstanceShallow( m_ragdolls.back()->getRagdollInstance() );

				// Remvove the old DismembermentDemoRagdollInstanceAndController
				delete m_ragdolls.back();
				m_ragdolls.popBack();

				// Determine the type of controller to use based on the demo variant
				DismembermentDemoRagdollInstanceAndController::ControllerType branchType = g_variants[ m_variantId ].m_powerDisembodiedLimbs ? DismembermentDemoRagdollInstanceAndController::POWERED : DismembermentDemoRagdollInstanceAndController::NONE;

				// Add the split DismembermentDemoRagdollInstanceAndController's to the list
				m_ragdolls.pushBack( new DismembermentDemoRagdollInstanceAndController( copy, m_originalRagdoll, branchType ) );

				// These are now owned by the DismembermentDemoRagdollInstanceAndController
				copy->removeReference();
			}

			m_world->unlock();
		}
		else
		{
			// Put the ragdoll back together

			m_world->lock();

			// Delete the existing ragdolls
			while ( m_ragdolls.getSize() )
			{
				delete m_ragdolls.back();
				m_ragdolls.popBack();
			}

			// Enable all constraints
			for ( int i = 0; i < m_originalRagdoll->m_constraints.getSize(); i++ )
			{
				m_originalRagdoll->m_constraints[ i ]->enable();
			}

			// Reset the pose of the ragdoll
			m_originalRagdoll->setPoseModelSpace( ragdollPose.getSyncedPoseModelSpace().begin(), m_worldFromModel );

			// Create a copy of the original ragdoll
			hkaRagdollInstance* copy = hkaRagdollUtils::copyRagdollInstanceShallow( m_originalRagdoll );

			// Create a new keyframed controller
			DismembermentDemoRagdollInstanceAndController* riac = new DismembermentDemoRagdollInstanceAndController( copy, m_originalRagdoll, DismembermentDemoRagdollInstanceAndController::KEYFRAMED );

			// Push it into the list of ragdolls
			m_ragdolls.pushBack( riac );

			// This now owned by the DismembermentDemoRagdollInstanceAndController
			copy->removeReference();

			m_world->unlock();
		}
	}

	// Set the ragdoll color
	{
		int color = m_drawRagdoll ? hkColor::rgbFromChars( 0, 128, 0, 128 ) : 0;

		m_world->lock();
		
		for ( int i = 0; i < m_originalRagdoll->getNumBones(); i++)
		{
			hkpRigidBody* rb = m_originalRagdoll->getRigidBodyOfBone(i);

			if ( rb != HK_NULL )
			{
				HK_SET_OBJECT_COLOR( (hkUlong)rb->getCollidable(), color );
			}
		}

		m_world->unlock();
	}

	// Draw the skeletons and skins
	for ( int ragdoll = 0; ragdoll < m_ragdolls.getSize(); ragdoll++ )
	{
		hkLocalArray< hkQsTransform > pose( m_ragdollSkeleton->m_numBones );
		pose.setSize( m_ragdollSkeleton->m_numBones );

		const hkQsTransform* inputWS = g_variants[ m_variantId ].m_powerDisembodiedLimbs ? ragdollPose.getSyncedPoseLocalSpace().begin() : m_ragdollSkeleton->m_referencePose;

		m_ragdolls[ ragdoll ]->getPoseWorldSpace( inputWS, pose.begin() );
		
		// Draw the pose
		if ( m_drawSkeleton )
		{
			AnimationUtils::drawPoseModelSpace( *m_ragdollSkeleton, pose.begin(), hkQsTransform::getIdentity(), 0x400080FF );
		}

		hkLocalArray< hkQsTransform > animationPoseMS( m_animationSkeleton->m_numBones );
		animationPoseMS.setSize( m_animationSkeleton->m_numBones );

		/// SKINNING

		// Map the pose to hi res
		if ( m_ragdolls[ ragdoll ]->getType() != DismembermentDemoRagdollInstanceAndController::KEYFRAMED )
		{
			m_animationFromRagdoll->mapPose( pose.begin(), m_animationSkeleton->m_referencePose, animationPoseMS.begin(), hkaSkeletonMapper::REFERENCE_POSE );
		}
		else
		{
			for ( int i = 0; i < m_animationSkeleton->m_numBones; i++ )
			{
				animationPoseMS[ i ].setMul( m_worldFromModel, animationPose.getSyncedPoseModelSpace()[ i ] );
			}
		}

		const int boneCount = m_animationSkeleton->m_numBones;

		if ( !m_drawSkin )
		{
			hkQsTransform offset( hkQsTransform::IDENTITY );
			offset.m_translation.set( 0.0f, 0.0f, -3.0f );

			for ( int i = 0; i < boneCount; i++ )
			{
				hkQsTransform tmp = animationPoseMS[ i ];
				animationPoseMS[ i ].setMul( offset, tmp );
			}
		}

		// Construct the composite world transform
		hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
		compositeWorldInverse.setSize( boneCount );

		const hkaRagdollInstance* ragdollInstance = m_ragdolls[ ragdoll ]->getRagdollInstance();

		// Skins in this demo are associated with rigid bodies.  For each rigid body...
		for ( int bodyIndex = 0; bodyIndex < ragdollInstance->m_rigidBodies.getSize(); bodyIndex++ )
		{
			// Lookup the mesh in the map
			hkaMeshBinding* mesh = m_skinFromRigidBody.getWithDefault( ragdollInstance->m_rigidBodies[ bodyIndex ], HK_NULL );

			// assumes either a straight map (null map) or a single one (1 palette)
			hkInt16* usedBones = mesh->m_mappings? mesh->m_mappings[0].m_mapping : HK_NULL;
			int numUsedBones = usedBones? mesh->m_mappings[0].m_numMapping : m_animationSkeleton->m_numBones;

			// Multiply through by the bind pose inverse world inverse matrices
			for (int p=0; p < numUsedBones; p++)
			{
				int boneIndex = usedBones? usedBones[p] : p;
				compositeWorldInverse[p].setMul( animationPoseMS[ boneIndex ], mesh->m_boneFromSkinMeshTransforms[ boneIndex ] );
			}

			AnimationUtils::skinMesh( *mesh->m_mesh, hkTransform::getIdentity(), compositeWorldInverse.begin(), *m_env->m_sceneConverter );
		}
	}


	// Step the animation
	m_character->stepDeltaTime( m_timestep );

	// Step the physics
	hkDefaultPhysicsDemo::stepDemo();

	return DEMO_OK;	// original animation demo return
}

DismembermentDemoRagdollInstanceAndController::DismembermentDemoRagdollInstanceAndController( hkaRagdollInstance* ragdollInstance, const hkaRagdollInstance* originalRagdollInstance, ControllerType type ):
	m_ragdollInstance( ragdollInstance ),
	m_originalRagdollInstance( originalRagdollInstance ),
	m_type( type )
{
	// Create a new keyframed rigid body controller if necessary
	m_ragdollRigidBodyController = ( m_type == KEYFRAMED ) ? new hkaRagdollRigidBodyController( ragdollInstance) : HK_NULL;

	// Mark all bones keyframed or dynamic according to the type of this instance
	for ( int i = 0; i < m_ragdollInstance->getNumBones(); i++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);

		// Initialize with quality type and collision filter
		if (rb != HK_NULL)
		{
			if ( type == KEYFRAMED )
			{
				setBodyKeyframed( rb );
			}
			else
			{
				setBodyDynamic( rb );
			}
		}
	}

	// Start the motors if this instance is powered
	if ( type == POWERED )
	{
		hkaRagdollPoweredConstraintController::startMotors( m_ragdollInstance );
	}

	m_ragdollInstance->addReference();
}

DismembermentDemoRagdollInstanceAndController::~DismembermentDemoRagdollInstanceAndController()
{
	if ( m_type == POWERED )
	{
		hkaRagdollPoweredConstraintController::stopMotors( m_ragdollInstance );
	}

	delete m_ragdollRigidBodyController;

	m_ragdollInstance->removeReference();
}

void DismembermentDemoRagdollInstanceAndController::driveToPose( const hkQsTransform* poseLocalSpace, hkQsTransform& worldFromModel, hkReal step )
{
	switch ( m_type )
	{
	// Rigid body controller
	case KEYFRAMED:
		m_ragdollRigidBodyController->driveToPose( step, poseLocalSpace, worldFromModel, HK_NULL );
		break;

	// Powered controller
	case POWERED:
		hkaRagdollPoweredConstraintController::driveToPose( m_ragdollInstance, poseLocalSpace );
		break;

	// None, other
	default:
		break;
	}
}

DismembermentDemoRagdollInstanceAndController::ControllerType DismembermentDemoRagdollInstanceAndController::getType() const
{
	return m_type;
}

int DismembermentDemoRagdollInstanceAndController::getRagdollRootBone() const
{
	// Find the bone corresponding to the first rigid body
	int bone = m_ragdollInstance->getBoneIndexOfRigidBody( m_ragdollInstance->m_rigidBodies[ 0 ] );

	return bone;
}

void DismembermentDemoRagdollInstanceAndController::getTransformOfRootRigidBody( hkQsTransform& xform ) const
{
	xform.m_translation = m_ragdollInstance->m_rigidBodies[ 0 ]->getPosition();
	xform.m_rotation = m_ragdollInstance->m_rigidBodies[ 0 ]->getRotation();
	xform.m_scale.setAll( 1.0f );
}

hkUint32 DismembermentDemoRagdollInstanceAndController::calcFilterInfo( hkpRigidBody* body ) const
{
	// Find the corresponding bone in the original ragdoll
	const int originalBone = m_originalRagdollInstance->getBoneIndexOfRigidBody( body );
	HK_ASSERT2( 0, originalBone >= 0, "Mismatch of ragdoll and bone" );
	const int originalParent = findNearestParentWithRigidBody( originalBone, m_originalRagdollInstance );

	// Find the corresponding bone in the current ragdoll
	const int currentBone = m_ragdollInstance->getBoneIndexOfRigidBody( body );
	HK_ASSERT2( 0, currentBone >= 0, "Mismatch of ragdoll and bone" );
	const int currentParent = findNearestParentWithRigidBody( currentBone, m_ragdollInstance );

	// Use the original bone ID.
	// If this is the root of a new ragdoll, no collision filtering for the parent
	const int bone = originalBone;
	const int parent = ( currentParent >= 0 ) ? originalParent : -1;

	hkUint32 filter = hkpGroupFilter::calcFilterInfo( DismembermentDemo::LAYER_RAGDOLL_KEYFRAMED, 1, bone+1, parent+1 );

	return filter;
}


// Given a rigid body (a ragdoll bone), make it dynamic, enable collisions with all bones except its parent,
// and set the layer and quality properties accordingly. No correction impulse!
void DismembermentDemoRagdollInstanceAndController::setBodyDynamic( hkpRigidBody* body )
{
	// The main idea is the use the bone IDs of the original
	// ragdoll as a way of collision filtering.  Each rigidBody
	// should collide with all other bones, except it's parent.
	// After splitting, many bones will not have parents.

	const hkUint32 filter = calcFilterInfo( body );

	body->setMotionType( hkpMotion::MOTION_DYNAMIC );
	body->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );
	body->setCollisionFilterInfo( filter );

	if ( body->getWorld() != HK_NULL )
	{
		body->getWorld()->updateCollisionFilterOnEntity( body, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
	}
}

// Given a rigid body (a ragdoll bone), makes it keyframed
void DismembermentDemoRagdollInstanceAndController::setBodyKeyframed( hkpRigidBody* body )
{
	const hkUint32 filter = calcFilterInfo( body );

	body->setMotionType(hkpMotion::MOTION_KEYFRAMED );
	body->setQualityType( HK_COLLIDABLE_QUALITY_KEYFRAMED );
	body->setCollisionFilterInfo( filter );

	if ( body->getWorld() != HK_NULL )
	{
		body->getWorld()->updateCollisionFilterOnEntity( body, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
	}
}

int DismembermentDemoRagdollInstanceAndController::findNearestParentWithRigidBody( int bone, const hkaRagdollInstance* ragdoll )
{
	// Given a bone and a ragdoll, find a parent bone that has a
	// rigid body.  If there are non rigid body bones, we must
	// iterate.
	int parent = bone;
	do
	{
		// Try the parent of the current bone
		parent = ragdoll->m_skeleton->m_parentIndices[ parent ];
	}
	while ( ( parent >= 0 ) && ( ragdoll->getRigidBodyOfBone( parent ) == HK_NULL ) );

	return parent;
}

void DismembermentDemoRagdollInstanceAndController::getPoseWorldSpace( const hkQsTransform* localSpaceIn, hkQsTransform* worldSpaceOut ) const
{
	// Similar to hkaRagdollInstance::getPoseWorldSpace, but fills
	// unmapped bones with the specified transforms, not the
	// reference pose

	// Find the first mapped bone
	int branch = 0;
	while ( m_ragdollInstance->getRigidBodyOfBone( branch ) == HK_NULL )
	{
		branch++;

		HK_ASSERT2( 0, branch < m_ragdollInstance->getNumBones(), "No rigid bodies found in this ragdoll" );
	}

	// Find the total transform to the first mapped bone
	hkQsTransform worldFromBranch;
	worldFromBranch.setFromTransformNoScale( m_ragdollInstance->getRigidBodyOfBone( branch )->getTransform() );
	hkaSkeletonUtils::getModelSpaceScale( *m_ragdollInstance->m_skeleton, m_ragdollInstance->m_skeleton->m_referencePose, branch, worldFromBranch.m_scale );

	hkQsTransform modelFromBranch( hkQsTransform::IDENTITY );

	int bone = branch;

	// For each bone except the root
	while ( bone >= 0 )
	{
		// Concatenate the transforms
		hkQsTransform tmp = modelFromBranch;
		modelFromBranch.setMul( localSpaceIn[ bone ], tmp );

		// Move up the chain
		bone = m_ragdollInstance->m_skeleton->m_parentIndices[ bone ];
	}

	hkQsTransform worldFromModel;
	worldFromModel.setMulMulInverse( worldFromBranch, modelFromBranch );

	m_ragdollInstance->getPoseWorldSpace( worldSpaceOut, worldFromModel, localSpaceIn );
}

const hkaRagdollInstance* DismembermentDemoRagdollInstanceAndController::getRagdollInstance() const
{
	return m_ragdollInstance;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char description[] = "This demo shows a ragdoll character being dismembered.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( DismembermentDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, DismembermentDemoVariant, g_variants, description );

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

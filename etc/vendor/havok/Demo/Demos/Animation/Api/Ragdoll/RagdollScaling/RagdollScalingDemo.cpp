/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/Animation/Api/Ragdoll/RagdollScaling/RagdollScalingDemo.h>

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
#include <Animation/Animation/Playback/Utilities/hkaSampleAndCombineUtils.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperUtils.h>

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

#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/hkpConstraintMotor.h>
#include <Physics/Dynamics/Constraint/Motor/Velocity/hkpVelocityConstraintMotor.h>
#include <Physics/Dynamics/Constraint/Motor/SpringDamper/hkpSpringDamperConstraintMotor.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

static const char helpString[] =
"Hold  \224/\225 to scale the character continuously\n"
"Press \226/\227 to scale the character in steps";

static const char* ASSET_ANIMATION = "Resources/Animation/HavokGirl/hkRunLoop.hkx";
static const char* ASSET_RAGDOLL = "Resources/Animation/Ragdoll/dismemberment_ragdoll.hkx";
static const char* ASSET_SKIN = "Resources/Animation/Ragdoll/dismemberment_skin.hkx";



RagdollScalingDemo::RagdollScalingDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env) 
{
	// Setup the camera
	{
		hkVector4 from( 0.0f, 5.0f, 1.0f );
		hkVector4 to  ( 0.0f, 0.0f, 0.0f );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	// Create physical world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_gravity.set( 0.0f, 0.0f, -0.0f, 0.0f );
		info.m_collisionTolerance = 0.05f;
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_DO_NOTHING;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	// Load the data
	m_loader = new hkLoader();

	// setup layer collision
	{
		// Replace filter
		hkpGroupFilter* groupFilter = new hkpGroupFilter();

		// We disable collisions between different layers to determine what behaviour we want
		groupFilter->disableCollisionsBetween( 2, 1 );

		m_world->setCollisionFilter( groupFilter, true);
		groupFilter->removeReference();
	}

	// load ragdoll rig, create ragdoll instance
	{
		// penetration_rig.hkx - highRes and lowRes  skeletons, ragdoll, mappers
		hkString assetFile = hkAssetManagementUtil::getFilePath( ASSET_RAGDOLL );
		hkRootLevelContainer* dataContainerRig = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, dataContainerRig != HK_NULL , "Could not load data asset");

		m_ragdoll = reinterpret_cast<hkaRagdollInstance*>( dataContainerRig->findObjectByType( hkaRagdollInstanceClass.getName() ) );
		HK_ASSERT2(0, m_ragdoll, "Couldn't load ragdoll setup");

		// Assign the ragdoll skeleton
		m_ragdollSkeleton = m_ragdoll->getSkeleton();

		// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
		// This makes both ragdoll controllers lees sensitive to angular effects and hence more effective
		const hkArray<hkpConstraintInstance*>& constraints = m_ragdoll->getConstraintArray();
		hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), m_ragdoll->getRigidBodyOfBone(0) );

		// Find the two mappings
		{
			// Iterate over all hkaSkeletonMapper objects
			void* objectFound = dataContainerRig->findObjectByType( hkaSkeletonMapperClass.getName() );
			while ( objectFound )
			{
				hkaSkeletonMapper* mapper = reinterpret_cast<hkaSkeletonMapper*>( objectFound );

				// Use the skeleton to determine which mapper is which
				// Assign the animation skeleton

				if ( mapper->m_mapping.m_skeletonA == m_ragdoll->getSkeleton() )
				{
					m_animationFromRagdoll = mapper;
					m_animationSkeleton = mapper->m_mapping.m_skeletonB;
				}

				if ( mapper->m_mapping.m_skeletonB == m_ragdoll->getSkeleton() )
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

		// Set all scale factors initially to 1.0f
		m_scale = 1.0f;

		m_worldFromModel.setIdentity();
		m_worldFromModel.m_rotation.set( 0,0,1,0 );
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( ASSET_ANIMATION );
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0 ), "No binding loaded");

		m_binding = ac->m_bindings[0];
	}

	// Create the animated character
	{
		m_character = new hkaAnimatedSkeleton( m_animationSkeleton );
		m_control = new hkaDefaultAnimationControl( m_binding, true );
	
		m_character->addAnimationControl( m_control );
	}

	// Set the initial pose of the character
	{
		hkaPose animationPose( m_animationSkeleton );
		hkaPose ragdollPose( m_ragdollSkeleton );

		// Sample the character's current animated pose
		m_character->sampleAndCombineAnimations( animationPose.accessUnsyncedPoseLocalSpace().begin(), HK_NULL );

		// Convert the animation pose to a ragdoll pose
		m_ragdollFromAnimation->mapPose( animationPose.getSyncedPoseModelSpace().begin(), m_ragdollSkeleton->m_referencePose, ragdollPose.accessUnsyncedPoseModelSpace().begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Set the pose of the ragdoll
		m_ragdoll->setPoseModelSpace( ragdollPose.getSyncedPoseModelSpace().begin(), m_worldFromModel );

		m_ragdoll->addToWorld( m_world, true );
	}

	// Convert the skins
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath( ASSET_SKIN );
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
	}


	// Create the new controllers
	m_ragdollRigidBodyController = new hkaRagdollRigidBodyController( m_ragdoll );
	
	// setup the graphics
	setupGraphics();

	m_world->unlock();

	// Disable warning about reposing the ragdoll.
	hkError::getInstance().setEnabled(0x71C72FE7, false);
}

RagdollScalingDemo::~RagdollScalingDemo()
{
	m_world->lock();

	// remove ragdoll from world
	if ( m_ragdoll->getWorld() )
	{
		m_ragdoll->removeFromWorld();
	}
	
	// The ragdoll bodies are loaded from a packfile will not have their destructors called (the memory is just dumped)
	// Because we change from keyframed to dynamic we need to make sure the dtor is called to destroy the motion
	for ( int i = 0; i < m_ragdoll->m_rigidBodies.getSize(); i++ )
	{
		hkpRigidBody* rb = m_ragdoll->m_rigidBodies[ i ];

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

 	delete m_ragdollRigidBodyController;
	delete m_character;
	delete m_control;

	delete m_loader;
}

hkDemo::Result RagdollScalingDemo::stepDemo()
{
	// Handle button presses...
	hkReal& oldScale = m_scale;
	hkReal newScale = oldScale;
	static const hkReal slowFactor = hkMath::pow( 2.0f, 1.0f / 100.0f );
	static const hkReal fastFactor = hkMath::pow( 2.0f, 1.0f / 10.0f );

	if ( m_env->m_gamePad->isButtonPressed( HKG_PAD_DPAD_UP ) )
	{
		newScale *= slowFactor;
	}

	if ( m_env->m_gamePad->isButtonPressed( HKG_PAD_DPAD_DOWN ) )
	{
		newScale /= slowFactor;
	}

	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_RIGHT ) )
	{
		newScale *= fastFactor;
	}

	if ( m_env->m_gamePad->wasButtonPressed( HKG_PAD_DPAD_LEFT ) )
	{
		newScale /= fastFactor;
	}

	// Test if the user has requested scaling
	if ( newScale != oldScale )
	{
		const hkReal scale = newScale / oldScale;

		m_world->lock();

		// Use the ragdoll scaling utility
		m_ragdoll->addReference();
		m_ragdoll->removeFromWorld();
		hkaRagdollUtils::scaleRagdoll( m_ragdoll, scale );
		m_ragdoll->addToWorld( m_world, false );
		m_ragdoll->removeReference();

		m_world->unlock();
		
		// Scale the skeleton mapper
		hkaSkeletonMapperUtils::scaleMapping( *m_ragdollFromAnimation, scale );
		hkaSkeletonMapperUtils::scaleMapping( *m_animationFromRagdoll, scale );

		// Scale the reference poses of the animated and ragdoll characters
		hkaSampleAndCombineUtils::scaleTranslations( scale, m_ragdollSkeleton->m_referencePose, m_ragdollSkeleton->m_numBones );
		hkaSampleAndCombineUtils::scaleTranslations( scale, m_animationSkeleton->m_referencePose, m_animationSkeleton->m_numBones );

		// Update the scale value
		oldScale = newScale;
	}


	{
		// Sample the animation and drive the ragdolls to the desired pose
		hkaPose animationPose( m_animationSkeleton );
		hkaPose ragdollPose( m_ragdollSkeleton );
		hkaPose upsampledPose( m_animationSkeleton );
		
		// Sample the character's current animated pose
		m_character->sampleAndCombineAnimations( animationPose.accessUnsyncedPoseLocalSpace().begin(), HK_NULL );

		// Scale the bone lengths of the poses
		hkaSampleAndCombineUtils::scaleTranslations( m_scale, ragdollPose.accessUnsyncedPoseLocalSpace().begin(), m_ragdollSkeleton->m_numBones );
		hkaSampleAndCombineUtils::scaleTranslations( m_scale, animationPose.accessUnsyncedPoseLocalSpace().begin(), m_animationSkeleton->m_numBones );

		// Scale the root (necessary for skinning)
		animationPose.accessUnsyncedPoseLocalSpace()[ 0 ].m_scale.mul4( m_scale );

		// Convert the animation pose to a ragdoll pose
		m_ragdollFromAnimation->mapPose( animationPose.getSyncedPoseModelSpace().begin(), m_ragdollSkeleton->m_referencePose, ragdollPose.accessUnsyncedPoseModelSpace().begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Map the ragdoll pose back to an animation pose
		m_animationFromRagdoll->mapPose( ragdollPose.getSyncedPoseModelSpace().begin(), m_animationSkeleton->m_referencePose, upsampledPose.accessUnsyncedPoseModelSpace().begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Copy the scales back into the upsampled pose
		hkaRagdollUtils::copyScales( upsampledPose.accessUnsyncedPoseModelSpace().begin(), animationPose.getSyncedPoseModelSpace().begin(), m_animationSkeleton->m_numBones );

		// Draw the upsampled Pose
		{
			hkQsTransform animWorldFromModel( m_worldFromModel );
			animWorldFromModel.m_translation( 0 ) = -1.0f;
			
			AnimationUtils::drawPose( upsampledPose, animWorldFromModel );
		}

		// Skin the animation
		{
			hkTransform worldFromModel( m_worldFromModel.m_rotation, m_worldFromModel.m_translation );
			worldFromModel.getTranslation()( 0 ) = 1.0f;
			
			const hkQsTransform* poseModelSpace = upsampledPose.getSyncedPoseModelSpace().begin();
			
			// Construct the composite world transform
			hkLocalArray<hkTransform> compositeWorldInverse( m_animationSkeleton->m_numBones );
			compositeWorldInverse.setSize( m_animationSkeleton->m_numBones );

			for ( int i = 0; i < m_numSkinBindings; i++ )
			{
				// assumes either a straight map (null map) or a single one (1 palette)
				hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
				int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : m_animationSkeleton->m_numBones;

				// Multiply through by the bind pose inverse world inverse matrices
				for (int p=0; p < numUsedBones; p++)
				{
					int boneIndex = usedBones? usedBones[p] : p;
					compositeWorldInverse[p].setMul( poseModelSpace[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
				}

				AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, worldFromModel, compositeWorldInverse.begin(), *m_env->m_sceneConverter );
			}
		}

		

		m_world->lock();

		m_ragdollRigidBodyController->driveToPose( m_timestep, ragdollPose.accessSyncedPoseLocalSpace().begin(), m_worldFromModel, HK_NULL );

		m_world->unlock();

		// Step the animation
		m_character->stepDeltaTime( m_timestep );
	}


	// Print the current scale factor
	{
		char buf[ 255 ];
		hkString::sprintf(buf, "Scale: %.3f", m_scale );
		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText( buf, 20, h-40, 0xffffffff, 1);
	}



	// Step the physics
	hkDefaultPhysicsDemo::stepDemo();

	return DEMO_OK;	// original animation demo return
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char description[] = "ROUS: Ragdolls of unusual size.";

HK_DECLARE_DEMO( RagdollScalingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, description, helpString );

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

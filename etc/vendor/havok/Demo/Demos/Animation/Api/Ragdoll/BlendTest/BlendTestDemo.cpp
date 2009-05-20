/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/Animation/Api/Ragdoll/BlendTest/BlendTestDemo.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Controller/PoweredConstraint/hkaRagdollPoweredConstraintController.h>
#include <Animation/Ragdoll/Controller/PoweredChain/hkaRagdollPoweredChainController.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaRagdollRigidBodyController.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

// Graphics Stuff
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Common/Visualize/hkDebugDisplay.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperUtils.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>
#include <Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

// Chain Includes
#include <Physics/Dynamics/Constraint/Chain/Powered/hkpPoweredChainData.h>
#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

#include <Physics/Dynamics/Constraint/Chain/HingeLimits/hkpHingeLimitsData.h>
#include <Physics/Dynamics/Constraint/Chain/RagdollLimits/hkpRagdollLimitsData.h>

#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyCentreOfMassViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

#include <Physics/Utilities/Constraint/Chain/hkpPoweredChainMapper.h>
#include <Physics/Utilities/Constraint/Chain/hkpPoweredChainMapperUtil.h>


inline void stripPrefix( hkString& strInOut )
{
	// Replace spaces by underscores.
	strInOut = strInOut.replace(' ','_');

	// Remove skeleton prefix.
	// This the text before the first underscore.
	const int endPrefix = strInOut.indexOf('_',0);

	if (endPrefix!=-1)
	{
		strInOut = strInOut.substr(endPrefix+1);
	}
}

// This is the string comparison we use to match names from the animation skeleton
// to names in the ragdoll skeleton. The filter just extracts the bone name,
// from either, so we will use the same filter for both skeletons.
// For example, it will match "Rig Pelvis" to "Ragdoll_Pelvis".
static int HK_CALL hkCompareWithoutPrefix( const char* str1, const char* str2 )
{
	hkString string1(str1);
	stripPrefix(string1);

	hkString string2(str2);
	stripPrefix(string2);

	return string1.compareToIgnoreCase( string2 );
}


struct BlendTestVariant
{
	const char*	 m_name;
	int    m_iterations;
	bool   m_fixRoot;
	bool   m_useKeyFrameUtility;
	bool   m_useConstraintChains;
	bool   m_addLimitConstraints;

	hkReal	m_motorTau;
	hkReal	m_motorMaxForce;
	const char* m_details;
};

static const char helpString[] = "A blending demo";

static const BlendTestVariant g_variants[] =
{
	{ "A0  Joints  4 iterations soft",				4, false, false, false, false, 0.2f, 100.0f, helpString },
	{ "A1  Joints  8 iterations soft",				8, false, false, false, false, 0.15f,100.0f, helpString },
	{ "A2  Joints 16 iterations soft",			   16, false, false, false, false, 0.1f, 100.0f, helpString },
	{ "A3  Joints  4 iterations hard",				4, false, false, false, false, 0.8f, 1000.0f, helpString },
	{ "A4  Joints  8 iterations hard",				8, false, false, false, false, 0.8f, 1000.0f, helpString },
	{ "A5  Joints 16 iterations hard",			   16, false, false, false, false, 0.8f, 1000.0f, helpString },
	{ "A6  Keyframe ",							  	4, false, true, false, false, 0.0f,  00.0f, helpString },
	{ "A7  Constraint Chains",					 	4, false, false, true, false, 0.6f,  1000.0f, helpString },
	{ "A8  Constraint Chains + Limits",			 	4, false, false, true, true,  0.6f,  1000.0f, helpString },
	{ "B0  Joints  4 iterations fixed soft",		4, true, false, false, false, 0.2f,  100.0f, helpString },
	{ "B1  Joints  8 iterations fixed soft",		8, true, false, false, false, 0.15f, 100.0f, helpString },
	{ "B2  Joints 16 iterations fixed soft",	   16, true, false, false, false, 0.2f,  100.0f, helpString },
	{ "B3  Joints  4 iterations fixed hard",		4, true, false, false, false, 0.8f, 500.0f, helpString },
	{ "B4  Joints  8 iterations fixed hard",		8, true, false, false, false, 0.8f, 500.0f, helpString },
	{ "B5  Joints 16 iterations fixed hard",       16, true, false, false, false, 0.8f, 500.0f, helpString },
	{ "B6  Keyframe fixed",				        	4, true, true, false,  false, 0.0f,   00.0f, helpString },
	{ "B7  Constraint Chains",				        4, true, false, true,  false, 0.6f,   1000.0f, helpString },
	{ "B8  Constraint Chains + Limits",		        4, true, false, true,  true,  0.6f,   1000.0f, helpString },
};

//void extractChainEntities(hkArray<hkpEntity*>& leftChainEntities, hkArray<hkpEntity*>& rightChainEntities, hkpPhysicsData* physicsData)
//{
//	const int numEntitiesLeftChain = 9;
//	const char* leftChainNames[numEntitiesLeftChain] = 
//	{
//		"Ragdoll L Hand",
//		"Ragdoll L ForeArm",
//		"Ragdoll L UpperArm",
//		"Ragdoll Spine2",
//		"Ragdoll Spine",
//		"Ragdoll Pelvis",
//		"Ragdoll L Thigh",
//		"Ragdoll L Calf",
//		"Ragdoll L Foot",
//	};
//
//	const int numEntitiesRightChain = 9;	
//	const char* rightChainNames[numEntitiesRightChain] = 
//	{
//		"Ragdoll R Hand",
//		"Ragdoll R ForeArm",
//		"Ragdoll R UpperArm",
//		"Ragdoll Spine2",
//		"Ragdoll Spine",
//		"Ragdoll Pelvis",
//		"Ragdoll R Thigh",
//		"Ragdoll R Calf",
//		"Ragdoll R Foot",
//	};
//
//
//	for (int a = 0; a < numEntitiesLeftChain; a++)
//	{
//		hkpRigidBody* limb = HK_NULL;
//		limb = physicsData->findRigidBodyByName(leftChainNames[a]);
//		if (limb)
//		{
//			leftChainEntities.pushBack(static_cast<hkpEntity*>(limb));
//		}
//		else
//		{
//			HK_ASSERT(0xab000004, 0);
//		}
//	}
//
//	for (int a = 0; a < numEntitiesRightChain; a++)
//	{
//		hkpRigidBody* limb = HK_NULL;
//		limb = physicsData->findRigidBodyByName(rightChainNames[a]);
//		if (limb)
//		{
//			rightChainEntities.pushBack(static_cast<hkpEntity*>(limb));
//		}
//		else
//		{
//			HK_ASSERT(0xab000005, 0);
//		}
//	}
//}





BlendTestDemo::BlendTestDemo(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env)
{
	// Disable warnings:
	hkError::getInstance().setEnabled(0xad67888e, false); //'Function not implemented.'

	m_motor = HK_NULL;
	m_otherMotor = HK_NULL;
	m_mapper = HK_NULL;

	m_frameCounter = 0;
	const BlendTestVariant& variant =  g_variants[ this->getDemoVariant() ];

	// Create the World.
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0,0,-10);
		info.setupSolverInfo( info.SOLVER_TYPE_4ITERS_MEDIUM );
		info.m_solverIterations = variant.m_iterations;
		info.m_enableDeactivation = false;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();	
	}

	// Replace filter.
	hkpGroupFilter* filter  = new hkpGroupFilter();
	m_world->setCollisionFilter( filter, true);
	filter->removeReference();

	// Set up the camera.
	{
		hkVector4 from( -2,-2,2 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	// Load Animations.
	{
		m_loader = new hkLoader();

		hkaSkeleton* animationSkeleton;

		// Get the rig.
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
			animationSkeleton = ac->m_skeletons[0];
		}

		// Create the animated skeleton for the animation.
		m_animatedSkeleton = new hkaAnimatedSkeleton( animationSkeleton );

		// Run Animation.
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRunLoop.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			hkaAnimationBinding* runBinding = ac->m_bindings[0];

			m_control[0] = new hkaDefaultAnimationControl (runBinding);
			m_control[0]->setMasterWeight(1.0f);
			m_control[0]->setPlaybackSpeed(1.0f);
			m_control[0]->setEaseInCurve(0, 0.3f, 0.6f, 1);	// Smooth
			m_control[0]->setEaseOutCurve(1, .6f, .3f, 0);	// Smooth
			m_animatedSkeleton->addAnimationControl( m_control[0] );
			m_control[0]->removeReference();
		}
	}

	int ragdollGroup = filter->getNewSystemGroup();

	// Create the floor
	{
		// Data specific to this shape.
		hkVector4 halfExtents(5.0f, 5.0f, 1.0f);

		hkpBoxShape* shape = new hkpBoxShape( halfExtents, 0 );

		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, -1.8f);
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
		if ( variant.m_fixRoot )
		{
			rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, ragdollGroup );
		}

		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		shape->removeReference();
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}

	// Create a moving box
	{
		// Data specific to this shape.
		hkVector4 halfExtents(0.5f, 0.5f, 0.3f);

		hkpBoxShape* shape = new hkpBoxShape( halfExtents, 0 );

		// To illustrate using the shape, create a rigid body by first defining a template.
		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 3.0f, 0.0f);
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( shape, 20.0f, rigidBodyInfo );

		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		shape->removeReference();
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();
	}

	//
	// Load the ragdoll
	//

	hkpPhysicsData* physicsData;
	hkpPhysicsSystem* physicsSystem;
	// load world
	{
		hkString filename = hkAssetManagementUtil::getFilePath( "Resources/Animation/Ragdoll/hires_ragdoll.hkx" );

		hkIstream infile( filename.cString() );
		HK_ASSERT( 0x215d080c, infile.isOk() );

		physicsData = hkpHavokSnapshot::load( infile.getStreamReader(), &m_loadedData );
		physicsSystem = physicsData->getPhysicsSystems()[0];
	}

	// List of limitConstraints (used when the old constraints are overwritten with chains);
	hkArray<hkpConstraintInstance*> limitConstraints;

	{

		hkArray<hkpConstraintInstance*> newConstraints;

		// Load the ragdoll setup
		{
			hkArray<hkpRigidBody*> rigidBodies; 
			{
				rigidBodies = physicsSystem->getRigidBodies();
			}

			m_oldData.setSize( rigidBodies.getSize() );

			m_otherMotor = new hkpPositionConstraintMotor();
			{
				m_otherMotor->m_tau = variant.m_motorTau;
				m_otherMotor->m_maxForce = variant.m_motorMaxForce;
				m_otherMotor->m_proportionalRecoveryVelocity = 5.0f;
				m_otherMotor->m_constantRecoveryVelocity = 0.2f;
			}

			const int numConstraints = physicsSystem->getConstraints().getSize();

			for (int cit=0; cit< numConstraints; cit++)
			{
				hkpConstraintInstance* powered = hkpConstraintUtils::convertToPowered(physicsSystem->getConstraints()[cit], m_otherMotor, true);
				newConstraints.pushBack(powered);
			}

			// Replace old constraints and add new ones.
			{
				for (int cit=0; cit<numConstraints;cit++)
				{
					physicsSystem->removeConstraint(0);
				}
				for (int cit2=0; cit2<numConstraints;cit2++)
				{
					physicsSystem->addConstraint(newConstraints[cit2]);
				}
			}
			
		
			hkaRagdollUtils::reorderAndAlignForRagdoll(rigidBodies, newConstraints);


			// Create constraint chains and a mapper. This can be done right after the constraints are properly ordered.
			if (variant.m_useConstraintChains)
			{
				hkpPoweredChainMapper::Config config;
				config.m_createLimitConstraints = variant.m_addLimitConstraints;
				hkArray<hkpPoweredChainMapper::ChainEndpoints> pairs;

				hkpRigidBody* leftHand = physicsData->findRigidBodyByName("Ragdoll L Hand"); HK_ASSERT(0xab000000, leftHand);
				hkpRigidBody* leftFoot = physicsData->findRigidBodyByName("Ragdoll L Foot"); HK_ASSERT(0xab000001, leftFoot);
				hkpRigidBody* rightHand = physicsData->findRigidBodyByName("Ragdoll R Hand"); HK_ASSERT(0xab000002, rightHand);
				hkpRigidBody* rightFoot = physicsData->findRigidBodyByName("Ragdoll R Foot"); HK_ASSERT(0xab000003, rightFoot);
				hkpRigidBody* headBone = physicsData->findRigidBodyByName("Ragdoll Head"); HK_ASSERT(0xab000003, headBone);
				hkpRigidBody* pelvisBone = physicsData->findRigidBodyByName("Ragdoll Pelvis"); HK_ASSERT(0xab000003, pelvisBone);

				pairs.setSize(3);
				pairs[0].m_start = leftHand;
				pairs[0].m_end   = rightFoot;
				pairs[1].m_start = rightHand;
				pairs[1].m_end   = leftFoot;
				pairs[2].m_start = headBone;
				pairs[2].m_end   = pelvisBone;

				hkArray<hkpConstraintInstance*> unusedConstraints;

				m_mapper = hkpPoweredChainMapper::buildChainMapper(config, newConstraints, pairs, &unusedConstraints);

//				int seeUnusedConsraints = 0;

			}


			m_ragdollSkeleton = hkaRagdollUtils::constructSkeletonForRagdoll(rigidBodies, newConstraints);

			{
				hkpRigidBody* testRigidBody = rigidBodies[0];
				const int systemGroup = hkpGroupFilter::getSystemGroupFromFilterInfo(testRigidBody->getCollidable()->getCollisionFilterInfo());
				hkpGroupFilterUtil::disableCollisionsBetweenConstraintBodies( newConstraints.begin(), numConstraints, systemGroup);
			}

			m_ragdollInstance = new hkaRagdollInstance(rigidBodies, newConstraints, m_ragdollSkeleton);

			// remove contraint references in local array
			for( int curConstraint = 0; curConstraint < newConstraints.getSize(); curConstraint++ )
			{
				newConstraints[curConstraint]->removeReference();
			}

			// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
			// This makes both ragdoll controllers lees sensitive to angular effects and hence more effective
			const hkArray<hkpConstraintInstance*>& constraints = m_ragdollInstance->getConstraintArray();
			hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), m_ragdollInstance->getRigidBodyOfBone(0) );
		}

		// Add ragdoll to the world.
		// Lock the pelvis.
		if ( variant.m_fixRoot )
		{
			m_ragdollInstance->getRigidBodyOfBone(0)->setMotionType(hkpMotion::MOTION_KEYFRAMED);
		}
		for (hkInt16 i=0; i< m_ragdollInstance->getNumBones(); i++)
		{
			m_ragdollInstance->getRigidBodyOfBone(i)->setCollisionFilterInfo( filter->calcFilterInfo(1, ragdollGroup ) );
			m_ragdollInstance->getRigidBodyOfBone(i)->setAngularDamping(0.0f);
		}

		m_ragdollInstance->addToWorld(m_world, true);

		// Construct Powered Chain Constraint Man using Adrian's code
		if (variant.m_useConstraintChains && "replace original motors")
		{
			m_motor = new hkpPositionConstraintMotor();
			m_motor->m_tau = 0.8f;
			m_motor->m_damping = 1.0f;
			m_motor->m_maxForce = variant.m_motorMaxForce;
			m_motor->m_minForce = -variant.m_motorMaxForce;
			m_motor->m_constantRecoveryVelocity = 0.5f;
			m_motor->m_proportionalRecoveryVelocity = 1.0f;

			//for (all motors) set motors (m_motor)
			for (int l = 0; l < m_mapper->getNumLinks(); l++)
			{
				m_mapper->setMotors(l, 0, m_motor);
				m_mapper->setMotors(l, 1, m_motor);
				m_mapper->setMotors(l, 2, m_motor);
			}
		
		}


		if (variant.m_useConstraintChains)
		{

			//
			// Add ragdoll here (including limit constraints), and remove all previous constraints.
			//

			hkpPoweredChainMapperUtil::addToWorld(m_world, m_mapper);



			// remove constraints from the existing system
			{
				for (int c = 0; c < newConstraints.getSize(); c++)
				{
					if (newConstraints[c]->getOwner())
					{
						m_world->removeConstraint( newConstraints[c] );
					}
				}
			}
		}
	}


	// Mapping between both skeletons.
	{
		// Lock the translation of most bones of the animations, except some.
		{
			const hkaSkeleton* animationSkeleton = m_animatedSkeleton->getSkeleton();

			// Locks all translations, except root.
			hkaSkeletonUtils::lockTranslations(*animationSkeleton, true);

			// Unlocks translation for pelvis and both thighs (triangle pelvis).
			const hkInt16 pelvis = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll Pelvis", hkCompareWithoutPrefix );//, &stringFilter);
			animationSkeleton->m_bones[pelvis]->m_lockTranslation = false;

			const hkInt16 lthigh = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll L_Thigh", hkCompareWithoutPrefix );//, &stringFilter);
			animationSkeleton->m_bones[lthigh]->m_lockTranslation = false;

			const hkInt16 rthigh = hkaSkeletonUtils::findBoneWithName( *animationSkeleton, "Ragdoll R_Thigh", hkCompareWithoutPrefix );//, &stringFilter);
			animationSkeleton->m_bones[rthigh]->m_lockTranslation = false;
		}

		// Create mappers.
		{
			hkaSkeletonMapperData high_low_data;
			hkaSkeletonMapperData low_high_data;
			{
				hkaSkeletonMapperUtils::Params params;

				params.m_skeletonA = m_animatedSkeleton->getSkeleton();
				params.m_skeletonB = m_ragdollSkeleton;
				params.m_compareNames = hkCompareWithoutPrefix;

				// Explicit mappings.
				// Re-parent the forearms twists to the fore arm.
				{
					hkaSkeletonMapperUtils::UserMapping mapping;
					mapping.m_boneIn = "HavokBipedRig L_ForeArm";
					mapping.m_boneOut = "Ragdoll L_ForeTwist";
					params.m_userMappingsBtoA.pushBack(mapping);
					mapping.m_boneIn = "HavokBipedRig R_ForeArm";
					mapping.m_boneOut = "Ragdoll R_ForeTwist";
					params.m_userMappingsBtoA.pushBack(mapping);
				}

				params.m_autodetectChains = false;

				hkaSkeletonMapperUtils::createMapping( params, high_low_data, low_high_data );
			}

			m_highToLowMapper = new hkaSkeletonMapper(high_low_data);
			m_lowToHighMapper = new hkaSkeletonMapper(low_high_data);
		}
	}

	m_playing = true;

	m_world->unlock();
}

BlendTestDemo::~BlendTestDemo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled(0xad67888e, true);

	m_world->lock();

	if (m_motor)
	{
		m_motor->removeReference();
	}
	if (m_otherMotor)
	{
		m_otherMotor->removeReference();
	}
	if (m_mapper)
	{
		m_mapper->removeReference();
	}

	m_lowToHighMapper->removeReference();
	m_highToLowMapper->removeReference();
	m_animatedSkeleton->removeReference();
	m_ragdollInstance->removeReference();

	hkaRagdollUtils::destroySkeleton(m_ragdollSkeleton);

	m_world->unlock();
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	m_loadedData->removeReference();

	delete m_loader;
}


hkDemo::Result BlendTestDemo::stepDemo()
{
	m_world->lock();

	if (m_motor)
	{
		if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP) || m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN) )
		{
			if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP) )
			{
				m_motor->m_maxForce *= 1.1f;
				m_otherMotor->m_maxForce = m_motor->m_maxForce;
			}
			else
			{
				m_motor->m_maxForce /= 1.1f;
				m_otherMotor->m_maxForce = m_motor->m_maxForce;
			}
		}

		hkString str; str.printf("Current torque limit: %f   Use up/down arrows to modify.", m_motor ? m_motor->m_maxForce : 0.0f );
		m_env->m_textDisplay->outputText(str, 10, 10);
	}


	const BlendTestVariant& variant =  g_variants[ this->getDemoVariant() ];

	// Ease in or out the ragdoll controller.
	{
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1))
		{
			m_playing = !m_playing;
		}
	}

	// Advance the animation.
	if (m_playing)
	{
		m_animatedSkeleton->stepDeltaTime( m_timestep );
	}

	// Create a runtime pose for the animation.
	hkaPose animPose(m_animatedSkeleton->getSkeleton());

	// Sample animation.
	m_animatedSkeleton->sampleAndCombineAnimations( animPose.accessUnsyncedPoseLocalSpace().begin(), animPose.getFloatSlotValues().begin() );

	// Draw the original animation.
	AnimationUtils::drawPose( animPose, hkQsTransform::getIdentity() );

	// Create a runtime pose for the ragdoll.
	hkaPose ragdollPose(m_ragdollInstance->getSkeleton());
	ragdollPose.setToReferencePose();

	// Map from animation to physics.
	m_highToLowMapper->mapPose( animPose, ragdollPose, hkaSkeletonMapper::CURRENT_POSE );

	// Set the weight.
	const hkaSkeleton* rSkel = m_ragdollInstance->getSkeleton();
	hkLocalBuffer<hkQsTransform> ragdollModelSpace( rSkel->m_numBones );

	// Drive the ragdoll to that pose.
	if (!variant.m_useKeyFrameUtility)
	{
		hkaSkeletonUtils::transformLocalPoseToModelPose( rSkel->m_numBones, rSkel->m_parentIndices, rSkel->m_referencePose, ragdollModelSpace.begin() );

		// Map the pose from the animation (high) to ragdoll (low).
		m_highToLowMapper->mapPose( animPose.getSyncedPoseModelSpace().begin(), rSkel->m_referencePose, ragdollModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

		// Drive the powered constraints to this ragdoll pose using a controller.
		{
			hkLocalBuffer<hkQsTransform> ragdollLocalSpace( rSkel->m_numBones );
			hkaSkeletonUtils::transformModelPoseToLocalPose( rSkel->m_numBones, rSkel->m_parentIndices, ragdollModelSpace.begin(), ragdollLocalSpace.begin() );

			if (g_variants[m_env->m_variantId].m_useConstraintChains)
			{
				hkaRagdollPoweredChainController::startMotors( m_ragdollInstance, m_mapper );
				hkaRagdollPoweredChainController::driveToPose( m_ragdollInstance, m_mapper, ragdollLocalSpace.begin() );
			}
			else
			{
				hkaRagdollPoweredConstraintController::startMotors( m_ragdollInstance );
				hkaRagdollPoweredConstraintController::driveToPose( m_ragdollInstance, ragdollLocalSpace.begin() );
			}
		}

		// Keyframe the root bone.
		if ( m_ragdollInstance->getRigidBodyOfBone(0)->getMotionType() == hkpMotion::MOTION_KEYFRAMED )
		{
			m_world->lock();
			const hkVector4& nextPos = ragdollPose.getSyncedPoseModelSpace()[0].m_translation;
			hkQuaternion nextOrient = ragdollPose.getSyncedPoseModelSpace()[0].m_rotation;
			hkpKeyFrameUtility::applyHardKeyFrame( nextPos, nextOrient, 1.0f /m_timestep, m_ragdollInstance->getRigidBodyOfBone(0) );
			m_world->unlock();
		}
	}
	else
	{
		hkaKeyFrameHierarchyUtility::ControlData controlData;
		{
			if ( m_ragdollInstance->getRigidBodyOfBone(0)->getMotionType() == hkpMotion::MOTION_KEYFRAMED )
			{
				controlData.m_hierarchyGain = 0.6f;
				controlData.m_velocityDamping = 0.0f;
				controlData.m_accelerationGain = 1.0f;
				controlData.m_velocityGain = 0.3f;

				controlData.m_positionGain = 0.08f;
				controlData.m_positionMaxLinearVelocity = 0.2f;
				controlData.m_positionMaxAngularVelocity = 0.5f;

				controlData.m_snapGain = 0.2f;
				controlData.m_snapMaxLinearVelocity  = 0.3f;
				controlData.m_snapMaxAngularVelocity = 0.3f;
				controlData.m_snapMaxLinearDistance  = 0.03f;
				controlData.m_snapMaxAngularDistance = 0.1f;
			}
			else
			{
				controlData.m_hierarchyGain = 0.6f;
				controlData.m_velocityDamping = 0.0f;
				controlData.m_accelerationGain = 1.0f;
				controlData.m_velocityGain = 0.2f;
				
				controlData.m_positionGain = 0.1f;
				controlData.m_positionMaxLinearVelocity = 0.2f;
				controlData.m_positionMaxAngularVelocity = 0.3f;
				
				controlData.m_snapGain = 0.1f;
				controlData.m_snapMaxLinearVelocity  = 0.3f;
				controlData.m_snapMaxAngularVelocity = 0.3f;
				controlData.m_snapMaxLinearDistance  = 0.03f;
				controlData.m_snapMaxAngularDistance = 0.1f;
			}
		}

		hkLocalArray< hkaKeyFrameHierarchyUtility::Output > output( m_ragdollInstance->m_rigidBodies.getSize() );
		output.setSize( m_ragdollInstance->m_rigidBodies.getSize() );

		hkaKeyFrameHierarchyUtility::KeyFrameData keyFrameData;
		{
			keyFrameData.m_worldFromRoot.setIdentity();
			keyFrameData.m_desiredPoseLocalSpace = ragdollPose.getSyncedPoseLocalSpace().begin();
			keyFrameData.m_internalReferencePose = m_oldData.begin();
		}
		
		hkaKeyFrameHierarchyUtility::BodyData bodyData;
		{
			bodyData.m_numRigidBodies = m_ragdollInstance->m_rigidBodies.getSize();
			bodyData.m_rigidBodies = m_ragdollInstance->m_rigidBodies.begin();
			bodyData.m_parentIndices = m_ragdollInstance->getSkeleton()->m_parentIndices;
			bodyData.m_controlDataIndices = 0;
		}

		if ( m_frameCounter == 0 )
		{
			hkaKeyFrameHierarchyUtility::initialize( bodyData, m_oldData.begin() );
		}

		hkaKeyFrameHierarchyUtility::applyKeyFrame( m_timestep, keyFrameData, bodyData, &controlData, output.begin() );

		// Show stress.
		{
			for (int i =0; i < bodyData.m_numRigidBodies;i++ )
			{
				int color = 0xff808080;
				hkReal stress = hkMath::sqrt( output[i].m_stressSquared );
				stress = hkMath::max2( stress - 0.5f, 0.0f );
				stress = hkMath::min2( stress * 0.05f, 1.0f );
				int colorOffset = int( stress * 127 );
				color += colorOffset << 16;
				color -= colorOffset << 8;
				color -= colorOffset << 0;
				HK_SET_OBJECT_COLOR( hkUlong( bodyData.m_rigidBodies[i]->getCollidable()), color );
			}
		}

		//hkRagdollBoneController controller( m_ragdollInstance );
		//controller.driveToPose( m_timestep, ragdollPose, hkQsTransform::getIdentity() );
	}

	m_world->unlock();

	hkDefaultPhysicsDemo::stepDemo();

	{
		// Get the pose from the ragdoll after stepping the physics.
		m_ragdollInstance->getPoseModelSpace( ragdollModelSpace.begin(), hkQsTransform::getIdentity() );

		// Map back from ragdoll to animation.
		{
			m_ragdollInstance->getPoseModelSpace( ragdollModelSpace.begin(), hkQsTransform::getIdentity() );

			const hkQsTransform* localBIn  = animPose.getSyncedPoseLocalSpace().begin();
			hkQsTransform* modelBOut = animPose.accessSyncedPoseModelSpace().begin();

			m_lowToHighMapper->mapPose( ragdollModelSpace.begin(), localBIn, modelBOut, hkaSkeletonMapper::CURRENT_POSE );
		}
	}

	// Draw the pose.
	AnimationUtils::drawPose( animPose, hkQsTransform::getIdentity(), 0xffff0000 );
	m_frameCounter++;
	return hkDemo::DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString2[] = "Press 1 to pause animation.\n" ;

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BlendTestDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, BlendTestVariant, g_variants, helpString2 );

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

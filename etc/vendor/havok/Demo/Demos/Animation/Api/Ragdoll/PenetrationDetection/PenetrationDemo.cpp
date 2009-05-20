/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/Animation/Api/Ragdoll/PenetrationDetection/PenetrationDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Rig
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Physics
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Penetration/hkaDetectRagdollPenetration.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

// phantoms
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics & Window Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Math/hkgBounds.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/System/hkgSystem.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayConvex.h>

static const hkVector4 UP(0,0,1);


// The hkaDetectRagdollPenetration requires an interface to perform raycast.
class PenetrationDemoRagdollRaycastInterface : public hkReferencedObject, public hkaRagdollRaycastInterface
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		PenetrationDemoRagdollRaycastInterface (hkpWorld* world) : m_world(world) {}

		virtual hkBool castRay (const hkpAabbPhantom* phantomIn, const hkVector4& fromWS, const hkVector4& toWS, hkVector4& hitPointWS, hkVector4& hitNormalWS );

	private:

		hkpWorld* m_world;

};

hkBool PenetrationDemoRagdollRaycastInterface::castRay ( const hkpAabbPhantom* phantomIn, const hkVector4& fromWS, const hkVector4& toWS, hkVector4& hitPointWS, hkVector4& hitNormalWS )
{
	hkpWorldRayCastInput raycastIn;
	hkpClosestRayHitCollector rayCollector;

	raycastIn.m_from = fromWS;
	raycastIn.m_to = toWS;
	raycastIn.m_filterInfo=hkpGroupFilter::calcFilterInfo( PenetrationDemo::LAYER_RAYCAST,0 );

	// do raycast in Aabb phantom
	rayCollector.reset();
	phantomIn->castRay( raycastIn, rayCollector );

	const hkBool didHit = rayCollector.hasHit();

	if (didHit)
	{
		const hkpWorldRayCastOutput& raycastOut = rayCollector.getHit();

		// calculate hitPoint in world space by interpolation
		hitPointWS.setInterpolate4(raycastIn.m_from, raycastIn.m_to, raycastOut.m_hitFraction);
		// set normal
		hitNormalWS = raycastOut.m_normal;

		return true;
	}
	else
	{
		return false;
	}
}

PenetrationDemo::PenetrationDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_time(0.0f),
	m_statusFlag(false),
	m_poseId(0),
	m_correctionFlag(true),
	m_turn(0.25f*HK_REAL_PI),
	m_pos(0.0f,-0.5f,0.9f)
{


	// graphic setup
	{
		forceShadowState(false);

		hkgViewport* viewport = m_env->m_window->getViewport(0);
		viewport->setDesiredState(viewport->getDesiredState() & ~HKG_ENABLED_CULLFACE);
	}

	// Setup the camera
	{
		hkVector4 from( -1.5f, 1.5f, 1.5f );
		hkVector4 to  ( 1.0f,-2.0f,0.5f );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	// Create physical world
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );  // our world is not very big
		info.m_gravity.setMul4(-10, UP);
		info.m_collisionTolerance = 0.05f;
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_DO_NOTHING;	// we do our own stuff

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

	}

	// Load the data
	m_loader = new hkLoader();

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Penetration/penetration_landscape1.hkx");

		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));

		HK_ASSERT2(0x27343635, scene, "No scene loaded");
		env->m_sceneConverter->convert( scene, hkgAssetConverter::CONVERT_ALL );

		hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
		HK_ASSERT2(0x27343635, physics, "No physics loaded");

		// Physics
		if (physics)
		{
			const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

			// Tie the two together
			for (int i=0; i<psys.getSize(); i++)
			{
				hkpPhysicsSystem* system = psys[i];

				// Change the layer of the rigid bodies
				for (int rb=0; rb < system->getRigidBodies().getSize(); rb++)
				{
					system->getRigidBodies()[rb]->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo( LAYER_LANDSCAPE,0 ));
				}

				// Associate the display and physics (by name)
				if (scene)
				{
					addPrecreatedDisplayObjectsByName( psys[i]->getRigidBodies(), scene );
				}

				// add the lot to the world
				m_world->addPhysicsSystem(system);
			}
		}

	}

	// setup layer collision
	{
		// Replace filter
		hkpGroupFilter* groupFilter = new hkpGroupFilter();

		// We disable collisions between different layers to determine what behaviour we want
		groupFilter->disableCollisionsBetween(LAYER_RAGDOLL_KEYFRAMED, LAYER_LANDSCAPE);
		groupFilter->disableCollisionsBetween(LAYER_RAGDOLL_PENETRATING, LAYER_LANDSCAPE);
		groupFilter->disableCollisionsBetween(LAYER_RAYCAST, LAYER_RAGDOLL_DYNAMIC);
		groupFilter->disableCollisionsBetween(LAYER_RAYCAST, LAYER_RAGDOLL_KEYFRAMED);
		groupFilter->disableCollisionsBetween(LAYER_RAYCAST, LAYER_RAGDOLL_PENETRATING);

		m_world->setCollisionFilter( groupFilter, true);
		groupFilter->removeReference();
	}

	// Two mappers (anim->ragdoll  and  ragdoll->anim)
	hkaSkeletonMapper* ragdollToHighResMapper = HK_NULL;
	hkaSkeletonMapper* highResToRagdollMapper = HK_NULL;

	// High resolution skeleton (poses)
	hkaSkeleton* skeletonHighRes = HK_NULL;


	//load rig, create ragdoll instance and extract poses
	{
		// three files exported from 3DS Max are needed in the demo:
		// penetration_landscape1.hkx or penetrationdemo2.hkx - small and large landscapes
		// penetration_rig.hkx - highRes and lowRes  skeletons, ragdoll, mappers
		// penetration_poses.hkx - animation with available posses
		// data from second file are stored in dataContainerRig
		// data from third file are stored in dataContainerPoses

		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Penetration/penetration_rig.hkx");
		hkRootLevelContainer* dataContainerRig = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, dataContainerRig != HK_NULL , "Could not load data asset");

		m_ragdollInstance = reinterpret_cast<hkaRagdollInstance*>( dataContainerRig->findObjectByType( hkaRagdollInstanceClass.getName() ));
		HK_ASSERT2(0, m_ragdollInstance, "Couldn't load ragdoll setup");

		for ( int i = 0; i < m_ragdollInstance->getNumBones(); i++)
		{
			hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);
			// Initialize with quality type and collision filter
			if (rb != HK_NULL)
			{
				_setBodyKeyframed( rb );
			}
		}

		// This routine iterates through the bodies pointed to by the constraints and stabilizes their inertias.
		// This makes both ragdoll controllers lees sensitive to angular effects and hence more effective
		const hkArray<hkpConstraintInstance*>& constraints = m_ragdollInstance->getConstraintArray();
		hkpInertiaTensorComputer::optimizeInertiasOfConstraintTree( constraints.begin(), constraints.getSize(), m_ragdollInstance->getRigidBodyOfBone(0) );

		// Add the ragdoll to the world
		m_ragdollInstance->addToWorld(m_world, true);

		// Get HighRes skeleton
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( dataContainerRig->findObjectByType( hkaAnimationContainerClass.getName() ));
		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		skeletonHighRes = ac->m_skeletons[0];

		// Find the two mappings
		{
			const hkaSkeleton* ragdollSkeleton = m_ragdollInstance->getSkeleton();
			void *objectFound = dataContainerRig->findObjectByType(hkaSkeletonMapperClass.getName());
			while (objectFound)
			{
				hkaSkeletonMapper* mapperFound = reinterpret_cast<hkaSkeletonMapper*> (objectFound);

				// Use the skeleton to determine which mapper is which
				if (mapperFound->m_mapping.m_skeletonA == ragdollSkeleton)
				{
					ragdollToHighResMapper = mapperFound;
				}
				else
				{
					HK_ASSERT(0,mapperFound->m_mapping.m_skeletonB == ragdollSkeleton);
					highResToRagdollMapper = mapperFound;
				}

				objectFound = dataContainerRig->findObjectByType(hkaSkeletonMapperClass.getName(), objectFound);
			}
			HK_ASSERT2(0, highResToRagdollMapper, "Couldn't load high-to-ragdoll mapping");
			HK_ASSERT2(0, ragdollToHighResMapper, "Couldn't load ragdoll-to-high mapping");
		}

	}

	// load poses from animation in HighRes and map them to LowRes
	{
		// Disable a warning about reposing the ragdoll.  This demo reposes the ragdoll appropriately, though this use case is rare.
		hkError::getInstance().setEnabled( 0x71C72FE7, false );

		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Penetration/penetration_poses.hkx");
		hkRootLevelContainer* dataContainerPoses = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, dataContainerPoses != HK_NULL , "Could not load rig asset");

		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( dataContainerPoses->findObjectByType( hkaAnimationContainerClass.getName() ));

        hkaAnimation* animation =  ac->m_animations[0];
		HK_ASSERT2(0x25586454, animation != HK_NULL , "Could not load animation");


		hkaPose*	poseHighRes = new hkaPose( skeletonHighRes );

		// fullfill array of posses
		for (int p=0; p < NUM_POSES; p++)
		{
			m_posesLowRes[p] = new hkaPose( m_ragdollInstance->getSkeleton() );

        	// Sample HighRes Pose
			hkReal sampleTime = p * (animation->m_duration/(NUM_POSES-1));
	       	animation->sampleTracks( sampleTime , poseHighRes->accessUnsyncedPoseLocalSpace().begin(), HK_NULL, HK_NULL );

			// We use this pose as the in+out pose to the mapper below
			hkArray<hkQsTransform> ragdollArrayModelSpace( m_ragdollInstance->getNumBones() );

			// Map the pose from the animation (highres) to ragdoll (lowres)
			highResToRagdollMapper->mapPose( poseHighRes->getSyncedPoseModelSpace().begin(), m_ragdollInstance->getSkeleton()->m_referencePose, ragdollArrayModelSpace.begin(), hkaSkeletonMapper::CURRENT_POSE );

			m_posesLowRes[p]->setPoseModelSpace(ragdollArrayModelSpace);

		}

		delete poseHighRes;

	}

	// setup the graphics
	setupGraphics();

	// init pose as necessary
	{
		// set rotation
		hkQuaternion q0;
		q0.setAxisAngle( UP , m_turn );

		m_currentTransform.setIdentity();
		m_currentTransform.m_rotation = q0;
		m_currentTransform.m_translation = m_pos;
	}

	// set Aabb from current init pose in world space, so that it encapsulates all bones
	{
		m_ragdollInstance->setPoseModelSpace(m_posesLowRes[m_poseId]->getSyncedPoseModelSpace().begin(), m_currentTransform);

		hkaPose ragdollPose( m_ragdollInstance->getSkeleton() );
		m_ragdollInstance->getPoseWorldSpace( ragdollPose.accessUnsyncedPoseModelSpace().begin() );


		hkAabb ragdollAabb;
		ragdollPose.getModelSpaceAabb(ragdollAabb);

		m_ragdollPhantom = new hkpAabbPhantom( ragdollAabb ,hkpGroupFilter::calcFilterInfo( PenetrationDemo::LAYER_RAYCAST,0 ) );

		m_world->addPhantom(m_ragdollPhantom);

	}

	// create detection object and raycast interface
	{
		// create raycast object
		m_ragdollRaycastInterface = new PenetrationDemoRagdollRaycastInterface(m_world);

		
		// setup DetectRagdollPenetration object
		hkaDetectRagdollPenetration::Setup setup;

		setup.m_ragdollSkeleton = m_ragdollInstance->getSkeleton();
		setup.m_raycastInterface = m_ragdollRaycastInterface;
		setup.m_ragdollPhantom = m_ragdollPhantom;

		m_detectRagdollPenetration = new hkaDetectRagdollPenetration(setup);
		
	}

	// display ragdoll transparent
	{
		for ( int i = 0; i < m_ragdollInstance->getNumBones(); i++)
		{
			hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);

			if ( rb != HK_NULL )
			{
				HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars( 0, 100, 0, 120 ));
			}

		}
	}

	m_world->unlock();
}

PenetrationDemo::~PenetrationDemo()
{

	m_world->lock();

	if (m_detectRagdollPenetration)
	{
	    m_detectRagdollPenetration->removeReference();
	}

	if (m_ragdollRaycastInterface)
	{
		static_cast<PenetrationDemoRagdollRaycastInterface*>(m_ragdollRaycastInterface)->removeReference();
	}

	// delete low res animation poses
	for (int i=0; i < NUM_POSES; i++ )
	{
		if (m_posesLowRes[i])
		{
			delete m_posesLowRes[i];
		}
	}

	if (m_ragdollPhantom->getWorld())
	{
		m_world->removePhantom( m_ragdollPhantom );
		m_ragdollPhantom->removeReference();
	}

	// remove ragdoll from world
	if (m_ragdollInstance->getWorld())
	{
		m_ragdollInstance->removeFromWorld();
	}

	// The ragdoll bodies are loaded from a packfile will not have their destructors called (the memory is just dumped)
	// Because we change from keyframed to dynamic we need to make sure the dtor is called to destroy the motion
	for ( int i = 0; i < m_ragdollInstance->getNumBones(); i++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);

		// Explictly call to the destructor
		if ( rb != HK_NULL)
		{
			rb->~hkpRigidBody();
		}

	}

	// need to delete world (delete refs to bodies about to disapear)
	// before we wipe the loader
	m_world->unlock();
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	delete m_loader;

}

hkDemo::Result PenetrationDemo::stepDemo()
{
	m_world->lock();

	// update local time
	m_time += m_timestep;

	// process control input
	{
		const hkReal incTurn = 0.015f;
		const hkReal incMove = 0.015f;

		const hkgPad* pad = m_env->m_gamePad;

		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_1) )
		{
			m_statusFlag = !m_statusFlag;
		}

		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_2) )
		{
			m_poseId++;
			m_poseId = (m_poseId > (NUM_POSES-1)) ? 0 : m_poseId;
		}

		if ( pad->wasButtonPressed(HKG_PAD_BUTTON_3) )
		{
			m_correctionFlag = !m_correctionFlag;
		}

		if ( pad->isButtonPressed(HKG_PAD_DPAD_RIGHT) )
		{
			m_turn -= incTurn;
		}

		if ( pad->isButtonPressed(HKG_PAD_DPAD_LEFT) )
		{
			m_turn += incTurn;
		}

		if ( pad->isButtonPressed(HKG_PAD_DPAD_DOWN)  && !(pad->isButtonPressed(HKG_PAD_BUTTON_0)) )
		{
			m_pos(0) += cos(m_turn + 0.5f*HK_REAL_PI)*incMove;
			m_pos(1) += sin(m_turn + 0.5f*HK_REAL_PI)*incMove;
		}

		if ( pad->isButtonPressed(HKG_PAD_DPAD_UP) && !(pad->isButtonPressed(HKG_PAD_BUTTON_0)) )
		{
			m_pos(0) -= cos(m_turn + 0.5f*HK_REAL_PI)*incMove;
			m_pos(1) -= sin(m_turn + 0.5f*HK_REAL_PI)*incMove;
		}

		if (pad->isButtonPressed(HKG_PAD_DPAD_DOWN) && pad->isButtonPressed(HKG_PAD_BUTTON_0) )
		{
			m_pos(2) -= incMove;
		}

		if (pad->isButtonPressed(HKG_PAD_DPAD_UP) && pad->isButtonPressed(HKG_PAD_BUTTON_0) )
		{
			m_pos(2) += incMove;
		}

	}

	// move and rotate pose as necessary
	{
		// set rotation
		hkQuaternion q0;
		q0.setAxisAngle( UP , m_turn );

		m_currentTransform.m_rotation = q0;
		m_currentTransform.m_translation = m_pos;
	}

	HK_TIMER_BEGIN("WholePenetration", HK_NULL);

	// drive ragdoll, detect penetration and smoothly
	{
		if (m_statusFlag)
		{

			// detect penetration
			{
				HK_TIMER_BEGIN("DetectPenetration", HK_NULL);

				
				// ModelPose is set directly to WorldPose used for penetration detection
				hkaPose ragdollPose( m_ragdollInstance->getSkeleton() );
				m_ragdollInstance->getPoseWorldSpace( ragdollPose.accessUnsyncedPoseModelSpace().begin() );

				// Method detectPenetration returns true if there is a penetration, or false if there isn't
				hkBool penetrationStatus; penetrationStatus = m_detectRagdollPenetration->detectPenetration( ragdollPose, m_penetratedBones );
				
				HK_TIMER_END();

			}

			// fully dynamics ragdoll
			stopRagdollFeedback();
		}
		else
		{

			// ragdoll driven by keyframed pose
			doRagdollFeedback();

			/// keyframed state, reset bone State
			m_detectRagdollPenetration->resetBoneStates();
		}
	}

	HK_TIMER_END();

	m_world->unlock();

	hkDefaultPhysicsDemo::stepDemo();

	// display detection pose
	displayDetectionPose();

	// display application status
	{
		char strOutStats[4096];
		int xPos = m_env->m_window->getTVDeadZoneH();
		int yPos = m_env->m_window->getTVDeadZoneV();

		char strStatus[16];
		{
			if (m_statusFlag)
				sprintf(strStatus,"%s","dynamic");
			else
				sprintf(strStatus,"%s","keyframed");
		}

		char strCorrection[4];
		{
			if (m_correctionFlag)
				sprintf(strCorrection,"%s","yes");
			else
				sprintf(strCorrection,"%s","no");
		}

		// display status info text after help
		if ( m_time > 15.0f )
		{
			sprintf( strOutStats, " Ragdoll: %s   Pose: %d/%d   Correction: %s ", strStatus ,m_poseId+1, NUM_POSES, strCorrection );
			m_env->m_textDisplay->outputText(strOutStats, xPos+20, yPos+20);
		}

	}


	return DEMO_OK;	// original animation demo return
}

// doRagdollFeedback() drives the rigid bodies that make up the ragdoll keyframed.
void PenetrationDemo::doRagdollFeedback()
{
	// set ragdoll to requested pose
	m_ragdollInstance->setPoseModelSpace(m_posesLowRes[m_poseId]->getSyncedPoseModelSpace().begin(), m_currentTransform);

	// set all bodies to keyframed motion
	for ( int i = 0; i < m_ragdollInstance->getNumBones(); i++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(i);
		// Initialize with quality type and collision filter
		if (rb != HK_NULL)
		{
			_setBodyKeyframed( rb );
		}

	}

}

// stopRagdollFeedback() turns the non-penetrating rigid bodies to fully dynamics, with collisions between them, except parent vs. child.
// The penetrated rigid bodies and their descendants are fully dynamics too, but with disable collision with landscape and correction impulse
// is applied to them!
void PenetrationDemo::stopRagdollFeedback()
{
	
	// get boneStatusArray from hkDetectPenetration object
	const hkArray<hkaDetectRagdollPenetration::BonePenetrationStatus>& boneStatusArray = m_detectRagdollPenetration->getBoneStatusArray();
	

	for ( int b = 0; b < m_ragdollInstance->getNumBones(); b++)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone(b);

		if (rb != HK_NULL)
		{
			const int parentId = m_ragdollInstance->getParentOfBone( b );

			if ( ( boneStatusArray[b] != hkaDetectRagdollPenetration::HK_NOP )  &&  m_correctionFlag )
			{
				if ( boneStatusArray[b] == hkaDetectRagdollPenetration::HK_YESP )
				{
					// apply correction force
					_setBodyDynamicWithCorrection( rb, b, parentId, true );
				}
				else
				{
					// don't apply correction force
					_setBodyDynamicWithCorrection( rb, b, parentId, false );
				}

			}
			else
			{
				_setBodyDynamic( rb, b, parentId );
			}
		}
	}
}

// Given a rigid body (a ragdoll bone), make it dynamic, enable collisions with all bones except its parent,
// and set the layer and quality properties accordingly. If body is penetrated, correction impulse is applied in
// penetration point in direction similar to normal of raycast triangle to move body out of landscape!
void PenetrationDemo::_setBodyDynamicWithCorrection( hkpRigidBody* rb, int boneId, int parentId, bool applyForce )
{

	// don't allow  collision with landscape;
	const hkUint32 newFi = hkpGroupFilter::calcFilterInfo( PenetrationDemo::LAYER_RAGDOLL_PENETRATING,1, boneId+1, parentId+1);

	if (( rb->getMotionType() == hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_MOVING) ||
		( rb->getCollisionFilterInfo() != newFi))
	{

		rb->setMotionType(hkpMotion::MOTION_DYNAMIC);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );
		rb->setCollisionFilterInfo(newFi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}
	}

	// Apply small impulse in collision point in landscape normal collision direction
	{

		hkVector4 hitPoint;  hitPoint.setZero4();
		hkVector4 hitNormal; hitNormal.setZero4();

		if ( applyForce )
		{

			// get hitPoint and hitNormal
			{
				for(int i = 0; i< m_penetratedBones.m_penetrations.getSize(); i++)
				{
					if( boneId ==  m_penetratedBones.m_penetrations[i].m_boneBeginIndex )
					{
						hitPoint = m_penetratedBones.m_penetrations[i].m_penetrationPoint;
						hitNormal = m_penetratedBones.m_penetrations[i].m_penetrationNormal;

					}
				}
			}

            // Impulse = limbmass*(vdesires - vactual*cos(alpha))
			const hkReal desiredVel = 1.5f;
			const hkReal impulseLimit = 15.0f;

			// get point velocity in hit point
			hkVector4 pointVel; rb->getPointVelocity(hitPoint,pointVel);
			hkReal velMag = pointVel.length3();

			hkReal cosAlpha;

			// correct problem - unable normalize very small vector
			if ( velMag > HK_REAL_EPSILON )
			{
				pointVel.normalize3();
				cosAlpha = hitNormal.dot3(pointVel);
			}
			else
			{
				cosAlpha = 0.0f;
			}

			hkReal impulseMag = rb->getMass()*( desiredVel-cosAlpha*velMag );

			impulseMag = (impulseMag > impulseLimit) ? impulseLimit : impulseMag;
			impulseMag = (impulseMag < -impulseLimit) ? -impulseLimit : impulseMag;

			hkVector4 correctionImpulse;
			correctionImpulse.setMul4( impulseMag,hitNormal );

			rb->applyPointImpulse( correctionImpulse,hitPoint );

			// hit point and correctImpulse visualization
			correctionImpulse.mul4(0.1f);
			HK_DISPLAY_STAR( hitPoint, 0.4f, hkColor::CYAN );
			HK_DISPLAY_ARROW( hitPoint,correctionImpulse, hkColor::ORANGE );

		}

	} // end apply small impulse


}

// Given a rigid body (a ragdoll bone), make it dynamic, enable collisions with all bones except its parent,
// and set the layer and quality properties accordingly. No correction impulse!
void PenetrationDemo::_setBodyDynamic(hkpRigidBody* rb, int boneId, int parentId)
{
	const hkUint32 newFi = hkpGroupFilter::calcFilterInfo(PenetrationDemo::LAYER_RAGDOLL_DYNAMIC, 1, boneId+1, parentId+1);

	if (( rb->getMotionType() == hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_MOVING) ||
		( rb->getCollisionFilterInfo() != newFi))
	{
		rb->setMotionType(hkpMotion::MOTION_DYNAMIC);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );
		rb->setCollisionFilterInfo(newFi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}

	}
}
// Given a rigid body (a ragdoll bone), makes it keyframed
void PenetrationDemo::_setBodyKeyframed(hkpRigidBody* rb)
{
	const hkUint32 fi = hkpGroupFilter::calcFilterInfo(PenetrationDemo::LAYER_RAGDOLL_KEYFRAMED, 1, 0, 0);

	if (( rb->getMotionType() != hkpMotion::MOTION_KEYFRAMED ) ||
		( rb->getQualityType() != HK_COLLIDABLE_QUALITY_KEYFRAMED) ||
		( rb->getCollisionFilterInfo() != fi))
	{
		rb->setMotionType(hkpMotion::MOTION_KEYFRAMED);
		rb->setQualityType( HK_COLLIDABLE_QUALITY_KEYFRAMED );
		rb->setCollisionFilterInfo(fi);

		if (rb->getWorld())
		{
			rb->getWorld()->updateCollisionFilterOnEntity( rb, HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}

	}
}

void PenetrationDemo::displayDetectionPose()
{
	const hkInt32 numBones = m_ragdollInstance->getNumBones();

	// get pose in world space
	hkArray<hkQsTransform> ragdollWorldSpaceArray( numBones );
	m_ragdollInstance->getPoseWorldSpace( ragdollWorldSpaceArray.begin() );

	hkaPose thePoseWS ( m_ragdollInstance->getSkeleton() );
	thePoseWS.setPoseModelSpace(ragdollWorldSpaceArray); // ModelPose is set directly to WorldPose

	AnimationUtils::drawPose( thePoseWS, hkQsTransform::getIdentity() , hkColor::GREENYELLOW );

	// Display end-up limbs and show bones indexes
	{
		hkVector4 endPointMS;
		hkVector4 bonePointFrom;
		hkVector4 bonePointTo;

		for (int b = 0; b < numBones; b++)
		{
			// Show bones ids in world space 3D position
			const hkVector4& pos = thePoseWS.getBoneModelSpace(b).getTranslation();
			char buf[3]; sprintf(buf, "%d", b);
			const hkString boneId(buf);
			m_env->m_textDisplay->outputText3D( boneId , pos(0), pos(1), pos(2), 0xffffffff, 1);

		}
	}

}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"PenetrationDemo demonstrates the usage of hkDetectPenetration class and technique how to smoothly change \
 animation driven ragdoll to fully dynamic ragdoll during penetration with landscape! Root bone (zero) has to be out of landscape on the 'correct' side!"\
" Usage: Select active pose (1/6) and correction mode (yes/no), move ragdoll to landscape and switch to dynamic state by pressing \221!\n"\
" Use \226/\227 to turn and \224/\225 to move the ragdoll\n" \
" Press \220 and \224/\225 to change the altitude\n" \
" Press \221 to change the ragdoll state\n" \
" Press \222 to change the active pose\n" \
" Press \223 to change the correction mode";

HK_DECLARE_DEMO(PenetrationDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, " Penetration Demo - Detection and correction of penetrated ragdoll ! ", helpString);

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

#include <Demos/Animation/Api/MotionExtraction/CharacterController/CharacterAnimationDemo.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyWalkState.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyStandState.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyInAirState.h>
#include <Demos/Animation/Api/MotionExtraction/CharacterController/StateMachine/MyStateMachine.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>


#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>

#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>


#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


	// Vertex Deformation
#include <Common/Base/Config/hkConfigSimd.h>
#include <Animation/Animation/Deform/Skinning/Fpu/hkaFPUSkinningDeformer.h>
#include <Animation/Animation/Deform/Skinning/Simd/hkaSimdSkinningDeformer.h>

	// Skeletal Animation
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Motion/Default/hkaDefaultAnimatedReferenceFrame.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

	// Skin Mesh
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>
#include <Common/SceneData/Mesh/hkxMesh.h>

#include <Common/SceneData/Scene/hkxScene.h>


	// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/SceneData/hkgVertexBufferWrapper.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Common/Visualize/hkDebugDisplay.h>

CharacterAnimationDemo::CharacterAnimationDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_up(0.0f, 0.0f, 1.0f)
{

	// Disable warnings: if no renderer									
	if( hkString::strCmp( m_env->m_options->m_renderer, "n" ) == 0 )
	{
		hkError::getInstance().setEnabled(0xf0d1e423, false); //'Could not realize an inplace texture of type PNG.'
	}

	// want to do software skinning always in this demo
	// see HardwareSkinningDemo for how to setup hardware palettes etc
	m_env->m_sceneConverter->setAllowHardwareSkinning(false);


	//
	// Create the World
	//
	{
		{
			hkpWorldCinfo info;
			info.setBroadPhaseWorldSize( 350.0f );
			info.m_gravity.set(0,0,-10);
			info.m_collisionTolerance = 0.1f;
			m_world = new hkpWorld( info );
			m_world->lock();

			hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

			setupGraphics();
		}

		// Load the level
		{
			hkVector4 tkScaleFactor(0.26f, 0.26f, 0.26f);

			//hkVector4 tkScaleFactor(0.18f, 0.18f, 0.18f);
			hkString fullname("Resources/Animation/Tk/CharacterController/level.tk");

			hkpShape* moppShape = GameUtils::loadTK2MOPP(fullname.cString(),tkScaleFactor, -1.0f);
			HK_ASSERT2(0x64232cc0, moppShape,"TK file failed to load to MOPP in GameUtils::loadTK2MOPP.");

			hkpRigidBodyCinfo ci;
			ci.m_shape = moppShape;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 0, 1 );
			ci.m_restitution = 1.0f;

			hkpRigidBody* entity = new hkpRigidBody(ci);
			moppShape->removeReference();
			m_world->addEntity(entity);
			entity->removeReference();
		}
	}


	//
	// Create Controller
	//
	{
		//
		//	Create a character proxy object
		//
		{
			// Construct a shape

			const hkReal totalHeight = 1.7f;
			const hkReal radius = .4f;
			const hkReal capsulePoint = totalHeight*0.5f - radius;

			hkVector4 vertexA(0,0, capsulePoint);
			hkVector4 vertexB(0,0,-capsulePoint);

			// Create a capsule to represent the character standing
			m_standShape = new hkpCapsuleShape(vertexA, vertexB, radius);

			// Construct a Shape Phantom
			m_phantom = new hkpSimpleShapePhantom( m_standShape, hkTransform::getIdentity() );

			// Add the phantom to the world
			m_world->addPhantom(m_phantom);

			// Fade out capsule so we can see character
#ifdef HK_DEBUG
			HK_SET_OBJECT_COLOR((hkUlong)m_phantom->getCollidable(), hkColor::rgbFromChars( 240, 200, 0, 20));
#else
			HK_REMOVE_GEOMETRY((hkUlong)m_phantom->getCollidable());
#endif

			m_phantom->removeReference();

			// Construct a character proxy
			hkpCharacterProxyCinfo cpci;
			cpci.m_position.set(-6, 25, 1);
			//cpci.m_position.set(-5, 20, 1);
			cpci.m_staticFriction = 0.0f;
			cpci.m_dynamicFriction = 1.0f;
			cpci.m_up.setNeg4( m_world->getGravity() );
			cpci.m_up.normalize3();
			cpci.m_userPlanes = 4;
			cpci.m_maxSlope = HK_REAL_PI / 3.f;

			cpci.m_shapePhantom = m_phantom;
			m_characterContext.m_characterProxy = new hkpCharacterProxy( cpci );
		}

		// World (an extra reference by the context)
		m_characterContext.m_world = m_world;
		m_characterContext.m_world->addReference();
	}

	// Animation Stuff
	{
		m_loader = new hkLoader();

		hkaSkeleton* girlSkeleton;
		// Get the rig
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
			girlSkeleton = ac->m_skeletons[0];
		}

		// Create the animated skeleton for the girl
		m_characterContext.m_animatedSkeleton = new hkaAnimatedSkeleton( girlSkeleton );

		// Run Animation
		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRunLoop.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			hkaAnimationBinding* runBinding = ac->m_bindings[0];

			m_characterContext.m_walkControl = new hkaDefaultAnimationControl (runBinding);
			m_characterContext.m_walkControl->setMasterWeight(1.0f);
			m_characterContext.m_walkControl->setPlaybackSpeed(1.0f);
			m_characterContext.m_walkControl->easeOut(0.0f);
			m_characterContext.m_walkControl->setEaseInCurve(0, 0, 1, 1);	// Smooth
			m_characterContext.m_walkControl->setEaseOutCurve(1, 1, 0, 0);	// Smooth
			m_characterContext.m_animatedSkeleton->addAnimationControl(m_characterContext.m_walkControl);
			m_characterContext.m_walkControl->removeReference();
		}

		// Idle Animation
		{

			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");

			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			hkaAnimationBinding* idleBinding = ac->m_bindings[0];

			m_characterContext.m_idleControl = new hkaDefaultAnimationControl (idleBinding);
			m_characterContext.m_idleControl->setMasterWeight(1.0f);
			m_characterContext.m_idleControl->easeIn(0.0f);
			m_characterContext.m_idleControl->setEaseInCurve(0, 0, 1, 1);	// Smooth
			m_characterContext.m_idleControl->setEaseOutCurve(1, 1, 0, 0);	// Smooth
			m_characterContext.m_animatedSkeleton->addAnimationControl(m_characterContext.m_idleControl);
			m_characterContext.m_idleControl->removeReference();
		}

		// Protect Animation (used in ragdoll demo)
		{

			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkProtect.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");

			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
			hkaAnimationBinding* protectBinding = ac->m_bindings[0];

			m_characterContext.m_protectControl = new hkaDefaultAnimationControl (protectBinding);
			m_characterContext.m_protectControl->setMasterWeight(1.0f);
			m_characterContext.m_protectControl->easeOut(0.0f);
			m_characterContext.m_protectControl->setEaseInCurve(0, 0, 1, 1);	// Smooth
			m_characterContext.m_protectControl->setEaseOutCurve(1, 1, 0, 0);	// Smooth
			m_characterContext.m_animatedSkeleton->addAnimationControl(m_characterContext.m_protectControl);
			m_characterContext.m_protectControl->removeReference();
		}

		// Convert the skin
		{
			const char* skinFile = "Resources/Animation/HavokGirl/hkLowResSkin.hkx";
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
		}


	}


	// State machine
	{
		m_stateMachine = new MyStateMachine();

		// Prepare State Machine
		{
			MyState* state = new MyWalkState();
			m_stateMachine->registerState (MY_STATE_WALKING, state);
			state->removeReference();

			state = new MyStandState();
			m_stateMachine->registerState (MY_STATE_STANDING, state);
			state->removeReference();

			state = new MyInAirState();
			m_stateMachine->registerState (MY_STATE_IN_AIR, state);
			state->removeReference();

		}
	}

	m_loadedData = HK_NULL;

	//
	// Setup the camera
	//
	{
		hkVector4 from = m_characterContext.m_characterProxy->getPosition();
		hkVector4 to = from ;
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );

		// so we can use the sticks on consoles
		if (m_env->m_options->m_trackballMode == 0)
		{
			m_forcedOnTrackball = true;
			m_env->m_window->getViewport(0)->setNavigationMode(HKG_CAMERA_NAV_TRACKBALL);
		}
	}

	m_world->unlock();
}

CharacterAnimationDemo::~CharacterAnimationDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true); 

	m_world->lock();

	m_world->removePhantom(m_characterContext.m_characterProxy->getShapePhantom());

	m_characterContext.m_animatedSkeleton->removeReference();
	m_characterContext.m_world->removeReference();
	m_characterContext.m_characterProxy->removeReference();

	m_standShape->removeReference();

	delete m_stateMachine;

	// Always last, remove the data loaded from animation and physics files
	delete m_loader;

	if (m_loadedData)
		m_loadedData->removeReference();

	if (m_forcedOnTrackball)
		m_env->m_window->getViewport(0)->setNavigationMode(HKG_CAMERA_NAV_FLY);

	m_world->unlock();
}


// Simulation methods
void CharacterAnimationDemo::getStateMachineInput(MyStateMachineInput& input) const
{
	// Directional Input
	const hkgPad* pad = m_env->m_gamePad;

	if (pad->getStickPosY ( 1 ) < 0)
	{
		input.m_inputDU = - 1.f;
	}
	if (pad->getStickPosY ( 1 ) > 0)
	{
		input.m_inputDU  = 1.f;
	}
	if (pad->getStickPosX ( 1 ) < 0)
	{
		input.m_inputLR = -1.f;
	}
	if (pad->getStickPosX ( 1 ) > 0)
	{
		input.m_inputLR = 1.f;
	}

	input.m_stepInfo.m_deltaTime = m_timestep;
	input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
	input.m_characterGravity = m_world->getGravity();
}

void CharacterAnimationDemo::stepWorldAndController(const MyStateMachineOutput& output)
{
	hkStepInfo si;

	si.m_deltaTime = m_timestep;
	si.m_invDeltaTime = 1.0f / m_timestep;


	//Apply the player character controller when the bounding boxes are correct
	if (!output.m_disableProxy)
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		const hkQsTransform& desiredMotion = output.m_desiredMotion;

		m_world->markForWrite();

		// Calculate the velocity we need in order to achieve the desired motion
		hkVector4 desiredVelocity;
		{
			hkVector4 desiredMotionWorld;
			hkQuaternion currentOrient;

			currentOrient.setAxisAngle(m_up, m_characterContext.m_currentAngle);
			desiredMotionWorld.setRotatedDir( currentOrient, desiredMotion.getTranslation() );

			// Divide motion by time
			desiredVelocity.setMul4 (1.0f / m_timestep, desiredMotionWorld );

			desiredVelocity.add4(output.m_additionalVelocity);

		}

		// Assume all desired angular motion is around the up axis
		hkReal desiredMotionAngle = desiredMotion.getRotation().getAngle();
		// Cater for inverted up axis
		if (desiredMotion.m_rotation.m_vec(2)<0)
		{
			desiredMotionAngle *= -1;
		}

		// There is nothing that can stop us rotating
		m_characterContext.m_currentAngle += desiredMotionAngle;

		// Feed output from state machine into character proxy
		m_characterContext.m_characterProxy->setLinearVelocity(desiredVelocity);

		m_characterContext.m_characterProxy->integrate( si, m_world->getGravity() );

		m_world->unmarkForWrite();

		HK_TIMER_END();
	}

	// Step the world.
	hkDefaultDemo::stepDemo();
}

void CharacterAnimationDemo::displayCurrentState() const
{
	MyStateType stateType = m_characterContext.m_currentState;
	char * stateStr;

	switch (stateType)
	{
		case MY_STATE_STANDING:
			stateStr = "Standing";	break;
		case MY_STATE_WALKING:
			stateStr = "Walking"; break;
		case MY_STATE_IN_AIR:
			stateStr = "In Air"; break;
		case MY_STATE_DYING:
			stateStr = "Dying"; break;
		case MY_STATE_DEAD:
			stateStr = "Dead"; break;
		case MY_STATE_GETTING_UP:
			stateStr = "Getting Up"; break;
		default:
			stateStr = "Other";	break;
	}
	char buffer[255];
	hkString::snprintf(buffer, 255, "State : %s", stateStr);
	const int h = m_env->m_window->getHeight();
	m_env->m_textDisplay->outputText(buffer, 20, h-40, 0xffffffff);
}


void CharacterAnimationDemo::updateCamera(const MyStateMachineOutput& output) const
{
	m_world->markForRead();

	hkVector4 target = m_characterContext.m_characterProxy->getPosition();

	hkQuaternion orient;
	orient.setAxisAngle(m_up, m_characterContext.m_currentAngle);

	// Camera Handling
	{
		hkVector4 from;
		m_env->m_window->getCurrentViewport()->getCamera()->getFrom((float*)&from);

		const hkReal height = 0.5f;
		hkVector4 forward;
		forward.set(0,-1,0);
		forward.setRotatedDir( orient, forward );

		hkVector4 desiredFrom, to;
		to = target;
		to.addMul4(-height, m_up);
		to.addMul4( 1.f, forward);

		desiredFrom = target;
		desiredFrom.addMul4(2 * height, m_up);
		desiredFrom.addMul4(-2.5f, forward);
		desiredFrom.sub4(from);
		desiredFrom.mul4(0.02f);
		from.add4(desiredFrom);

		setupDefaultCameras(m_env, from, to, m_up, 1.0f);
	}

	m_world->unmarkForRead();
}



hkDemo::Result CharacterAnimationDemo::stepDemo()
{

	m_world->lock();

	// Get input for the state machine from the keyboard
	MyStateMachineInput input;
	getStateMachineInput(input);

	// Update the state machine
	MyStateMachineOutput output;
	m_stateMachine->update(m_characterContext, input, output);

	m_world->unlock();

	// Display current state
	displayCurrentState();

	// Use the output of the state machine to drive the character controller
	stepWorldAndController(output);


	// Update the camera
	updateCamera(output);

	// Render the girl
	{
		hkaPose pose (m_characterContext.m_animatedSkeleton->getSkeleton());

		// Get the pose based on the character controller and state
		getCharacterPose(output, pose);

		// Render this pose
		renderPose(pose);
	}

	return hkDefaultDemo::stepDemo();
}

// Gets the pose from the girl, based on controller position
void CharacterAnimationDemo::getCharacterPose (const MyStateMachineOutput& output, hkaPose& poseOut) const
{
	// Sample the active animations and combine into a single pose
	m_characterContext.m_animatedSkeleton->sampleAndCombineAnimations( poseOut.accessUnsyncedPoseLocalSpace().begin(), poseOut.getFloatSlotValues().begin() ) ;
}

void CharacterAnimationDemo::renderPose(const hkaPose& pose) const
{
	const hkaSkeleton* skeleton = m_characterContext.m_animatedSkeleton->getSkeleton();

	// Sample the active animations and combine into a single pose
	const int numBones = skeleton->m_numBones;

	m_world->markForRead();
	// World From Model (accumulated motion) is based on the position of the proxy and the current angle of rotation
	hkQsTransform worldFromModel (m_characterContext.m_characterProxy->getPosition(), hkQuaternion(m_up, m_characterContext.m_currentAngle));
	m_world->unmarkForRead();

	hkTransform graphicsTransform;
	worldFromModel.copyToTransform(graphicsTransform);

	// Grab the pose in model space
	const hkArray<hkQsTransform>& poseInModelSpace = pose.getSyncedPoseModelSpace();

	// Construct the composite world transform
	hkLocalArray<hkTransform> compositeWorldInverse( numBones );
	compositeWorldInverse.setSize( numBones );

	// Skin the meshes
	for (int i=0; i < m_numSkinBindings; i++)
	{
		// assumes either a straight map (null map) or a single one (1 palette)
		hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
		int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : numBones;

		// Multiply through by the bind pose inverse world inverse matrices
		for (int p=0; p < numUsedBones; p++)
		{
			int boneIndex = usedBones? usedBones[p] : p;
			compositeWorldInverse[p].setMul( poseInModelSpace[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
		}

		AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, graphicsTransform, compositeWorldInverse.begin(), *m_env->m_sceneConverter );
	}

}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows the Animation system running on a character controlled by the Havok Physics SDK. " \
"The motion of the character proxy (physics) is driven by the animation system.";

HK_DECLARE_DEMO(CharacterAnimationDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Animation drives a character proxy.", helpString);

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

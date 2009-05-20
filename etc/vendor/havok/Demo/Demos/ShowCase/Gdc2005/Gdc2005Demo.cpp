/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>
#include <Demos/ShowCase/Gdc2005/Components/GdcProxyListener.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileWriter.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

#include <Animation/Animation/Ik/FootPlacement/hkaFootPlacementIkSolver.h>


// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Camera/hkxCamera.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxIndexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

#include <Demos/DemoCommon/Utilities/GameUtils/TweakerUtils.h>

// State Machine
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Character control
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

// Ragdoll
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>

// Mapper
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>

#include <Animation/Ragdoll/PoseMatching/hkaPoseMatchingUtility.h>

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

// Viewers
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpBroadphaseViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpContactPointViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpSimulationIslandViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpPhantomDisplayViewer.h>
#include <Common/Visualize/Process/hkDebugDisplayProcess.h>

#include <Physics/Utilities/Collide/Filter/ConstrainedSystem/hkpConstrainedSystemFilter.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/DemoCommon/Utilities/GameUtils/TweakerUtils.h>

extern const hkClass Gdc2005DemoOptionsClass;

const hkVector4 UP(0,1,0);

Gdc2005Demo::Gdc2005Demo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE),
	m_randomNumberGen(36532),
	m_dampedForwardSpeed(0),
	m_dampedTurnAngle(0),
	m_currentWarpPoint(0)
{
	// Disable warnings we know about
	hkError::getInstance().setEnabled( 0x234f224a, false ); // user allocated motion
	//only if no renderer									
	if( hkString::strCmp( m_env->m_options->m_renderer, "n" ) == 0 )
	{
		hkError::getInstance().setEnabled(0xf0d1e423, false); //'Could not realize an inplace texture of type PNG.'
	}

	// Load override settings if present
	{
		hkBinaryPackfileReader pw;

		hkIfstream settingsFile("GdcDemoSettingsOverride.bin");
		if (settingsFile.isOk())
		{
			pw.loadEntireFile( settingsFile.getStreamReader() );

			Gdc2005DemoOptions* options = (Gdc2005DemoOptions*)pw.getContents(Gdc2005DemoOptionsClass.getName());
			m_options = *options;
		}
	}

	//
	// Setup the camera and lights
	//
	{
		hkVector4 from( 11, 8, 8 );
		hkVector4 to  (  0,  0,  0 );
		hkVector4 up  = UP;
		setupDefaultCameras( env, from, to, up, 0.05f, 50 );
		hkgCamera* currentCamera = env->m_window->getCurrentViewport()->getCamera();
		currentCamera->setFOV( 30 );
		m_numCameras = 0;

		// turn on trackball
        if (m_env->m_options->m_trackballMode == 0) // 0 == fly
		{
			m_forcedTrackballOn = true;
			// Trackball on consoles does nothing really, so is just a PC mouse thing
			// We handle Pad stick0 to also move the camera for consoles.
			m_env->m_window->getViewport(0)->setNavigationMode( HKG_CAMERA_NAV_TRACKBALL );
		}


		// add flaslight as first light
		setSoleDirectionLight(m_env, const_cast<float*>(HKG_VEC3_X), 0xffffffff);
	}

	// Make the world
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

	// See what kind of renderer capabilities we have
	HKG_MESHBLENDING_SUPPORT skinningSupport = m_env->m_window->getMeshBlendingSupport();
	m_bUseHardwareSkinning = (skinningSupport != HKG_MESHBLENDING_NOSUPPORT );
	m_bUseWorldHardwareSkinning = (skinningSupport == HKG_MESHBLENDING_SOFTWARE_WORLD ); // must premultiply bones by world matrix on this impl

	m_env->m_mousePickingEnabled = false; // we handle our own

	// Make an asset loader
	m_loader = new hkLoader();

	// Add the default sky box
	setupSkyBox(m_env);

	// default graphical debug viewers for the phantoms etc to follow)
	setupGraphics();

	// Load the level and construct a mopp
	initLevel();

	// Load some objects that we can clone and chuck around
	initDynamicObjects();

	// Replace filter
	hkpGroupFilter* groupFilter = new hkpGroupFilter();

	// We disable collisions between different layers to determine what behaviour we want
	// For example, by disabling collisions between RAYCAST (used by foot ik) and DEBRIS, we make
	// sure the feet are never placed on top of debris objects.
	groupFilter->disableCollisionsBetween(LAYER_LANDSCAPE, LAYER_RAGDOLL_KEYFRAMED);
	groupFilter->disableCollisionsBetween(LAYER_PROXY, LAYER_DEBRIS);
	groupFilter->disableCollisionsBetween(LAYER_PROXY, LAYER_RAGDOLL_KEYFRAMED);
	groupFilter->disableCollisionsBetween(LAYER_PROXY, LAYER_RAYCAST);
	groupFilter->disableCollisionsBetween(LAYER_PROXY, LAYER_RAGDOLL_DYNAMIC);
	groupFilter->disableCollisionsBetween(LAYER_DEBRIS, LAYER_RAYCAST);
	groupFilter->disableCollisionsBetween(LAYER_MOVABLE_ENVIRONMENT, LAYER_RAGDOLL_KEYFRAMED);
	groupFilter->disableCollisionsBetween(LAYER_RAGDOLL_KEYFRAMED, LAYER_RAYCAST);
	groupFilter->disableCollisionsBetween(LAYER_RAYCAST, LAYER_RAGDOLL_DYNAMIC);
	groupFilter->disableCollisionsBetween(LAYER_PICKING, LAYER_LANDSCAPE);
	groupFilter->disableCollisionsBetween(LAYER_PICKING, LAYER_PROXY);
	groupFilter->disableCollisionsBetween(LAYER_PICKING, LAYER_RAGDOLL_KEYFRAMED);

	m_world->setCollisionFilter( groupFilter, true);
	groupFilter->removeReference();

	// Create the character proxy and state machine
	initCharacterController();

	// Create the animation state machine and command processor
	initAnimation();

	// Create the foot ik controller
	initFootIK ();

	// Create the look at controller
	initLookAt();

	// Creates the ragdoll and ragdoll controller
	initRagdoll();

	// Create and initialise the pose matching database
	initPoseMatching();

	// Init the reach controller
	initReach();

	m_currentTransform.setIdentity();
	m_currentTransform.m_translation = m_characterProxy->getPosition();

	m_horizontalDisplacement.setZero4();

	m_tweaking = false;
	m_selected = "/";	// Start at root

	m_world->unlock();
}

// Destructor : it calls the different quit..() methods and deallocates all memory
Gdc2005Demo::~Gdc2005Demo()
{
	// Re-enable warnings
	hkError::getInstance().setEnabled( 0x234f224a, true );
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_world->markForWrite();

	quitPoseMatching();
	quitRagdoll();
	quitLevel();
	quitFootIK();

	m_animationStateMachine->removeReference();
	m_animationMachine->removeReference();

	m_world->removePhantom( m_characterProxy->getShapePhantom() );
	m_characterProxy->removeReference();
	m_proxyListener->removeReference();

	// reset default body creation
	m_env->m_displayHandler->setAutoDisplayCreationState(true);

	if (m_proxyDisplay)
	{
		m_env->m_window->getContext()->lock();
		int idx = m_env->m_displayWorld->findDisplayObject( m_proxyDisplay );
		if (idx>=0)
		{
			m_env->m_displayWorld->removeDisplayObject( idx );
			m_proxyDisplay->removeReference(); // ref given back from world
		}
		m_proxyDisplay->removeReference();
		m_env->m_window->getContext()->unlock();
	}

	// need to delete world (delete refs to bodies about to disapear)
	// before we wipe the loader
	if(m_world)
	{
		m_world->removeReference();
		m_world = HK_NULL;
	}

	delete m_loader;

	// leave the graphics in some sort of state (reset fov to default)
	m_env->m_window->getViewport(0)->getCamera()->setFOV(45.0f);
	if (m_forcedTrackballOn)
	{
		m_env->m_window->getViewport(0)->setNavigationMode( HKG_CAMERA_NAV_FLY );
	}

	// want to do software skinning by default in the demos
	m_env->m_sceneConverter->setAllowHardwareSkinning(false);
}


// Called every step, stepDemo() is the heart of every demo in the demo framework
hkDemo::Result Gdc2005Demo::stepDemo()
{
	m_world->lock();

	// Handle tweaking. Select == SHIFT on PC (a bit obscure..)
	HK_TIMER_BEGIN_LIST("CPU preformance", "Tweaking");
	{
		// Handle any input for tweaking (in Gdc2005DemoInput.cpp)
		handleTweakingInput();

		// If interactive mode is off, then do nothing else
		if (!m_options.m_interactiveTweaking)
		{
			return DEMO_OK;
		}

		// Update the demo from the options (in Gdc2005DemoInput.cpp)
		optionUpdate();
	}

	m_world->unlock();

	// Step the physics
	HK_TIMER_SPLIT_LIST("Physics");
	{
		 hkDefaultPhysicsDemo::stepDemo();
	}

	m_world->lock();

	// The input to the animation machine, it gets modified along the way
	GdcStateInput stateInput;
	stateInput.m_options = &m_options;
	stateInput.m_animMachine = m_animationMachine;
	stateInput.m_context = m_animationStateMachine;
	stateInput.m_shouldWalk = m_forwardSpeed > 0.01f; //m_env->m_gamePad->getStickPosY(0) > 0.01f;
	stateInput.m_shouldGetUp = false;

	// Handle other user input (movement, mouse picking, etc)
	HK_TIMER_SPLIT_LIST("User Input");
	{
		// in Gdc2005DemoInput.cpp
		handleUserInput(stateInput);

		// Warp character?
		if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_R2) )
		{
			// Cycle to next warp point
			const int numWarpPoints = m_warpPoints.getSize();
			m_currentWarpPoint = (m_currentWarpPoint + 1) % numWarpPoints;
			warpCharacter(m_currentWarpPoint);
			return DEMO_OK;
		}
	}

	// Synchronize walk and run so it transitions smoothly
	HK_TIMER_SPLIT_LIST("GaitSync");
	{
		hkaDefaultAnimationControl* walkControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( GDC_WALK_CONTROL );
		hkaDefaultAnimationControl* runControl = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( GDC_RUN_CONTROL );

		const hkReal runWeight = m_dampedForwardSpeed;
		const hkReal walkWeight = 1.0f - m_dampedForwardSpeed;

		// Sync playback speeds
		{
			const hkReal totalW = runWeight+walkWeight+1e-6f;
			const hkReal walkP = walkWeight / totalW;
			const hkReal runP = runWeight / totalW;
			const hkReal runWalkRatio = runControl->getAnimationBinding()->m_animation->m_duration / walkControl->getAnimationBinding()->m_animation->m_duration;
			runControl->setPlaybackSpeed( (1-runP) * runWalkRatio + runP );
			walkControl->setPlaybackSpeed( (1-walkP) * (1.0f / runWalkRatio) + walkP );
		}


		const hkaDefaultAnimationControl* control = (hkaDefaultAnimationControl*)m_animatedSkeleton->getAnimationControl( GDC_MOVE_CONTROL );
		const hkReal controlWeight = control->getWeight() / control->getMasterWeight();
		runControl->setMasterWeight( runWeight * controlWeight );
		walkControl->setMasterWeight( walkWeight * controlWeight );
	}

	// Step all the animation controls
	HK_TIMER_SPLIT_LIST("Control update");
	{
		// Advance the active animations
		m_animatedSkeleton->stepDeltaTime( m_timestep );
	}

	HK_TIMER_SPLIT_LIST("CheckSupport");

	const hkUint32 currentAnimationState = m_animationStateMachine->getCurrentState();

	// Check support
	hkpSurfaceInfo ground;
	{
		hkVector4 down; down.setNeg4( UP );
		m_characterProxy->checkSupport(down, ground);

		m_timeUnsupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED) ? 0.0f : m_timeUnsupported + m_timestep;

		stateInput.m_isSupported = (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED) || (m_timeUnsupported < 0.45f);
	}

	// Update character controller based on the animation (extracted motion)
	HK_TIMER_SPLIT_LIST("Motion");
	{
		// Compute the motion for the proxy
		hkQsTransform desiredMotion;
		{
			m_animatedSkeleton->getDeltaReferenceFrame(m_timestep, desiredMotion);

			// Add extra vertical rotation depending on the left-right input
			desiredMotion.m_rotation.setAxisAngle( UP, m_dampedTurnAngle );
		}

		//Apply the player character controller
		{
			// Calculate the velocity we need stateInput order to achieve the desired motion
			hkVector4 desiredVelocity;
			{
				hkVector4 desiredMotionWorld;
				hkQuaternion currentOrient;

				desiredMotionWorld.setRotatedDir( m_currentTransform.m_rotation, desiredMotion.getTranslation() );

				// Divide motion by time
				desiredVelocity.setMul4 (1.0f / m_timestep, desiredMotionWorld );
			}

			// There is nothing that can stop us rotating
			m_currentTransform.m_rotation.mul( desiredMotion.m_rotation );
			m_currentTransform.m_rotation.normalize();

			// Adjust velocity
			if (currentAnimationState != GDC_JUMP_STATE)
			{
				// in these states we ignore the motion coming from the animation
				if ((currentAnimationState == GDC_IN_AIR_STATE) || (currentAnimationState == GDC_DYING_STATE) || (currentAnimationState == GDC_DEAD_STATE))
				{
					desiredVelocity = m_characterProxy->getLinearVelocity();
				}
				else
				{
					// this is the common case: add the motion velocity to the downward part of the proxy velocity (not upward though)
					hkReal vertComponent = hkMath::min2(0.0f, static_cast<hkReal>(m_characterProxy->getLinearVelocity().dot3(UP)));
					desiredVelocity.addMul4(vertComponent, UP);
				}
			}
			else
			{
				// when jumping, add the motion velocity to the vertical proxy velocity
				hkReal vertComponent = m_characterProxy->getLinearVelocity().dot3(UP);
				desiredVelocity.addMul4(vertComponent, UP);
			}

			// Just started jumping? Add some extra impulse
			if (stateInput.m_shouldJump)
			{
				desiredVelocity.addMul4(m_options.m_Proxy.m_jumpImpulse, UP);
			}

			// Add some gravity
			if (m_animationStateMachine->getCurrentState() != GDC_STAND_STATE || !(ground.m_supportedState == hkpSurfaceInfo::SUPPORTED))
			{
				desiredVelocity.addMul4( m_timestep, m_world->getGravity() );
			}

			// Avoid levitation: if the feet are on air, push the character down. Optionally, push it sideways if only one is on air
			// Only do this when footIK is enabled, otherwise the character proxy will slide down slopes (including sloped planes
			// from contact with stairs)
			if ( m_options.m_IK.m_footIk && (m_animationStateMachine->getCurrentState() == GDC_STAND_STATE) && (ground.m_supportedState == hkpSurfaceInfo::SUPPORTED) )
			{
				// Two legs stateInput air : push down
				if (m_footOnAir[RIGHT_LEG] && m_footOnAir[LEFT_LEG])
				{
					desiredVelocity.addMul4( m_timestep * 10.0f, m_world->getGravity() );
				}
				else
				{
					if (m_options.m_Proxy.m_pushIfFootInAir)
					{
						hkVector4 leftWS; leftWS.setRotatedDir(m_currentTransform.getRotation(), hkVector4(1.0f, 0, 0.3f));

						// One leg stateInput air : push sideways
						if (m_footOnAir[RIGHT_LEG])
						{
							desiredVelocity.addMul4( m_timestep * 10.0f, m_world->getGravity() );
							desiredVelocity.addMul4( -m_timestep * 50.0f, leftWS );
						}

						// One leg stateInput air : push sideways
						if (m_footOnAir[LEFT_LEG])
						{
							desiredVelocity.addMul4( m_timestep * 10.0f, m_world->getGravity() );
							desiredVelocity.addMul4( m_timestep * 50.0f, leftWS );
						}
					}

				}
			}

			/*
			** Apply the character controller : set velocities and integrate
			** We also keep track of any error (velocity loss), which we use in the next step to "catch up"
			** For example, when going upstairs, the proxy may lose velocity (due to collisions with steps), but we don't
			** want to fall behind (or otherwise the feet will slip off the steps), we use a horizontal displacement for this.
			*/
			if (( m_animationStateMachine->getCurrentState()!=GDC_DYING_STATE ) && (m_animationStateMachine->getCurrentState()!=GDC_DEAD_STATE))
			{
				HK_TIMER_SPLIT_LIST("CControl");

					// Add horizontal displacement (try to make the proxy catch up)
					const hkReal catchUpGain = (m_proxyListener->m_onMovingSurface) ? 0 : m_options.m_Proxy.m_horizontalGain;
					desiredVelocity.addMul4(1.0f/m_timestep * catchUpGain , m_horizontalDisplacement);

				// Feed output from state machine into character proxy
				m_characterProxy->setLinearVelocity(desiredVelocity);

				// Next expected position
				hkVector4 expectedPosition; expectedPosition.setAddMul4(m_characterProxy->getPosition(), desiredVelocity, m_timestep);

				hkStepInfo si;
				si.m_deltaTime = m_timestep;
				si.m_invDeltaTime = 1.0f / m_timestep;

				m_characterProxy->integrate( si, m_world->getGravity() );

				// Update the horizontal displacement
				const hkVector4& finalPosition = m_characterProxy->getPosition();
				hkVector4 horizontalFinalPosition; horizontalFinalPosition.setAddMul4(finalPosition, UP, -finalPosition.dot4(UP));
				hkVector4 horizontalExpectedPosition; horizontalExpectedPosition.setAddMul4(expectedPosition, UP, -expectedPosition.dot4(UP));
				m_horizontalDisplacement.setSub4(horizontalExpectedPosition, horizontalFinalPosition);

				{
					// Clamp the error
					const hkReal maxHorizontalDisplacement = m_options.m_Proxy.m_maxHorizontalSeparation;
					const hkReal maxHorizontalDisplacementSqr = maxHorizontalDisplacement * maxHorizontalDisplacement;
					const hkReal errorSqr = m_horizontalDisplacement.lengthSquared3();
					if (errorSqr>maxHorizontalDisplacementSqr)
					{
						m_horizontalDisplacement.mul4(hkMath::sqrt(maxHorizontalDisplacementSqr/errorSqr));
					}
				}


			}
		}

	}

	// If we stay in the air for too long, die !
	if (currentAnimationState == GDC_IN_AIR_STATE)
	{
		const hkReal timeInState = m_animationStateMachine->getCurrentTime() - m_animationStateMachine->getLastStateChange();
		if (timeInState > m_options.m_Proxy.m_safeFallTime)
		{
			stateInput.m_shouldDie = true;
		}
	}

	// Keep track on the decoupling between the proxy and the character (in the vertical axis)
	if (( m_animationStateMachine->getCurrentState()!=GDC_DYING_STATE ) && (m_animationStateMachine->getCurrentState()!=GDC_DEAD_STATE))
	{
		hkVector4 desiredPosition = m_characterProxy->getPosition();
		desiredPosition.addMul4(m_verticalDisplacement, UP);
		desiredPosition.add4(m_horizontalDisplacement);

		const hkVector4& currentPosition = m_currentTransform.getTranslation();

		// Vertical : do gain and clamp
		hkReal newVerticalPosition;
		{
			const hkReal desiredVerticalPosition = desiredPosition.dot3(UP);
			const hkReal currentVerticalPosition = currentPosition.dot3(UP);

			const hkReal diff = desiredVerticalPosition - currentVerticalPosition;

			hkReal verticalGain = m_options.m_Proxy.m_verticalGain;			// Gain
			newVerticalPosition = currentVerticalPosition + diff * verticalGain;

			// Clamp the vertical separation
			{
				const hkReal resultDiff = newVerticalPosition - desiredVerticalPosition;

				const hkReal maxSeparation = m_options.m_Proxy.m_maxVerticalSeparation;	// The maximum we allow the items to separate

				if ( resultDiff > maxSeparation )
				{
					newVerticalPosition = desiredVerticalPosition + maxSeparation;
				}

				if ( resultDiff < -maxSeparation )
				{
					newVerticalPosition = desiredVerticalPosition - maxSeparation;
				}
			}
		}

		// Horizontal : no need to do gain or clamp (is done somewhere else)
		hkVector4 newHorizontalPosition; newHorizontalPosition.setAddMul4(desiredPosition, UP, -desiredPosition.dot3(UP));

		m_currentTransform.m_translation.setAddMul4(newHorizontalPosition, UP, newVerticalPosition);
	}

	HK_TIMER_SPLIT_LIST("Animation Components")

	// Sample the animation, modify it (ik, etc) and skin it
	// Most of the interesting stuff is done here
	applyAnimationComponents(stateInput);

	HK_TIMER_END_LIST();

	// update flashlight / disable it etc
	updateFlashlight( m_options.m_Display.m_flashlight );

	// Possibly, display the current state (walk, jump, die, etc)
	if (m_options.m_Display.m_showState)
	{
		displayCurrentState();
	}

	// Update animation state machine
	{
		m_animationStateMachine->update( m_timestep, &stateInput );
	}

	// Advance the animation machine
	m_animationMachine->update( m_timestep );

	// Update the proxy display
	hkTransform t; t.setIdentity();
	t.setTranslation( m_characterProxy->getPosition() );
	hkUlong id = (hkUlong)(m_characterProxy->getShapePhantom()->getCollidable());
	m_env->m_displayHandler->updateGeometry(t, id, 0);

	m_world->unlock();

	return DEMO_OK;
}

// Samples the animation and applies different components (foot ik, look at, skin, camera, etc)
void Gdc2005Demo::applyAnimationComponents(GdcStateInput& stateInput)
{

	// We will use an hkaPose object and pass it through the different components
	// We allocate the memory for the pose in a local buffer
	HK_TIMER_BEGIN_LIST("ApplyAnimationComponents", "Init")

	hkLocalBuffer<char> poseBuffer( hkaPose::getRequiredMemorySize( m_animatedSkeleton->getSkeleton() ) );
	hkaPose pose(m_animatedSkeleton->getSkeleton(), poseBuffer.begin());

	// Start by retrieving the animation from the animated skeleton
	HK_TIMER_SPLIT_LIST("Animation");
	{
		// Sample the active animations and combine into a single pose
		m_animatedSkeleton->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin() );
		
		if (m_options.m_Display.m_originalPose)
		{
			AnimationUtils::drawPose( pose, m_currentTransform );
		}
	}

	// Store the original feet configuration for the foot placement
	const hkQsTransform originalLeftFootMS = pose.getBoneModelSpace(m_footPlacementComponent[LEFT_LEG]->m_setup.m_ankleIndex);
	const hkQsTransform originalRightFootMS = pose.getBoneModelSpace(m_footPlacementComponent[RIGHT_LEG]->m_setup.m_ankleIndex);

	// If we are in death state, replace the pose by a good pose from which we can get up, as we will use the pose in order
	// to drive the ragdoll
	HK_TIMER_SPLIT_LIST("PoseMatch");
	{
		if (m_animationStateMachine->getCurrentState()==GDC_DEAD_STATE)
		{
			hkReal error;

			// in GdcRagdoll.cpp
			matchPose( pose, error);

			hkReal vel = m_ragdollInstance->getRigidBodyOfBone(0)->getLinearVelocity().lengthSquared3();
			if (error < m_options.m_GetUp.m_autoGetup && ( vel < 0.01f))
			{
				stateInput.m_shouldGetUp = true;
			}
		}
		else
		{
			m_currentMatch = -1;
		}
	}

	// If we are getting up, blend the getting up animation with the current ragdoll animation to avoid discontinuities
	HK_TIMER_SPLIT_LIST("Get Up");
	{
		if (m_animationStateMachine->getCurrentState()==GDC_GETTING_UP_STATE)
		{
			// in GdcRagdoll.cpp
			doGetup( pose );
		}
		else
		{
			m_initGetup = false;
		}
	}

	// Modify the pose and the vertical displacement of the character to do foot placement (on both feet),
	// in all states except DEAD
	HK_TIMER_SPLIT_LIST("FootIK");
	{
		const bool shouldDoFootIk =	 (m_options.m_IK.m_footIk) &&
			(	m_animationStateMachine->getCurrentState()==GDC_WALK_STATE ||
				m_animationStateMachine->getCurrentState()==GDC_STAND_STATE ||
				m_animationStateMachine->getCurrentState()==GDC_LAND_STATE ||
				m_animationStateMachine->getCurrentState()==GDC_GETTING_UP_STATE ||
				m_animationStateMachine->getCurrentState()==GDC_DIVE_STATE );

		// in GdcFootIk.cpp
		doFootIK(shouldDoFootIk, originalLeftFootMS, originalRightFootMS, pose, m_verticalDisplacement);
	}

	// Drive the ragdoll to the incoming pose. Use a different method depending on whether the character is dead (doRagdoll() == use powered
	// constraint controller) or alive (doRagdollFeedback() == use rigid body controller)
	// Also, modify the pose so it reflects the ragdoll
	HK_TIMER_SPLIT_LIST("Ragdoll Ctrl");
	{
		if (( m_animationStateMachine->getCurrentState()==GDC_DYING_STATE ) || (m_animationStateMachine->getCurrentState()==GDC_DEAD_STATE))
		{
			// in GdcRagdoll.cpp
			doRagdoll(pose);
			stopRagdollFeedback();

		}
		else
		{
			// in GdcRagdoll.cpp
			doRagdollFeedback( pose );
		}
	}

	// Check the environment and possibly modify the pose to try to place a hand on top of the closest object
	HK_TIMER_SPLIT_LIST("Hand IK");
	{
		bool shouldDoReach = false;
		if (m_options.m_IK.m_handIkMode != hkGdcIkOptions::HANDIK_OFF)
		{
			switch (m_animationStateMachine->getCurrentState())
			{
				case GDC_STAND_STATE:
					shouldDoReach = true;
					break;
				case GDC_WALK_STATE:
					shouldDoReach = (m_options.m_IK.m_handIkMode == hkGdcIkOptions::HANDIK_ALWAYS);
					break;
				default:
					shouldDoReach = false;
			}
		}

		// in GdcReach.cpp
		doReach(shouldDoReach, LEFT_ARM, pose);
		doReach(shouldDoReach, RIGHT_ARM, pose);
	}

	// Modify the pose to turn the head of the character towards a point of interest (either the camera or another object)
	HK_TIMER_SPLIT_LIST("Look At");
	{
		hkBool shouldDoLookAt;

		const hkUint32 currentState = m_animationStateMachine->getCurrentState();
		switch (m_options.m_IK.m_lookAtMode)
		{
			case hkGdcIkOptions::LOOKAT_OFF:
				shouldDoLookAt = false;
				break;
			case hkGdcIkOptions::LOOKAT_STANDING:
				shouldDoLookAt = currentState == GDC_STAND_STATE;
				break;
			case hkGdcIkOptions::LOOKAT_ALIVE:
				shouldDoLookAt = (currentState != GDC_DYING_STATE) && (currentState != GDC_DEAD_STATE);
				break;
			default: // ALWAYS
				shouldDoLookAt = true;
		}

		hkVector4 lookAtTargetWS;

		if (shouldDoLookAt)
		{
			if (m_options.m_IK.m_lookAtTarget == hkGdcIkOptions::LOOKAT_CAMERA)
			{
				m_env->m_window->getCurrentViewport()->getCamera()->getFrom((float*)&lookAtTargetWS);
			}
			else
			{
				// LOOKAT_LAST_OBJECT
				const int lastItemIdx = m_chuckedItems.getSize()-1;
				if (lastItemIdx>0)
				{
					lookAtTargetWS = m_chuckedItems[lastItemIdx]->getPosition();
				}
				else
				{
					shouldDoLookAt = false;
				}
			}
		}

		// in GdcLookAt.cpp
		doLookAt(shouldDoLookAt, lookAtTargetWS, pose);
	}

	// Use the incoming pose and render the skinned character
	HK_TIMER_SPLIT_LIST("Skinning");
	{
		// in GdcSkinning.cpp
		doSkinning (pose);

		if (m_options.m_Display.m_finalPose)
		{
			AnimationUtils::drawPose( pose, m_currentTransform, 0xffff0000);
		}
	}

	// Use the incoming pose to update the camera target
	HK_TIMER_SPLIT_LIST("Camera Update")
	{
		// in GdcCamera.cpp
		doCamera (pose);
	}

	HK_TIMER_END_LIST();

}


void Gdc2005Demo::displayCurrentState() const
{
	char * stateStr;

	switch (m_animationStateMachine->getCurrentState())
	{
	case GDC_WALK_STATE:
		stateStr = "WALK";	break;
	case GDC_LAND_STATE:
		stateStr = "Landing"; break;
	case GDC_IN_AIR_STATE:
		stateStr = "In Air"; break;
	case GDC_JUMP_STATE:
		stateStr = "Jumping"; break;
	case GDC_STAND_STATE:
		stateStr = "Standing"; break;
	case GDC_DYING_STATE:
		stateStr = "Dying"; break;
	case GDC_DEAD_STATE:
		stateStr = "Dead"; break;
	case GDC_GETTING_UP_STATE:
		stateStr = "Getting Up"; break;
	case GDC_DIVE_STATE:
		stateStr = "Jazz"; break;
	default:
		stateStr = "ERROR";	break;
	}

	char buffer[255];
	hkString::snprintf(buffer, 255, "State : %s", stateStr);
	m_env->m_textDisplay->outputText(buffer, 20, 270, 0xffffffff);
}


void Gdc2005Demo::setupContexts(hkArray<hkProcessContext*>& contexts)
{
	if (m_world && (m_physicsViewersContext->findWorld(m_world) < 0) )
	{
		m_physicsViewersContext->addWorld(m_world);
	}

	contexts.pushBack( m_physicsViewersContext );
	m_debugViewerNames.pushBack( hkpShapeDisplayViewer::getName() );
	m_debugViewerNames.pushBack( hkDebugDisplayProcess::getName() );
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = "GDC 2005 Demo [Press Select or Shift for options]";

HK_DECLARE_DEMO(Gdc2005Demo, HK_DEMO_TYPE_COMPLETE|HK_DEMO_TYPE_SERIALIZE, "GDC 2005 Demo", helpString);

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

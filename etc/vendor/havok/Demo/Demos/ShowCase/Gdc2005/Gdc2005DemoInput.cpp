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


void Gdc2005Demo::handleUserInput(struct GdcStateInput& stateInput)
{
	const hkgKeyboard& keys = m_env->m_window->getKeyboard();

	// Show help on the keys if the user asks for it
	if ( keys.getKeyState('H') ||
		keys.getKeyState(HKG_VKEY_F1) ||
		( m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_L1)
		&& m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_R1)
		&& m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_0) )
		)
	{
		showKeys();
	}

	// For reference, on PC keyboard:
	//  HKG_VKEY_DELETE == HKG_PAD_BUTTON_L1
	//  HKG_VKEY_END == HKG_PAD_BUTTON_R1
	//  HKG_VKEY_INSERT == HKG_PAD_BUTTON_L2
	//  HKG_VKEY_HOME == HKG_PAD_BUTTON_R2

	// See if we want to chuck anything into the scene
	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) )
	{
		hkVector4 startPos;
		m_env->m_window->getCurrentViewport()->getCamera()->getFrom( (float*)&startPos );

		hkVector4 velocity;
		m_env->m_window->getCurrentViewport()->getCamera()->getDir( (float*)&velocity );
		velocity.mul4( m_options.m_Physics.m_launchVelocity );

		chuckSomething( startPos, velocity );
	}

	// See if we want to shot the guy (right click or L2)
	if ( m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_L2) ||
		(m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON) )
	{
		hkVector4 rayStart;
		hkVector4 rayEnd;

		// unproject current 'mouse' pos.
		hkgCamera* cam = m_env->m_window->getViewport(0)->getCamera();
		int xPos = m_env->m_window->getMouse().getPosX();
		int yPos = m_env->m_window->getMouse().getPosY();
		cam->unProject( xPos, yPos, 0, m_env->m_window->getWidth(), m_env->m_window->getHeight(), (float*)&rayStart);
		cam->unProject( xPos, yPos, 1, m_env->m_window->getWidth(), m_env->m_window->getHeight(), (float*)&rayEnd);

		fireShot( rayStart, rayEnd, m_options.m_Physics.m_shotStrength ); // num == strength
	}

	// See if we want to drag objects around
	if( !m_mouseActive && ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_R1) ||
		keys.getKeyState(HKG_VKEY_SPACE)) ) // Space == Button0 on PC == jump
	{
    	mouseDown(); // pick object under cursor
	}
	else if ( m_mouseActive )
	{
		if( !m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_R1) &&
			!m_env->m_window->getKeyboard().getKeyState(HKG_VKEY_SPACE) )
		{
			mouseUp(); // default physics game impl : delete mouse spring
		}
		else
		{
			mouseDrag(); // default physics game impl : unproject mouse and move spring.
		}
	}

	// Desired direction
	// If we are on a console we are using a stick and want that to dictate
	// the desired direction. Otherwise we will use the dpad ( keyboard dir keys )
	updateUserControl();

	// Die ? Get up? Jump?
	{
		const hkUint32 currentAnimationState = m_animationStateMachine->getCurrentState();
		const bool usingGamepad = m_env->m_window->hasGamePads() && !m_env->m_options->m_forceKeyboardGamepad;

		// DIE?
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) && currentAnimationState != GDC_DYING_STATE && currentAnimationState != GDC_DEAD_STATE && currentAnimationState != GDC_GETTING_UP_STATE)
		{
			stateInput.m_shouldDie = true;
		}
		else
		{
			stateInput.m_shouldDie = false;
		}

		// DIVE?
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_3) && currentAnimationState != GDC_DYING_STATE && currentAnimationState != GDC_DEAD_STATE && currentAnimationState != GDC_GETTING_UP_STATE)
		{
			stateInput.m_shouldDive = true;
		}
		else
		{
			stateInput.m_shouldDive = false;
		}

		// GET UP?
		if (m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) && currentAnimationState == GDC_DEAD_STATE)
		{
			stateInput.m_shouldGetUp = true;
		}
		else
		{
			stateInput.m_shouldGetUp = false;
		}

		// JUMP?
		bool jumpPressed;
		if (usingGamepad)
		{
			jumpPressed = m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0);
		}
		else
		{
			jumpPressed = keys.wasKeyPressed('4');
		}
		if (jumpPressed && currentAnimationState!=GDC_DYING_STATE && currentAnimationState!=GDC_IN_AIR_STATE && currentAnimationState!=GDC_DEAD_STATE && currentAnimationState!=GDC_JUMP_STATE)
		{
			stateInput.m_shouldJump = true;
		}
		else
		{
			stateInput.m_shouldJump = false;
		}

	}
}

void Gdc2005Demo::handleTweakingInput()
{
	const bool usingGamepad = m_env->m_window->hasGamePads() && !m_env->m_options->m_forceKeyboardGamepad;
	const hkgKeyboard& keys = m_env->m_window->getKeyboard();

	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_SELECT) ||
		keys.wasKeyPressed('T') )
	{
		m_tweaking = !m_tweaking;
	}

	if (m_tweaking)
	{
		// Show help on the keys
		showKeys();

		if (m_selected[0]!='/')
			m_selected = "/";

		extern const hkClass Gdc2005DemoOptionsClass;
		const hkClass& klass = Gdc2005DemoOptionsClass;
		hkgPad* pad = m_env->m_gamePad;

		if (pad->wasButtonPressed(HKG_PAD_DPAD_DOWN))
		{
			m_selected = TweakerUtils::getNextSiblingPath(m_selected, &m_options, klass);
		}

		if (pad->wasButtonPressed(HKG_PAD_DPAD_UP))
		{
			m_selected = TweakerUtils::getPrevSiblingPath(m_selected, &m_options, klass);
		}

		if (pad->wasButtonPressed(HKG_PAD_DPAD_LEFT))
		{
			m_selected = TweakerUtils::getParentPath(m_selected, &m_options, klass);
		}

		if (pad->wasButtonPressed(HKG_PAD_DPAD_RIGHT))
		{
			m_selected = TweakerUtils::getChildPath(m_selected, &m_options, klass);
		}

		if (usingGamepad)
		{
			if (pad->wasButtonPressed(HKG_PAD_BUTTON_0 ))
			{
				TweakerUtils::tweakData(m_selected, &m_options, klass, -0.1f);
			}

			if (pad->wasButtonPressed(HKG_PAD_BUTTON_3))
			{
				TweakerUtils::tweakData(m_selected, &m_options, klass, 0.1f);
			}

			if (pad->wasButtonPressed(HKG_PAD_BUTTON_2))
			{
				TweakerUtils::clearData(m_selected, &m_options, klass);
			}
		}
		else
		{
			if ( keys.wasKeyPressed(HKG_VKEY_SUBTRACT) || keys.wasKeyPressed(HKG_VKEY_SPACE) ||
				keys.wasKeyPressed(HKG_VKEY_OEM_MINUS) || keys.wasKeyPressed(HKG_VKEY_OEM_COMMA) )
			{
				TweakerUtils::tweakData(m_selected, &m_options, klass, -0.1f);
			}

			if ( keys.wasKeyPressed(HKG_VKEY_ADD) || keys.wasKeyPressed(HKG_VKEY_OEM_PLUS) || keys.wasKeyPressed(HKG_VKEY_OEM_PERIOD) )
			{
				TweakerUtils::tweakData(m_selected, &m_options, klass, 0.1f);
			}

			if ( keys.wasKeyPressed('0') || keys.wasKeyPressed(HKG_VKEY_DELETE) || keys.wasKeyPressed(HKG_VKEY_NUMPAD0) )
			{
				TweakerUtils::clearData(m_selected, &m_options, klass);
			}
		}

		TweakerUtils::displayData(m_selected, &m_options, klass, *m_env->m_textDisplay, 40, 40);

		m_env->m_textDisplay->outputText( hkString("Tweaking: ") + m_selected, 40, 15, 0xffffffff);

		// Clean out the pad so the keypresses don't propagate below.
		hkString::memSet( pad, 0, sizeof(hkgPad));
	}
}

void Gdc2005Demo::optionUpdate()
{
	const hkVector4 UP(0,1,0);

	// Handle load save

	//Set capsule size.
	{
		const hkReal totalHeight = m_options.m_Proxy.m_height;
		const hkReal radius = m_options.m_Proxy.m_radius;
		const hkReal capsulePoint = totalHeight*0.5f - radius;

		updateProxyDisplay(m_options.m_Proxy.m_radius, m_options.m_Proxy.m_height);

		hkVector4 vertexA(0, capsulePoint * 2 + radius, 0);
		hkVector4 vertexB(0, radius, 0);
		hkpCapsuleShape* capsule = static_cast<hkpCapsuleShape*>( const_cast<hkpShape*>(m_characterProxy->getShapePhantom()->getCollidable()->getShape() ));
		capsule->setRadius( radius );
		capsule->setVertex(0, vertexA);
		capsule->setVertex(1, vertexB);
	}

	// Set gravity
	hkVector4 gravity;
	gravity.setMul4( -m_options.m_Physics.m_gravity, UP);
	m_world->setGravity( gravity );

	// Ragdoll bodies (green)
	if (m_options.m_Display.m_ragdoll != m_ragdollDisplayBodiesVisible )
	{
		toggleRagdollVisibility();
	}

	if (m_options.m_Display.m_proxy != m_proxyVisible)
	{
		toggleProxyVisibility();
	}

	// Graphics
	hkDefaultDemo::forceShadowState( m_options.m_Display.m_shadows );

	if (m_lightmapDisplay)
	{
		if (m_options.m_Display.m_lightmaps)
		{
			int vertsIn = m_env->m_displayWorld->findDisplayObject( m_vertexColorDisplay );
			if (vertsIn >= 0 )
			{
				m_env->m_displayWorld->removeDisplayObject( vertsIn );
				m_vertexColorDisplay->release(); // already have a ref anyway
				m_env->m_displayWorld->addDisplayObject( m_lightmapDisplay );
			}
		}
		else
		{
			int lmIn = m_env->m_displayWorld->findDisplayObject( m_lightmapDisplay );
			if (lmIn >= 0 )
			{
				m_env->m_displayWorld->removeDisplayObject( lmIn );
				m_lightmapDisplay->release(); // already have a ref anyway
				m_env->m_displayWorld->addDisplayObject( m_vertexColorDisplay );
			}
		}
	}

	hkReal w = hkMath::max2(
		  m_animatedSkeleton->getAnimationControl(GDC_DEATH_CONTROL)->getWeight(),
		  m_animatedSkeleton->getAnimationControl(GDC_DYING_CONTROL)->getWeight());

	setMotors(m_ragdollInstance,
		m_options.m_Matching.m_force * w,
		m_options.m_Matching.m_tau,
		m_options.m_Matching.m_proportinalRecovery,
		m_options.m_Matching.m_constantRecovery);

	// Do rebuild walls, save and load from XML, etc., when we have finished tweaking
	if (m_tweaking == false)
	{

		switch (m_options.m_Misc.m_settings)
		{
			case hkGdcMiscOptions::SAVE:
				{
					hkBinaryPackfileWriter pw;
					pw.setContents(&m_options, Gdc2005DemoOptionsClass);

					hkOfstream settingsFile("GdcDemoSettings.bin");
					if (settingsFile.isOk())
					{
						hkPackfileWriter::Options o;
						pw.save( settingsFile.getStreamWriter(), o );
					}
				}
				break;
			case hkGdcMiscOptions::LOAD:
				{
					hkBinaryPackfileReader pw;

					hkIfstream settingsFile("GdcDemoSettings.bin");
					if (settingsFile.isOk())
					{
						pw.loadEntireFile( settingsFile.getStreamReader() );
						Gdc2005DemoOptions* options = (Gdc2005DemoOptions*)pw.getContents(Gdc2005DemoOptionsClass.getName());
						m_options = *options;
					}
				}
				break;
			case hkGdcMiscOptions::DEFAULT:
				{
					// Copy from defaults
					m_options = Gdc2005DemoOptions();
					m_selected = "/";
				}
				break;
			case hkGdcMiscOptions::NONE:
				break;
		}
		m_options.m_Misc.m_settings = hkGdcMiscOptions::NONE;

		if (m_options.m_Physics.m_rebuildWall)
		{
			rebuildBrickWall();
			m_options.m_Physics.m_rebuildWall = false; // reset
		}
	}
}

void Gdc2005Demo::showKeys()
{
	bool usingGamepad = m_env->m_window->hasGamePads() && !m_env->m_options->m_forceKeyboardGamepad;
	const char* str;
	if (usingGamepad)
	{
		str = "\220: Jump\n\221: Die / GetUp\n\222: Fire Object\n\223: SpecialMove\n"
			"R2: Warp\n"
			"\232 or RMouse: Punch\n\233 or Space: Drag\nSelect: Tweakables\n\n"
			"\223: Tweak value up\n\220: Tweak value down\n\222: Tweak clear value";
	}
	else // keyboard
	{
		str = "Space : Jump\n'1'   : Die / GetUp\n'2'   : Fire Object\n'3'   : SpecialMove\n"
			"Home  : Warp\n"
			"RMouse: Punch\n'P' or '4' : Drag\n'T'   : Tweakables\n\n"
			"'+' or '>' : Tweak value up\n '-' or '<': Tweak value down\n'0' or Del : Tweak clear value";
	}

	m_env->m_textDisplay->outputText(str, (int)(m_env->m_window->getWidth()/2) + 20, (int)20, 0xffffffff, 1);
}


#define INPUT_DEADZONE 0.2f // PS2 style .. large
static void _deadzone(float& stickX, float& stickY)
{
	const float deadzoneScale = 1.0f/(1.0f-INPUT_DEADZONE);
	stickX = ( stickX < -INPUT_DEADZONE) ? (stickX + INPUT_DEADZONE) * deadzoneScale : (
		( stickX > INPUT_DEADZONE ) ? (stickX - INPUT_DEADZONE) * deadzoneScale : 0);
	stickY = ( stickY < -INPUT_DEADZONE) ? (stickY + INPUT_DEADZONE) * deadzoneScale : (
		( stickY > INPUT_DEADZONE ) ? (stickY - INPUT_DEADZONE) * deadzoneScale : 0);
}

void Gdc2005Demo::updateUserControl()
{

	const hkVector4 UP(0,1,0);

	// PC: Arrow keys (stick1) does 'old' style relative control (mapped to dpad, so can't use dpad aswell)
	//     IJKL (stick0) does new style control

	// All others: Stick1 (left stick) does new style control
	//             Stick0 (right stick) controls camera (in/out, left/right)
	//             Dpad   controls camera orbit (up, down, and left right too to be easier to control)

	//do we have a gamepad?
	bool haveProperGamePad = m_env->m_window->hasGamePads() && !(m_env->m_options->m_forceKeyboardGamepad);

	float stick0X;
	float stick0Y;
	float stick1X;
	float stick1Y;
	float dpadStickX = 0.0f;
	float dpadStickY = 0.0f;
	if (!haveProperGamePad) // PC etc
	{
		stick0X = m_env->m_gamePad->getStickPosX(1);
		stick0Y = m_env->m_gamePad->getStickPosY(1);
		stick1X = m_env->m_gamePad->getStickPosX(0);
		stick1Y = m_env->m_gamePad->getStickPosY(0);
	}
	else
	{
		stick0X = m_env->m_gamePad->getStickPosX(0);
		stick0Y = m_env->m_gamePad->getStickPosY(0);
		stick1X = m_env->m_gamePad->getStickPosX(1);
		stick1Y = m_env->m_gamePad->getStickPosY(1);
		dpadStickX  = m_env->m_gamePad->getButtonState() & HKG_PAD_DPAD_RIGHT ? 1.0f : m_env->m_gamePad->getButtonState() & HKG_PAD_DPAD_LEFT ? -1.0f : 0;
		dpadStickY = m_env->m_gamePad->getButtonState() & HKG_PAD_DPAD_UP ? 1.0f : m_env->m_gamePad->getButtonState() & HKG_PAD_DPAD_DOWN ? -1.0f : 0;
	}

	const bool inputStick0X = hkMath::fabs( stick0X ) > 0.01f;
	const bool inputStick0Y = hkMath::fabs( stick0Y ) > 0.01f;

	// ignore char move on stick1 if in mouse mode
	const bool mouseMode = (m_env->m_gamePad->getButtonState() & HKG_PAD_BUTTON_L1) != 0;
	const bool inputStick1X = hkMath::fabs( stick1X ) > 0.01f && !mouseMode;
	const bool inputStick1Y = hkMath::fabs( stick1Y ) > 0.01f && !mouseMode;

	bool zeroInput = !(inputStick1Y | inputStick0Y | inputStick1X | inputStick0X);
	hkgCamera* currentCamera = m_env->m_window->getViewport(0)->getCamera();

	if (!haveProperGamePad)
	{
		if (inputStick0Y || inputStick0X)
		{
			m_forwardSpeed = stick0Y;
			const hkReal stickDiff = (m_forwardSpeed - m_dampedForwardSpeed);
			const hkReal stickGain = m_options.m_Misc.m_walkToRunSpeed;
			m_dampedForwardSpeed += stickDiff * stickGain;
			m_dampedTurnAngle = -0.04f * stick0X;
		}
	}
	else // stick0 and dpad does the camera
	{
		if (inputStick0Y || inputStick0X || (hkMath::fabs(dpadStickX) > 0.01f) || (hkMath::fabs(dpadStickY) > 0.01f))
		{
			// Need to control camera
			// Simple rotate about the point of interest (orbit left/right, zoom in/out)
			_deadzone(stick0X, stick0X);

			if (inputStick0X)
			{
				currentCamera->rotateAboutTo(0.1f*stick0X, (const float*)&UP, true );
			}

			if (inputStick0Y)
			{
				currentCamera->dolly(-0.1f*stick0Y, false, true );
			}
			// Dpad orbit up down (and left right to be used)
			if (hkMath::fabs(dpadStickY) > 0.01f)
			{
				float r[3];
				currentCamera->getRight(r);
				currentCamera->rotateAboutTo(-0.03f*dpadStickY, r, true);
			}

			if (hkMath::fabs(dpadStickX) > 0.01f)
			{
				currentCamera->rotateAboutTo(0.03f*dpadStickX, (const float*)&UP, true );
			}
		}
	}

	// normal stick style (maps to IJKL keys on PC)
	if (inputStick1Y || inputStick1X)
	{
		hkVector4 dv, tv, rv;
		currentCamera->getDir((float*)&dv); tv.setCross( UP, dv); dv.setCross(tv, UP );
		currentCamera->getRight((float*)&rv); tv.setCross( UP, rv); rv.setCross(tv, UP );
		dv.fastNormalize3(); rv.fastNormalize3();

		// accont for deadzone (as hkg will deadzone, but does not normalize the non deadzone back to 0..1)
		_deadzone(stick1X, stick1Y);

		dv.mul4( stick1Y );
		rv.mul4( stick1X );
		m_desiredDirection.setAdd4(dv, rv);

		float dirSquared = stick1X*stick1X + stick1Y*stick1Y;
		if ( dirSquared > 0.05f) // going somewhere..
		{
			m_forwardSpeed = hkMath::sqrt( dirSquared );

			// see how far off we are from facing along desired:
			hkRotation rm; rm.set( m_currentTransform.getRotation() );
			hkVector4 facingDir = rm.getColumn(2);
			hkVector4 normalizedDirection( m_desiredDirection ); normalizedDirection.fastNormalize3();

			hkVector4 tc; tc.setCross(facingDir, normalizedDirection );
			hkReal cosTheta = facingDir.dot3( normalizedDirection );
			hkReal angle = hkMath::acos( cosTheta );
			if (tc.dot3(UP) < 0)
			{
				angle *= -1;
			}
			m_dampedTurnAngle = 0.1f * angle;

			const hkReal stickDiff = (m_forwardSpeed - m_dampedForwardSpeed);
			const hkReal stickGain = m_options.m_Misc.m_walkToRunSpeed * cosTheta;
			m_dampedForwardSpeed += stickDiff * stickGain;
		}
		else // zero effect on stick1
		{
			zeroInput |= !(inputStick0Y || inputStick0X);
		}
	}


	if (zeroInput)
	{
		m_forwardSpeed = 0;
		m_dampedForwardSpeed -= m_dampedForwardSpeed*0.5f;
		m_dampedTurnAngle = 0;
	}
}

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

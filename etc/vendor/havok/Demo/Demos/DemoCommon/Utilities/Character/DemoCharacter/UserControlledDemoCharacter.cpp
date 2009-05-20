/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/UserControlledDemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/DemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

#include <Graphics/Common/Camera/hkgCamera.h>
#include <Common/Visualize/hkDebugDisplay.h>

UserControlledDemoCharacter::UserControlledDemoCharacter( DemoCharacter* character )
: m_demoCharacter( character )
{
	m_demoCharacter->addReference();
	m_characterTurnSpeed = .04f * 60;
	m_forwardSpeed = 0;

	m_cameraToHeight = 1.f;
	m_cameraToForward = 2.f;
	m_cameraFromHeight = 3.f;
	m_cameraFromBack = 3.f;	
	m_cameraGain = .2f;

	m_makeOccludingObjectsTransparent = true;

	// Setup the default action keys
	{
		m_actionsKeys[ACTION_DIE]	= HKG_PAD_BUTTON_1;
		m_actionsKeys[ACTION_GETUP] = HKG_PAD_BUTTON_1;
		m_actionsKeys[ACTION_JUMP]	= HKG_PAD_BUTTON_0;
		m_actionsKeys[ACTION_DIVE]	= HKG_PAD_BUTTON_3;
	}
}

UserControlledDemoCharacter::~UserControlledDemoCharacter()
{
	m_demoCharacter->removeReference();
}

void UserControlledDemoCharacter::setActionKey(CharacterAction action, HKG_PAD_BUTTON key)
{
	m_actionsKeys[action] = key;
}

void UserControlledDemoCharacter::getUserInputForCharacter( hkDemoEnvironment* env, CharacterStepInput& input,  CharacterActionInfo& actionInfo, const hkReal maxVelocity )
{
	input.m_straffeLeftRightVelocity = 0; // Not used
	input.m_turnVelocity = 0;

	{
		//do we have a gamepad?
		bool haveProperGamePad = env->m_window->hasGamePads() && !(env->m_options->m_forceKeyboardGamepad);

		float stick0X;
		float stick0Y;
		float stick1X;
		float stick1Y;

		if (!haveProperGamePad) // PC etc
		{
			stick0X = env->m_gamePad->getStickPosX(1);
			stick0Y = env->m_gamePad->getStickPosY(1);
			stick1X = env->m_gamePad->getStickPosX(0);
			stick1Y = env->m_gamePad->getStickPosY(0);
		}
		else
		{
			stick0X = env->m_gamePad->getStickPosX(0);
			stick0Y = env->m_gamePad->getStickPosY(0);
			stick1X = env->m_gamePad->getStickPosX(1);
			stick1Y = env->m_gamePad->getStickPosY(1);
		}

		const bool inputStick0X = hkMath::fabs( stick0X ) > 0.01f;
		const bool inputStick0Y = hkMath::fabs( stick0Y ) > 0.01f;
		const bool inputStick1X = hkMath::fabs( stick1X ) > 0.01f;
		const bool inputStick1Y = hkMath::fabs( stick1Y ) > 0.01f;

		bool zeroInput = !(inputStick1Y | inputStick0Y | inputStick1X | inputStick0X);

		if (inputStick0Y || inputStick0X)
		{
			input.m_forwardVelocity = stick0Y;

			// We want to get to a run faster than in the GDC demo
			input.m_forwardVelocity = hkMath::min2(input.m_forwardVelocity * 6.f * maxVelocity, 1.f * maxVelocity);
			// Disallow turning on the spot
			//m_forwardSpeed = hkMath::max2(m_forwardSpeed, .02f );

			input.m_turnVelocity = -m_characterTurnSpeed * stick0X;
		}

		if (zeroInput)
		{
			input.m_forwardVelocity -= input.m_forwardVelocity*0.5f;
			input.m_turnVelocity = 0;
		}

		{
			const bool usingGamepad = env->m_window->hasGamePads() && !env->m_options->m_forceKeyboardGamepad;
		
			// JUMP?
			//bool jumpPressed;
			if (usingGamepad)
			{
				actionInfo.m_wasJumpPressed = env->m_gamePad->wasButtonPressed(m_actionsKeys[ACTION_JUMP]);
			}
			else
			{
				const hkgKeyboard& keys = env->m_window->getKeyboard();
				actionInfo.m_wasJumpPressed = keys.wasKeyPressed('4');
			}

			if (actionInfo.m_wasJumpPressed)
			{
				input.m_jumpVelocity = 5.5f;
			}
			else
			{
				input.m_jumpVelocity = 0.0f;
			}
			
			// These are tied to the same key; we figure it out from context later on
			actionInfo.m_wasDiePressed = env->m_gamePad->wasButtonPressed(m_actionsKeys[ACTION_DIE]);
			actionInfo.m_wasGetUpPressed = env->m_gamePad->wasButtonPressed(m_actionsKeys[ACTION_GETUP]);
			actionInfo.m_wasDivePressed = env->m_gamePad->wasButtonPressed(m_actionsKeys[ACTION_DIVE]);
		}
	}
}

void UserControlledDemoCharacter::getCameraFromTo( hkVector4& from, hkVector4& to ) const
{
	hkTransform wFc;
	m_demoCharacter->getProxy()->getTransform( wFc );

	hkVector4 forward;
	forward.setRotatedDir(wFc.getRotation(), m_demoCharacter->getProxy()->getForwardLocal() );

	to = wFc.getTranslation();
	to.addMul4( m_cameraToHeight, m_demoCharacter->getProxy()->getUpLocal());
	to.addMul4( m_cameraToForward, forward);

	from = wFc.getTranslation();
	from.addMul4(m_cameraFromHeight, m_demoCharacter->getProxy()->getUpLocal());
	from.addMul4(-m_cameraFromBack, forward);

}

// Objects colors
#define NORMAL_GRAY				hkColor::rgbFromChars( 192, 192, 192, 255 )
#define TRANSPARENT_GREY		hkColor::rgbFromChars( 192, 192, 192, 125 )

void UserControlledDemoCharacter::updateCameraWithSmoothing( hkDemoEnvironment* env, hkpWorld* world, const hkVector4& newFrom, const hkVector4& newTo )
{
	env->m_window->getContext()->lock();

	hkVector4 from;
	hkVector4 to;
	{
		// Interpolate camera from last position
		env->m_window->getCurrentViewport()->getCamera()->getFrom((float*)&from);
		hkVector4 fromDisplacement; fromDisplacement.setSub4( newFrom, from );
		fromDisplacement.mul4( m_cameraGain );
		from.add4(fromDisplacement);

		env->m_window->getCurrentViewport()->getCamera()->getTo((float*)&to);
		hkVector4 toDisplacement; toDisplacement.setSub4( newTo, to );
		toDisplacement.mul4( m_cameraGain );
		to.add4( toDisplacement );
	}

	// Make object in the line of sight transparent
	if (m_makeOccludingObjectsTransparent)
	{
		// Cast down to landscape to get an accurate position
		hkpWorldRayCastInput raycastIn;
		
		// Reverse direction for raycasting
		hkTransform wFc;
		m_demoCharacter->getProxy()->getTransform( wFc );
		raycastIn.m_from.setAdd4( wFc.getTranslation(), m_demoCharacter->getProxy()->getUpLocal() ); // XXX m_characterRb->getPosition() is wrong
		raycastIn.m_to = from;
		raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(0);

		//HK_DISPLAY_LINE(raycastIn.m_from, raycastIn.m_to, hkColor::WHITE);

		hkpAllRayHitCollector collector;

		world->castRay( raycastIn, collector);

		for (int i = 0; i < m_transparentObjects.getSize(); ++i )
		{
			HK_SET_OBJECT_COLOR(m_transparentObjects[i], NORMAL_GRAY );
		}
		m_transparentObjects.clear();

		// Loop over all collected objects
		for(int i=0; i < collector.getHits().getSize();i++)
		{				
			hkpWorldRayCastOutput raycastOut = collector.getHits()[i];
			hkpRigidBody* rb = hkGetRigidBody(raycastOut.m_rootCollidable);
			if (rb && rb->isFixedOrKeyframed())
			{
				HK_SET_OBJECT_COLOR((hkUlong)raycastOut.m_rootCollidable, TRANSPARENT_GREY );

				m_transparentObjects.pushBack((hkUlong)raycastOut.m_rootCollidable);
			}
		}
	}

	hkgWindow* w = env->m_window;
	HK_ASSERT(0, w != HK_NULL );
	for(int i = 0; i < w->getNumViewports(); ++i)
	{
		hkgCamera* c = w->getViewport(i)->getCamera();

		float upN[3];
		hkgVec3Copy(upN, &m_demoCharacter->getProxy()->getUpLocal()(0));
		hkgVec3Normalize(upN);
		// set up camera
		c->setFrom(&from(0));
		c->setTo(&to(0));
		c->setUp(upN);
		c->orthogonalize();
		c->computeModelView();
		c->computeProjection();

		w->getViewport(i)->setFlyModeUp( upN );
	}
	env->m_window->getContext()->unlock();
}



void UserControlledDemoCharacter::updateCamera( hkDemoEnvironment* env, hkpWorld* world )
{

	hkVector4 desiredFrom;
	hkVector4 desiredTo;
	getCameraFromTo( desiredFrom, desiredTo );
	m_cameraGain = .2f;

	updateCameraWithSmoothing( env, world, desiredFrom, desiredTo );

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

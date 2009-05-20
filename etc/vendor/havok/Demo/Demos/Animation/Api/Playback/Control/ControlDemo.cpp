/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Control/ControlDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControlListener.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

// Graphics Stuff
#include <Graphics/Common/Window/hkgWindow.h>


// A sample implementation of the control listener interface
// This show how to handle control events.
class MyControlListener : public hkReferencedObject, public hkaAnimationControlListener, public hkaDefaultAnimationControlListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		MyControlListener(hkDemoEnvironment* env, hkVector4& textPos)
		{
			m_env = env;
			m_pos = textPos;
		}


		virtual void loopOverflowCallback(hkaDefaultAnimationControl* control, hkReal deltaTime, hkUint32 overflows)
		{
			m_env->m_textDisplay->outputText3D( "Overflow", m_pos(0), m_pos(1), m_pos(2), 0xffffffff, 30);

			// Ease out this animation in 0.5 sec
			control->easeOut(0.5f);
		}

		virtual void loopUnderflowCallback(hkaDefaultAnimationControl* control, hkReal deltaTime, hkUint32 underflows)
		{
			m_env->m_textDisplay->outputText3D( "Underflow", m_pos(0), m_pos(1), m_pos(2), 0xffffffff, 30);

			// Ease out this animation in 0.5 sec
			control->easeOut(0.5f);
		}

		virtual void easedInCallback(hkaDefaultAnimationControl* control, hkReal deltaTime)
		{
			// Animation has Eased in so we'll reverse the play direction
			m_env->m_textDisplay->outputText3D( "Eased In", m_pos(0), m_pos(1), m_pos(2), 0xffffffff, 30);
		}

		virtual void easedOutCallback(hkaDefaultAnimationControl* control, hkReal deltaTime)
		{
			m_env->m_textDisplay->outputText3D( "Eased Out", m_pos(0), m_pos(1), m_pos(2), 0xffffffff, 30);

			// Ease back in 0.5 second
			control->easeIn(0.5f);
		}

		virtual void controlDeletedCallback(hkaAnimationControl* control)
		{
			m_env->m_textDisplay->outputText3D( "Deleted", m_pos(0), m_pos(1), m_pos(2), 0xffffffff, 30);
		}

	private:

		hkVector4 m_pos;

		hkDemoEnvironment* m_env;
};

ControlDemo::ControlDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from( 12.0f, 1.0f, 2.0f );
		hkVector4 to  ( 0.0f ,1.0f ,0.0f );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100.0f );
	}

	m_loader = new hkLoader();

	// Get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkLongAnim.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_animation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding = ac->m_bindings[0];
	}


	// 1 Control
	{
		description[0] = "x1";
		m_control[0] = new hkaDefaultAnimationControl( m_binding );
		m_control[0]->setPlaybackSpeed(1.0f);

		hkVector4 outputPos(0, -4, -1.3f);
		m_listener[0] = new MyControlListener(env, outputPos);
		// Listen for overflow
		m_control[0]->addDefaultControlListener(m_listener[0]);
		// Listen for deletion
		m_control[0]->addAnimationControlListener(m_listener[0]);

		m_skeletonInstance[0] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[0]->addAnimationControl( m_control[0] );
		m_skeletonInstance[0]->setReferencePoseWeightThreshold(1.0f);
		m_control[0]->removeReference();
	}

	// 2 Control speed x0.5
	{
		description[1] = "x0.5";
		m_control[1] = new hkaDefaultAnimationControl( m_binding );
		m_control[1]->setPlaybackSpeed(0.5f);

		hkVector4 outputPos(0, -2, -1.3f);
		m_listener[1] = new MyControlListener(env, outputPos);
		// Listen for overflow
		m_control[1]->addDefaultControlListener(m_listener[1]);
		// Listen for deletion
		m_control[1]->addAnimationControlListener(m_listener[1]);

		m_skeletonInstance[1] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[1]->addAnimationControl( m_control[1] );
		m_skeletonInstance[1]->setReferencePoseWeightThreshold(1.0f);
		m_control[1]->removeReference();
	}

	// 3 reverse
	{
		description[2] = "x-1";
		m_control[2] = new hkaDefaultAnimationControl( m_binding );
		m_control[2]->setPlaybackSpeed(-1.0f);

		hkVector4 outputPos(0, 0, -1.3f);
		m_listener[2] = new MyControlListener(env, outputPos);
		// Listen for overflow
		m_control[2]->addDefaultControlListener(m_listener[2]);
		// Listen for deletion
		m_control[2]->addAnimationControlListener(m_listener[2]);

		m_skeletonInstance[2] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[2]->addAnimationControl( m_control[2] );
		m_skeletonInstance[2]->setReferencePoseWeightThreshold(1.0f);
		m_control[2]->removeReference();
	}

	// 4 reverse x1.5
	{
		description[3] = "x-1.5";
		m_control[3] = new hkaDefaultAnimationControl( m_binding );
		m_control[3]->setPlaybackSpeed(-1.5f);

		hkVector4 outputPos(0, 2 , -1.3f);
		m_listener[3] = new MyControlListener(env, outputPos);
		// Listen for overflow
		m_control[3]->addDefaultControlListener(m_listener[3]);
		// Listen for deletion
		m_control[3]->addAnimationControlListener(m_listener[3]);

		m_skeletonInstance[3] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[3]->addAnimationControl( m_control[3] );
		m_skeletonInstance[3]->setReferencePoseWeightThreshold(1.0f);
		m_control[3]->removeReference();
	}

	// 5 cycle once only
	{
		description[4] = "once only";
		m_control[4] = new hkaDefaultAnimationControl( m_binding, true, 1 );
		m_control[4]->setPlaybackSpeed(1.0f);

		hkVector4 outputPos(0, 4 , -1.3f);
		m_listener[4] = new MyControlListener(env, outputPos);
		// Listen for overflow
		m_control[4]->addDefaultControlListener(m_listener[4]);
		// Listen for deletion
		m_control[4]->addAnimationControlListener(m_listener[4]);

		m_skeletonInstance[4] = new hkaAnimatedSkeleton( m_skeleton );
		m_skeletonInstance[4]->addAnimationControl( m_control[4] );
		m_skeletonInstance[4]->setReferencePoseWeightThreshold(1.0f);
		m_control[4]->removeReference();
	}

	// The easiest way to get debug lines
	setupGraphics( );

}


ControlDemo::~ControlDemo()
{
	for (int i=0; i < HK_NUM_CHARACTERS; i++)
	{
		m_skeletonInstance[i]->removeReference();
		if (m_listener[i])
			delete m_listener[i];
	}

	delete m_loader;
	hkDefaultDemo::setupLights(m_env); // assumes the hkx will have some.
}

hkDemo::Result ControlDemo::stepDemo()
{
	hkaPose pose (m_skeleton);

	// Advance the active animations
	for (int i=0; i < HK_NUM_CHARACTERS; i++)
	{
		m_skeletonInstance[i]->stepDeltaTime( m_timestep );

		// Sample the active animations and combine into a single pose
		m_skeletonInstance[i]->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set(0, i*2-4.0f ,0);
		AnimationUtils::drawPose( pose, worldFromModel, 0xffff0000 );
	} // all animations


	//draw the text
	for (int t=0; t < HK_NUM_CHARACTERS; t++)
	{
		hkString txt(description[t]);
		m_env->m_textDisplay->outputText3D( txt, 0, t*2-4.0f ,-1, 0xffffffff, 1);
	}

	// Destroy the controls after a period of time
	{
		static int countdown = 2000;
		countdown--;
		if (countdown == 0)
		{
			for (int i=0; i < HK_NUM_CHARACTERS; i++)
			{
				m_control[i]->removeReference();
			}
		}
	}
	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows the different control and playback speeds an animation control can have. " \
"A listener has been added to each control. When the control loops (either overflows or underflows) " \
"the listeners eases out the animation. Once the animation has fully eased out the listener eases it back in.";

HK_DECLARE_DEMO(ControlDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Illustrates the default control", helpString);

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

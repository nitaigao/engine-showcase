/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/FloatTracks/FloatTracksDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Animation/Animation/Rig/hkaPose.h>

// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/Interleaved/hkaInterleavedUncompressedAnimation.h>
#include <Animation/Animation/Animation/WaveletCompressed/hkaWaveletCompressedAnimation.h>
#include <Animation/Animation/Animation/DeltaCompressed/hkaDeltaCompressedAnimation.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>

#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

	// For debug drawing
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>


FloatTracksDemo::FloatTracksDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	// Disable reports: 									
	if(m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x432434a4, false); 
		hkError::getInstance().setEnabled(0x36118e94, false);
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(  0.0f, -7.0f,  1.0f);
		hkVector4 to  (  0.0f,  0.0f,   -1.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	m_loader = new hkLoader();

	// Get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoopWithFloats.hkx");	// Rig is also in this file
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// Get the animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoopWithFloats.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "No animation loaded");
		m_originalAnimation = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_originalBinding = ac->m_bindings[0];
	}

	for ( int i = 0; i < NUM_ANIMATION_TYPES; ++i )
	{
		m_bindings[i] = new hkaAnimationBinding();	
		*(m_bindings[i]) = *(m_originalBinding);	// Copy all other binding data - we overwrite th animation associated with this new binding below.

		switch(i)
		{
			case 0: // Uncompressed
			{
				m_bindings[i]->m_animation = m_originalAnimation;
				break;
			}
			case 1: // Delta
			{
				hkaDeltaCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;

				hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>( m_originalAnimation );
				hkaDeltaCompressedAnimation* d = new hkaDeltaCompressedAnimation(*uncompressedAnimation, p);
				m_animations[i] = d;
				m_bindings[i]->m_animation = d;

				break;
			}
			case 2: // Wavelet
			{
				hkaWaveletCompressedAnimation::CompressionParams p;
				p.m_quantizationBits = 16;
				p.m_blockSize = 8;
				p.m_truncProp = .25;

				hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>( m_originalAnimation );
				hkaWaveletCompressedAnimation* w = new hkaWaveletCompressedAnimation(*uncompressedAnimation, p);
				m_animations[i] = w;
				m_bindings[i]->m_animation = w;

				break;
			}
			case 3: // Spline
			{
				hkaSplineCompressedAnimation::TrackCompressionParams p;
				hkaSplineCompressedAnimation::AnimationCompressionParams a;

				hkaInterleavedUncompressedAnimation* uncompressedAnimation = static_cast<hkaInterleavedUncompressedAnimation*>( m_originalAnimation );
				hkaSplineCompressedAnimation* sa = new hkaSplineCompressedAnimation( *uncompressedAnimation, p, a );
				m_animations[i] = sa;
				m_bindings[i]->m_animation = sa;

				break;
			}
		}

		// Create an animation control
		hkaDefaultAnimationControl* ac = new hkaDefaultAnimationControl(m_bindings[i]);
		ac->setPlaybackSpeed( 0.1f );

		// Create a new animated skeleton
		hkaAnimatedSkeleton* skeleton = new hkaAnimatedSkeleton( m_skeleton );
		m_activeSkeletons.pushBack( skeleton );

		// Bind the control to the skeleton
		skeleton->addAnimationControl( ac );

		// The animated skeleton now owns the control
		ac->removeReference();
	}

	setupGraphics( );

	m_paused = false;
	m_singleStep = true;
}

FloatTracksDemo::~FloatTracksDemo()
{
	// Re-enable reports
	hkError::getInstance().setEnabled(0x432434a4, true); 
	hkError::getInstance().setEnabled(0x36118e94, true);

	delete m_loader;

	// Delete the active skeletons
	for (int s=0; s< m_activeSkeletons.getSize(); s++)
	{
		m_activeSkeletons[s]->removeReference();
		delete m_bindings[s];
		if( s != 0)
		{
			m_animations[s]->removeReference();
		}
	}
}

hkDemo::Result FloatTracksDemo::stepDemo()
{
	{
		m_env->m_textDisplay->outputText( (!m_paused?
			"Running (Hit \221 to pause)":
			"Paused. (Hit \221 to unpause, hit \222 to single-step)"), 20, 180 );
	}

	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1) )
	{
		m_paused = !m_paused;
	}

	if ( m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_2) )
	{
		m_singleStep = true;
	}

	for (int i = 0; i < m_activeSkeletons.getSize(); ++i )
	{
		hkaAnimatedSkeleton* inst = m_activeSkeletons[i];

		// Advance the animation
		if(!m_paused || (m_paused && m_singleStep))
		{
			inst->stepDeltaTime( m_timestep );
		}

		hkaPose pose (inst->getSkeleton());
		inst->sampleAndCombineAnimations( pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin()  );

		const hkReal xPos = (0.5f + i - (m_activeSkeletons.getSize()/2.0f)) * 1.75f;

		char* compressionTypes[] = {"UNCOMPRESSED", "DELTA", "WAVELET", "SPLINE" };

		hkString compressionType = hkString( compressionTypes[i] );
		m_env->m_textDisplay->outputText3D( compressionType.cString(), xPos-1.0f, 0, -1.0f +(i%2)*-1.45f , 0xffffffff );
	
		for(int j = 0; j < m_skeleton->m_numFloatSlots; j++)
		{
			hkString s;
			s.printf("%s : %.3f", m_skeleton->m_floatSlots[j],  pose.getFloatSlotValue(j)  );
			m_env->m_textDisplay->outputText3D( s.cString(), xPos-1.0f, 0, -1.0f +(i%2)*-1.45f - (j+1)*0.18f, 0xffffffff );
		}

		// Draw
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);
		worldFromModel.m_translation.set( xPos, 0, 0);
		AnimationUtils::drawPose( pose, worldFromModel );
	}

	m_singleStep = false;

	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A single interleaved animation containing both transform (bone) tracks and float tracks is loaded, and 3 compressed versions are created. All 4 animations are played back at 1/10th speed so the float track data can be seen." ;

HK_DECLARE_DEMO(FloatTracksDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Shows float tracks being sampled.", helpString);

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

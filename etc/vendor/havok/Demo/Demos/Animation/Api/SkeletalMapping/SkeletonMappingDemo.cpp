/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/SkeletalMapping/SkeletonMappingDemo.h>

#include <Common/Base/Reflection/hkClass.h>

#include <Common/Base/Container/LocalArray/hkLocalArray.h>

// Serialization
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>



#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>

	// Skeletal Animation
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapperUtils.h>

int HK_CALL hkRemoveHighLowPrefixFilter(const char* str1, const char* str2)
{
	hkString string1(str1);
	hkString string2(str2);

	if (string1.beginsWith("High "))
	{
		string1 = string1.substr(5);
	}
	if (string2.beginsWith("High "))
	{
		string2 = string2.substr(5);
	}

	if (string1.beginsWith("Low "))
	{
		string1 = string1.substr(4);
	}
	if (string2.beginsWith("Low "))
	{
		string2 = string2.substr(4);
	}

	return string1.compareToIgnoreCase( string2 );
}

SkeletonMappingDemo::SkeletonMappingDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{

	// Disable reports: 									
	if( m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x54e4323e, false); 		
	}


	m_loader = new hkLoader();

	//
	// Setup the camera
	//
	{
		hkVector4 from(  300.0f, -60.0f,  15.0f);
		hkVector4 to  (  0.0f,  0.0f,   15.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	// Skeletons
	{

		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Mapping/bipedHighRig.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "Couldn't load high-res skeleton");
			m_highSkeleton = ac->m_skeletons[0];

		}

		{
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Mapping/bipedLowRig.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "Couldn't load low-res skeleton");
			m_lowSkeleton = ac->m_skeletons[0];

		}

	}

	// Animations
	{

		{
			// High res animation
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Mapping/bipedHighAnim.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "Couldn't load high-res animation");
			m_highAnimation = ac->m_animations[0];

			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "Couldn't load high-res binding");
			m_highBinding = ac->m_bindings[0];

			// Create an animation control
			hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl(m_highBinding);

			// Create a new animated skeleton
			m_highAnimatedSkeleton = new hkaAnimatedSkeleton( m_highSkeleton );

			// Bind the control to the skeleton
			m_highAnimatedSkeleton->addAnimationControl( control );

			// The animated skeleton now owns the control
			control->removeReference();
		}

		{
			// Low res animation
			hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Mapping/bipedLowAnim.hkx");
			hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
			HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
			hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

			HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0), "Couldn't load low-res animation");
			m_lowAnimation = ac->m_animations[0];

			HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "Couldn't load low-res binding");
			m_lowBinding = ac->m_bindings[0];

			// Create an animation control
			hkaDefaultAnimationControl* control = new hkaDefaultAnimationControl(m_lowBinding);

			// Create a new animated skeleton
			m_lowAnimatedSkeleton = new hkaAnimatedSkeleton( m_lowSkeleton );

			// Bind the control to the skeleton
			m_lowAnimatedSkeleton->addAnimationControl( control );

			// The animated skeleton now owns the control
			control->removeReference();
		}

	}

	// Add some extra constraint data on the bones
	// The only two bones that can move are the root and the pelvis
	{
		{
			hkaSkeletonUtils::lockTranslations(*m_lowSkeleton);

			const hkInt16 pelvis = hkaSkeletonUtils::findBoneWithName(*m_lowSkeleton, "Low Pelvis");
			m_lowSkeleton->m_bones[pelvis]->m_lockTranslation = false;
		}

		{
			hkaSkeletonUtils::lockTranslations(*m_highSkeleton);

			const hkInt16 pelvis = hkaSkeletonUtils::findBoneWithName(*m_highSkeleton, "High Pelvis");
			m_highSkeleton->m_bones[pelvis]->m_lockTranslation = false;
		}
	}


	setupGraphics( );

	// Create mappers
	hkaSkeletonMapperData high_low_data;
	hkaSkeletonMapperData low_high_data;
	{
		hkaSkeletonMapperUtils::Params params;

		params.m_skeletonA = m_highSkeleton;
		params.m_skeletonB = m_lowSkeleton;
		params.m_compareNames = hkRemoveHighLowPrefixFilter;

		// We enforce a chain from the start of the spine to the neck
		hkaSkeletonMapperUtils::UserChain chain;
		chain.m_start = "Pelvis";
		chain.m_end = "Neck";
		params.m_userChains.pushBack(chain);

		hkaSkeletonMapperUtils::createMapping(params, high_low_data, low_high_data);

		low_high_data.m_keepUnmappedLocal = false;
		high_low_data.m_keepUnmappedLocal = false;
	}

	m_highToLowMapper = new hkaSkeletonMapper(high_low_data);
	m_lowToHighMapper = new hkaSkeletonMapper(low_high_data);

}

SkeletonMappingDemo::~SkeletonMappingDemo()
{
	// Re-enable reports
	hkError::getInstance().setEnabled(0x54e4323e, true); 

	m_highAnimatedSkeleton->removeReference();
	m_lowAnimatedSkeleton->removeReference();
	m_highToLowMapper->removeReference();
	m_lowToHighMapper->removeReference();
	delete m_loader;

}

hkDemo::Result SkeletonMappingDemo::stepDemo()
{
	// Advance the animation of both a high-res and low-res animated skeletons
	m_highAnimatedSkeleton->stepDeltaTime( m_timestep );
	m_lowAnimatedSkeleton->stepDeltaTime ( m_timestep );

	// HIGH LOW HIGH
	{
		// Transform used for display
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY );

		// Get a high-res pose from the high resolution animated skeleton
		hkaPose poseHigh (m_highSkeleton);
		m_highAnimatedSkeleton->sampleAndCombineAnimations( poseHigh.accessUnsyncedPoseLocalSpace().begin(), poseHigh.getFloatSlotValues().begin());

		// Draw the high res pose
		AnimationUtils::drawPose( poseHigh, worldFromModel, 0x7fffffff);

		// Construct a low-res pose in the reference pose
		hkaPose poseLow (m_lowSkeleton);
		poseLow.setToReferencePose();

		// Map the high-res pos into the low-res pose
		m_highToLowMapper->mapPose(poseHigh, poseLow, hkaSkeletonMapper::CURRENT_POSE);

		// Draw the low res pose
		worldFromModel.m_translation.set(20,0,0);
		AnimationUtils::drawPose( poseLow, worldFromModel, 0x7f00ff00 );

		// Now, map the los-res pos back to the high-res pose
		m_lowToHighMapper->mapPose( poseLow, poseHigh, hkaSkeletonMapper::CURRENT_POSE);

		// Draw the high res pose
		worldFromModel.m_translation.set(40,0,0);
		AnimationUtils::drawPose( poseHigh, worldFromModel, 0x7fffffff );
	}

	// LOW HIGH LOW
	{
		// Transform used for display
		hkQsTransform worldFromModel (hkQsTransform::IDENTITY);

		// Get a low-res pose from the low resolution animated skeleton
		hkaPose poseLow (m_lowSkeleton);
		m_lowAnimatedSkeleton->sampleAndCombineAnimations( poseLow.accessUnsyncedPoseLocalSpace().begin(), poseLow.getFloatSlotValues().begin() );

		// Draw the low-res pose
		worldFromModel.m_translation.set(0, 0, 90);
		AnimationUtils::drawPose( poseLow, worldFromModel, 0x7f00ff00);

		// Construct a high-res pose in the reference pose
		hkaPose poseHigh (m_highSkeleton);
		poseHigh.setToReferencePose();

		// Map the low-res pos to the high-res
		m_lowToHighMapper->mapPose(poseLow, poseHigh, hkaSkeletonMapper::CURRENT_POSE);

		// Draw it
		worldFromModel.m_translation.set(20,0,90);
		AnimationUtils::drawPose( poseHigh, worldFromModel, 0x7fffffff );

		// Now, map the high-res pose back to the low-res pose
		m_highToLowMapper->mapPose(poseHigh, poseLow, hkaSkeletonMapper::CURRENT_POSE);

		// And draw the low-res pose
		worldFromModel.m_translation.set(40,0,90);
		AnimationUtils::drawPose( poseLow, worldFromModel, 0x7f00ff00 );
	}

	return DEMO_OK;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows a low-res skeleton being driven by an animation of a high-res skeleton";

HK_DECLARE_DEMO(SkeletonMappingDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Retargetting different rigs", helpString);

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

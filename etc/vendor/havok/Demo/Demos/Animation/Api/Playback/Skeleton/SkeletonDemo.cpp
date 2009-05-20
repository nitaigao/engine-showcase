/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/Animation/Api/Playback/Skeleton/SkeletonDemo.h>
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
#include <Animation/Animation/Rig/hkaPose.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>


SkeletonDemo::SkeletonDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env)
{
	// Load the data
	m_loader = new hkLoader();

	// Convert the scene
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from( 3,3,1 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	setupGraphics();
}

SkeletonDemo::~SkeletonDemo()
{
	// Remove scene data from memory
	delete m_loader;
}

hkDemo::Result SkeletonDemo::stepDemo()
{
	const int numBones = m_skeleton->m_numBones;

	hkaPose initialPose (m_skeleton);
	initialPose.setToReferencePose();

	AnimationUtils::drawPose( initialPose, hkQsTransform::getIdentity() );

	// Display bone names
	// Names are not zero terminated yet
	for (hkInt16 b=0; b < numBones; b++)
	{
		const hkaBone* bone = m_skeleton->m_bones[b];
		const hkVector4& pos = initialPose.getBoneModelSpace(b).getTranslation();
		m_env->m_textDisplay->outputText3D( bone->m_name, pos(0), pos(1), pos(2), 0xffffffff, 1);
	}

	return hkDemo::DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A single skeleton with bone names. ";

HK_DECLARE_DEMO(SkeletonDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "A Skeleton...wooo....", helpString);

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

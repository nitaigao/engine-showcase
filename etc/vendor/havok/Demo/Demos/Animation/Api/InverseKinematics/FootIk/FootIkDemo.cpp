/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Animation/Api/InverseKinematics/FootIk/FootIkDemo.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

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
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>

#include <Animation/Animation/Rig/hkaPose.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>

// flatland ( MOPP landscape util )
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayConvex.h>

// raycasting
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

// foot fix ik
#include <Animation/Animation/Ik/FootPlacement/hkaFootPlacementIkSolver.h>

class FootIkDemoRaycastInterface : public hkReferencedObject, public hkaRaycastInterface
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		FootIkDemoRaycastInterface (hkpMoppBvTreeShape* moppShape) : m_moppBvTreeShape(moppShape) {}

		virtual hkBool castRay ( const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut );

	private:
		hkpMoppBvTreeShape* m_moppBvTreeShape;
};

static hkVector4 UP (0,0,1);

FootIkDemo::FootIkDemo( hkDemoEnvironment* env )
:	hkDefaultAnimationDemo(env), m_ikOn(true)
{

	// Disable warnings: if no renderer									
	if( hkString::strCmp( m_env->m_options->m_renderer, "n" ) == 0 )
	{
		hkError::getInstance().setEnabled(0xf0d1e423, false); //'Could not realize an inplace texture of type PNG.'
	}

	forceShadowState(true);

	//
	// Setup the cameray
	//
	{
		hkVector4 from( -3.5, -3.5, 3.5 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 100 );
	}

	// scene loader
	m_loader = new hkLoader();

	// get the rig
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkRig.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];
	}

	// get the idle animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkIdle.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[0] =  ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[0] = ac->m_bindings[0];
	}

	// get the walk animation and the binding
	{
		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/HavokGirl/hkWalkLoop.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numAnimations > 0 ), "No animation loaded");
		m_animation[1] = ac->m_animations[0];

		HK_ASSERT2(0x27343435, ac && (ac->m_numBindings > 0), "No binding loaded");
		m_binding[1] = ac->m_bindings[0];

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

	// Create the skeleton
	m_skeletonInstance = new hkaAnimatedSkeleton( m_skeleton );

	// Set the fill threshold
	m_skeletonInstance->setReferencePoseWeightThreshold( 0.05f );

	// animation controller
	for( int aci = 0; aci < NUM_ANIMS; aci++ )
	{
		m_control[aci] = new hkaDefaultAnimationControl( m_binding[aci] );
		m_control[aci]->setPlaybackSpeed( 0.80f );

		m_skeletonInstance->addAnimationControl( m_control[aci] );
		m_control[aci]->removeReference();
	}

	// Set ease curves explicitly
	m_control[0]->setMasterWeight( 0.0f );
	m_control[0]->easeOut(0.0f);

	m_control[1]->setEaseInCurve(0, 0, 1, 1);
	m_control[1]->setEaseOutCurve(1, 1, 0, 0);
	m_control[1]->easeIn(0.0f);
	m_control[1]->setMasterWeight( 1.0f );

	// setup the graphics
	setupGraphics( );

	// create the landscape
	{
		// Create some geometry for display purposes
		{
			m_geometry = new hkGeometry;

			// landscape parameters
			int side = 32;
			int numVertices = side * side;
			m_geometry->m_vertices.setSize( numVertices );

			int numTriangles = 2 * ( side - 1 ) * ( side - 1 );
			m_geometry->m_triangles.setSize( numTriangles );

			// build the landscape given this above parameters
			buildLandscape( side );

			m_displayConvex = new hkDisplayConvex( m_geometry ); // owns the geom given.
			m_geometryArray.pushBack( m_displayConvex );

			// add the geometry to the display manager
			hkDebugDisplay::getInstance().addGeometry( m_geometryArray, hkTransform::getIdentity(), 0x1001, 0, 0);

			// set the landscape colour to green
			hkDebugDisplay::getInstance().setGeometryColor( 0xff00a030, 0x1001, 0 );
		}

		// Create a mesh for the footIK to raycast against
		{
			// create a mesh shape so that we can use the raycast functionality
			m_storageMeshShape = new hkpSimpleMeshShape( 0.01f );

			// setup the storage mesh
			m_storageMeshShape->m_vertices = m_geometry->m_vertices;
			m_storageMeshShape->m_triangles.setSize( m_geometry->m_triangles.getSize() );

			for( int ti = 0; ti < m_geometry->m_triangles.getSize(); ti++ )
			{
				m_storageMeshShape->m_triangles[ti].m_a = m_geometry->m_triangles[ti].m_a;
				m_storageMeshShape->m_triangles[ti].m_b = m_geometry->m_triangles[ti].m_b;
				m_storageMeshShape->m_triangles[ti].m_c = m_geometry->m_triangles[ti].m_c;
			}
		}
	}

	// Disable warnings about the inneficiencies of building mopp's at runtime
	hkError::getInstance().setEnabled( 0x6E8D163B, false );

	hkpMoppCode* moppCode = hkpMoppUtility::buildCode(m_storageMeshShape, hkpMoppCompilerInput());

	m_moppBvTreeShape = new hkpMoppBvTreeShape(m_storageMeshShape, moppCode);
	moppCode->removeReference();

	// character root bone World Space transform
	m_currentMotion.setIdentity();
	hkVector4 translation = m_currentMotion.getTranslation();
	translation(2) = 1.0f;//0.87f;
	m_currentMotion.setTranslation( translation );

	// find the bone indices
	{
		m_raycastInterface = new FootIkDemoRaycastInterface (m_moppBvTreeShape);

		hkaFootPlacementIkSolver::Setup setupData;

		// COMMON
		{
			setupData.m_skeleton = m_skeleton;
			setupData.m_kneeAxisLS.set(0,0,1); // Z
			setupData.m_footEndLS.set(0.0f, 0.0f, 0.0f);
			setupData.m_worldUpDirectionWS.set(0,0,1);
			setupData.m_modelUpDirectionMS.set(0,0,1);
			setupData.m_originalGroundHeightMS = -0.82f;
			setupData.m_minAnkleHeightMS = -0.9f;
			setupData.m_maxAnkleHeightMS = -0.1f;
			setupData.m_cosineMaxKneeAngle = -0.99f;
			setupData.m_cosineMinKneeAngle = 0.99f;
			setupData.m_footPlantedAnkleHeightMS = -0.8f;
			setupData.m_footRaisedAnkleHeightMS = -0.2f;
			setupData.m_raycastDistanceUp = 1.0f;
			setupData.m_raycastDistanceDown = 1.0f;
		}

		// LEFT LEG
		{
			setupData.m_hipIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L Thigh" );
			setupData.m_kneeIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L Calf" );
			setupData.m_ankleIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig L Foot" );

			m_footPlacementComponent[LEFT_LEG] = new hkaFootPlacementIkSolver(setupData);
		}

		// RIGHT LEG
		{
			setupData.m_hipIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig R Thigh" );
			setupData.m_kneeIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig R Calf" );
			setupData.m_ankleIndex = hkaSkeletonUtils::findBoneWithName( *m_skeleton, "HavokBipedRig R Foot" );

			m_footPlacementComponent[RIGHT_LEG] = new hkaFootPlacementIkSolver(setupData);
		}

	}

}


FootIkDemo::~FootIkDemo()
{
	// Re-enable warnings									
	hkError::getInstance().setEnabled(0xf0d1e423, true);

	m_skeletonInstance->removeReference();

	delete m_raycastInterface;
	delete m_footPlacementComponent[LEFT_LEG];
	delete m_footPlacementComponent[RIGHT_LEG];

	// remove our landscape from the display manager
	hkDebugDisplay::getInstance().removeGeometry( 0x1001, 0, 0 );

	// delete our geometry
	m_geometryArray.clear();
	delete m_displayConvex;

	m_storageMeshShape->removeReference();
	m_moppBvTreeShape->removeReference();

	delete m_loader;
}


hkDemo::Result FootIkDemo::stepDemo()
{

	// update ik on/off
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		m_ikOn = !m_ikOn;
	}

	// update shadows on/off
	if( m_env->m_gamePad->wasButtonPressed( HKG_PAD_BUTTON_2 ) )
	{
		forceShadowState(!m_env->m_options->m_enableShadows);
	}

	// display current ik blending state
	{
		char buf[64];
		hkString::sprintf( buf, "(\221) Foot Placement : %s. (\222) Shadows %s",
			m_ikOn ? "On" : "Off", m_env->m_options->m_enableShadows ? "On" : "Off" );
		const int h = m_env->m_window->getHeight();
		m_env->m_textDisplay->outputText( buf, 20, h-40, 0xffffffff, 1);
	}

	// rotate the character as necessary
	{
		hkReal turn = m_env->m_gamePad->getStickPosX( 1 );

		turn *= -2.0f * HK_REAL_PI / 180.0f;
		hkQuaternion q;
		q.setAxisAngle( UP, turn );
		m_currentMotion.m_rotation.mul( q );
	}

	// Grab accumulated motion
	{
		hkQsTransform deltaMotion;
		deltaMotion.setIdentity();
		m_skeletonInstance->getDeltaReferenceFrame( m_timestep, deltaMotion );

		hkQsTransform temp;
		temp.setMul( m_currentMotion, deltaMotion );
		m_currentMotion = temp;
		m_currentMotion.m_rotation.normalize();
	}

	const int boneCount = m_skeleton->m_numBones;

	// Advance the active animations
	m_skeletonInstance->stepDeltaTime( 0.016f );

	hkaPose thePose (m_skeleton);

	// Sample the active animations and combine into a single pose
	m_skeletonInstance->sampleAndCombineAnimations( thePose.accessUnsyncedPoseLocalSpace().begin(), thePose.getFloatSlotValues().begin()  );

	// FOOT IK
	hkReal errorLeft;
	hkReal errorRight;
	{

		hkaFootPlacementIkSolver::Input footPlacementInput;
		footPlacementInput.m_worldFromModel = m_currentMotion;
		footPlacementInput.m_onOffGain = 0.2f;
		// we set all gains to 1.0 since the landscape is smooth
		footPlacementInput.m_groundAscendingGain = 1.0f;
		footPlacementInput.m_groundDescendingGain = 1.0f;
		footPlacementInput.m_footRaisedGain = 1.0f;
		footPlacementInput.m_footPlantedGain = 1.0f;
		footPlacementInput.m_footPlacementOn = m_ikOn;
		footPlacementInput.m_raycastInterface = m_raycastInterface;


		// LEFT LEG
		{
			footPlacementInput.m_originalAnkleTransformMS = thePose.getBoneModelSpace(m_footPlacementComponent[LEFT_LEG]->m_setup.m_ankleIndex);

			hkaFootPlacementIkSolver::Output footPlacementOutput;
			m_footPlacementComponent[LEFT_LEG]->doFootPlacement(footPlacementInput, footPlacementOutput, thePose);

			errorLeft = footPlacementOutput.m_verticalError;
		}

		// RIGHT LEG
		{
			footPlacementInput.m_originalAnkleTransformMS = thePose.getBoneModelSpace(m_footPlacementComponent[RIGHT_LEG]->m_setup.m_ankleIndex);

			hkaFootPlacementIkSolver::Output footPlacementOutput;
			m_footPlacementComponent[RIGHT_LEG]->doFootPlacement(footPlacementInput, footPlacementOutput, thePose);

			errorRight = footPlacementOutput.m_verticalError;
		}

	}

	// Skin & render
	{
		// Grab the pose in world space
		const hkArray<hkQsTransform>& poseInWorld = thePose.getSyncedPoseModelSpace();

		// Convert accumlated info to graphics matrix
		hkTransform graphicsTransform;
		m_currentMotion.copyToTransform(graphicsTransform);

		// Construct the composite world transform
		hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
		compositeWorldInverse.setSize( boneCount );

		// Skin the meshes
		for (int i=0; i < m_numSkinBindings; i++)
		{
			// assumes either a straight map (null map) or a single one (1 palette)
			hkInt16* usedBones = m_skinBindings[i]->m_mappings? m_skinBindings[i]->m_mappings[0].m_mapping : HK_NULL;
			int numUsedBones = usedBones? m_skinBindings[i]->m_mappings[0].m_numMapping : boneCount;

			// Multiply through by the bind pose inverse world inverse matrices
			for (int p=0; p < numUsedBones; p++)
			{
				int boneIndex = usedBones? usedBones[p] : p;
				compositeWorldInverse[p].setMul( poseInWorld[ boneIndex ], m_skinBindings[i]->m_boneFromSkinMeshTransforms[ boneIndex ] );
			}

			AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, graphicsTransform, compositeWorldInverse.begin(), *m_env->m_sceneConverter );
		}
	}

	// Move the character vertically depending on the error
	// We use the lowest/negative errors (going down) since IK can solve positive errors (going up) easier
	{
		m_currentMotion.m_translation(2) += hkMath::min2(errorLeft, errorRight)  * 0.2f;
	}

	return hkDemo::DEMO_OK;
}

hkBool FootIkDemoRaycastInterface::castRay( const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut)
{

	// raycast info structures
	hkpShapeRayCastInput raycastIn;
	hkpShapeRayCastOutput raycastOut;

	raycastIn.m_from = fromWS;
	raycastIn.m_to = toWS;

	// cast the ray into the landscape
	m_moppBvTreeShape->castRay( raycastIn, raycastOut );

	// check if we've hit anything
	if( raycastOut.hasHit() )
	{
		hitFractionOut = raycastOut.m_hitFraction;
		normalWSOut = raycastOut.m_normal;
		return true;
	}

	else
	{
		// did not hit anything
		return false;
	}

}


static hkReal getLandscapeHeight( int i, int j )
{
	// return a smooth rolling value for the landscape
	hkReal h = ( hkMath::cos( 7.25f * ( (hkReal)j + i ) ) ) + ( 0.5f * ( hkMath::sin( 4.0f * i ) ) );
	h = h + 1.1f;

	return h * 0.3f;

}

void FootIkDemo::buildLandscape( int side )
{
	// create a rolling landscape for our character to walk across

	// create the vertices
	{
		const float offset = 0.0f;
		{
			for( int i = 0; i < side; i++ )
			{
				for( int j = 0; j < side; j++ )
				{
					float h = getLandscapeHeight( i, j );

					m_geometry->m_vertices[ ( i * side + j ) ](0) = i * 1.0f - side * 0.5f;
					m_geometry->m_vertices[ ( i * side + j ) ](2) = h - offset;
					m_geometry->m_vertices[ ( i * side + j ) ](1) = j * 1.0f - side * 0.5f;
					m_geometry->m_vertices[ ( i * side + j ) ](3) = 0.0f;
				}
			}
		}
	}

	// Create the triangles
	{
		int index = 0;
		hkUint16 corner = 0;

		for( int i = 0; i < side - 1; i++ )
		{
			for( int j = 0; j < side - 1; j++ )
			{
				m_geometry->m_triangles[index].set( hkUint16(corner), hkUint16(corner + side), hkUint16(corner + 1));
				m_geometry->m_triangles[ index + 1 ].set( hkUint16(corner + 1), hkUint16(corner + side), hkUint16(corner + side + 1) );

				index += 2;
				corner++;
			}

			corner++;
		}
	}

}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A simple foot placement demo that uses an ik solver to position the character's feet on the surface " \
"of the landscape. Press button \221 to switch on IK.";

HK_DECLARE_DEMO(FootIkDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Foot Placement Example", helpString);

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

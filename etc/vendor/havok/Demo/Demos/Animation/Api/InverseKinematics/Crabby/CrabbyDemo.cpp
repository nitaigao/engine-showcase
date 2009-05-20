/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Animation/Api/InverseKinematics/Crabby/CrabbyDemo.h>

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
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Collision Shapes
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// IK
#include <Animation/Animation/Ik/ThreeJoints/hkaThreeJointsIkSolver.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>

// raycasting
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>

// Common animation Utilities
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// Graphics Stuff
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Common/Window/hkgWindow.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayConvex.h>

static const hkVector4 ORIGIN(0,0,0);
static const hkVector4 UP(0,0,1);
static const hkVector4 DOWN(0,0,-1);
static const hkVector4 FORWARD(1,0,0);
static const hkVector4 BACK(-1,0,0);
static const hkVector4 RIGHT(0,1,0);
static const hkVector4 LEFT(0,-1,0);

static const hkInt16 TERRAIN_SIDE(64); // side of the terrain

static const hkReal realBODYDIST(2.0f); // attitude of body on terrain
static const hkVector4 vecBODYDIST(0.0f,0.0f,realBODYDIST);

// function returns
static void _getPositionOrientation( const hkVector4& p0, const hkVector4& p1, const hkVector4& p2, const hkVector4& p3, hkReal& heighOut, hkVector4& normalOut, hkRotation& rotInOut )
{
	// 3D Least Square solution - optimal plane in 3D space
	// Plane normal is a solution of matrix equation At*A*x=At*b
	// The trick is: plane at 3D space Ax+By+D=-Cz -> (A/C)x+(B/C)y+D/C=-z

	// Set right site vector b = -z
	hkVector4 b;

	{
		b.set(p0(2),p1(2),p2(2),p3(2));
		b.mul4(-1.0f);
	}

	hkMatrix4 A;
	hkMatrix4 At;
	hkMatrix4 AtxA4;

	{
		A.setRows(p0,p1,p2,p3);

		// third column is equal to 1
		A(0,2) = 1.0f; A(1,2) = 1.0f; A(2,2) = 1.0f; A(3,2) = 1.0f;
		// four column to zeros
		A(0,3) = 0.0f; A(1,3) = 0.0f; A(2,3) = 0.0f; A(3,3) = 0.0f;

		// Set transpose A matrix
		At.setTranspose(A);

		// bug in hkMatrix4, method setTranspose() forces last column to be (0,0,0,1);
		At(3,3) = 0.0f;

		AtxA4.setMul(At,A);

	}

	// create 3x3 hkMatrix3 AtxA3 from 4x4 hkMatrix4
	hkMatrix3 AtxA3;

	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				AtxA3(i,j) = AtxA4(i,j);
			}
		}
	}

	// create right side of equation hkVector4 Atxb
	hkVector4 Atxb;

	{
		Atxb.setZero4();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Atxb(i)+=At(i,j)*b(j);
			}
		}
	}


	// solve system of linear equation and get generalized plane coefficients
	{
		AtxA3.invert(HK_REAL_EPSILON);

		hkVector4 coeff;
		coeff.setMul3(AtxA3,Atxb);

		const hkReal lenCoeff = hkMath::sqrt( coeff(0)*coeff(0) + coeff(1)*coeff(1) + 1.0f);

		hkVector4 normal;

		normal(0) = coeff(0)/lenCoeff;
		normal(1) = coeff(1)/lenCoeff;
		normal(2) = 1.0f/lenCoeff;

		//const hkReal p = coeff(2)/lenCoeff;

		// create rotated coordinate system
		hkVector4 axisZ;
		axisZ = normal;
		axisZ.normalize3();

		hkVector4 axisY;
		axisY.setCross(axisZ , FORWARD);
		axisY.normalize3();

		hkVector4 axisX;
		axisX.setCross(axisY,axisZ);
		axisX.normalize3();

		// set normal out
		{
			normalOut = axisZ;
		}

		// calculate z position
		{
			//heighOut = -p/normal(2);
			heighOut = 0.25f * ( p0(2)+p1(2)+p2(2)+p3(2) );
		}
		// update inout rotation matrix
		{
			hkMatrix3 rot; rot.setCols( axisX, axisY , axisZ );
			rotInOut.mul(rot);
		}
	}

}

class CrabbyDemoLegMovement
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, CrabbyDemoLegMovement);

		CrabbyDemoLegMovement (hkReal radius, hkReal delay, const hkVector4& x, const hkVector4& y, const hkVector4& z )
		: m_radius(radius), m_rotAngle(delay)
		{
			m_rotation.setCols(x,y,z);
		}

		void getLegMovement(hkReal vel, hkReal timestep,  hkVector4& outMoveVec );

	private:

		hkReal m_radius;
		hkReal m_rotAngle;

		hkRotation m_rotation;

};

void CrabbyDemoLegMovement::getLegMovement(hkReal vel, hkReal timestep,  hkVector4& outMoveVec )
{
	const hkReal drot =  vel/m_radius * timestep;

	m_rotAngle += drot;

	hkVector4 vec;

	vec(0) = m_radius * hkMath::cos(m_rotAngle);
	vec(1) = m_radius * hkMath::sin(m_rotAngle);
	vec(2) = 0.0f;

	// transform to coordinate system related to model
	outMoveVec.setMul3( m_rotation,vec );
}

CrabbyDemo::CrabbyDemo(hkDemoEnvironment* env)
:	hkDefaultAnimationDemo(env)
{

	forceShadowState(true);

	// Load the data
	m_loader = new hkLoader();

	// Convert the scene and get the skeleton ans skins
	{

		hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/SpiderCrab/crabby_the_spider.hkx");
		hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
		HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");
		hkaAnimationContainer* ac = reinterpret_cast<hkaAnimationContainer*>( container->findObjectByType( hkaAnimationContainerClass.getName() ));

		HK_ASSERT2(0x27343435, ac && (ac->m_numSkeletons > 0), "No skeleton loaded");
		m_skeleton = ac->m_skeletons[0];

		HK_ASSERT2(0x27343435, ac->m_numSkins > 0, "No skins loaded");
		m_skinBindings = ac->m_skins;
		m_numSkinBindings = ac->m_numSkins;

		hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
		HK_ASSERT2(0x27343435, scene , "No scene loaded");

		// Make graphics output buffers for the skins
		env->m_sceneConverter->convert( scene, hkgAssetConverter::NO_MATERIALS );

	}

	// setup the lights and shadows
	{
		//hkDefaultDemo::setupLights(env);

		// want to see shadows on crab feet etc
		forceShadowState(true);

	}

	// Setup the camera
	{
		hkVector4 from( 20,-20,20 );
		hkVector4 to  ( 0,0,0 );
		hkVector4 up  ( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up, 0.1f, 1000.0f );

	}

	setupGraphics();

	// create the landscape
	{

		// Create some geometry for display purposes
		{
			m_geometry = new hkGeometry;

			// landscape parameters
			//int side = 64;
			int numVertices = TERRAIN_SIDE * TERRAIN_SIDE;
			m_geometry->m_vertices.setSize( numVertices );

			int numTriangles = 2 * ( TERRAIN_SIDE - 1 ) * ( TERRAIN_SIDE - 1 );
			m_geometry->m_triangles.setSize( numTriangles );

			// build the landscape given this above parameters
			buildLandscape( TERRAIN_SIDE );

			m_displayConvex = new hkDisplayConvex( m_geometry ); // owns the geom given.
			m_geometryArray.pushBack( m_displayConvex );

			// add the geometry to the display manager
			hkDebugDisplay::getInstance().addGeometry( m_geometryArray, hkTransform::getIdentity(), 0x1001, 0, 0);

			// set the landscape color to green
			hkDebugDisplay::getInstance().setGeometryColor( 0xff00a030, 0x1001, 0 );
		}

		// Create a mesh to raycast against
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

	// setup mesh and raycasting
	{
		hkpMoppCode* moppCode = hkpMoppUtility::buildCode(m_storageMeshShape, hkpMoppCompilerInput());

		m_moppBvTreeShape = new hkpMoppBvTreeShape(m_storageMeshShape, moppCode);
		moppCode->removeReference();

	}

	// create crabby
	{

		// create model pose
		m_crabbyModelPose = new hkaPose(m_skeleton);
		m_crabbyModelPose->setToReferencePose();

		//create solvers and setups and add them to storage arrays

		// create front left leg
		{
			hkaThreeJointsIkSolver::Setup setupFrontLeft;

			setupFrontLeft.m_hingeAxisMS = UP;

			setupFrontLeft.m_firstJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontLeft1");
			setupFrontLeft.m_secondJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontLeft2");
			setupFrontLeft.m_thirdJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontLeft3");
			setupFrontLeft.m_endBoneIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontLeft4");

			m_targetPointBoneIndexes[0] = setupFrontLeft.m_endBoneIdx; // set first target point id

			hkaThreeJointsIkSolver* solverFrontLeft = new hkaThreeJointsIkSolver(setupFrontLeft, *m_crabbyModelPose);
			m_solvers.pushBack(solverFrontLeft);

		}

		// create front right leg
		{
			hkaThreeJointsIkSolver::Setup setupFrontRight;

			setupFrontRight.m_hingeAxisMS = UP;

			setupFrontRight.m_firstJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontRight1");
			setupFrontRight.m_secondJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontRight2");
			setupFrontRight.m_thirdJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontRight3");
			setupFrontRight.m_endBoneIdx= hkaSkeletonUtils::findBoneWithName (*m_skeleton, "frontRight4");

			m_targetPointBoneIndexes[1] = setupFrontRight.m_endBoneIdx; // set second target point id

			hkaThreeJointsIkSolver* solverFrontRight = new hkaThreeJointsIkSolver( setupFrontRight, *m_crabbyModelPose );
			m_solvers.pushBack(solverFrontRight);

		}

		// create back left leg
		{
			hkaThreeJointsIkSolver::Setup setupBackLeft;

			setupBackLeft.m_hingeAxisMS = UP;

			setupBackLeft.m_firstJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backLeft1");
			setupBackLeft.m_secondJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backLeft2");
			setupBackLeft.m_thirdJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backLeft3");
			setupBackLeft.m_endBoneIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backLeft4");

			m_targetPointBoneIndexes[2] = setupBackLeft.m_endBoneIdx; // set third target point id

			hkaThreeJointsIkSolver* solverBackLeft = new hkaThreeJointsIkSolver( setupBackLeft, *m_crabbyModelPose );
			m_solvers.pushBack(solverBackLeft);

		}

		// create back right leg
		{
			hkaThreeJointsIkSolver::Setup setupBackRight;

			setupBackRight.m_hingeAxisMS = UP;

			setupBackRight.m_firstJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backRight1");
			setupBackRight.m_secondJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backRight2");
			setupBackRight.m_thirdJointIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backRight3");
			setupBackRight.m_endBoneIdx = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "backRight4");

			m_targetPointBoneIndexes[3] = setupBackRight.m_endBoneIdx; // fourth target point id

			hkaThreeJointsIkSolver* solverBackRight = new hkaThreeJointsIkSolver( setupBackRight, *m_crabbyModelPose );
			m_solvers.pushBack(solverBackRight);

		}

		// setup crabby leg movement
		{
			const hkReal radiusLegMovement = 1.0f;

			// front left leg
			CrabbyDemoLegMovement* legFrontLeft;
			legFrontLeft = new CrabbyDemoLegMovement ( radiusLegMovement , 0.0f, LEFT, UP, FORWARD );
			m_legMovement.pushBack(legFrontLeft);

			// front right leg
			CrabbyDemoLegMovement* legFrontRight;
			legFrontRight = new CrabbyDemoLegMovement ( radiusLegMovement , HK_REAL_PI/2.0f, LEFT, UP, FORWARD );
			m_legMovement.pushBack(legFrontRight);

			// back left leg
			CrabbyDemoLegMovement* legBackLeft;
			legBackLeft = new CrabbyDemoLegMovement ( radiusLegMovement , HK_REAL_PI, LEFT, UP, FORWARD );
			m_legMovement.pushBack(legBackLeft);

			// back right leg
			CrabbyDemoLegMovement* legBackRight;
			legBackRight = new CrabbyDemoLegMovement ( radiusLegMovement , 3.0f*HK_REAL_PI/2.0f, LEFT, UP, FORWARD );
			m_legMovement.pushBack(legBackRight);

		}

		// setup init target points
		{
			for(int i = 0; i < NLEGS; i++)
			{
				m_targetPointsBonesMS[i] = m_crabbyModelPose->getBoneModelSpace(m_targetPointBoneIndexes[i]).getTranslation();
			}
		}

	} // end create crabby

	// setup motion control variables
	{

		// init position and rotation
		const hkReal initTurn = 0.0f;
		const hkVector4 initPosition( 0.0f, 0.0f, 0.0f );

		// init position and rotation
		m_time = 0.0f;
		m_vel = 0.0f;

		m_bodyPlaneNormal = UP;

		// setup init motion transformation
		m_turn = initTurn;
		hkQuaternion initQ; initQ.setAxisAngle( m_bodyPlaneNormal, m_turn );
		m_currentMotion.m_rotation = initQ;
		m_currentMotion.m_translation = initPosition;

		// setup init worldFromModel transformation
		m_worldFromModel.setIdentity();
		m_worldFromModel.m_rotation = m_currentMotion.m_rotation;
		m_worldFromModel.m_translation = m_currentMotion.m_translation;
		m_modelFromWorld.setInverse(m_worldFromModel);


		hkVector4 initTargetPointWS[NLEGS];

		{
			for (int i = 0; i < (NLEGS); i++)
			{
				// transform points to world space
				initTargetPointWS[i].setTransformedPos( m_worldFromModel, m_targetPointsBonesMS[i] );

				// do raycast
				doRaycast( initTargetPointWS[i] );
			}
		}

		// calculate final init rotation and position(z)
		{

			hkReal highWS;
			hkRotation rotation; rotation.setIdentity();

			_getPositionOrientation( initTargetPointWS[0], initTargetPointWS[1], initTargetPointWS[2], initTargetPointWS[3], highWS, m_bodyPlaneNormal, rotation);

			//correction - distance body center from surface plane
			{
				hkVector4 bodyDistWS;
				bodyDistWS.setMul3( rotation, vecBODYDIST );
				highWS += bodyDistWS(2);
			}
			// or use simplify version
			//highWS += realBODYDIST;

			hkVector4 translation;  translation = m_currentMotion.getTranslation();
			translation(2) += highWS;
			m_currentMotion.setTranslation( translation );
  			m_currentMotion.m_rotation.setAxisAngle( m_bodyPlaneNormal, m_turn ) ;

		}

		// update worldFromModel transformation with new rotation
		m_worldFromModel.m_rotation = m_currentMotion.m_rotation;
		m_worldFromModel.m_translation = m_currentMotion.m_translation;
		m_modelFromWorld.setInverse(m_worldFromModel);

	}


}

template <typename T>
static void removeReferenceArrayElements(hkArray<T*>& arr)
{
	for( int i = 0; i < arr.getSize(); ++i )
	{
		arr[i]->removeReference();
	}
}

template <typename T>
static void deleteArrayElements(hkArray<T*>& arr)
{
	for( int i = 0; i < arr.getSize(); ++i )
	{
		delete arr[i];
	}
}

CrabbyDemo::~CrabbyDemo()
{
	delete m_crabbyModelPose;

	removeReferenceArrayElements(m_solvers);
	deleteArrayElements(m_legMovement);

	// remove our landscape from the display manager
	hkDebugDisplay::getInstance().removeGeometry( 0x1001, 0, 0 );

	// delete our geometry
	delete m_displayConvex;

	m_storageMeshShape->removeReference();
	m_moppBvTreeShape->removeReference();

	delete m_loader;
}

hkDemo::Result CrabbyDemo::stepDemo()
{

	// update local time
	m_time += m_timestep;

	// check for any input changes
	{
		const hkReal incTurn = 0.03f;
		const hkReal incMove = 0.05f;

		const hkgPad* pad = m_env->m_gamePad;

		if ((m_env->m_gamePad->getButtonState() & HKG_PAD_DPAD_LEFT) != 0)
		{
			m_turn += incTurn;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_RIGHT) != 0)
		{
			m_turn -= incTurn;
		}

		if ((pad->getButtonState() & HKG_PAD_DPAD_DOWN) != 0)
		{
			m_vel -= incMove;
		}
		if ((pad->getButtonState() & HKG_PAD_DPAD_UP) != 0)
		{
			m_vel += incMove;
		}
	}

	// move and rotate pose as necessary
	{
		// set rotation
		hkRotation rotation;
		rotation.setAxisAngle( m_bodyPlaneNormal, m_turn );

		m_currentMotion.m_rotation.set( rotation );

		// only forward move direction allowed
		m_vel = m_vel > 0.0f ? m_vel : 0.0f;

		// bound movement on terrain
		{
			const hkReal terrainBoundary = 0.5f * TERRAIN_SIDE - 6.0f;

			if ( (hkMath::fabs( m_currentMotion.m_translation(0)) > terrainBoundary) ||
				 (hkMath::fabs( m_currentMotion.m_translation(1)) > terrainBoundary))
			{
				hkVector4 toOriginDir;
				toOriginDir.setNeg3( m_currentMotion.getTranslation() );
				toOriginDir.normalize3();

				hkVector4 fromBodyDir;
				fromBodyDir.setRotatedDir( m_currentMotion.getRotation(), RIGHT );
				fromBodyDir.normalize3();

				const hkReal cosAngle = toOriginDir.dot3( fromBodyDir );
				const hkReal angle = hkMath::acos(cosAngle);

				// stop if moving direction is out of terrain
				m_vel =  (angle < (0.25f*HK_REAL_PI) ) ? m_vel : 0.0f;

			}
		}

		// set translation // model is rotated around Z axis about pi/2
		hkVector4 dpos;

		dpos(0) = cos(m_turn + 0.5f*HK_REAL_PI )*m_vel*m_timestep;
		dpos(1) = sin(m_turn + 0.5f*HK_REAL_PI )*m_vel*m_timestep;
		dpos(2) = 0.0f;

		m_currentMotion.m_translation.add4(dpos);

	}

	// update according to heightfield surface
	{
		hkVector4 targetPointWS;
		hkVector4 arrayTargetPointWS[NLEGS]; // fifth point is body point

		{
			for (int i = 0; i < (NLEGS); i++)
			{
				// transform points to world space
				arrayTargetPointWS[i].setTransformedPos( m_worldFromModel, m_targetPointsBonesMS[i] );

				// do raycast
				doRaycast( arrayTargetPointWS[i] );

			}
		}

		hkReal heighZPlane;
		hkVector4 heighZBody;

		hkRotation rotation;  rotation.set( m_currentMotion.getRotation() );

		_getPositionOrientation(arrayTargetPointWS[0],arrayTargetPointWS[1],arrayTargetPointWS[2],arrayTargetPointWS[3], heighZPlane, m_bodyPlaneNormal, rotation);

		// get translationZBody
		{

			// transform points to world space
			const hkInt16 index = hkaSkeletonUtils::findBoneWithName (*m_skeleton, "body");
			heighZBody.setTransformedPos( m_worldFromModel,m_crabbyModelPose->getBoneModelSpace(index).getTranslation());

			// do raycast
			doRaycast( heighZBody );
		}

		// calculate final height
		{
			// Z translation is finally calculated as weighted mean from tranlationZPlane and translationZBody
			// mean = w1*z1 + w2*z2 / (w1+w2)
			const hkReal w1 = 0.4f;
			const hkReal w2 = 0.6f;
			const hkReal wsum = w1 + w2;

			hkReal meanHeigh = ( w1*heighZPlane + w2*heighZBody(2) ) / wsum ;

	        //correction - distance body center from surface plane
			{
				hkVector4 bodyDistWS;
				bodyDistWS.setMul3( rotation, vecBODYDIST );
				meanHeigh += bodyDistWS(2);
			}
			// or use simplify version
			//meanHeigh += realBODYDIST;


			// update current translation and rotation with final height
			hkVector4 translation;	translation = m_currentMotion.getTranslation();
			translation(2) = meanHeigh;
			m_currentMotion.setTranslation( translation );
			m_currentMotion.m_rotation.set( rotation );

		}


	}

	// update world to model transformation
	{
		m_worldFromModel.m_rotation = m_currentMotion.getRotation();
		m_worldFromModel.m_translation = m_currentMotion.getTranslation();
		m_modelFromWorld.setInverse(m_worldFromModel);
	}

	// core calculation
	{
		hkInt16	id = 0;

		hkVector4 targetPointWS;
		hkVector4 targetPointBeforeRayMS;
		hkVector4 targetPointAfterRayMS;

        hkVector4 arrayTargetPointWS[NLEGS];

		hkVector4 legMoveMS;

		hkArray<hkaThreeJointsIkSolver*>::iterator solver;

		for ( solver = m_solvers.begin(); solver != m_solvers.end(); solver++ )
		{

			// calculate leg movement depended on current velocity and timestep
			m_legMovement[id]->getLegMovement( m_vel, m_timestep, legMoveMS );

			// add leg movement contrib to model space target point
			targetPointBeforeRayMS.setAdd4( m_targetPointsBonesMS[id],legMoveMS );

			// transform to world space
			targetPointWS.setTransformedPos( m_worldFromModel, targetPointBeforeRayMS );

			// do raycast in world space
			doRaycast( targetPointWS );

			HK_DISPLAY_STAR(targetPointWS, 0.1f, hkColor::BLUE);

			// transform to model space
			targetPointAfterRayMS.setTransformedPos( m_modelFromWorld, targetPointWS );

			// do steps, don't follow surface if zBefore > zAfter
			targetPointAfterRayMS(2) = (targetPointBeforeRayMS(2) >  targetPointAfterRayMS(2)) ? targetPointBeforeRayMS(2) : targetPointAfterRayMS(2);

			// solve IK
			(*solver)->solve( targetPointAfterRayMS, *m_crabbyModelPose );

			id++;
		}

	}

	// Create World Pose
	hkaPose theWorldPose (m_skeleton);

	// transform pose in model space to world space
	{
		hkArray<hkQsTransform> arrayWorldPose( m_skeleton->m_numBones );
		hkaSkeletonUtils::transformModelPoseToWorldPose (m_skeleton->m_numBones, m_worldFromModel, m_crabbyModelPose->getSyncedPoseModelSpace().begin(), arrayWorldPose.begin());

		// plot World space skeleton
		theWorldPose.setPoseModelSpace(arrayWorldPose);

		// plot model pose
		//AnimationUtils::drawPose( *m_crabbyModelPose , hkQsTransform::getIdentity() );
		// plot world pose
		AnimationUtils::drawPose( theWorldPose , hkQsTransform::getIdentity() );
	}

	// Construct the composite world transform
	{
		hkInt32 boneCount; boneCount = m_skeleton->m_numBones;

		hkLocalArray<hkTransform> compositeWorldInverse( boneCount );
		compositeWorldInverse.setSize( boneCount );

		const hkArray<hkQsTransform>& poseInWorld = theWorldPose.getSyncedPoseModelSpace();

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

			AnimationUtils::skinMesh( *m_skinBindings[i]->m_mesh, hkTransform::getIdentity(), compositeWorldInverse.begin(), *m_env->m_sceneConverter );

		}
	}

	// Display star at origin
	//HK_DISPLAY_STAR(ORIGIN, 20.0, hkColor::RED);

	return DEMO_OK;	// original animation demo return
}

void CrabbyDemo::doRaycast( hkVector4& pointWS )
{

	const hkReal rayUpDistance = 10.0f;
	const hkReal rayDownDistance = 10.0f;
	const hkReal rayLength = rayUpDistance + rayDownDistance;

	// ray cast
	hkBool rayHit = false;
	hkVector4 rayNormalWS;
	hkReal rayFraction = HK_REAL_MAX;

	{
		// setup the raycast endpoints
		hkVector4 legToPointWS;    legToPointWS.setAddMul4( pointWS, m_bodyPlaneNormal, -rayDownDistance );
		hkVector4 legFromPointWS;	legFromPointWS.setAddMul4( pointWS, m_bodyPlaneNormal,  rayUpDistance );

		rayHit = castRay (legFromPointWS, legToPointWS, rayFraction, rayNormalWS);

		if ( rayHit )
		{
			pointWS.setAddMul4( legFromPointWS, m_bodyPlaneNormal, (-rayLength*rayFraction) );
		}

	}


}

hkBool CrabbyDemo::castRay( const hkVector4& fromPointWS, const hkVector4& toPointWS, hkReal& hitFractionOut, hkVector4& normalWSOut)
{

	// raycast info structures
	hkpShapeRayCastInput raycastIn;
	hkpShapeRayCastOutput raycastOut;

	raycastIn.m_from = fromPointWS;
	raycastIn.m_to = toPointWS;

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

hkReal CrabbyDemo::getLandscapeHeight( int i, int j )
{
	// return a smooth rolling value for the landscape
	hkReal h =  5.0f * ( hkMath::cos( 0.2f * hkReal(i) ) ) * ( hkMath::cos( 0.1f * hkReal(j) ) );
	//hkReal h = 0.5f*( hkMath::cos( 0.5f * ( (hkReal)j + i ) ) ) + ( 0.5f * ( hkMath::sin( 1.0f * i ) ) );

	return h;

}

void CrabbyDemo::buildLandscape( int side )
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
"Crabby - Inverse kinematics of three joints system (3 hinges + plane rotation)";

HK_DECLARE_DEMO(CrabbyDemo, HK_DEMO_TYPE_ANIMATION | HK_DEMO_TYPE_SERIALIZE, "Crabby - 3 joints IK solver example", helpString);

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

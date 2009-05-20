/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CRABBYDEMO_H
#define HK_CRABBYDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// display
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>

#define NLEGS	4

class CrabbyDemo : public hkDefaultAnimationDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CrabbyDemo(hkDemoEnvironment* env);
		
		~CrabbyDemo();

		Result stepDemo(); 

	private:

		void buildLandscape( int side );
		hkReal getLandscapeHeight( int i, int j );

		void doRaycast( hkVector4& pointWS );		
		hkBool castRay( const hkVector4& fromPointWS, const hkVector4& toPointWS, hkReal& hitFractionOut, hkVector4& normalWSOut);
		
		// 
		class hkLoader* m_loader;

		class hkaSkeleton* m_skeleton;
		
		class hkaAnimation* m_animation;
		class hkaAnimationBinding* m_binding;

		class hkaMeshBinding** m_skinBindings;

		hkInt32 m_numSkinBindings;

		// Local time
		hkReal m_time;

		// velocity and turn
		hkReal m_turn;
		hkReal m_vel;
		
		// control the spider movement
		hkQsTransform m_currentMotion;

		// spider body plane normal
		hkVector4 m_bodyPlaneNormal;
		
		// to/from world model transform
		hkQsTransform m_worldFromModel;
		hkQsTransform m_modelFromWorld;

		// model and world poses
		hkaPose*	m_crabbyModelPose;
		
         //IK solvers and setups
		hkArray<class hkaThreeJointsIkSolver*> m_solvers;
		
		// vector of indexes of endJoints and initial endJoints positions
		hkInt16 m_targetPointBoneIndexes[NLEGS];
		hkVector4 m_targetPointsBonesMS[NLEGS];

		// array of legs movement actuators
		hkArray<class CrabbyDemoLegMovement*> m_legMovement;
				
		// Display geometry for the landscape
		hkGeometry* m_geometry;

		// allows the geometry to be handled by the display manager
		class hkDisplayConvex* m_displayConvex;
		hkArray<class hkDisplayGeometry*> m_geometryArray;

		// Havok shape used to gain raycast functionality
		hkpSimpleMeshShape* m_storageMeshShape;
		class hkpMoppBvTreeShape* m_moppBvTreeShape;
		
		
};

#endif	// HK_CRABBYDEMO_H

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

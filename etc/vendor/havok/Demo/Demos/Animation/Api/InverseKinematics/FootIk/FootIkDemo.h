/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef FOOT_IK_DEMO_H
#define FOOT_IK_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>
#include <Demos/DemoCommon/Utilities/Animation/AnimationUtils.h>

// display
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>

#define NUM_ANIMS 2

class hkaPose;
class hkaFootPlacementIkSolver;

class FootIkDemo : public hkDefaultAnimationDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		FootIkDemo( hkDemoEnvironment* env );

		~FootIkDemo();

		Result stepDemo(); 


	protected:

		// bones indices
		enum WhichLeg
		{
			LEFT_LEG,
			RIGHT_LEG,
			MAX_LEGS
		};

		// creates the triangulated landscape
		void buildLandscape( int side );


	private:

		class hkLoader* m_loader;

		class hkaSkeleton* m_skeleton;

		class hkaAnimation* m_animation[ NUM_ANIMS ];

		class hkaMeshBinding** m_skinBindings;

		hkInt32 m_numSkinBindings;

		class hkaAnimatedSkeleton* m_skeletonInstance;

		class hkaAnimationBinding* m_binding[ NUM_ANIMS ];

		class hkaDefaultAnimationControl* m_control[ NUM_ANIMS ];

		// Display geometry for the landscape
		hkGeometry* m_geometry;

		// allows the geometry to be handled by the display manager
		class hkDisplayConvex* m_displayConvex;
		hkArray<class hkDisplayGeometry*> m_geometryArray;

		// Havok shape used to gain raycast functionality
		hkpSimpleMeshShape* m_storageMeshShape;
		class hkpMoppBvTreeShape* m_moppBvTreeShape;

		// controls the character movement
		hkQsTransform m_currentMotion;

		// foot placement info
		class hkaRaycastInterface* m_raycastInterface;
		hkaFootPlacementIkSolver* m_footPlacementComponent [MAX_LEGS];
		hkBool m_ikOn;

};

#endif // FOOT_IK_DEMO_H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef CHARACTER_ATTACHMENTS_H
#define CHARACTER_ATTACHMENTS_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>

#include <Animation/Animation/Rig/hkaPose.h>

#include <Animation/Ragdoll/Controller/RigidBody/hkaKeyFrameHierarchyUtility.h>

// Helper to view collision filter setup
#include <Demos/DemoCommon/Utilities/GameUtils/CollisionFilterViewerUtil/CollisionFilterViewerUtil.h>

class hkaRagdollInstance;
class hkaAnimatedSkeleton;
class hkaSkeletonMapper;

// This demo shows how to compensate for undesired motion of attached bodies (due to, for example, excessive acclerations in 
// character motion) by selectively using appropriate constraint and constraint limits, inertia scaling and angular damping.
class CharacterAttachments : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CharacterAttachments(hkDemoEnvironment* env);
		
		~CharacterAttachments();

		hkDemo::Result stepDemo();

		void handleInput();

		hkDemo::Result stepDemoInternal(hkBool settling );

	protected:

		void driveRagoll(const hkaPose& ragdollPose);

			// Add and remove viewers at runtime.
		void addViewer(const char* name, int& tag);
		void removeViewer(int& tag);
		
			// Helper to try and avoid a sudden snap when turning on driving for the ragdoll
		void shiftAllBodiesToAnimationPoseCenter();

		class hkLoader* m_loader; // Animation
		hkPackfileReader::AllocatedData* m_loadedData; // Physics

	
			// Animation
		class hkaDefaultAnimationControl* m_control;
		class hkaAnimatedSkeleton* m_animatedSkeleton;

			// Ragdoll
		class hkaRagdollInstance* m_ragdollInstance;
		class hkaSkeleton* m_ragdollSkeleton;
		class hkaSkeletonMapper* m_highToLowMapper; 

			// Worker data which must have lifetime of ragdoll
		hkaKeyFrameHierarchyUtility::WorkElem m_oldKeyFrameHierarchyData[100];

		// The ragdoll and attached bodies - stored for later access
		hkArray<hkpRigidBody*> m_dynamicBodies;

		hkBool m_initializeKeyframing;
		hkBool m_playing;
		hkBool m_driving;

		hkQsTransform m_accumulatedMotion;

			// Helper viewer to see collisions enabled.
		CollisionFilterViewerUtil m_filterViewer;

		int m_constraintViewerTag;
		int m_inertiaViewerTag;
		int m_numViewersOn;
};

#endif // CHARACTER_ATTACHMENTS_H

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

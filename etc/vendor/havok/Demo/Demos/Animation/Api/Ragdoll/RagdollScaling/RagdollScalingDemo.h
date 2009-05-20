/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RAGDOLL_SCALING_DEMO_H
#define HK_RAGDOLL_SCALING_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

class RagdollScalingDemo : public hkDefaultPhysicsDemo
{
    public:

        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

        RagdollScalingDemo(hkDemoEnvironment* env);

        ~RagdollScalingDemo();

        hkDemo::Result stepDemo();

    private:


        class hkLoader* m_loader;

		const class hkaSkeleton* m_ragdollSkeleton;
		const class hkaSkeleton* m_animationSkeleton;
		class hkaAnimationBinding* m_binding;
		class hkaDefaultAnimationControl* m_control;
		class hkaAnimatedSkeleton* m_character;
		class hkaRagdollInstance* m_ragdoll;
   		class hkaSkeletonMapper* m_ragdollFromAnimation;
		class hkaSkeletonMapper* m_animationFromRagdoll;
		class hkaRagdollRigidBodyController* m_ragdollRigidBodyController;

		hkQsTransform m_worldFromModel;
		hkReal m_scale;

		class hkaMeshBinding** m_skinBindings;
		hkInt32 m_numSkinBindings;
};

#endif  // HK_RAGDOLL_SCALING_DEMO_H

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

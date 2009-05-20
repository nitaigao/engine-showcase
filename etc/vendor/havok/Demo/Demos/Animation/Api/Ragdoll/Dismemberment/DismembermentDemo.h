/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DISMEMBERMENT_DEMO_H
#define HK_DISMEMBERMENT_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

class DismembermentDemo : public hkDefaultPhysicsDemo
{
    public:

        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

        DismembermentDemo(hkDemoEnvironment* env);

        ~DismembermentDemo();

        hkDemo::Result stepDemo();

        // Collision Layers used in this demo.
        // By enabling and disabling collisions between these layers, we can tweak the behaviour of the demo
        enum
        {
            LAYER_LANDSCAPE = 1,
            LAYER_RAGDOLL_KEYFRAMED = 2,
            LAYER_RAGDOLL_DYNAMIC = 3,
        };

    private:

		hkQsTransform m_worldFromModel;

		class hkaAnimationBinding* m_binding;

		class hkaSkeletonMapper* m_ragdollFromAnimation;
		class hkaSkeletonMapper* m_animationFromRagdoll;

		const class hkaSkeleton* m_animationSkeleton;
		const class hkaSkeleton* m_ragdollSkeleton;

		class hkaRagdollInstance* m_originalRagdoll;

        class hkLoader* m_loader;

		class hkaDefaultAnimationControl* m_control;
		class hkaAnimatedSkeleton* m_character;

		class hkaMeshBinding** m_skinBindings;
		hkInt32 m_numSkinBindings;

		hkPointerMap< class hkpRigidBody*, class hkaMeshBinding* > m_skinFromRigidBody;

		hkArray< class DismembermentDemoRagdollInstanceAndController* > m_ragdolls;

		class hkPseudoRandomGenerator* m_randomizer;

		bool m_drawRagdoll;
		bool m_drawSkin;
		bool m_drawSkeleton;
};


// Helper class
class DismembermentDemoRagdollInstanceAndController : public hkReferencedObject
{

	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION);

		enum ControllerType
		{
			NONE,
			KEYFRAMED,
			POWERED
		};

		DismembermentDemoRagdollInstanceAndController( class hkaRagdollInstance* ragdollInstance, const hkaRagdollInstance* originalRagdollInstance, ControllerType type );
		~DismembermentDemoRagdollInstanceAndController();

		void driveToPose( const hkQsTransform* poseLocalSpace, hkQsTransform& worldFromModel, hkReal step );

		ControllerType getType() const;

		const hkaRagdollInstance* getRagdollInstance() const;

		void getPoseWorldSpace( const hkQsTransform* localSpaceIn, hkQsTransform* worldSpaceOut ) const;
	
	private:

		void setBodyKeyframed( hkpRigidBody* rb );
		void setBodyDynamic( hkpRigidBody* rb );


		static int findNearestParentWithRigidBody( int bone, const hkaRagdollInstance* ragdoll );

		int getRagdollRootBone() const;
		void getTransformOfRootRigidBody( hkQsTransform& xform ) const;
	
		hkUint32 calcFilterInfo( hkpRigidBody* body ) const;

		class hkaRagdollInstance* m_ragdollInstance;
		const class hkaRagdollInstance* m_originalRagdollInstance;
		class hkaRagdollRigidBodyController* m_ragdollRigidBodyController;
		ControllerType m_type;
};


#endif  // HK_DISMEMBERMENT_DEMO_H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PENETRATION_H
#define HK_PENETRATION_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

// Ragdoll penetration detection
#include <Animation/Ragdoll/Penetration/hkaDetectRagdollPenetration.h>


#define NUM_POSES 6

class PenetrationDemo : public hkDefaultPhysicsDemo
{
    public:

        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

        PenetrationDemo(hkDemoEnvironment* env);

        ~PenetrationDemo();

        hkDemo::Result stepDemo();

        void stopRagdollFeedback(); // dynamic state
        void doRagdollFeedback();   // keyframed state

        void displayDetectionPose();
        static void displayRootCdBody( hkpWorld* world, const hkpCollidable* collidable, hkpShapeKey key);

    public:

        // Collision Layers used in this demo.
        // By enabling and disabling collisions between these layers, we can tweak the behaviour of the demo
        enum
        {
            LAYER_LANDSCAPE = 1,
            LAYER_RAGDOLL_DYNAMIC = 2,
            LAYER_RAGDOLL_KEYFRAMED = 3,
            LAYER_RAGDOLL_PENETRATING = 4,
            LAYER_RAYCAST = 5
        };

    private:

        //
        void _setBodyKeyframed( hkpRigidBody* rb );
        void _setBodyDynamic( hkpRigidBody* rb, int boneId, int parentId );
        void _setBodyDynamicWithCorrection( hkpRigidBody* rb, int boneId, int parentId, bool applyForce );


        hkReal m_time;

        // Ragdoll states (keyframed = false or ragdoll = true)
        hkBool m_statusFlag;

        // Current pose
        int m_poseId;

        // With or without correction flag
        hkBool m_correctionFlag;

        // Array of available poses
        class hkaPose* m_posesLowRes[NUM_POSES];

        // Detect penetration classes
        class hkaRagdollRaycastInterface* m_ragdollRaycastInterface;
        class hkpAabbPhantom* m_ragdollPhantom;

        class hkaDetectRagdollPenetration* m_detectRagdollPenetration;

        hkaDetectRagdollPenetration::Output m_penetratedBones;

        // current velocity and turn
        hkReal m_turn;
        hkVector4 m_pos;

        // The current reference frame transformation (world-from-model)
        hkQsTransform m_currentTransform;

        // The ragdoll instance - it's kept at all times
        class hkaRagdollInstance* m_ragdollInstance;

        class hkLoader* m_loader;

};

#endif  // HK_PENETRATIONDEMO_H

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

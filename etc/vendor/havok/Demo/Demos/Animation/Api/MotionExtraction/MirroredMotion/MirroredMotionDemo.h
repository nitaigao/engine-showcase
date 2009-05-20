/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef MIRRORED_MOTION_DEMO_H
#define MIRRORED_MOTION_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>


	// Demonstrate runtime mirroring of animations using the hkMirroredSkeletalAnimation class
class MirroredMotionDemo : public hkDefaultAnimationDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		MirroredMotionDemo( hkDemoEnvironment* env );

		~MirroredMotionDemo();

		Result stepDemo(); 

		void makeFakeInput();

	private:

		class hkLoader* m_loader;

		class hkaSkeleton* m_skeleton;

		// [0] is loaded from a file, [1] is mirrored
		class hkaAnimation* m_animation[2];
		class hkaAnimationBinding* m_binding[2];
		class hkaDefaultAnimationControl* m_control[2];
		class hkaAnimatedSkeleton* m_skeletonInstance[2];

		// These are loaded from a file
		class hkaMeshBinding** m_skinBindings[2];
		class hkaBoneAttachment** m_attachments[2];
		hkInt32 m_numSkinBindings[2];
		hkInt32 m_numAttachments[2];

		int m_axisCounter;
		hkDemoEnvironment *m_env;

		// An array of the graphics meshes that are attached to the skeleton
		hkArray<class hkgDisplayObject*> m_attachmentObjects[2];

		hkBool m_wireframe;
		hkBool m_drawSkin;
		hkBool m_paused;

		hkQsTransform m_accumulatedMotion[2];
		hkBool m_useExtractedMotion;
		hkReal m_separation;
};


#endif // MIRRORED_MOTION_DEMO_H

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

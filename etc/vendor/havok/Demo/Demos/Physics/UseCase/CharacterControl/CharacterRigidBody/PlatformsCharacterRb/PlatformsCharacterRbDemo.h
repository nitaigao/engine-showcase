/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PLATFORMS_CHARACTER_RIGIDBODY_DEMO_H
#define HK_PLATFORMS_CHARACTER_RIGIDBODY_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpCharacterRigidBody;
class hkpCharacterContext;
class hkLoader;

struct hkpSurfaceInfo;

class PlatformsCharacterRbDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		PlatformsCharacterRbDemo(hkDemoEnvironment* env);
		
		~PlatformsCharacterRbDemo();

		hkDemo::Result stepDemo();

		// Move keyframed horizontal platform left <-> right
		void moveHorizontalPlatform();
		// Move keyframed vertical platform up <-> down 
		void moveVerticalPlatform();

		void cameraHandling();	

	protected:

		hkLoader* m_loader;

		hkReal m_time;

		hkReal m_currentAngle;
		hkQuaternion m_currentOrient;

		// Horizontal platform
		hkpRigidBody* m_horPlatform;
		// Vertical platform
		hkpRigidBody* m_verPlatform;		

		hkpShape* m_standShape;
		
		hkpCharacterRigidBody* m_characterRigidBody;
		hkpCharacterContext* m_characterContext;

		hkpSurfaceInfo*	m_previousGround;
		hkInt32 m_framesInAir;
		
		hkArray<hkUlong> m_objectIds;
		
};

#endif // HK_PLATFORMS_CHARACTER_RIGIDBODY_DEMO_H

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

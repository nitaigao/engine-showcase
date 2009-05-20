/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PRIORITY_DEMO_H
#define HK_CHARACTER_PRIORITY_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;
class MyCharacterPriorityListener;

#define NUM_OBJECTS 3

/// This game shows how the character controller uses priority to handle insolvable situations.
/// By default, surfaces are automatically given one of three priorities based on their motion type:
/// 0 : Dynamic Motion types
/// 1 : Keyframed motion
/// 2 : Fixed motion
/// However in this game each body is of KEYFRAMED motion type and we explicitly set each body's priority
/// dynamically through the processConstraints() callback
/// The priority ranges from Low Priority (Yellow) to High Priority (Red). 

class CharacterPriorityDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CharacterPriorityDemo(hkDemoEnvironment* env);
		
		~CharacterPriorityDemo();

		hkDemo::Result stepDemo();

	protected:

		hkpCharacterProxy* m_characterProxy;

		hkpCharacterContext* m_characterContext;

		hkpShapePhantom* m_phantom;

		hkReal m_currentTime;

		int m_prevObj;

		hkpRigidBody* m_objects[NUM_OBJECTS];

		MyCharacterPriorityListener* m_listener;
};

#endif // HK_CHARACTER_PRIORITY_DEMO_H

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

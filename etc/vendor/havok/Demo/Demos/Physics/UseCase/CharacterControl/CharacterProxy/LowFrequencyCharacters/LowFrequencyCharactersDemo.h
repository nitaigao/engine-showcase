/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LOW_FREQUENCY_CHARACTER_DEMO_H
#define HK_LOW_FREQUENCY_CHARACTER_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;
class MyCharacterListener;

// The number of NPC
	const int NUM_NPC = 100;

// User is player 0
const int NUM_CHARACTERS = (NUM_NPC + 1);

// We use this to determine how frequently to update the characters
const int HK_SCHEDULE_FREQUENCY = 12345;

// The number of slots in our round robin schedule
const int NUM_SLOTS = 3;

class LowFrequencyCharactersDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		LowFrequencyCharactersDemo(hkDemoEnvironment* env);
		
		~LowFrequencyCharactersDemo();

		hkDemo::Result stepDemo();

	protected:

		hkBool shouldSimulate(int characterNumber, int currentTick);

		void swapPhantomShape(hkpShape* newShape, const hkVector4& position);

		hkpCharacterProxy* m_characterProxy[ NUM_CHARACTERS ];

		hkpCharacterContext* m_characterContext[ NUM_CHARACTERS ];

		hkReal m_currentAngle;

		// An array of heightfield values we use for this demo
		hkReal* m_data;

		// Used to round robin the character controllers
		// Different controllers run at 1, .5 and .25 the simulation frequency
		int m_tick;

		int m_numBoxes;
		int m_numCapsules;
		int m_numSpheres;

};

#endif // HK_LOW_FREQUENCY_CHARACTER_DEMO_H

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

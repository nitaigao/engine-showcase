/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_MotionTypeChangeDemo_H
#define HK_MotionTypeChangeDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

class hkpRigidBody;

static const hkReal FACE_RADIUS = 5.5f;
static const hkReal NUMERAL_ANGLE = HK_REAL_PI/6.0f;
static const int NUM_HAND_PIECES = 5;

static const int STATE_SWITCH_PERIOD = 230;
static const int NUM_KEYFRAMED_STEPS = 150;

// Springs = 1 / Hinge = 0
#define CONSTRAINT_TYPE 0

// An enum for the various game states
enum
{
	DEMO_STATE_FIXED = 0,
	INIT_FIXED_STATE,
	MID_FIXED_STATE,

	DEMO_STATE_KEYFRAMED,
	INIT_KEYFRAMED_STATE,
	MID_KEYFRAMED_STATE,

	DEMO_STATE_DYNAMIC,
	INIT_DYNAMIC_STATE,
	MID_DYNAMIC_STATE

};

class MotionTypeChangeDemo : public hkDefaultPhysicsDemo
{
	public:

		MotionTypeChangeDemo(hkDemoEnvironment* env);
		~MotionTypeChangeDemo();

		Result stepDemo();

		// Set the clock to fixed mode
		void setClockToFixed();

		// Set the clock to keyframed mode
		void setClockToKeyframed();

		// Set the clock to dynamic mode
		void setClockToDynamic();


		// State the demo is currently in
		int	m_demoState;

		// State of the current game mode
		int m_fixedState;
		int m_keyframedState;
		int m_dynamicState;

		// Used to switch game state modes
		int m_stateSwitchTimer;

		// Keyframed source position and orientation
		hkVector4 m_keyframedSourceP;
		hkQuaternion m_keyframedSourceQ;

		// Keyframed destination position and orientation
		hkVector4 m_keyframedDestinationP;
		hkQuaternion m_keyframedDestinationQ;

		// Colours of the hand for different game states
		hkUint32 m_fixedColour;
		hkUint32 m_dynamicColour;

		int m_keyframeCounter;

		hkpRigidBody*			m_pointer;
		hkArray<hkpRigidBody*>	m_handSpheres;

};

#endif	// HK_MotionTypeChangeDemo_H




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

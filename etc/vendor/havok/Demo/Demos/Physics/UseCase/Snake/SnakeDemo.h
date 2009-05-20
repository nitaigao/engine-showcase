/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SNAKEDEMO_H
#define HK_SNAKEDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

/// create a snake creature out of powered hinges
/// left and right to  steer, forward and back to change speed

class hkpRigidBody;
class hkpHingeConstraintData;
class hkpLimitedHingeConstraintData;
class hkpConstraintInstance;

class SnakeDemo : public hkDefaultPhysicsDemo
{
	public:

		enum
		{
			LAYER_SNAKE = 1,
			LAYER_GROUND = 3
		};

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SnakeDemo(hkDemoEnvironment* env);

		hkDemo::Result stepDemo();

		struct Options
		{
			HK_DECLARE_REFLECTION();
			Options()
				:	m_waveLength(4),
					m_wavePeriod(1.5),
					m_waveAmplitude(1.0)
			{
			}
			hkReal m_waveLength; //+default(4), +absmin(1), +absmax(10)
			hkReal m_wavePeriod; //+default(1.5), +absmin(.25), +absmax(3)
			hkReal m_waveAmplitude; //+default(1), +absmin(.25), +absmax(3)
		};

		DEMO_OPTIONS_DECLARE(Options);

	private:

		// create a snake body segment with a wheel on the bottom.
		// the wheel provides asymetric friction, which is essential for this type of snake locomotion
		hkpRigidBody* createSegment( hkVector4& segmentPosition, hkVector4& segmentHalfSize );

		// link together two segments of a snake with a powered hinge and add the motor
		void linkSegment( hkpRigidBody* front, hkpRigidBody* back, hkVector4& position, hkVector4& axis );

		// simple snake locomotion technique.  
		void slither( hkReal dt );

		hkArray< hkpLimitedHingeConstraintData* > m_muscles;
		hkArray< hkpHingeConstraintData* > m_wheels;

		hkReal m_waveFront;

		hkReal m_segmentLength;
		hkReal m_steering;
};

#endif // HK_SNAKEDEMO_H

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

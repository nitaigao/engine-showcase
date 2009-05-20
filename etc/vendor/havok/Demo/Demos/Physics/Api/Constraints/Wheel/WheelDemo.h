/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows a single wheel constraint.
// You can slide the wheel along the suspension axis with the mouse spring.
// You can steer/accelerate/decelerate the wheel using the arrow keys/direction buttons.
// Suspension axis is green line.
// Steering axis is yellow line.
// Limits of suspension are white lines.
// Axle is blue line.

#ifndef HK_WHEELAPI_H
#define HK_WHEELAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkWheelViewer;	
class hkpWheelConstraintData;


class WheelDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		WheelDemo(hkDemoEnvironment* env);
		~WheelDemo();
		Result stepDemo(); 

	private:

		hkpWheelConstraintData* m_wheelConstraint;
		hkpRigidBody* m_wheelRigidBody;
		hkpRigidBody* m_chassis;
		hkReal m_steeringAngle;

		hkWheelViewer*	m_viewer;	

		hkpRigidBody* createDisc(hkReal radius,hkReal height, hkReal mass, hkVector4& position);

};

#endif	// HK_WHEELAPI_H


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

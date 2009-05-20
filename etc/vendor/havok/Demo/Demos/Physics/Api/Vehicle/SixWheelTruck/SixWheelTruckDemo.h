/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIX_WHEEL_TRUCK_H
#define HK_SIX_WHEEL_TRUCK_H

///[TUTORIAL 4]

///[Introduction] Introduction

#include <Demos/Physics/Api/Vehicle/Car/CarDemo.h>

///[Introduction]
/// A demo that shows how to use the Vehicle Kit to construct a six-wheeled vehicle. The
/// constructor creates ten six-wheeled vehicles and a landscape to drive on. This uses the
/// hkpVehicleInstance action in <hkvehicle/hkpVehicleInstance.h>.
/// The stepDemo() function updates the first vehicle based on user input (steering, acceleration etc.),
/// and updates the camera to follow this vehicle.
/// This type of vehicle could be used to represent a World War 2 half-track, or similar vehicle
/// that has multiple wheels (or wheels and treads) but only steers with the front two wheels.
/// [IMAGE Resources/SixWheelTruck.png]

class SixWheelTruckDemo: public CarDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			// Constructor.
		SixWheelTruckDemo(hkDemoEnvironment* env);
};

#endif // HK_SIX_WHEEL_TRUCK_H

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

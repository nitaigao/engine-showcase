/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TANK_H
#define HK_TANK_H

///[TUTORIAL 5]

///[Introduction] Introduction
///[TankSteeringChange] The Tank Steering System

#include <Demos/Physics/Api/Vehicle/Car/CarDemo.h>

///[Introduction]
/// A demo that shows how to use the Vehicle API to construct a tank. The
/// constructor creates tanks and a landscape to drive on. This uses the
/// hkpVehicleInstance action in <hkvehicle/hkpVehicleInstance.h>.
/// The stepDemo() function updates the tank based on user input (steering, acceleration etc.),
/// and updates the camera to follow the tank.
/// The tank overrides the hkDefaultSteering compoment in the Vehicle SDK to allow it
/// turn on the spot. [IMAGE Resources/Tank.png]

class TankDemo: public CarDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			// Constructor.
		TankDemo(hkDemoEnvironment* env);

		virtual Result stepDemo();

	private:
		int m_numTimesteps;
};

#endif // HK_TANK_H

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

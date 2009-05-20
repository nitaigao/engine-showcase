/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

///[TUTORIAL 6]

#include <Demos/demos.h>

#include <Demos/Physics/Api/Vehicle/Tank/TankSteering.h>


///[TankSteeringChange]
/// This is what the tank changes in its steering. The rear wheels simply turn in the opposite
/// direction to the front wheels.
///
void TankSteering::calcSteering(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredInfoOutput, SteeringAnglesOutput& steeringOutput )
{
	hkpVehicleDefaultSteering::calcMainSteeringAngle( deltaTime, vehicle, filteredInfoOutput, steeringOutput );

	// Wheels.
	for (int w_it = 0; w_it < m_doesWheelSteer.getSize(); w_it++)
	{
		if ( m_doesWheelSteer[w_it] )
		{
			steeringOutput.m_wheelsSteeringAngle [w_it] = steeringOutput.m_mainSteeringAngle;
		}
		else
		{
			// Steer with front and back wheels to simulate a tank.
			steeringOutput.m_wheelsSteeringAngle [w_it] = -steeringOutput.m_mainSteeringAngle;
		}
	}
}
///>

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

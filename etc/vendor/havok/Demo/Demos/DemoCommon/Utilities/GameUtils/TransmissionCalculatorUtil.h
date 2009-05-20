/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRANSMISSION_CALCULATOR_UTIL_H
#define HK_TRANSMISSION_CALCULATOR_UTIL_H

	/// A utility class to calculate the primaryTransmissionRatio for a vehicle. The answer
	/// returned is intended only as a guideline. It will probably need to be tweaked a little
	/// to give the exact desired performance. See the "Tuning tips" of the Havok User Guide
	/// for more details.
	/// The formula is copied here:
	/// TopSpeedCar[m/s] = TopSpeedOfCar[mph] * 1.605 / 3.6 
	/// MaxWheelAngularVel = TopSpeedCar[m/s] / wheelRadius 
	/// MaxWheelRPM = MaxWheelAngularVel * 60 / 2 * PI 
	/// PrimirayTransmissionRatio = maxEngineRPM / MaxWheelRPM 
class TransmissionCalculatorUtil
{
	public:
		TransmissionCalculatorUtil(){}
		~TransmissionCalculatorUtil(){}

		static hkReal calculatePrimaryTransmissionRatio(hkReal vehicleTopSpeed, hkReal wheelRadius, hkReal maxEngineRpm)
		{
			hkReal topSpeedCar = (vehicleTopSpeed * 1.605f) / 3.6f;

			hkReal maxWheelAngularVel = topSpeedCar / wheelRadius; 

			hkReal maxWheelRpm = (maxWheelAngularVel * 60.0f) / (2.0f * HK_REAL_PI); 
	
			return maxEngineRpm / maxWheelRpm;
		}
};

#endif // HK_TRANSMISSION_CALCULATOR_UTIL_H

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

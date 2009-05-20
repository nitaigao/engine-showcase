/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_VEHICLE_DISPLAY_H
#define HK_VEHICLE_DISPLAY_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h>

	/// A class with utility functions to display tyres, speedometer, skidmarks etc. for a vehicle.
class VehicleDisplayUtils
{
	public:

		VehicleDisplayUtils() {}

			//
			// Some public helper functions.
			//

		struct DisplayMeterInfo
		{
			float m_value;
			float m_maxValue;
			float m_stepSize;

			int m_posX;
			int m_posY;
			int m_radius;
			int m_pointerRadius;

			hkUint32 m_scaleColor;
			hkUint32 m_pointerColor;
		};

		struct VehicleDataAndDisplayInfo
		{
			// Needed for tyremarks.
			hkpVehicleInstance* m_vehicle;

			// Needed to smooth the RPM meter.
			hkReal m_lastRPM;		
		};

		static void displayMeter(hkDemoEnvironment* environment, const DisplayMeterInfo& info);

			/// Update the RPM and MPH info for the vehicle so it can be displayed.
		static void updateInfo( hkDemoEnvironment* environment,
								VehicleDataAndDisplayInfo& vehicleInfo );

			/// Display tyremarks when the vehicle skids.
		static void updateTyremarks( hkReal timestep, 
									hkpVehicleInstance* vehicle );
};

#endif // HK_VEHICLE_DISPLAY_H

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

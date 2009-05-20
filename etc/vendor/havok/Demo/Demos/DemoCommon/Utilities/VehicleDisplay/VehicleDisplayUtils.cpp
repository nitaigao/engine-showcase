/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>


#include <Demos/DemoCommon/Utilities/VehicleDisplay/VehicleDisplayUtils.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/TyreMarks/hkpTyremarksInfo.h>

#include <Common/Visualize/hkDebugDisplay.h>

void VehicleDisplayUtils::displayMeter( hkDemoEnvironment* environment, const DisplayMeterInfo& info)
{

	char buffer[128];
	buffer[1] = 0;
	buffer[0] = 'o';
	const float unusedAngle = 0.8f;
	const float angleScale = (2.0f * 3.14f - 2.0f * unusedAngle) / info.m_maxValue;

	//
	// Display outer rim.
	//
	{
		buffer[0] = 'o';
		for ( float x = 0; x < info.m_maxValue * 1.01f; x += info.m_stepSize )
		{
			const float angle = x * angleScale + unusedAngle;
			const float sx = -hkMath::sin( angle ) * info.m_radius;
			const float cx =  hkMath::cos( angle ) * info.m_radius;
			environment->m_textDisplay->outputText(buffer, int(sx + info.m_posX), int(cx + info.m_posY), info.m_scaleColor);
		}
	}

	//
	// Display pointer.
	//
	{
	    buffer[0] = '*';
		const float angle = info.m_value * angleScale + unusedAngle;
		float sx = -hkMath::sin( angle ) * info.m_pointerRadius;
		float cx =  hkMath::cos( angle ) * info.m_pointerRadius;

		environment->m_textDisplay->outputText(buffer, int(sx + info.m_posX), int(cx + info.m_posY), info.m_pointerColor);
		sx *= 0.7f;
		cx *= 0.7f;
		environment->m_textDisplay->outputText(buffer, int(sx + info.m_posX), int(cx + info.m_posY), info.m_pointerColor);
		sx *= 0.5f;
		cx *= 0.5f;
		environment->m_textDisplay->outputText(buffer, int(sx + info.m_posX), int(cx + info.m_posY), info.m_pointerColor);
		sx *= 0.0f;
		cx *= 0.0f;
		environment->m_textDisplay->outputText(buffer, int(sx + info.m_posX), int(cx + info.m_posY), info.m_pointerColor);
	}
}

void VehicleDisplayUtils::updateInfo(hkDemoEnvironment* environment,
									 VehicleDataAndDisplayInfo& vehicleInfo )
{
	char buffer[256];

	//
	// RPM.
	//
	{
		DisplayMeterInfo info;

		hkReal rpm = 0.9f * vehicleInfo.m_lastRPM + 0.1f * vehicleInfo.m_vehicle->calcRPM();
		vehicleInfo.m_lastRPM = rpm;

		info.m_value = rpm;
		info.m_maxValue = 8000.0f;
		info.m_stepSize = 800.0f;
		info.m_scaleColor = 0x8000c0c0;

		info.m_posX = 70;
		info.m_posY = 100;
		info.m_radius = 40;
		info.m_pointerRadius = 30;

		info.m_pointerColor = 0xffffffff;

		displayMeter( environment, info );

		hkString::sprintf(buffer, "rpm %3.0f", vehicleInfo.m_vehicle->calcRPM() );
		environment->m_textDisplay->outputText(buffer, info.m_posX-30, info.m_posY + info.m_radius);

		//
		// Gear.
		//
		{
			hkString::sprintf(buffer, "gear = %i", vehicleInfo.m_vehicle->m_currentGear );
			environment->m_textDisplay->outputText(buffer, info.m_posX-30, info.m_posY + info.m_radius + 16);
		}
	}

	//
	// MPH.
	//
	{ 
		float mph = vehicleInfo.m_vehicle->calcMPH();

		DisplayMeterInfo info;

		info.m_value = mph;
		info.m_maxValue = 180.0f;
		info.m_stepSize = 20.0f;
		info.m_scaleColor = 0x8000c0c0;

		info.m_posX = 270;
		info.m_posY = 100;
		info.m_radius = 40;
		info.m_pointerRadius = 30;

		info.m_pointerColor = 0xffffffff;

		displayMeter( environment, info );

		hkString::sprintf(buffer, "mph %3.0f", mph );
		environment->m_textDisplay->outputText(buffer, info.m_posX-30, info.m_posY + info.m_radius);
	}
}

void VehicleDisplayUtils::updateTyremarks( hkReal timestep,
										  hkpVehicleInstance* vehicle )
{
	// Update the skidmarks. Note: as the vehicle is integrated, we msut add our 
	// current velocity to our skidmarks to compensate for the missing integration 
	// of the contact point information.
	vehicle->m_tyreMarks->updateTyremarksInfo( timestep, vehicle );

	for (int wheel = 0; wheel < vehicle->m_data->m_numWheels; wheel++ )
	{
		const hkpTyremarksWheel &tyremarks_wheel = *vehicle->m_tyreMarks->m_tyremarksWheel[wheel];
		const int num_points = tyremarks_wheel.m_tyremarkPoints.getSize();

		const hkVector4* startl = &tyremarks_wheel.getTyremarkPoint(0).m_pointLeft;
		const hkVector4* startr = &tyremarks_wheel.getTyremarkPoint(0).m_pointRight;

		for (int p_it=1; p_it < num_points; p_it++)
		{
			const hkpTyremarkPoint* tyremark_point = &tyremarks_wheel.getTyremarkPoint(p_it);

			const hkVector4* endl = &tyremark_point->m_pointLeft;
			const hkVector4* endr = &tyremark_point->m_pointRight;

			hkReal alpha = (*startl)(3);
			if ( alpha )
			{
				// Note: alpha display does not work with the havok graphics engine but
				// the next lines should be ok. To trim the alpha values you can use 
				// m_minSkidmarkEnergy and m_maxSkidmarkEnergy.
				const long color = int( alpha ) <<24;
				HK_DISPLAY_LINE( *startl, *endl, color);
				HK_DISPLAY_LINE( *startr, *endr, color);
			}
			
			startl = endl;
			startr = endr;
		}
	}
}

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

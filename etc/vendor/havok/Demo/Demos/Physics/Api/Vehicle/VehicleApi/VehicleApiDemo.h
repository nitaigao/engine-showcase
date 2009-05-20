/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VEHICLEAPI_H
#define HK_VEHICLEAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

class VehicleApiDemo : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		VehicleApiDemo(hkDemoEnvironment* env);

		virtual ~VehicleApiDemo();

			/// Steer the vehicle. Step the hkpWorld. Sync the display wheels. Update the camera.
		Result stepDemo(); 

	private:

			// Build the landscape to drive on and add it to m_world.
		void buildLandscape();

			// This function actually "builds" the vehicle from a set of bodies and a hardcoded 
			// list of parameters (stored in the VehicleSetup object).
		void createVehicle(hkpRigidBody* chassis);

		void createDisplayWheels(hkReal radius = 0.4f, hkReal thickness = 0.2f);

		hkpVehicleInstance* m_vehicle;
	
		// Camera that follows the vehicle.
		hkp1dAngularFollowCam m_camera;

			// User input to drive vehicle.
		hkReal m_inputXPosition;
		hkReal m_inputYPosition;
		
			// For use with calls to the displayHandler. Only needed to display the wheels.
		int m_tag;

			// m_displayWheelId is for display reference only.
		hkArray<int> m_displayWheelId;
};

#endif // HK_VEHICLEAPI_H

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

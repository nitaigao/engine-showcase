/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_VEHICLE_API_UTILS_H
#define HK_VEHICLE_API_UTILS_H

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h>
#include <Physics/Vehicle/DriverInput/Default/hkpVehicleDefaultAnalogDriverInput.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>

class hkDisplayGeometry;
class hkpConvexVerticesShape;

	/// A class with utility functions to create display tyres for a vehicle.
class VehicleApiUtils
{
	public:

		VehicleApiUtils() {}

			//
			// Some public helper functions
			//

		static void HK_CALL syncDisplayWheels(hkDemoEnvironment* environment, 
									hkpVehicleInstance& vehicle,
									const hkArray<int>& wheels,
									int tag);

		static void HK_CALL createDisplayWheels(hkDemoEnvironment* environment,
										int tag,
										int displayId,
										hkReal radius = 0.4f,
										hkReal thickness = 0.2f);

		static void HK_CALL createCamera( hkp1dAngularFollowCam& camera );


		static void HK_CALL updateCamera(hkDemoEnvironment* environment,
										const hkpRigidBody& vehicleChassis,
										hkReal timestep,
										hkp1dAngularFollowCam& camera
										);
		
			/// Steer the vehicle.
		static void HK_CALL steer(hkgPad* pad, 
							hkReal& inputX,
							hkReal& inputY,
							hkpVehicleInstance& vehicle,
							hkReal timestep);

			/// Create and return an hkpConvexVerticesShape that looks like a car.
		static hkpConvexVerticesShape* HK_CALL createCarChassisShape();

		static void HK_CALL reorient(hkBool buttonPressed, hkpAction* action, hkpWorld* world);

	private:

			/// This function facilitates construction of a "wheel" shape, which we use for
			/// display purposes only (the vehicle is not supported by its wheels, it is a
			/// "ray-cast" vehicle). The Disc shape is circular but has a slice removed to
			/// make it more obvious that when it is spinning or static.
		static hkpShape* HK_CALL createDisc(hkReal radius, hkReal thickness, int numSides = 10);

			/// Calls VehicleApiUtils::createDisc.
		static void HK_CALL createDisplayWheel(hkArray<hkDisplayGeometry*>& displayGeometry,
										hkReal radius = 0.3f,
										hkReal thickness = 0.1f);
};

#endif // HK_VEHICLE_API_UTILS_H

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

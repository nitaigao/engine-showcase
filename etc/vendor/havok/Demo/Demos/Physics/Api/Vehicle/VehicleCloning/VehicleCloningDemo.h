/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VEHICLECLONING_H
#define HK_VEHICLECLONING_H

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>
#include <Demos/DemoCommon/Utilities/VehicleDisplay/VehicleDisplayUtils.h>

///
/// This demo shows how to clone vehicles. Cloned vehicles share as many components as possible.
/// The simplest way to clone vehicles (and the one used here) is to group their components in
/// a hkpPhysicsSystem and clone that.
/// If there are vehicle components that you do not want to share you can either replace them
/// once the vehicles have been cloned, or implement a custom cloning method that only shares
/// the components you specify.
/// In order to correctly display the wheels, references to the cloned vehicles are stored in an array.
/// Wheels are not physical objects, and cannot currently be saved as part of a hkpPhysicsSystem.
///
class VehicleCloning : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		VehicleCloning(hkDemoEnvironment* env, hkBool createWorld = true);

		virtual ~VehicleCloning();

		virtual Result stepDemo(); 

	protected:

			// Set up hkpWorld
		void setUpWorld();

			// Build the landscape to drive on and add it to m_world.
		void buildLandscape();

		hkpPhysicsSystem* createVehicle();

		void createWheels( int numVehicles );
			
	public:
			// The number of wheels in each vehicle.
		int m_numWheels;

			// m_displayWheelId is used for displaying vehicle wheels only.
			// It is a hkObjectArray with one element for each vehicle.
			// Each vehicle element is an hkArray with one element for each wheel.
		hkObjectArray< hkArray<int> > m_displayWheelId;

			// An array of hkpVehicleInstance references used to get the positions of
			// the vehicles in order to correctly display the wheels.
		hkArray<hkpVehicleInstance*> m_vehicles;

		class FlatLand* m_track;
		class LandscapeContainer* m_landscapeContainer;

			// For use with calls to the displayHandler.
		int m_tag;
};

#endif // HK_VEHICLECLONING_H

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

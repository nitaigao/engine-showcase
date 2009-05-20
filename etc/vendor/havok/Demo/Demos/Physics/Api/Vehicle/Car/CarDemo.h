/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CAR_H
#define HK_CAR_H

///[TUTORIAL 1]

///[Introduction] Introduction
///[stepGame] stepDemo()

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleSetup.h>
#include <Demos/DemoCommon/Utilities/VehicleDisplay/VehicleDisplayUtils.h>

class hkpVehicleInstance;
class hkp1dAngularFollowCam;
class hkDefaultAnalogDriverInput;
class hkpReorientAction;
class hkRagdoll;

///[Introduction]
/// A demo that shows how to use the Vehicle Kit to construct basic vehicles including hkActions
/// used to update the vehicles, and a simple controller to drive the first vehicle. The constructor
/// creates ten basic four-wheeled vehicles and a MOPP landscape to drive on. stepDemo() updates
/// the first vehicle based on user input (steering, acceleration etc.), and updates the camera to
/// follow the first vehicle. [IMAGE Resources/Car.png]

class CarDemo : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CarDemo(hkDemoEnvironment* env, hkBool createWorld = true, int numWheels = 4, int numVehicles = 1);

		virtual ~CarDemo();

		void makeFakeInput();
		virtual Result stepDemo(); 

	protected:

		virtual void steer();

			// Set up hkpWorld
		void setUpWorld();

			// Build the landscape to drive on and add it to m_world.
		void buildLandscape();

			// This function creates the vehicle from a set of bodies and a hardcoded 
			// list of parameters (stored in the VehicleSetup object).
		hkpVehicleInstance* createVehicle( VehicleSetup& vehicleSetup, hkpRigidBody* chassis);

		void createDisplayWheels(hkReal radius = 0.4f, hkReal thickness = 0.2f);

		void createDodgeBoxes(hkUint16 num, hkVector4& halfExtents, hkVector4& startPos, hkVector4& diffPos);

		static void createRagdollGrid( hkpWorld* world, int x_size, int y_size, hkReal xStep, hkReal yStep, hkArray<hkRagdoll*>&	ragdollsOut);
			
	public:

		// The number of wheels in each vehicle.
		int m_numWheels;
		int m_numVehicles;

		hkArray< VehicleDisplayUtils::VehicleDataAndDisplayInfo > m_vehicles;

	public:
	
		hkp1dAngularFollowCam m_camera;

		hkReal m_inputXPosition;
		hkReal m_inputYPosition;
		
		hkReferencedObject* m_track;

			// For use with calls to the displayHandler.
		int m_tag;

			// m_displayWheelId is for display reference only.
			// An hkObjectArray with one element for each vehicle. Each vehicle element in turn
			// is an hkArray with one element for each wheel.
		hkObjectArray< hkArray<int> > m_displayWheelId;

			// This action uprights the vehicle when it turns over. 	 
		hkpReorientAction* m_reorientAction;

		hkArray<hkRagdoll*>	m_ragdolls;

};

#endif // HK_CAR_H

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

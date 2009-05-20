/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MOTORCYCLE_H
#define HK_MOTORCYCLE_H

///[TUTORIAL 1]

///[Introduction] Introduction
///[SetUpConstaint] Set up Motorcycle Constraint
///[MotorcycleSteer] Motorcycle Steering

#include <Demos/Physics/Api/Vehicle/Car/CarDemo.h>

class hkpGenericConstraintData;

///[Introduction]
/// This demo that shows how to use the Vehicle Kit and Generic Constraint Kit to construct a
/// basic motorcycle. The constructor creates ten motorcycles and a landscape to drive on. This demo
/// uses the hkpVehicleInstance action in <hkvehicle/hkpVehicleInstance.h>.
/// The stepDemo() function updates the motorcycle based on user input (steering, acceleration etc.),
/// and updates the camera to follow the motorcycle.
/// The motorcycle uses an angular constraint to keep it upright and to bank while turning.
/// [IMAGE Resources/Motorcycle.png]

class MotorcycleDemo: public CarDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			// Constructor.
		MotorcycleDemo(hkDemoEnvironment* env);

			// Destructor.
		virtual ~MotorcycleDemo();

			// Steer the vehicle.
		virtual void steer();

	private:

			// Upright the motorcycle e.g. if has crashed, tipped over, become stuck etc.
		void uprightMotorcycle();

			// Constraint to keep a motorcyle upright and to tilt it while turning.
		hkArray<hkpConstraintInstance*> m_constraint;
		hkArray<hkpGenericConstraintData*> m_genericConstraint;

			// Index for Matrix basis for the constraint.
		int m_basisIndexA;
		int m_basisIndexB;
};

#endif // HK_MOTORCYCLE_H

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

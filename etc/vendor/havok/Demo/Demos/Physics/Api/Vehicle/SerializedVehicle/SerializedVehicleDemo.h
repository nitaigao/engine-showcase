/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SerializedVehicle_H
#define HK_SerializedVehicle_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>

#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

class hkpVehicleInstance;
class hkpRigidBody;

//
// This simple game uses the minimum amount of code necessary to add a vehicle
// action to a rigid body (including a camera and steering controller).
//
//
class SerializedVehicle : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SerializedVehicle(hkDemoEnvironment* env);
		virtual ~SerializedVehicle();

		virtual Result stepDemo(); 

	protected:

		virtual void saveLandscape( hkpRigidBody* landscape, hkString path, bool binaryFormat = true);
		virtual void loadLandscape( hkpWorld* world, hkString path, bool binaryFormat = true );
		virtual void saveVehicle( hkpPhysicsSystem* vehicleSystem, hkString path, bool binaryFormat = true );
		virtual void loadVehicle( hkpWorld* world, hkpPhysicsSystem* &vehicleSystem, hkString path, bool binaryFormat = true );

		hkpVehicleInstance* getVehicle();
		void setUpWorld();

	public:

		hkPackfileReader::AllocatedData* m_loadedDataVehicle;
		hkPackfileReader::AllocatedData* m_loadedDataLandscape;

		hkp1dAngularFollowCam	m_camera;
		hkpPhysicsSystem*		m_vehicleSystem;
		hkpRigidBody*			m_landscape;

		hkReal m_inputXPosition;
		hkReal m_inputYPosition;
};

#endif // HK_SerializedVehicle_H

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

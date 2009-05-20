/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PLANET_GRAVITY_DEMO_H
#define HK_PLANET_GRAVITY_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>

class hkpAllRayHitCollector;
class hkpCharacterContext;
class hkpCharacterRigidBody;
class hkpClosestRayHitCollector;
class hkpConstraintInstance;
class hkpDefaultCharacterStates;
class hkgLight;
class hkpLimitedHingeConstraintData;
class hkpLinearParametricCurve;
class hkpPhysicsData;
struct hkpSurfaceInfo;

// This demo shows how to use the rigid body character controller and actions to create a world with local gravity.
class PlanetGravityDemo : public hkDefaultPhysicsDemo
{
	// Container for the state of a turret
	struct Turret
	{
		// Constraint (powered hinge) constraining turret RB to its planet. Used to rotate turret.
		hkpConstraintInstance* constraint;
		hkpLimitedHingeConstraintData* hinge;

		hkpRigidBody* turretRigidBody;

		// Time left until the turret can fire again
		hkReal cooldown;
	};

	public:
		PlanetGravityDemo( hkDemoEnvironment* env );
		~PlanetGravityDemo();

		hkpWorld* loadWorld( const char* path, hkpPhysicsData** physicsData, hkPackfileReader::AllocatedData** memData );

		Result stepDemo();
		void cleanupWorld();
		static void applyScaledLinearImpulse(hkpRigidBody* rb, hkVector4& impulse);

	private:
		// Updates camera information based on member variable values
		void handleCamera();

		// Update orientation of the character's rigid body based on the specified hkRotation
		hkBool reorientCharacter(const hkRotation& rotation);

	public:
		// rigid body character controller
 		static hkpCharacterRigidBody* m_characterRigidBody;

		// Current world's up vector. This changes as the simulation progresses.
 		static hkVector4 m_worldUp;

		// Character controller's rigid body's forward direction based on camera, user input and local gravity
 		static hkVector4 m_characterForward;

	protected:
		// Cast a curved ray along "curve", interpolated to "numSamplePoints" points.
		hkReal castCurvedRay( hkpClosestRayHitCollector& output, const hkpLinearParametricCurve& curve, int numSamplePoints = 0 );

		hkPackfileReader::AllocatedData* m_loadedData;
		class hkpPhysicsData* m_physicsData;

		// The shape used when the character is standing
		hkpShape* m_standShape;
		// The shape used when the character is crouching
		hkpShape* m_crouchShape;

		hkpCharacterContext * m_characterContext;
		hkpSurfaceInfo*	m_previousGround;
		hkInt32 m_framesInAir;

		// Camera's forward vector changed by mouse movement
		hkVector4 m_cameraForward;

		// Camera's position in 3D space.
		hkVector4 m_cameraPosition;

		// Camera's up vector. Changes as gravity changes.
		hkVector4 m_cameraUp;

		// Flag for detachable camera.
		hkBool m_detachedCamera;

		// Array of turret states to maintain during simulation
		hkArray<Turret> m_turrets;

		// The world's single light attached to the camera
		hkgLight* m_flashLight;
};

#endif // HK_PLANET_GRAVITY_DEMO_H

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

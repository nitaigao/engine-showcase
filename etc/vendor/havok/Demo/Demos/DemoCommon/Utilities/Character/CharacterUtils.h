/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_UTILS_H
#define HK_CHARACTER_UTILS_H

class hkpCharacterRigidBody;
struct hkpCharacterInput;
class hkpCharacterContext;


class CharacterUtils
{
	public:

		// Calls check support
	static void HK_CALL setCharacterInput( const hkpCharacterRigidBody* characterRb, hkpCharacterInput& input, const hkVector4& gravity);

		// Character control from mouse and/or dpads and sticks
		// based on what is appropriate for platform
	static void HK_CALL getUserInputForCharacter( hkDemoEnvironment* env, float& deltaAngle, float& dirX, float& dirY );

	struct Sensivity
	{
		Sensivity(){ m_mouseX = m_mouseY = m_padX = m_padY = 1.0f; }
		hkReal m_mouseX;
		hkReal m_mouseY;
		hkReal m_padX;
		hkReal m_padY;
	};


	static void HK_CALL getUserInputForCharacter( hkDemoEnvironment* env, const Sensivity& sensivity, float& deltaAngle, float& deltaElevation, float& dirX, float& dirY );

		// Utility to create a default state machine
	static hkpCharacterContext* HK_CALL createDefaultCharacterContext();

		// Convenience function to display character state as text on screen
	static void HK_CALL displayCharacterState(hkpCharacterContext* context, hkDemoEnvironment* env );

		// Compute the blend params that will produce the desired velocity
	static void computeBlendParams( hkReal desiredVel, hkReal walkVel, hkReal runVel, hkReal walkDur, hkReal runDur, hkReal& blend, hkReal& walkSpeed, hkReal& runSpeed );

};

struct CharacterControls 
{
	// These values are from -1 to 1, and m_forward is a unit vector for convenience we are using the default character controller state machine
	// TODO - update the default state machine (or do a new one) that is more usable for AI
	hkReal m_forwardBack;
	hkReal m_straffeLeftRight;
	hkVector4 m_forward;

	bool m_fire; // Left mouse button
	bool m_fireRmb; // Rigt mouse button

	hkBool m_wantJump;
};


class SimpleCharacter : public hkReferencedObject
{
	public:

			// Create a default first person character
		SimpleCharacter( const hkVector4& position, const hkVector4& up );

		SimpleCharacter( hkpCharacterRigidBody* characterRb, hkpCharacterContext* context );

		~SimpleCharacter();
	
		void update( hkReal timestep, const CharacterControls& controls, bool showForwardDirection = true );

	public:
		SimpleCharacter() {}

			/// create a simple character with a capsule shape, the \a capsuleHeight is the total height of the capsule
		void createDefaultCharacterController( const hkVector4& position, const hkVector4& up, hkReal capsuleHeight = 2.0f, hkReal capsuleRadius = 0.6f );

		hkpCharacterRigidBody* m_characterRb;
		hkpCharacterContext* m_characterRbContext;

		hkReal m_currentAngle;
		hkReal m_currentElevation;

		hkVector4 m_gravity;
};


struct FirstPersonCharacterCinfo
{
	FirstPersonCharacterCinfo();

	hkVector4 m_position;
	hkVector4 m_direction;
	hkVector4 m_up;
	hkReal m_gravityStrength;
	hkReal m_capsuleHeight;		// the total height
	hkReal m_capsuleRadius;
	hkReal m_eyeHeight;		// the height of the eyes relative to the characters center

	hkBool m_invertUpDown;
	hkReal m_verticalSensitivity;
	hkReal m_horizontalSensitivity;
	hkReal m_sensivityPadX;
	hkReal m_sensivityPadY;

	hkReal m_maxUpDownAngle;
	hkBool m_canDetachFromCharacter;
	hkUint32 m_numFramesPerShot;
	hkReal m_forwardBackwardSpeedModifier;
	hkReal m_leftRightSpeedModifier;

	// Optional, default to HK_NULL. Specify if you want a custom character representation.
	hkpCharacterRigidBody* m_characterRb;
	hkpCharacterContext* m_context;
};

class hkpFirstPersonGun;


class FirstPersonCharacter : public SimpleCharacter
{
	public:

			// Create a default first person character
		FirstPersonCharacter( FirstPersonCharacterCinfo& cinfo, hkDemoEnvironment* env, hkpWorld* world );

		~FirstPersonCharacter();

		void update( hkReal timestep);

		hkpFirstPersonGun* setGun( hkpFirstPersonGun* listener );

	public:

		void getForwardDir( hkVector4& forward );

			/// sets the current angles based on the forward direction
		void setForwardDir( const hkVector4& forward );

			// Helper functions
		void setInputFromUserControls( hkDemoEnvironment* env, CharacterControls& controls );

		void updateCamera( hkpWorld* world, hkgWindow* window );

		void reset(hkpWorld* world);

		void getViewTransform(hkTransform& viewTransformOut);

		hkpRigidBody* getRigidBody();
	

	public:

		hkBool m_invertUpDown;
		hkReal m_verticalSensitivity;
		hkReal m_horizontalSensitivity;
		hkReal m_sensivityPadX;
		hkReal m_sensivityPadY;
		hkReal m_eyeHeight;

		hkReal m_maxUpDownAngle;
		hkBool m_canDetachFromCharacter;
		hkUint32 m_numFramesPerShot;
		hkReal m_forwardBackwardSpeedModifier;
		hkReal m_leftRightSpeedModifier;

		// Flags
		hkBool m_hasUserControl;
		hkBool m_makeOccludingObjectsTransparent;

		int m_gunCounter;
		int m_gunCounterRmb;

		hkpWorld* m_world;
		hkDemoEnvironment* m_env;

		hkpFirstPersonGun* m_currentGun;
	public:

		hkArray< hkUlong > m_transparentObjects;
};


#endif // HK_CHARACTER_UTILS_H

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

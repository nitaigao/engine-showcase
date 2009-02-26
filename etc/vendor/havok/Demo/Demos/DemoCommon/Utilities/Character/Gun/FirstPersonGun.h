/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIRST_PERSON_GUN_H
#define HK_FIRST_PERSON_GUN_H


class FirstPersonCharacter;
struct CharacterControls;

class FirstPersonGun : public hkReferencedObject
{
	public:

		FirstPersonGun()
		{
			m_keyboardKey = 0x71; // HKG_VKEY_F2
		}

			/// Use to create gun display
		virtual void initGun( hkDemoEnvironment* env, hkpWorld* world, FirstPersonCharacter* character ){}

			/// Fire bullets
		virtual void fireGun( hkDemoEnvironment* env, hkpWorld* world, const hkTransform& viewTransform ) = 0;

			/// Use to update display, remove bullets from world etc
		virtual void stepGun( hkReal timeStep, hkDemoEnvironment* env, hkpWorld* world, FirstPersonCharacter* character, CharacterControls* controls ){}

			/// Use to clean up gun display
		virtual void quitGun( hkDemoEnvironment* env, hkpWorld* world, FirstPersonCharacter* character ){}

			/// Get the name of the gun
		virtual char* getName() { return ""; }

		virtual void reset(hkpWorld* world) {}

		struct SweepSphereOut
		{
			hkContactPoint m_contactPoint;
			hkpShapeKey m_shapeKey;
			hkpRigidBody* m_body;
		};

		static hkResult HK_CALL sweepSphere(const hkpWorld* world, const hkVector4& sweepStart, hkReal radius,  const hkVector4& sweepEnd, SweepSphereOut& out );

		static void     HK_CALL calcVelocityToTarget(const hkVector4& position, const hkVector4& target, const hkVector4& gravity, hkReal speed, hkVector4& velocity);

	public:

		HKG_KEYBOARD_VKEY m_keyboardKey;
};


#endif // HK_FIRST_PERSON_GUN_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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

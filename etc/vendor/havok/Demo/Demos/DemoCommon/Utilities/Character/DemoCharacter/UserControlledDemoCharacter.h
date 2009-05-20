/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_USER_CONTROLLER_DEMO_CHARACTER_H
#define HK_USER_CONTROLLER_DEMO_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/CharacterStepInput.h>

class DemoCharacter;
class hkDemoEnvironment;
struct CharacterActionInfo;

// This class will become a set of static utilities

class UserControlledDemoCharacter : public hkReferencedObject
{
	public:

		UserControlledDemoCharacter( DemoCharacter* demoCharacter );

		virtual ~UserControlledDemoCharacter( );

		void getUserInputForCharacter( hkDemoEnvironment* env, CharacterStepInput& input, CharacterActionInfo& actionInfo, hkReal maxVelocity);

		// Follow the character
		void updateCamera( hkDemoEnvironment* env, hkpWorld* world );

		// Use at the start of the demo to set the initial camera
		void getCameraFromTo( hkVector4& from, hkVector4& to ) const;

		void updateCameraWithSmoothing( hkDemoEnvironment* env, hkpWorld* world, const hkVector4& newFrom, const hkVector4& newTo );

		DemoCharacter* getDemoCharacter() const { return m_demoCharacter; }

		void setActionKey(CharacterAction action, HKG_PAD_BUTTON key);

	public:

		DemoCharacter* m_demoCharacter;

		// Set this to how fast the character can turn per second (in radians)
		hkReal m_characterTurnSpeed;

		// Stored here to smoothly go to 0 speed when the controller is released
		hkReal  m_forwardSpeed; 

		// User settable Camera parameters
		hkReal m_cameraToHeight;
		hkReal m_cameraToForward;
		hkReal m_cameraFromHeight;
		hkReal m_cameraFromBack;
		hkReal m_cameraGain;
		hkBool m_makeOccludingObjectsTransparent; // if you enable this you may have to call: m_env->m_displayHandler->setAllowColorChangeOnPrecreated( true );

		// Used to make occluding objects transparent if m_makeOccludingObjectsTransparent
		hkArray< hkUlong > m_transparentObjects;

		HKG_PAD_BUTTON m_actionsKeys[MAX_ACTIONS];

};


#endif // HK_SUER_CONTROLLER_DEMO_CHARACTER_H

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

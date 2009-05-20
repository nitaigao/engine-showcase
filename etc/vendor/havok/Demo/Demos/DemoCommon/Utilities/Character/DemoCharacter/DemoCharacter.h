/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO_CHARACTER_H
#define HK_DEMO_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>

class hkDemoEnvironment;
class DemoCharacter;
class CharacterProxy;

class CharacterFactory : public hkReferencedObject
{
	public:

		DemoCharacter* createCharacter( const hkVector4& position, const hkQuaternion& rotation, const hkVector4& gravity, int filterInfo, hkDemoEnvironment* env );

		virtual DemoCharacter* createCharacterUsingProxy( CharacterProxy* proxy, const hkVector4& gravity, hkDemoEnvironment* env ) = 0;

		static CharacterProxy* HK_CALL createProxy( const hkVector4& position, const hkQuaternion& rotation, const hkVector4& gravity, int filterInfo );
};

struct DemoCharacterCinfo
{
	class CharacterProxy*	m_characterProxy;
};

class DemoCharacter : public hkReferencedObject
{
	public:

		DemoCharacter( DemoCharacterCinfo& info );

		virtual ~DemoCharacter( );

			// Update the character position
		virtual void update( hkReal timestep, hkpWorld* world, const struct CharacterStepInput& input, struct CharacterActionInfo* actionInfo = HK_NULL ) = 0;

			// Remove this later
		virtual void display( hkReal timestep, hkDemoEnvironment* env ) = 0;

			// access to the proxy
		CharacterProxy* getProxy() const;

			// get maximum velocity
		virtual hkReal getMaxVelocity() const = 0;

	protected:

		CharacterProxy* m_characterProxy;
};

#endif // HK_DEMO_CHARACTER_H

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

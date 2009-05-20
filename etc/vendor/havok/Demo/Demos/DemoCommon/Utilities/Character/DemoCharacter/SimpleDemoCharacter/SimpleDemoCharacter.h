/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMPLE_DEMO_CHARACTER_H
#define HK_SIMPLE_DEMO_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/DemoCharacter.h>

struct SimpleDemoCharacterCinfo : public DemoCharacterCinfo
{
	hkVector4		m_gravity;
	hkReal			m_maxVelocity;
};

class SimpleDemoCharacter : public DemoCharacter
{
	public:

		SimpleDemoCharacter( SimpleDemoCharacterCinfo& info );

		virtual void update( hkReal timestep, hkpWorld* world, const CharacterStepInput& input, struct CharacterActionInfo* actionInfo = HK_NULL );

		virtual void display( hkReal timestep, hkDemoEnvironment* env );

		virtual hkReal getMaxVelocity() const;

	protected:

		hkVector4			m_gravity;

		hkReal				m_maxVelocity;
};


class SimpleCharacterFactory : public CharacterFactory
{
	public:

		virtual DemoCharacter* createCharacterUsingProxy( CharacterProxy* proxy, const hkVector4& gravity, hkDemoEnvironment* env );
};


#endif // HK_SIMPLE_DEMO_CHARACTER_H

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

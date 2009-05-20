/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/DemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/RigidBodyCharacterProxy/RigidBodyCharacterProxy.h>

DemoCharacter::DemoCharacter( DemoCharacterCinfo& info ) 
	: m_characterProxy( info.m_characterProxy ) 
{ 
	m_characterProxy->addReference(); 
}

DemoCharacter::~DemoCharacter( )
{ 
	m_characterProxy->removeReference(); 
}

// access to the proxy
CharacterProxy* DemoCharacter::getProxy() const 
{ 
	return m_characterProxy; 
}

DemoCharacter* CharacterFactory::createCharacter( const hkVector4& position, const hkQuaternion& rotation, const hkVector4& gravity, int filterInfo, hkDemoEnvironment* env )
{
	return createCharacterUsingProxy( createProxy( position, rotation, gravity, filterInfo ), gravity, env );
}

CharacterProxy* HK_CALL CharacterFactory::createProxy( const hkVector4& position, const hkQuaternion& rotation, const hkVector4& gravity, int filterInfo )
{
	RigidBodyCharacterProxyCinfo rbinfo;
	rbinfo.m_characterHeight = 2.0f;
	rbinfo.m_characterRadius = 0.4f;
	rbinfo.m_forwardLocal.set(1,0,0);
	rbinfo.m_upLocal.setNeg4(gravity);
	rbinfo.m_upLocal.normalize3();
	rbinfo.m_collisionFilterInfo = filterInfo;
	rbinfo.m_position = position;
	rbinfo.m_rotation = rotation;

	RigidBodyCharacterProxy* proxy = new RigidBodyCharacterProxy( rbinfo );
	return proxy;
}

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

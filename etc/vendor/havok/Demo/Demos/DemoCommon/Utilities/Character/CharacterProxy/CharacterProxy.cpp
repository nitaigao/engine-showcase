/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>

CharacterProxy::CharacterProxy( CharacterProxyCinfo& info ) 
	: m_forwardLocal(info.m_forwardLocal), 
	  m_upLocal(info.m_upLocal),
	  m_turnVelocity(0.0f)
{
		
}

// Get the forward direction in local space
const hkVector4& CharacterProxy::getForwardLocal() const
{
	return m_forwardLocal;
}

// The the up direction in local space
const hkVector4& CharacterProxy::getUpLocal() const
{
	return m_upLocal;
}

// Get the angular velocity of the proxy
hkReal CharacterProxy::getTurnVelocity() const
{
	return m_turnVelocity;
}

// Set the linear velocity of the proxy
void CharacterProxy::setTurnVelocity( const hkReal turnVelocity)
{
	m_turnVelocity = turnVelocity;
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

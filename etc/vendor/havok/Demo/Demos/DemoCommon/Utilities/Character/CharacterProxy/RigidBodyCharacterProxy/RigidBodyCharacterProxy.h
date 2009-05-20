/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RB_PROXY_CHARACTER_H
#define HK_RB_PROXY_CHARACTER_H

#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>

struct RigidBodyCharacterProxyCinfo : public CharacterProxyCinfo
{
	// Character height
	hkReal		m_characterRadius;
	hkReal		m_characterHeight;

	// Filter info
	hkUint32	m_collisionFilterInfo;
};

class RigidBodyCharacterProxy : public CharacterProxy
{
	public:

		RigidBodyCharacterProxy( RigidBodyCharacterProxyCinfo& info );

		virtual ~RigidBodyCharacterProxy();
		 
		virtual void addToWorld( hkpWorld* world ); 

			// Get the current transform of the character
		virtual void getTransform( hkTransform& transform ) const;

			// Get the current transform of the character
		virtual void setTransform( const hkTransform& transform );

		virtual const hkVector4& getPosition();

			// Get the linear velocity of the proxy
		virtual void getLinearVelocity( hkVector4& velOut ) const;

			// Set the linear velocity of the proxy
		virtual void setLinearVelocity( const hkVector4& newVelocity );

			// Return true if the proxy is supported
		virtual bool isSupported( hkReal timestep ) const;

			// Get the forward direction in local space
		const hkVector4& getCharacterForwardLocal() const;

			// The the up direction in local space
		const hkVector4& getCharacterUpLocal() const;

		//const hkVector4& getGravity() const;

		virtual void setCollisionFilterInfo(hkUint32 filterInfo);
		virtual hkUint32 getCollisionFilterInfo( );

		hkpWorldObject* getWorldObject();

		class hkpCharacterRigidBody* m_characterRb;
};

#endif // HK_RB_PROXY_CHARACTER_H

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

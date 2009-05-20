/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PROXY_CHARACTER_H
#define HK_CHARACTER_PROXY_CHARACTER_H

struct CharacterProxyCinfo
{
	hkVector4	m_forwardLocal;
	hkVector4	m_upLocal;		// Note that this must be perpendicular to the m_forwardLocal direction
	hkVector4	m_position;
	hkQuaternion m_rotation;
};

class CharacterProxy : public hkReferencedObject
{
	public:

		CharacterProxy( CharacterProxyCinfo& info );

		virtual void addToWorld( hkpWorld* world ) = 0; 

			// Get the current transform of the character
		virtual void getTransform( hkTransform& transform ) const = 0;

			// Get the current transform of the character
		virtual void setTransform( const hkTransform& transform ) = 0;

			// Get the potision
		virtual const hkVector4& getPosition() = 0;

			// Get the linear velocity of the proxy
		virtual void getLinearVelocity( hkVector4& velOut ) const = 0;

			// Set the linear velocity of the proxy
		virtual void setLinearVelocity( const hkVector4& newVelocity ) = 0;

			// Get the angular velocity of the proxy
		virtual hkReal getTurnVelocity() const;

			// Set the linear velocity of the proxy
		virtual void setTurnVelocity( const hkReal turnVelocity );
	
			// Return true if the proxy is supported
		virtual bool isSupported( hkReal timestep ) const = 0;

			// Get the forward direction in local space
		const hkVector4& getForwardLocal() const;

			// The the up direction in local space
		const hkVector4& getUpLocal() const;

		//const hkVector4& getGravity() const;

			// Set the collision filter information on the collision representation
		virtual void setCollisionFilterInfo(hkUint32 filterInfo) = 0;

			// Set the collision filter information on the collision representation
		virtual hkUint32 getCollisionFilterInfo( ) = 0;

		virtual hkpWorldObject* getWorldObject() = 0;


	private:

		// For character
		hkVector4 m_forwardLocal;
		hkVector4 m_upLocal;

		// For steering 
		hkReal m_turnVelocity;

};

#endif // HK_CHARACTER_PROXY_CHARACTER_H

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

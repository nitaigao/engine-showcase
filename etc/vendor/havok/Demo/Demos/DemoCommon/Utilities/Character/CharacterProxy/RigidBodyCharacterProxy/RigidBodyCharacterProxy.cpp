/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/RigidBodyCharacterProxy/RigidBodyCharacterProxy.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

RigidBodyCharacterProxy::RigidBodyCharacterProxy( RigidBodyCharacterProxyCinfo& info )
: CharacterProxy( info )
{
		
	//
	// Create a capsule to represent the character standing
	//

	hkReal totalHeight = info.m_characterHeight;
	hkReal radius = info.m_characterRadius;
	hkReal lineSegLen = totalHeight - radius - radius;

	hkVector4 vertexA; vertexA.setMul4( radius, info.m_upLocal );
	hkVector4 vertexB; vertexB.setMul4( radius + lineSegLen, info.m_upLocal );
	hkpCapsuleShape* characterShape = new hkpCapsuleShape(vertexA, vertexB, radius);

	//
	// Construct a character rigid body
	//
	hkpCharacterRigidBodyCinfo charInfo;
	charInfo.m_shape = characterShape;
	charInfo.m_up = info.m_upLocal;

	charInfo.m_position = info.m_position;
	charInfo.m_rotation = info.m_rotation;

	charInfo.m_collisionFilterInfo = info.m_collisionFilterInfo;

	m_characterRb = new hkpCharacterRigidBody( charInfo );
	characterShape->removeReference();
}

RigidBodyCharacterProxy::~RigidBodyCharacterProxy()
{
	m_characterRb->removeReference();
}


void RigidBodyCharacterProxy::addToWorld( hkpWorld* world )
{
	world->addEntity( m_characterRb->getRigidBody() );
}


// Get the current transform of the character
void RigidBodyCharacterProxy::getTransform( hkTransform& transform ) const
{
	transform = m_characterRb->getRigidBody()->getTransform();
}

// Get the current transform of the character
void RigidBodyCharacterProxy::setTransform( const hkTransform& transform )
{	
	// XXX todo - assert the transform keeps the character upright
	hkTransform fixed = transform;
	fixed.getRotation().renormalize();
	m_characterRb->getRigidBody()->setTransform( transform );
}

const hkVector4& RigidBodyCharacterProxy::getPosition()
{
	return m_characterRb->getRigidBody()->getTransform().getTranslation();
}

// Get the linear velocity of the proxy
void RigidBodyCharacterProxy::getLinearVelocity( hkVector4& velOut ) const
{
	velOut = m_characterRb->getRigidBody()->getLinearVelocity();
}

// Set the linear velocity of the proxy
void RigidBodyCharacterProxy::setLinearVelocity( const hkVector4& newVelocity )
{
	m_characterRb->getRigidBody()->setLinearVelocity( newVelocity );
}

// Return true if the proxy is supported
bool RigidBodyCharacterProxy::isSupported( hkReal timestep ) const
{
	// Check support
	hkpSurfaceInfo ground;

	hkStepInfo stepInfo; stepInfo.m_deltaTime = timestep; stepInfo.m_invDeltaTime = 1.f / timestep;
	m_characterRb->checkSupport(stepInfo, ground);
	return ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;
}

void RigidBodyCharacterProxy::setCollisionFilterInfo(hkUint32 filterInfo)
{
	m_characterRb->getRigidBody()->setCollisionFilterInfo(filterInfo);
	m_characterRb->getRigidBody()->getWorld()->updateCollisionFilterOnEntity( m_characterRb->getRigidBody(),
		HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK,	HK_UPDATE_COLLECTION_FILTER_IGNORE_SHAPE_COLLECTIONS	);
}

hkUint32 RigidBodyCharacterProxy::getCollisionFilterInfo( )
{
	return 	m_characterRb->getRigidBody()->getCollisionFilterInfo( );
}


hkpWorldObject* RigidBodyCharacterProxy::getWorldObject()
{
	return m_characterRb->getRigidBody();
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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityDemo.h>
#include <Demos/Physics/UseCase/PlanetGravity/SimpleGravityPhantom.h>
#include <Demos/Physics/UseCase/PlanetGravity/PlanetGravityAction.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

void SimpleGravityPhantom::addOverlappingCollidable( hkpCollidable* handle )
{
	hkpRigidBody* theBody = hkGetRigidBody( handle );

	if( theBody 
		&& theBody->getMotionType() != hkpMotion::MOTION_FIXED
		&& !hkString( theBody->getName() ).beginsWith( "planet" )
		&& !hkString( theBody->getName() ).beginsWith( "Turret" ) )
	{
		hkBool actionFound = false;

		for( int i = 0; i < theBody->getNumActions(); i++ )
		{
			if( theBody->getAction(i)->getUserData() == PlanetGravityAction::HK_SPHERE_ACTION_ID )
			{
				if( static_cast<PlanetGravityAction*>( theBody->getAction(i) )->accessPhantomId()
					== reinterpret_cast<hkUlong>( this ) )
				{
					// Do not add action if one already exists for this rigid body
					actionFound = true;
					break;
				}
			}
		}

		if( !actionFound )
		{
			// Add an action
			PlanetGravityAction* newGravity = new PlanetGravityAction( m_body, theBody, m_hullCollidable, reinterpret_cast<hkUlong>( this ) );
			theBody->getWorld()->addAction( newGravity );
			newGravity->removeReference();
		}
	}

	hkpAabbPhantom::addOverlappingCollidable( handle );
}

void SimpleGravityPhantom::removeOverlappingCollidable( hkpCollidable* handle )
{
	hkpRigidBody* theBody = hkGetRigidBody( handle );
	hkBool actionFound = false;

	if( theBody != HK_NULL )
	{
		for( int i = 0; i < theBody->getNumActions(); i++ )
		{
			// Is there already a PlanetGravityAction on the planet?
			if( theBody->getAction(i)->getUserData() == PlanetGravityAction::HK_SPHERE_ACTION_ID )
			{
				if( static_cast<PlanetGravityAction*>( theBody->getAction(i) )->accessPhantomId()
					== reinterpret_cast<hkUlong>( this ) )
				{
					if( !actionFound )
					{
						theBody->getWorld()->removeActionImmediately( theBody->getAction(i) );
						actionFound = true;
					}
					else
					{
						HK_ASSERT2( 0x0, 0, "Add remove not synchronized.\n" );
					}
				}
			}
		}
	}

	hkpAabbPhantom::removeOverlappingCollidable( handle );
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

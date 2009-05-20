/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickConstraintListener.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>

BrickConstraintListenerEntities::BrickConstraintListenerEntities()
{
}

BrickConstraintListenerEntities::~BrickConstraintListenerEntities() 
{
}


///Called when a constraint is removed from the world.
void BrickConstraintListenerEntities::constraintRemovedCallback( hkpConstraintInstance* constraint ) 
{
	hkpRigidBody* bodyA = constraint->getRigidBodyA();
	hkpRigidBody* bodyB = constraint->getRigidBodyB();

	// update filter info of the affected bodies
	int res = BrickFilter::removeConstaintFromMask(bodyA,bodyB);
	if(res==1)
	{
		bodyA->getWorld()->updateCollisionFilterOnEntity(
			bodyA,
			HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK ,
			HK_UPDATE_COLLECTION_FILTER_IGNORE_SHAPE_COLLECTIONS
		);
	}
	else if(res==2)
	{	
		bodyB->getWorld()->updateCollisionFilterOnEntity(
			bodyB,
			HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK,
			HK_UPDATE_COLLECTION_FILTER_IGNORE_SHAPE_COLLECTIONS
		);
	}
}

BrickConstraintListenerNotify::BrickConstraintListenerNotify(WallSimulation* simulation):
m_simulation(simulation)
{
}


///Called when a constraint is removed from the world.
void BrickConstraintListenerNotify::constraintRemovedCallback( hkpConstraintInstance* constraint ) 
{
	hkpRigidBody* bodyA = constraint->getRigidBodyA();
	hkpRigidBody* bodyB = constraint->getRigidBodyB();

	BrickFilter::removeConstaintFromMask(bodyA,bodyB);
	m_simulation->notifyBrokenConstraintInWall( BrickFilter::getWallID(*bodyA) );
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

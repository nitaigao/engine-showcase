/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline int WallSimulation::reserveWallID()
{
	return m_nextWallID++;
}

inline void WallSimulation::addObjectToParallel( hkpRigidBody* obj )
{
	if(m_nonWallObjects.indexOf( obj ) == -1)
		m_nonWallObjects.pushBack( obj );
	m_parallelWorld->addEntity( obj, HK_ENTITY_ACTIVATION_DO_NOT_ACTIVATE );
	obj->removeReference();
}

inline WallWrapper* WallSimulation::getWallFromParallel(int wallID)
{
	HK_ASSERT2(0x69e2f414, m_wallsInSimulationWrappers.hasKey(wallID), "The requested wall is not on the list");
	return ( m_wallsInSimulationWrappers.getWithDefault(wallID, HK_NULL) );
}

inline hkpRigidBody* WallSimulation::getWallFromMain(int wallID)
{
	HK_ASSERT2(0x532475d9, m_wallsInSimulationBodies.hasKey(wallID), "The requested wall is not on the list");
	return ( m_wallsInSimulationBodies.getWithDefault(wallID, HK_NULL) );
}

inline void WallSimulation::getAllWallsBodies(hkArray<hkpRigidBody*>& bodies)
{
	hkPointerMap<int,hkpRigidBody*>::Iterator body_iter = m_wallsInSimulationBodies.getIterator();
	while(m_wallsInSimulationBodies.isValid(body_iter))
	{
		bodies.pushBack(m_wallsInSimulationBodies.getValue(body_iter));
		body_iter = m_wallsInSimulationBodies.getNext(body_iter);
	}
}

// NB not used by now, must be more general....
inline void WallSimulation::attachWallTo(int wallID, hkpRigidBody* otherBody)
{
	getWallFromParallel( wallID )->attachToGround(otherBody);
}

inline hkBool WallSimulation::isWallInSimulation(int wallID, const hkpRigidBody* wall) const
{
	// the wall is "present" iff it's in both the parallel and main walls list
	return ( (wallID > 0) && m_wallsInSimulationWrappers.hasKey(wallID) && (m_wallsInSimulationBodies.getWithDefault(wallID, HK_NULL)==wall) );
}

inline void WallSimulation::addWallBodyToMainWorld(hkpRigidBody* wallBody)
{
	m_mainWorld->addEntity(wallBody);
}

inline void WallSimulation::notifyBrokenConstraintInWall(int wallID)
{
	HK_ASSERT2(0x6460165c, m_wallsInSimulationWrappers.hasKey(wallID) ,"The wall is not in the simulation");
	if(m_wallsToBeChecked.indexOf(wallID)==-1)
		m_wallsToBeChecked.pushBack(wallID);
}

inline void WallSimulation::setWallLastImpactVelocities(int wallID, const hkVector4& linear, const hkVector4& angular)
{
	BodyVelocities* velocities = m_wallsLastImpactVelocities.getWithDefault( wallID, HK_NULL );
	HK_ASSERT2(0x14794b75, velocities!=HK_NULL, "no sored velocity for this wall");
	velocities->linear = linear;
	velocities->angular = angular;
}

// runs the simulation
inline void WallSimulation::simulate()
{
	m_parallelWorld->stepDeltaTime( m_timeStep );
}

inline void WallSimulation::findSample( const WallWrapper* const wrapper, hkArray<int>& sampleBricks )
{
	hkReal perc = 0.25f;
	int jump = static_cast<int>( hkMath::ceil( wrapper->getNumBricks()*perc ) );

	int i=0;
	while(i<wrapper->getNumBricks())
	{
		sampleBricks.pushBack( i );
		i+=jump;
	}
}


inline void WallSimulation::transformPointFromWorldToWallLocal(const hkpRigidBody& wall, const hkVector4& worldPoint, hkVector4& localPoint)
{
	localPoint.setTransformedInversePos( wall.getTransform(), worldPoint);
	hkAabb aabb;
	// get wall AABB..
	wall.getCollidable()->getShape()->getAabb(hkTransform::getIdentity(), 0, aabb);
	// ...and express the contact point with respect to the lower left corner
	localPoint(0) += aabb.m_max(0);  // it's like adding width/2 and height/2
	localPoint(1) += aabb.m_max(1);
}

inline WallFractureCollisionListener* WallSimulation::getCollisionListener()
{
	return m_collisionListener;
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

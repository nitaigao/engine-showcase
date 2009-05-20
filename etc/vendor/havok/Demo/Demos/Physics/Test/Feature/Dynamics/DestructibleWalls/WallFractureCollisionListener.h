/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef WALL_FRACTURE_COLLISION_LISTENER
#define WALL_FRACTURE_COLLISION_LISTENER

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include<Physics/Dynamics/Constraint/Response/hkpSimpleCollisionResponse.h>


class WallSimulation;
class WallWrapper;



class WallFractureCollisionListener : public hkReferencedObject, public hkpCollisionListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	WallFractureCollisionListener(WallSimulation*, hkBool activate=true);
	~WallFractureCollisionListener();

	// hkpCollisionListener interface implementation
	virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event ) {}

	// hkpCollisionListener interface implementation
	virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {}

	/// The callback initiating the fracture
	virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event );

	// called after collision detection has been executed
	//virtual void contactProcessCallback( hkpContactProcessEvent& event ) {};

	inline void activate();
	inline void deactivate();
	
private:
	void mySolveSingleContact( const hkContactPoint& cp, hkTime time, const hkpSimpleConstraintUtilCollideParams& params, 
		const hkpMotion* bodyA, const hkpMotion* bodyB, hkpDynamicsContactMgr* contactMgr, hkpSimpleCollisionResponse::SolveSingleOutput& output );
private:
	// the proxy to the parallel simulation
	WallSimulation* m_parallelSimulation;
	// true by default
	hkBool m_isActive;
};

#include<Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureCollisionListener.inl>

#endif

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

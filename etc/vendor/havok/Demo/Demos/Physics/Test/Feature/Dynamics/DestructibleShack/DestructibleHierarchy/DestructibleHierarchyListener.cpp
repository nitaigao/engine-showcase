/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>





#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchyListener.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleShack/DestructibleHierarchy/DestructibleHierarchy.h>

#include <Physics/Dynamics/Constraint/Response/hkpSimpleCollisionResponse.h>
#include <Physics/ConstraintSolver/SimpleConstraints/hkpSimpleConstraintUtil.h>



namespace
{
	// Dummy contact mgr, required by hkpSimpleCollisionResponse::solveSingleContact()
	class NullContactMgr : public hkpDynamicsContactMgr
	{
	public:
		NullContactMgr() : hkpDynamicsContactMgr( hkpContactMgr::TYPE_NULL_CONTACT_MGR ){}
		virtual Type getType() const { return hkpDynamicsContactMgr::Type(-1); }
		virtual hkContactPointId addContactPointImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp ) { return HK_INVALID_CONTACT_POINT; }
		virtual hkResult reserveContactPointsImpl( int numPoints ) { return HK_FAILURE; }
		virtual void removeContactPointImpl( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner ) {}
		virtual void processContactImpl( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData ) {}
		virtual void cleanup() {}
		virtual ToiAccept addToiImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& propertiesOut ) { return TOI_REJECT; }
		virtual void removeToiImpl( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties ) {}
		virtual void processToi( struct hkpToiEvent& event, hkReal rotateNormal, class hkArray<class hkpEntity*>& outToBeActivated ) {}

	};
}

static hkReal computeSingleImpulse(const hkpRigidBody* bodyA, const hkpRigidBody* bodyB, const hkContactPoint& contactPoint)
{
	hkpMotion* motion[2];
	motion[0] = bodyA->getRigidMotion();
	motion[1] = bodyB->getRigidMotion();

	hkpSimpleConstraintUtilCollideParams params;
	params.m_externalSeperatingVelocity = 0.0f;
	params.m_extraSeparatingVelocity = 0.0f; 
	params.m_extraSlope = 0.0f;

	params.m_friction = hkpMaterial::getCombinedFriction( bodyA->getMaterial().getFriction(), bodyB->getMaterial().getFriction() ); 
	params.m_restitution = hkpMaterial::getCombinedRestitution( bodyA->getMaterial().getRestitution(), bodyB->getMaterial().getRestitution() );

	NullContactMgr nullContactMgr;
	hkpSimpleCollisionResponse::SolveSingleOutput result;
	hkReal time[2];
	for (int m = 0; m < 2; m++)
	{
		time[m] = motion[m]->getMotionState()->getSweptTransform().getBaseTime(); 
		hkReal invDeltaTime = motion[m]->getMotionState()->getSweptTransform().getInvDeltaTime();
		if (invDeltaTime > 0.0f)
		{
			time[m] += 1.0f / invDeltaTime;
		}
	}
	// one of the bodies may be fixed, and has it's time set to 0.0f
	hkReal maxTime = hkMath::max2(time[0], time[1]);

	hkpSimpleCollisionResponse::solveSingleContact( contactPoint, maxTime, params, motion[0], motion[1], &nullContactMgr, result );

	return result.m_impulse;
}

static hkUint16 getDestructibleNodeLookupIndex(const hkpCdBody& body)
{
	hkUint16 lookupIdx = hkUint16((hkUlong(body.getShape()->getUserData()) >> 16) & 0xffff);
	if (!lookupIdx && body.getParent())
	{
		lookupIdx = hkUint16((hkUlong(body.getParent()->getShape()->getUserData())>>16) & 0xffff);
	}
	return lookupIdx;
}

static hkpShapeKey getCdBodyShapeKey(const hkpCdBody& body)
{
	hkpShapeKey shapeKey = body.getShapeKey();
	if (shapeKey == HK_INVALID_SHAPE_KEY && body.getParent())
	{
		shapeKey = body.getParent()->getShapeKey();
	}
	return shapeKey;
}


void DestructibleHierarchyListener::contactPointAddedCallback(hkpContactPointAddedEvent& event)
{

	hkUint16 lookupIdx = 0;
	hkpShapeKey shapeKey = HK_INVALID_SHAPE_KEY;
	hkpRigidBody* body = HK_NULL;

	lookupIdx = getDestructibleNodeLookupIndex(*event.m_bodyA);
	if ( lookupIdx )
	{
		shapeKey = getCdBodyShapeKey(*event.m_bodyA);
		body = hkGetRigidBody(event.m_bodyA->getRootCollidable());
	}
	else 
	{
		lookupIdx = getDestructibleNodeLookupIndex(*event.m_bodyB);

		if ( lookupIdx  )
		{
			shapeKey = getCdBodyShapeKey(*event.m_bodyB);
			body = hkGetRigidBody(event.m_bodyB->getRootCollidable());
		}
		else
		{	
			// this happens when non-destructible parts of rigid bodies collide.
			return;
		}
	}

	// collided with a tagged shape
	hkReal impulse;
	{
		hkpRigidBody* bodies[] = { hkGetRigidBody(event.m_bodyA->getRootCollidable()), hkGetRigidBody(event.m_bodyB->getRootCollidable() ) };
		impulse = computeSingleImpulse(bodies[0], bodies[1], *event.m_contactPoint);
	}
	HK_ASSERT2(0xad5677da, impulse >= 0.0f, "Invalid impulse computed.");

		// Ignore small impulses -- likely to be due to resting contact. 
		// Set threshold to twice the impulse applied to the object every frame when resting on a flat surface
	//const hkReal threshold = body->getMass() * 9.81f / 30.0f * 2.0f; 
	//if (impulse < threshold)
	//{
	//	return;
	//}

	DestructibleHierarchy* destructibleHierarchy = DestructibleHierarchy::getDestructibleHierarchy(lookupIdx);
	unsigned int nodeIdx = DestructibleHierarchy::getNodeIndex(lookupIdx);
	HK_ASSERT2(0xad6784dd, nodeIdx < (unsigned int)(destructibleHierarchy->m_nodes.getSize()), "node index out of range");

	DestructibleHierarchyCollisionEvent ev;
	ev.m_hierarchy = destructibleHierarchy;
	ev.m_nodeIdx = nodeIdx;
	ev.m_impulse = impulse;
	ev.m_contactPoint = *event.m_contactPoint;
	ev.m_body = body;

	if (!event.isToi())
	{
		ev.m_contactMgr = event.m_internalContactMgr;
		ev.m_contactPointId = event.asManifoldEvent().m_contactPointId;
		ev.m_shapeKey = shapeKey;
	}
	else
	{
		HK_ASSERT2(0xad678a8d, false, "Toi events are not fully handled.");
		ev.m_contactMgr = HK_NULL;
		ev.m_contactPointId = 0;
		ev.m_shapeKey = HK_INVALID_SHAPE_KEY;
	}
	m_collisionEvents.pushBack(ev);
}    

void DestructibleHierarchyListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
	for(int e = 0; e < m_collisionEvents.getSize(); e++)
	{
		DestructibleHierarchyCollisionEvent& ev = m_collisionEvents[e];
		if (ev.m_contactMgr && ev.m_contactMgr == event.m_internalContactMgr && ev.m_contactPointId == event.m_contactPointId)
		{
			m_collisionEvents.removeAt(e);
			e--;
		}
	}
}

void DestructibleHierarchyListener::processCollisionEvents()
{
	for (int e = 0; e < m_collisionEvents.getSize(); e++)
	{
		// process me
		DestructibleHierarchyCollisionEvent& event = m_collisionEvents[e];

		event.m_hierarchy->applyDamage(event.m_nodeIdx, event.m_impulse, m_collisionEvents);
	}

	m_collisionEvents.clear();
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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureCollisionListener.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/SetBodyVelocityAsCriticalOperation.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallWrapper.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/Dynamics/Constraint/Contact/hkpSimpleContactConstraintData.h>

#include <Physics/ConstraintSolver/SimpleConstraints/hkpSimpleConstraintUtil.h>
#include <Physics/Dynamics/Constraint/Response/hkpSimpleCollisionResponse.h>

//
#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Base/Math/SweptTransform/hkSweptTransformUtil.h>
//

WallFractureCollisionListener::WallFractureCollisionListener(WallSimulation* simulation, hkBool active):
m_parallelSimulation(simulation),
m_isActive(active)
{
	HK_ASSERT2(0x17df35bc, m_parallelSimulation!=HK_NULL, "Adding a NULL world simulation to the collision listener");
}

WallFractureCollisionListener::~WallFractureCollisionListener()
{
}


// Dummy contact mgr, required by hkpSimpleCollisionResponse::solveSingleContact()
class MyNullContactMgr : public hkpDynamicsContactMgr
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	MyNullContactMgr() : hkpDynamicsContactMgr( hkpContactMgr::TYPE_NULL_CONTACT_MGR ){}
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

/// The callback initiating the fracture
void WallFractureCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{	
	if(m_isActive)
	{
		//const hkReal velThreshold = 1.0f;
		// find the wall and the body causing the fracture
		hkpRigidBody* wall = reinterpret_cast<hkpRigidBody*>(event.m_callbackFiredFrom);
		hkpRigidBody* other = reinterpret_cast<hkpRigidBody*>(hkSelectOther(wall, hkGetRigidBody(event.m_collidableA), hkGetRigidBody(event.m_collidableB)));

		// get ids of colliding objects
		int ID1 = BrickFilter::getWallID(*wall);
		int ID2 = BrickFilter::getWallID(*other);
		// check if they are in simulation
		if( !m_parallelSimulation->isWallInSimulation(ID1, wall) )
			ID1 = -1;  
		if( !m_parallelSimulation->isWallInSimulation(ID2, other) )
			ID2 = -1; 

		// if at least one of the bodies is in the simulation we must notify the collision event
		if(ID1!=-1 || ID2!= -1)
		{
			/*hkpSimpleContactConstraintData* constraintData = const_cast<hkpSimpleContactConstraintData*>(event.m_contactData);*/

			//if(!event.isToi())   // remove comment to discard TOI events
			{
				// it's TOI event, ContraintData is not provided, we must compute the impulse
				hkpSimpleConstraintUtilCollideParams params;
				{
					params.m_maxImpulse = 400.0f;
					params.m_externalSeperatingVelocity = /*0.0f;*/ event.m_projectedVelocity;
					params.m_extraSeparatingVelocity = .0f;
					params.m_extraSlope = .0f;
					params.m_friction = hkpMaterial::getCombinedFriction( wall->getMaterial().getFriction(), other->getMaterial().getFriction() );
					params.m_restitution = hkpMaterial::getCombinedRestitution( wall->getMaterial().getRestitution(), other->getMaterial().getRestitution() );
				}
				hkpSimpleCollisionResponse::SolveSingleOutput output;

				MyNullContactMgr ncm;
				//hkpDynamicsContactMgr* cm = const_cast<hkpDynamicsContactMgr*>(event.getContactMgr());
				// ask to solve this collision event
				
				hkVector4 linearA, linearB, angularA, angularB;
				linearA = wall->getLinearVelocity();
				linearB = other->getLinearVelocity();
				angularA = wall->getAngularVelocity();
				angularB = other->getAngularVelocity();

				//hkpSimpleCollisionResponse::solveSingleContact(*(event.m_contactPoint), wall->getWorld()->getCurrentTime(), params, 
				//	wall->getMotion(), other->getMotion(), &ncm, output);
				mySolveSingleContact(*(event.m_contactPoint), wall->getWorld()->getCurrentTime(), params, 
					wall->getMotion(), other->getMotion(), &ncm, output);
	
				// and notify the collision event along with the impulse, the contact point and all collision data
				hkVector4 impulse( event.m_contactPoint->getSeparatingNormal() );
				impulse.mul4( output.m_impulse );

				{
					CollisionBodiesVelocities cbv( linearA,linearB,angularA,angularB );
					m_parallelSimulation->notifyCollisionEvent( ID1, ID2, impulse, event.m_contactPoint->getPosition(), cbv);
				}
			}
			//else
			//{
			//	// it's a PSI event, we take a pointer do the ConstraintData where the solver results will be stored.
			//	//eventData = new WallCollisionEventData(ID1, ID2, constraintData);
			//}
		} // end ID1 || ID2
	} // end IsActive
}


void WallFractureCollisionListener::mySolveSingleContact( const hkContactPoint& cp, hkTime time, const hkpSimpleConstraintUtilCollideParams& params, 
														 const hkpMotion* bodyA, const hkpMotion* bodyB, hkpDynamicsContactMgr* contactMgr, hkpSimpleCollisionResponse::SolveSingleOutput& output )
{

	hkpSimpleConstraintInfoInitInput inA;
	{
		inA.m_invMass = bodyA->getMassInv();
		hkVector4 massCenter; 
		hkSweptTransformUtil::calcCenterOfMassAt( bodyA->m_motionState, time, massCenter );
		inA.m_massRelPos.setSub4( cp.getPosition(), massCenter );
		bodyA->getInertiaInvWorld( inA.m_invInertia );
	}

	hkpSimpleConstraintInfoInitInput inB;
	{
		inB.m_invMass = bodyB->getMassInv();
		hkVector4 massCenter; 
		hkSweptTransformUtil::calcCenterOfMassAt( bodyB->m_motionState, time, massCenter );
		inB.m_massRelPos.setSub4( cp.getPosition(), massCenter );
		bodyB->getInertiaInvWorld( inB.m_invInertia );
	}

	hkpBodyVelocity velA;
	hkpBodyVelocity velB;
	{
		velA.m_linear  = bodyA->getLinearVelocity();
		velA.m_angular = bodyA->getAngularVelocity();
		velB.m_linear  = bodyB->getLinearVelocity();
		velB.m_angular = bodyB->getAngularVelocity();
	}

	// use collision-related velocities for calculation of m_velocityKeyframedA/B


	hkpBodyVelocity origVelA; 
	hkSweptTransformUtil::getVelocity( bodyA->m_motionState, origVelA.m_linear, origVelA.m_angular);
	hkpBodyVelocity origVelB; 
	hkSweptTransformUtil::getVelocity( bodyB->m_motionState, origVelB.m_linear, origVelB.m_angular);

	hkpSimpleConstraintInfo info;
	hkRotation directions;
	hkVector4Util::buildOrthonormal( cp.getNormal(), directions );

	hkSimpleConstraintUtil_InitInfo( inA, inB, directions, info );

	// This takes the m_extarnalSeparatingVelocity param as the current collision-detection velocities 
	output.m_impulse = hkSimpleConstraintUtil_Collide( info, const_cast<hkpSimpleConstraintUtilCollideParams&>(params), velA, velB );
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

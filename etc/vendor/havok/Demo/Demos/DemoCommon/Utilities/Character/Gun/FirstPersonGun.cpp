/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/Gun/FirstPersonGun.h>

#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>



hkResult FirstPersonGun::sweepSphere(const hkpWorld* world, const hkVector4& sweepStart, hkReal radius, const hkVector4& sweepEnd, SweepSphereOut& out )
{
	// do linear sweep

	//Create casting object
	hkpSphereShape* sphereShape = new hkpSphereShape( radius );
	hkTransform startTransform; startTransform.setIdentity();
	startTransform.getTranslation() = sweepStart ;

	hkpCollidable collidable( sphereShape, &startTransform );

	hkpClosestCdPointCollector castCollector;
	{
		hkpLinearCastInput linearCastInput;
		linearCastInput.m_to = sweepEnd;
		world->linearCast( &collidable, linearCastInput, castCollector);
	}

	sphereShape->removeReference();

	if (castCollector.hasHit())
	{
		out.m_contactPoint = castCollector.getHitContact();
		hkpRigidBody* hitEntity = hkGetRigidBody( castCollector.getHit().m_rootCollidableB );
		out.m_body = hitEntity;
		out.m_shapeKey = castCollector.getHit().m_shapeKeyB;
		return HK_SUCCESS;
	}
	else
	{

		out.m_body = HK_NULL;
		out.m_contactPoint.setPosition( sweepEnd );
		out.m_contactPoint.setNormal( hkVector4::getZero() );
		out.m_shapeKey = HK_INVALID_SHAPE_KEY;
		return HK_FAILURE;
	}
}

void FirstPersonGun::calcVelocityToTarget(const hkVector4& position, const hkVector4& target, const hkVector4& gravity, hkReal speed, hkVector4& velocity)
{
	hkVector4 dist; dist.setSub4(target, position); 
	hkReal distLen = dist.length3();
	if (distLen > HK_REAL_EPSILON)
	{
		hkReal time = distLen / speed;
		hkVector4 extraVelocity; extraVelocity.setMul4(-0.5f * time, gravity);
		hkReal extraVelLen = extraVelocity.length3();
		if (extraVelLen > speed)
		{
			// clip extra velocity .. to 45degree deflection
			extraVelocity.mul4(speed/extraVelLen);
		}
		velocity.setAddMul4(extraVelocity, dist, speed / distLen);
	}
	else
	{
		// Ignore:
		velocity = dist;
	}

}

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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

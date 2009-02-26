/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Character/Gun/BallGun.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>


BallGun::BallGun( int numBullets, const char* blueprintName )
{
	m_bulletRadius = 0.2f;
	m_bulletVelocity = 200.0f;
	m_maxBulletsInWorld = numBullets;
	m_addedBodies.setCapacity(numBullets);
	m_addedDisplayObjects.setCapacity(numBullets);
	m_damageMultiplier = 50.0f;
	m_bulletMass = 50.f;
	m_displayShape = HK_NULL;
	m_context = HK_NULL;
	m_displayHandler = HK_NULL;
	m_bulletOffsetFromCenter.setZero4();
	m_blueprintName = blueprintName;
}

BallGun::BallGun( hkgDisplayContext* ctx, hkgDisplayHandler* dspHandler, int numBullets, const hkgDisplayObject* dspShape )
{
	m_bulletRadius = 0.2f;
	m_bulletVelocity = 200.0f;
	m_maxBulletsInWorld = numBullets;
	m_addedBodies.setCapacity(numBullets);
	m_addedDisplayObjects.setCapacity(numBullets);
	m_damageMultiplier = 50.0f;
	m_bulletMass = 50.f;
	m_displayShape = dspShape;
	if (m_displayShape) m_displayShape->addReference();
	m_context = ctx;
	m_displayHandler = dspHandler;
	m_bulletOffsetFromCenter.setZero4();
	m_blueprintName = HK_NULL;
}

BallGun::~BallGun()
{
	reset( HK_NULL );
	if (m_displayShape) 
	{
		m_context->lock();
		m_displayShape->removeReference();
		m_context->unlock();
	}
}


void BallGun::fireGun( hkDemoEnvironment* env, hkpWorld* world, const hkTransform& viewTransform )
{
	hkVector4 forwardVector = viewTransform.getRotation().getColumn(0);

	hkVector4 sweepStart;
	{
		sweepStart.set(0.0f, 1.5f, 0.0f);
		sweepStart.setAdd4(viewTransform.getTranslation(), forwardVector);
	}

	hkpRigidBodyCinfo ci;
	{
		ci.m_shape                   = new hkpSphereShape( m_bulletRadius );
		ci.m_mass                    = m_bulletMass;
		ci.m_qualityType             = HK_COLLIDABLE_QUALITY_BULLET;
		ci.m_allowedPenetrationDepth = 0.01f;
		ci.m_restitution             = 0.2f;
		ci.m_friction                = 1.0f;
		ci.m_motionType              = hkpMotion::MOTION_SPHERE_INERTIA;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( ci.m_shape, m_bulletMass*2.0f, ci );
		ci.m_mass                    = m_bulletMass;
		ci.m_position                . setAdd4(sweepStart, m_bulletOffsetFromCenter);
		ci.m_linearDamping           = 0;
		ci.m_angularDamping          = 0.4f;
	}

	hkReal sweepLength = 200.0f;
	hkVector4 sweepEnd; sweepEnd.setAddMul4(sweepStart, forwardVector, sweepLength);
	FirstPersonGun::SweepSphereOut target;
	if (HK_SUCCESS == FirstPersonGun::sweepSphere(world, sweepStart, m_bulletRadius, sweepEnd, target))
	{
		FirstPersonGun::calcVelocityToTarget(ci.m_position, target.m_contactPoint.getPosition(), world->getGravity(), m_bulletVelocity, ci.m_linearVelocity);
	}
	else
	{
		ci.m_linearVelocity.setMul4( m_bulletVelocity, forwardVector );
	}

	hkpRigidBody* bullet = new hkpRigidBody( ci );
	ci.m_shape->removeReference();

	bullet->m_damageMultiplier = m_damageMultiplier;

	if (m_displayShape)
	{
		// create a display object for it that shares all except the transform
		HK_ASSERT(0, m_context);
		HK_ASSERT(0, m_displayHandler);
		hkgDisplayObject* newDisplayObject = m_displayShape->copy(HKG_DISPLAY_OBJECT_SHARE_GEOM, m_context);
		m_displayHandler->addPrecreatedDisplayObject( (hkUlong)(bullet->getCollidable()), newDisplayObject );
		m_addedDisplayObjects.enqueue(newDisplayObject);
	}
	else
	{
		m_addedDisplayObjects.enqueue(HK_NULL);
	}

	world->addEntity( bullet );

	m_addedBodies.enqueue( bullet );
	if (m_addedBodies.getSize() > m_maxBulletsInWorld)
	{
		hkpRigidBody* rb;
		m_addedBodies.dequeue( rb );
		if ( rb->getWorld() )
		{
			world->removeEntity( rb );
		}
		rb->removeReference();

		hkgDisplayObject* dsp;
		m_addedDisplayObjects.dequeue(dsp);
		if (dsp) dsp->removeReference();
	}
}

void BallGun::reset(hkpWorld* world)
{
	while (!m_addedBodies.isEmpty() )
	{
		hkpRigidBody* rb;
		m_addedBodies.dequeue( rb );
		if ( rb->getWorld() )
		{
			rb->getWorld()->removeEntity( rb );
		}
		rb->removeReference();
	}

	while (!m_addedDisplayObjects.isEmpty())
	{
		hkgDisplayObject* dsp;
		m_addedDisplayObjects.dequeue(dsp);
		if (dsp) dsp->removeReference();
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

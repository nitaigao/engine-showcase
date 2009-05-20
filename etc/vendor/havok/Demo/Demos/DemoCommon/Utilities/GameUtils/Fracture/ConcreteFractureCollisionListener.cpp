/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/ConcreteFractureCollisionListener.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/SetBodyVelocityAsCriticalOperation.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/FractureUtil.h>

ConcreteFractureCollisionListener::ConcreteFractureCollisionListener( hkArray<hkpRigidBody*>* outputArrayForFracturedBodies, hkReal fractureVelocity )
	: FractureCollisionListener(outputArrayForFracturedBodies)
{
	m_fractureVelocity = fractureVelocity;
	m_craterDiameter = 0.2f;
}



void ConcreteFractureCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{
	m_randomSeed++;

		// Check whether our velocity is strong enough to fracture the body.
		// You might also want to check the type of object, the mass, volume and the material
		// (soft ball hardly fractures a piece of glass)
	if (event.m_projectedVelocity < -m_fractureVelocity )
	{
			//
			// get and fix the direction of the normal;
			//
		hkContactPoint cp = *event.m_contactPoint;
		if (event.m_callbackFiredFrom->getCollidable() == event.m_collidableA)
		{
			hkVector4 tmp; tmp.setNeg4(cp.getSeparatingNormal());
			cp.setSeparatingNormal(tmp);
		}

			// get the hitting body, e.g. the bullet
		hkpRigidBody* otherBody = reinterpret_cast<hkpRigidBody*>(hkUlong(event.m_callbackFiredFrom) ^ hkUlong(event.m_collidableA->getOwner()) ^ hkUlong(event.m_collidableB->getOwner()));

			// Remember the bullets velocity so we can revert the bullet body back to its original velocity.
			//
			// The reasons is:
			//  - that this callback is called just before the bullet's velocities are updated
			//    (either in the PSI-integrate step or in a TOI-event-handling function), 
			//  - and at the world is locked at the time of this callback, so the bullet still interacts with the old/original
			//    unfractured body (instead of with the intended fractured pieces),
			// 
			// So, to have a 'more' proper fracturing interaction we do:
			//  - reset the velocity of the fracturing bullet once the fractured body is replaced by 
			//    its resulting pieces and the world is unlocked,
			//  - reintegrate and re-collide the bullet body, so that it collides with the fractured pieces
			//
		hkVector4 linearVel = otherBody->getLinearVelocity();
		hkVector4 angularVel = otherBody->getAngularVelocity();
		SetBodyVelocityAsCriticalOperation* callback = new SetBodyVelocityAsCriticalOperation(otherBody, linearVel, angularVel, true);

			//
			// Fracture the piece
			// 
		hkpRigidBody* originalBody = static_cast<hkpRigidBody*>(event.m_callbackFiredFrom);
		HK_ASSERT2(0xad23eb33, originalBody, "This listener must be attached as a hkpEntity-CollisonListener.");
		originalBody->removeCollisionListener(this);
		hkpWorld* world = otherBody->getWorld();
		hkInplaceArray<hkpRigidBody*, 32> newBodies;
		createFracturedRigidBodyPieces(world, originalBody, cp, newBodies);

			//
			// Replace the body with fractured pieces.
			//
		world->removeEntity(originalBody);
		world->addEntityBatch(reinterpret_cast<hkpEntity**>(newBodies.begin()), newBodies.getSize());

			//
			//	Add our undo velocities to the delayed operation queue. It will be executed as
			//  soon as all our fractured pieces are all added to the world.
			//
		if (!otherBody->isFixedOrKeyframed() )
		{
			if (!event.m_callbackFiredFrom->isFixedOrKeyframed())
			{
				// Warning: this can cause multiple TOI events. It is not safe with fixedOrKeyframed bodies. 
				world->queueCallback(callback);
			}
		}
		callback->removeReferenceLockUnchecked();

		//
		// If an external hkArray<hkpRigidBody*> is supplied then export the references to new bodies.
		//
		if (m_fracturedBodies)
		{
			m_fracturedBodies->insertAt(m_fracturedBodies->getSize(), newBodies.begin(), newBodies.getSize() );
		}
		else
		{
			for (int i = 0; i < newBodies.getSize(); i++)
			{
				newBodies[i]->removeReference();
			}
		}

	}
}


	/// This function replaces the original body with its fractured pieces. It uses a helper function to perform the actual fracture shapes.
	/// The shapes are then used here to create actual rigid bodies. 
	///
	/// This function serves as an example of how to use the fracture code. It is not intended to be used in a game
void ConcreteFractureCollisionListener::createFracturedRigidBodyPieces(hkpWorld* world, hkpRigidBody* body, const hkContactPoint& contactPointW, hkArray<hkpRigidBody*>& fracturedBodies)
{
		//	Preserve the color if it is stored in a property
	int oldColor = body->getProperty( HK_PROPERTY_DEBUG_DISPLAY_COLOR ).getInt();

	hkPseudoRandomGenerator generator(m_randomSeed);

		//
		//	Set up the fracture input 
		//
	FractureUtil::ConcreteFractureInput input;
	{
		// Assign shape to be fractured
		{
			HK_ASSERT2(0xadbbed3e, body->getCollidable()->getShape(), "Body must have a shape.");
			const hkpShape* shape = body->getCollidable()->getShape();
			input.m_shape = static_cast<const hkpConvexShape*>(shape);
		}
		// Fracture point information
		input.m_fractureOrigin.getPosition().setTransformedInversePos( body->getTransform(), contactPointW.getPosition());
		input.m_fractureOrigin.getSeparatingNormal().setRotatedInverseDir( body->getTransform().getRotation(), contactPointW.getNormal() );
		input.m_fractureOrigin.setDistance( contactPointW.getDistance() );

		// Basic parameters
		input.m_outerRadius = m_craterDiameter * 0.50f;
		input.m_innerRadius = m_craterDiameter * 0.25f;
		input.m_depth = m_craterDiameter * 0.25f;
		input.m_numSideFacets = 5;

		input.m_generateCenterPiece = false;

		input.m_generateBulletHoleDebris = false;

		// Randomization
		input.m_generator = &generator;
		input.m_randomizeOrientationAroundNormal = true;

		// Smallest object Volume
		input.m_minVolumeOfGeneratedShapes = 0.001f;// 0.3f * 0.3f * 0.3f;
	}

	//
	//  Create convex vertices shapes for the fractured pieces.
	//
	FractureUtil::ConcreteFractureOutput fracturedShapes;
	FractureUtil::fractureConcrete(input, fracturedShapes);

	//
	//  Convert new pieces to rigid bodies
	//
	hkArray<hkpRigidBody*>& newBodies = fracturedBodies;
	{
		hkpRigidBodyCinfo rigidBodyCinfo;	body->getCinfo(rigidBodyCinfo);

		hkReal originalVolume = body->getProperty(HK_PROPERTY_RIGID_BODY_VOLUME).getReal();
		if ( originalVolume == 0)
		{
			originalVolume = FractureUtil::calcShapeVolume( input.m_shape );
		}

			//
			//  Make body movable and
			//	limit the velocities of the fractured pieces
			//
		rigidBodyCinfo.m_maxLinearVelocity  = 50.0f;
		rigidBodyCinfo.m_maxAngularVelocity = 5.0f;
		rigidBodyCinfo.m_angularDamping     = 0.3f;
		rigidBodyCinfo.m_linearDamping      = 0.1f;
		rigidBodyCinfo.m_motionType         = hkpMotion::MOTION_DYNAMIC;
		if (rigidBodyCinfo.m_qualityType == HK_COLLIDABLE_QUALITY_FIXED)
		{
			rigidBodyCinfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
		}
		rigidBodyCinfo.m_allowedPenetrationDepth = -1.0f;
		rigidBodyCinfo.m_mass = ( rigidBodyCinfo.m_mass > 0.0f ) ? rigidBodyCinfo.m_mass : 1.0f;

			//
			// Create each of the rigid bodies in its principle axis frame. This is the space, where the inertia tensor
			// becomes a diagonal matrix.
			//

		FractureUtil::createRigidBodiesFromShapes(rigidBodyCinfo, originalVolume, FractureUtil::EXPORT_GEOMETRY_IN_PROPERTY, fracturedShapes.m_bulletHoleDebrisShapes, newBodies, HK_NULL);

		FractureUtil::createRigidBodiesFromShapes(rigidBodyCinfo, originalVolume, FractureUtil::EXPORT_GEOMETRY_IN_PROPERTY, fracturedShapes.m_shapes, newBodies, HK_NULL);
	}

	//
	// Remove shape references.
	//
	fracturedShapes.removeReferencesToShapes();

		//
		// set object color as property. 
		// 
	if (oldColor)
	{
		for (int i = 0; i < newBodies.getSize(); i++)
		{
			newBodies[i]->addProperty( HK_PROPERTY_DEBUG_DISPLAY_COLOR, oldColor );
		}
	}

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

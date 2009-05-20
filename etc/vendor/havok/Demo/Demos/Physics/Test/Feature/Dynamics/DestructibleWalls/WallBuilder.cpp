/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallBuilder.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>


#include <Physics/Dynamics/Constraint/Breakable/hkpBreakableConstraintData.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickConstraintListener.h>



BrickWallBuilder::BrickWallBuilder()
{
}

BrickWallBuilder::~BrickWallBuilder()
{
}

void BrickWallBuilder::buildBrickWall(const BrickwallBuilderDescriptor& bwDescriptor, hkArray<hkpRigidBody*>& bricksOut, hkArray<hkpConstraintInstance*>& constraintsOut ) 
{
	hkVector4 halfExtents( bwDescriptor.m_brickShape->getHalfExtents() );

	// move start point depending on wall size
	hkVector4 posX( hkVector4::getZero() ); 
	posX(0) = -bwDescriptor.m_width * halfExtents(0);
	posX(0) += halfExtents(0);
	posX(1) = -halfExtents(1); 
	
	// reserve space for all bricks and constraints
	bricksOut.reserve(bwDescriptor.m_height * bwDescriptor.m_width * 2);
	constraintsOut.reserve( (2 * bwDescriptor.m_height - 1) * (2 * bwDescriptor.m_width - 1) + bwDescriptor.m_width - 1);

	hkReal thresholdDelta = (bwDescriptor.m_strength > bwDescriptor.m_lowerThreshold)? (bwDescriptor.m_strength - bwDescriptor.m_lowerThreshold) /(bwDescriptor.m_height*2-1) : 0.0f;

	// set brick properties, used for all the bricks 
	hkpRigidBodyCinfo boxInfo;
	computeBrickInfo(bwDescriptor.m_brickShape, bwDescriptor.m_brickMass, boxInfo);
		
	// QUI
	int colOffset=bwDescriptor.m_height;

	for(int x=0;x<bwDescriptor.m_width;x++)   // along the ground, left to right
	{
		hkVector4 pos(posX);
		
		// breaking threshold for this row
		hkReal rowThreshold = bwDescriptor.m_strength;

		for(int y=0; y<bwDescriptor.m_height; y++)  // bottom to top
		{
			pos(1) += (halfExtents(1) + bwDescriptor.m_brickShape->getRadius()) * 2.0f;

			
			// build rigid body for brick
			int brickIndOne;

			{
				brickIndOne=bricksOut.getSize();
				boxInfo.m_position.setRotatedDir( bwDescriptor.m_transform.getRotation() /*bwDescriptor.m_orientation*/,pos);
				boxInfo.m_position.add4( bwDescriptor.m_transform.getTranslation() /*bwDescriptor.m_position*/);
				boxInfo.m_rotation.set( bwDescriptor.m_transform.getRotation() ); //bwDescriptor.m_orientation;
				bricksOut.pushBack(new hkpRigidBody(boxInfo));
			}

			// At each step 2 constraints are built:
			hkBool constraintsAdded[] = {false, false};
			// 1 - A constraint to the ground OR a constraint to the brick below the one just built
			if( y == 0 ) {  
				// attach the first row to the ground (if requested)
				if(bwDescriptor.m_attachToGround)
					constraintsOut.pushBack(buildBreakableConstraintInstance(bricksOut[brickIndOne], bwDescriptor.m_theGround, rowThreshold*1000));
			} else {
				// create a breakable constraint from the brick and the one below
				constraintsOut.pushBack(buildBreakableConstraintInstance(bricksOut[brickIndOne], bricksOut[brickIndOne - 1], rowThreshold));
				constraintsAdded[0]=true;
			}

			// 2 - a constraint to the brick to the left
			if( x > 0 )  // check if there is a previous column
			{
				// create a constraint from the new brick and the corresponding brick from the previous column 
				// the first row is made of unbreakable constraints
				if(y==0 && bwDescriptor.m_attachToGround)
					constraintsOut.pushBack(buildConstraintInstance(bricksOut[brickIndOne], bricksOut[brickIndOne - colOffset]));
				else
					constraintsOut.pushBack(buildBreakableConstraintInstance(bricksOut[brickIndOne], bricksOut[brickIndOne - colOffset], rowThreshold));
				constraintsAdded[1]=true;
			}

			if(bwDescriptor.m_setCollisionFilter) 
			{
				// Set filter info 
				bool b = constraintsAdded[0];
				bool l = constraintsAdded[1];
				hkUint32 filterInfo = BrickFilter::calcFilterInfo(y, x, bwDescriptor.m_wallID, l, b);
				bricksOut[brickIndOne]->setCollisionFilterInfo(filterInfo);
			}
			rowThreshold -= thresholdDelta;
		} // end of for(..y..)
		
		// advance to next col
		posX(0) += bwDescriptor.m_gapWidth + halfExtents(0)*2.0f;

	} // end of for(..x..)
}

void BrickWallBuilder::computeBrickInfo(const hkpShape* shape, const hkReal& mass, hkpRigidBodyCinfo& boxInfo) 
{
	// set properties for a brick
	boxInfo.m_mass = mass;
	hkpMassProperties massProperties;
	
	computeBrickPropertiesShape(shape, mass, massProperties);

	setProperties(massProperties, boxInfo);

	boxInfo.m_shape = shape;
}

void BrickWallBuilder::setProperties(const hkpMassProperties& massProperties, hkpRigidBodyCinfo& boxInfo) 
{
	boxInfo.m_mass = massProperties.m_mass;
	boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
	boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	boxInfo.m_solverDeactivation = boxInfo.SOLVER_DEACTIVATION_MEDIUM;
	boxInfo.m_restitution = 0.0f; // to save CPU 
	boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING; //HK_COLLIDABLE_QUALITY_DEBRIS;
	boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
}


hkFixedConstraintData* BrickWallBuilder::makeContraintData( hkpRigidBody* body1, hkpRigidBody* body2 )
{
	hkFixedConstraintData* cData = new hkFixedConstraintData(body1, body2);

	// Now we set the transform of the attached box in reference box space (the product
	// of inv(refToWorld) * attToWorld)
	hkTransform attToRef;
	{
		hkTransform refToWorld = body2->getTransform();
		hkTransform attToWorld = body1->getTransform();
		attToRef.setMulInverseMul(refToWorld, attToWorld);
	}

	// This modifies the internal parameters of the constraint as follows:
	cData->setTransformInRef(attToRef);

	return cData;
}


hkpBreakableConstraintData* BrickWallBuilder::makeConstraintBreakable(hkpConstraintData* old, const hkReal& threshold)
{
	// Create breakable constraint wrapper
	hkpBreakableConstraintData* breaker = new hkpBreakableConstraintData( old );
	breaker->setThreshold( threshold );
	breaker->setRemoveWhenBroken(true);
	breaker->setRevertBackVelocityOnBreak(true);

	return breaker;
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

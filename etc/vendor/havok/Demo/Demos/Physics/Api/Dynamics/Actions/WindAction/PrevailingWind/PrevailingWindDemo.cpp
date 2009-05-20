/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Dynamics/Actions/WindAction/PrevailingWind/PrevailingWindDemo.h>

#include <Physics/Utilities/Actions/Wind/hkpWindAction.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>

// We display the wind vectors
//#include <Common/Visualize/Shape/hkDisplayBox.h>

#include <Common/Base/Types/Color/hkColor.h>
#include <Common/Visualize/hkDebugDisplay.h>

// We need the following constraints
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

// pos is the position of its base.
void PrevailingWindDemo::createWindmill ( hkpWorld* world, const hkpWind* wind, const hkVector4& pos )
{
	const hkReal heightOfPole = 6.0f;
	const hkReal lengthOfShaft = 1.5f;
	// Proportion of shaft, from the propeller end, that it attaches to the pole
	const hkReal shaftPivotProportion = 0.2f;
	const hkReal shaftMass = 1.0f;
	const hkReal widthOfTailFin = 1.0f;
	const int numberOfBlades = 5;
	const hkReal bladeLength = 2.2f;
	const hkReal bladeWidth = 0.5f;
	const hkReal bladeMass = 0.2f;

	// We make the shaft-pivot constraint happy by ensuring the forces due to gravity are balanced.
	hkReal finMass;
	{
		const hkReal bladeMoment = ( numberOfBlades * bladeMass ) * ( bladeWidth + lengthOfShaft * shaftPivotProportion );
		const hkReal shaftMoment = shaftMass * ( 0.5f - shaftPivotProportion );
		const hkReal tailDistance = widthOfTailFin * 0.5f + lengthOfShaft * ( 1.0f - shaftPivotProportion );
		finMass = ( bladeMoment - shaftMoment ) / tailDistance;
	}

	hkpRigidBody* verticalPole;
	{
		hkpShape* shape = new hkpCylinderShape( hkVector4( 0.0f, 0.0f, 0.0f ), hkVector4( 0.0f, heightOfPole, 0.0f ), 0.1f );
		hkpRigidBodyCinfo info;
		{
			info.m_shape = shape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position = pos;
		}
		verticalPole = new hkpRigidBody( info );
		world->addEntity( verticalPole );
		shape->removeReference();
	}
	
	hkpRigidBody* head;
	{
		hkArray<hkpShape*> shapeArray;
		hkArray<hkpMassElement> massElements;

		// Add the shaft and calculate its mass properties.
		hkpShape* shaft;
		{
			hkVector4 halfExtents( 0.1f, 0.1f, lengthOfShaft / 2.0f );
			
			shaft = new hkpBoxShape( halfExtents, 0.0f );
			shapeArray.pushBack( shaft );
			
			hkpMassElement shaftMassElement;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties( halfExtents, shaftMass, shaftMassElement.m_properties );
			massElements.pushBack( shaftMassElement );			
		}

		hkpShape* tailFin;
		{
			hkVector4 halfExtents( 0.1f, widthOfTailFin, widthOfTailFin * 0.5f );
			hkVector4 translate( 0.0f, 0.0f, (widthOfTailFin + lengthOfShaft) * 0.5f );
			hkpBoxShape* finShape = new hkpBoxShape( halfExtents, 0.0f );
			tailFin = new hkpConvexTranslateShape( finShape, translate );
			finShape->removeReference();

			shapeArray.pushBack( tailFin );

			hkpMassElement finMassElement;

			hkpInertiaTensorComputer::computeBoxVolumeMassProperties( halfExtents, finMass, finMassElement.m_properties );
			finMassElement.m_transform.setIdentity();
			finMassElement.m_transform.setTranslation( translate );
			massElements.pushBack( finMassElement );
		}

		hkpShape* shape = new hkpListShape( shapeArray.begin(), shapeArray.getSize() );

		hkpRigidBodyCinfo info;
		{
			hkpMassProperties massProperties;
			{
				hkpInertiaTensorComputer::combineMassProperties(massElements, massProperties);
			}
			info.m_shape = shape;
			info.m_motionType = hkpMotion::MOTION_DYNAMIC;
			info.m_mass = massProperties.m_mass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_position.setAdd4( pos, hkVector4( 0.0f, heightOfPole + 0.2f, ( 0.5f - shaftPivotProportion ) * lengthOfShaft ) );
			//info.m_angularDamping = 0.4f;
		}
		
		head = new hkpRigidBody( info );
		tailFin->removeReference();
		shaft->removeReference();

		world->addEntity( head );
		
		hkpWindAction* action = new hkpWindAction( head, wind, 0.1f );
		world->addAction(action);
		action->removeReference();

		shape->removeReference();
	}

	// Create the constraint which keeps the pole on top.
	{
		hkpHingeConstraintData* hc = new hkpHingeConstraintData();
		hkVector4 pivot;
		pivot.setAdd4( pos, hkVector4(0.0f, heightOfPole, 0.0f) );
		
		hkVector4 axis( 0.0f, 1.0f, 0.0f );

		hc->setInWorldSpace(verticalPole->getTransform(), head->getTransform(), pivot, axis);
		
		hkpConstraintInstance* constraint = new hkpConstraintInstance( verticalPole, head, hc );
		world->addConstraint( constraint ); 
		constraint->removeReference();
		hc->removeReference();  
	}
	
	hkpRigidBody* propeller;
	{
		hkArray<hkpShape*> shapeArray;
		hkArray<hkpMassElement> massElements;

		for (int i = 0; i < numberOfBlades; i++)
		{
			hkpShape* blade;
			hkVector4 halfExtents( bladeWidth / 2.0f, bladeLength / 2.0f, 0.1f );
			hkTransform transform;
			{
				hkTransform transform0;
				{
					hkVector4 translate( bladeWidth * 0.48f, bladeLength * 0.6f, 0.0f );
					hkQuaternion rotation;
					{
						hkVector4 axis( 0.0f, 1.0f, 0.0f );
						rotation.setAxisAngle( axis, HK_REAL_PI * 0.25 );
						rotation.normalize();
					}
					transform0.setTranslation( translate );
					transform0.setRotation( rotation );
				}
				hkTransform transform1;
				{
					transform1.setIdentity();
					hkQuaternion rotation;
					{
						hkVector4 axis( 0.0f, 0.0f, 1.0f );
						rotation.setAxisAngle( axis, (HK_REAL_PI * 2.0f * i) / (hkReal) numberOfBlades );
						rotation.normalize();
					}
					transform1.setRotation( rotation );
				}
				transform.setMul( transform1, transform0 );
			}
			hkpBoxShape* bladeShape = new hkpBoxShape( halfExtents, 0.0f );
			blade = new hkpConvexTransformShape( bladeShape, transform );
			bladeShape->removeReference();

			shapeArray.pushBack( blade );

			hkpMassElement bladeMassElement;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties( halfExtents, bladeMass, bladeMassElement.m_properties );
			bladeMassElement.m_transform = transform;
			massElements.pushBack( bladeMassElement );
		}

		hkpShape* shape = new hkpListShape( shapeArray.begin(), shapeArray.getSize() );

		hkpRigidBodyCinfo info;
		{
			hkpMassProperties massProperties;
			{
				hkpInertiaTensorComputer::combineMassProperties(massElements, massProperties);
			}
			info.m_shape = shape;
			info.m_motionType = hkpMotion::MOTION_DYNAMIC;
			info.m_mass = massProperties.m_mass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_position.setAdd4( pos, hkVector4( 0.0f, heightOfPole + 0.2f, - shaftPivotProportion * lengthOfShaft - ( bladeWidth ) ) );
			//info.m_angularDamping = 0.3f;
		}
		
		propeller = new hkpRigidBody( info );
		
		for ( int i = 0; i < numberOfBlades; ++i )
		{
			shapeArray[i]->removeReference();
		}

		world->addEntity( propeller );
		
		hkpWindAction* action = new hkpWindAction( propeller, wind, 0.1f );
		world->addAction(action);
		action->removeReference();

		shape->removeReference();
	}

	// Create the constraint which keeps propeller attached to the head.
	{
		hkpHingeConstraintData* hc = new hkpHingeConstraintData();
		hkVector4 pivot;
		pivot.setAdd4( pos, hkVector4(0.0f, heightOfPole + 0.2f, - shaftPivotProportion * lengthOfShaft - ( bladeWidth ) ) );
		
		hkVector4 axis( 0.0f, 0.0f, 1.0f );

		hc->setInWorldSpace( head->getTransform(), propeller->getTransform(), pivot, axis );
		
		hkpConstraintInstance* constraint = new hkpConstraintInstance( head, propeller, hc );
		world->addConstraint( constraint ); 
		constraint->removeReference();
		hc->removeReference();  
	}
	
	propeller->removeReference();
	head->removeReference();
	verticalPole->removeReference();
}

void PrevailingWindDemo::createPalmTree ( hkpWorld* world, const hkpWind* wind, const hkVector4& pos )
{
	const hkReal trunkHeight = 4.0f;
	const hkReal trunkBottomRadius = 0.5f;
	const hkReal trunkTopRadius = 0.2f;
	const hkReal trunkStiffness = 0.1f;
	const hkReal segmentMass = 0.6f;
	const int numberOfSegments = 4;
	const hkReal segmentGap = 0.2f;
	const int numberOfFronds = 6;
	const hkReal frondWidth = 2.0f;
	const hkReal frondLength = 3.0f;
	const hkReal frondMass = 0.4f;
	
	// The trunk
	hkArray<hkpRigidBody*> trunk;

	const hkReal segmentHeight = (trunkHeight - ((numberOfSegments - 1) * segmentGap)) / numberOfSegments;
	const hkReal radiusIncrement = (trunkBottomRadius - trunkTopRadius) / numberOfSegments;

	for ( int i = 0; i < numberOfSegments; i++ )
	{
		hkpShape* segmentShape;
		hkpRigidBodyCinfo info;
		{
			hkVector4 bottom( 0.0f, (segmentHeight + segmentGap) * i, 0.0f );
			hkVector4 top( 0.0f, (segmentHeight + segmentGap) * i + segmentHeight, 0.0f );
			hkReal radius = trunkBottomRadius - (radiusIncrement * i);
			segmentShape = new hkpCylinderShape( bottom, top, radius, 0.03f );
				
			info.m_shape = segmentShape;
			info.m_position = pos;
			
			if (i == 0)
			{
				info.m_motionType = hkpMotion::MOTION_FIXED;
			}
			else
			{
				hkpMassProperties massProperties;
				{			
					hkpInertiaTensorComputer::computeCylinderVolumeMassProperties( bottom, top, radius, segmentMass, massProperties );
				}
				info.m_motionType = hkpMotion::MOTION_DYNAMIC;
				info.m_mass = massProperties.m_mass;
				info.m_inertiaTensor = massProperties.m_inertiaTensor;
				info.m_centerOfMass = massProperties.m_centerOfMass;
			}
		}
		
		hkpRigidBody* segment = new hkpRigidBody( info );
		segmentShape->removeReference();
		
		trunk.pushBack( segment );
		world->addEntity( segment );

		if (i > 0)
		{
			hkpWindAction* action = new hkpWindAction( segment, wind, 0.1f );
			world->addAction(action);
			action->removeReference();
		}
	}

	
	for ( int i = 1; i < numberOfSegments; i++ )
	{
		// We model the connection between the segments with a ragdoll constraint.
		hkpRagdollConstraintData* rdc;
		{
			hkReal planeMin = HK_REAL_PI * -0.025f;
			hkReal planeMax = HK_REAL_PI *  0.025f;
			hkReal twistMin = HK_REAL_PI * -0.025f;
			hkReal twistMax = HK_REAL_PI *  0.025f;
			hkReal coneMin  = HK_REAL_PI * -0.05f;
			hkReal coneMax  = HK_REAL_PI *  0.05f;

			rdc = new hkpRagdollConstraintData();

			rdc->setPlaneMinAngularLimit( planeMin );
			rdc->setPlaneMaxAngularLimit( planeMax );
			rdc->setTwistMinAngularLimit( twistMin );
			rdc->setTwistMaxAngularLimit( twistMax );

			hkVector4 twistAxis( 0.0f, 1.0f, 0.0f );
			hkVector4 planeAxis( 0.0f, 0.0f, 1.0f );
			hkVector4 pivot( 0.0f, (segmentHeight + segmentGap) * i, 0.0f );

			rdc->setInBodySpace( pivot, pivot, planeAxis, planeAxis, twistAxis, twistAxis );
			rdc->setAsymmetricConeAngle( coneMin, coneMax );

			//world->createAndAddConstraintInstance( trunk[i - 1], trunk[i], rdc )->removeReference();

			hkpConstraintInstance* constraint = new hkpConstraintInstance( trunk[i - 1], trunk[i], rdc );
			world->addConstraint(constraint);

			hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor( 0 );
			motor->m_tau = trunkStiffness;
			motor->m_maxForce = 1000.0f;
			motor->m_constantRecoveryVelocity = 0.1f;

			rdc->setTwistMotor( motor ); 
			rdc->setConeMotor( motor ); 
			rdc->setPlaneMotor( motor ); 
			rdc->setMotorsActive(constraint, true);

			motor->removeReference();

			constraint->removeReference();
			rdc->removeReference();
		}
	}

	// The angle that the leaves make with the ground in their half lifted position.
	hkQuaternion tilt;
	{
		hkVector4 axis( 0.0f, 0.0f, 1.0f );
		tilt.setAxisAngle( axis, HK_REAL_PI * 0.1f );
	}
	hkQuaternion tiltRot;

	// The fronds
	for ( int i = 0; i < numberOfFronds; i++ )
	{
		hkQuaternion rotation;
		{
			hkVector4 axis( 0.0f, 1.0f, 0.0f );
			rotation.setAxisAngle( axis, HK_REAL_PI * 2.0f * ( i / (hkReal) numberOfFronds ) );
			rotation.normalize();
		}

		hkpShape* frondShape;
		hkpRigidBodyCinfo info;
		{
			hkVector4 vertexA( 0.0f, 0.0f, 0.0f );
			hkVector4 vertexB( frondLength, 0.0f, frondWidth / 2.0f );
			hkVector4 vertexC( frondLength, 0.0f, - frondWidth / 2.0f );
				
			frondShape = new hkpTriangleShape( vertexA, vertexB, vertexC, 0.01f );
			info.m_shape = frondShape;
			
			hkVector4 relPos;
			relPos.setRotatedDir( rotation, hkVector4( trunkTopRadius + 0.3f, trunkHeight, 0.0f ) );

			info.m_position.setAdd4( pos, relPos );
			
			hkpMassProperties massProperties;
			{
				hkReal mass = frondMass;
				hkpInertiaTensorComputer::computeTriangleSurfaceMassProperties( vertexA, vertexB, vertexC, mass, 0.01f, massProperties );
			}

			info.m_motionType = hkpMotion::MOTION_DYNAMIC;
			info.m_mass = massProperties.m_mass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			info.m_centerOfMass = massProperties.m_centerOfMass;

			tiltRot.setMul( rotation, tilt );
			info.m_rotation = tiltRot;
		}
		
		hkpRigidBody* frond = new hkpRigidBody( info );
		frondShape->removeReference();
		
		world->addEntity( frond );

		hkpWindAction* action = new hkpWindAction( frond, wind, 0.1f );
		world->addAction(action);
		action->removeReference();

		
		// We model the connection between the fronds and the trunk with a ragdoll constraint.
		hkpRagdollConstraintData* rdc;
		{
			hkReal planeMin = HK_REAL_PI * -0.005f;
			hkReal planeMax = HK_REAL_PI *  0.005f;
			hkReal twistMin = HK_REAL_PI * -0.05f;
			hkReal twistMax = HK_REAL_PI *  0.05f;
			hkReal coneMin  = HK_REAL_PI * -0.2f;
			hkReal coneMax  = HK_REAL_PI *  0.2f;

			rdc = new hkpRagdollConstraintData();

			rdc->setPlaneMinAngularLimit( planeMin );
			rdc->setPlaneMaxAngularLimit( planeMax );
			rdc->setTwistMinAngularLimit( twistMin );
			rdc->setTwistMaxAngularLimit( twistMax );

			hkVector4 twistAxisFrond( 1.0f, 0.0f, 0.0f );
			hkVector4 twistAxisTrunk;
			twistAxisTrunk.setRotatedDir( tiltRot, twistAxisFrond );
			
			hkVector4 planeAxisFrond( 0.0f, 0.0f, 1.0f );
			hkVector4 planeAxisTrunk;
			planeAxisTrunk.setRotatedDir( tiltRot, planeAxisFrond );
			
			hkVector4 pivotFrond( 0.0f, 0.0f, 0.0f );
			hkVector4 pivotTrunk;
			pivotTrunk.setRotatedDir( rotation, hkVector4( trunkTopRadius + 0.3f, trunkHeight, 0.0f ) );

			rdc->setInBodySpace( pivotTrunk, pivotFrond, planeAxisTrunk, planeAxisFrond, twistAxisTrunk, twistAxisFrond );
			rdc->setAsymmetricConeAngle( coneMin, coneMax );

			world->createAndAddConstraintInstance( trunk[ numberOfSegments - 1 ], frond, rdc )->removeReference();
			rdc->removeReference();
		}
	}
}

// A demo which shows drag being applied to several objects.
PrevailingWindDemo::PrevailingWindDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	// X and Z are the half-extents of the floor, and Y is the height of the walls.
	hkReal x = 20.0f;
	hkReal y = 4.0f;
	hkReal z = 20.0f;

	//
	// Setup the camera.
	//
	{
		hkVector4 from( 0.0f, y * (4.0f), z * 2.0f );
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register all collision agents.
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	// Create the wind
	{
		m_wind = new hkpPrevailingWind( hkVector4( 1.0f, 0.5f, 0.5f ) );
		m_wind->addOscillation( hkVector4( 0.6f, 0.0f, 0.2f ), 5.0f, 1.0f, 0.75f);
		m_wind->addOscillation( hkVector4( 1.5f, 1.0f, 0.0f ), 13.0f);
		m_wind->addOscillation( hkVector4( 2.5f, 0.0f, 1.0f ), 31.0f, 2.0f );
		// Let the wind update the oscillators.
		m_world->addWorldPostSimulationListener( m_wind );
	}

	hkVector4 areaSize( x, y, z );
	
	// Create the floor
	{
		hkpRigidBody* lowerFloor;
		hkVector4 fixedBoxSize( 15.0f , 0.5f , 15.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );

		hkpRigidBodyCinfo info;
		{
			info.m_shape = fixedBoxShape;
			info.m_motionType = hkpMotion::MOTION_FIXED;
			info.m_position.set(0.0f, -0.5f, 0.0f);
		}

		lowerFloor = new hkpRigidBody(info);
		m_world->addEntity(lowerFloor);
		
		lowerFloor->removeReference();
		fixedBoxShape->removeReference();
	}

	createWindmill ( m_world, m_wind, hkVector4( -4.0f, 0.0f, 2.0f ) );
	createPalmTree ( m_world, m_wind, hkVector4( 4.0f, 0.0f, -3.0f ) );
	createPalmTree ( m_world, m_wind, hkVector4( 8.0f, 0.0f, 5.0f ) );
	
	m_world->unlock();
}

PrevailingWindDemo::~PrevailingWindDemo()
{
	m_wind->removeReference();
}

hkDemo::Result PrevailingWindDemo::stepDemo()
{
	hkVector4 worldPoint( 0.0f, 8.0f, 0.0f);
	hkVector4 windAtWorldPoint;
	{
		m_wind->getWindVector( worldPoint, windAtWorldPoint );
	}
	HK_DISPLAY_ARROW( worldPoint, windAtWorldPoint, hkColor::BLUE);

	return hkDefaultPhysicsDemo::stepDemo();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Demonstrates a prevailing wind on constrained objects.";

HK_DECLARE_DEMO(PrevailingWindDemo, HK_DEMO_TYPE_PRIME, "Demonstrates a prevailing wind on constrained objects", helpString);

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

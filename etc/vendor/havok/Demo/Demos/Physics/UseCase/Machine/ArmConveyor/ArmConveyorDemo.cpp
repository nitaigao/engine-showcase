/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/Machine/ArmConveyor/ArmConveyorDemo.h>


#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Utilities/Actions/Motor/hkpMotorAction.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

ArmConveyorDemo::ArmConveyorDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkReal zoom = 1.3f;
		hkVector4 from(20*zoom, 20*zoom, 25*zoom);
		hkVector4 to(0, 0, 0);
		hkVector4 up(0, 1, 0);
		setupDefaultCameras( m_env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_SOFT);
		info.m_gravity.set(0,-10,0);

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}


		// Group collision filter so we can disable collisions on a per group
	{
		hkpGroupFilter* collisionFilter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( collisionFilter );

		m_world->setCollisionFilter(collisionFilter);

		collisionFilter->removeReference();
	}

		// We shift the shaft away from the wheels so it doesn't interfere with them
	hkReal shaftShift = 5.5f;
		// This is the (half) length of the shaft
	hkReal shaftHalfLength = 7.0f;

		// The wheels must be the length of the shaft apart
	hkVector4 wheelPos0(-shaftHalfLength,0,0);
	hkVector4 wheelPos1(shaftHalfLength,0,0);

		////////////////////////////
		// WHEELS
		////////////////////////////

	hkpRigidBody *wheel0 = createDisc(6, 100, wheelPos0);
	m_world->addEntity( wheel0 );
	wheel0->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_KEYFRAME );
	wheel0->removeReference();
	constrainCog(wheel0, m_world);// This constrains the wheel around a central Z-axis axle
	addMotorAction(wheel0, m_world);	

	
	hkpRigidBody *wheel1 = createDisc(6, 100, wheelPos1);
	m_world->addEntity( wheel1 );
	wheel1->removeReference();
	wheel1->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_KEYFRAME );
	constrainCog(wheel1, m_world);// This constrains the wheel around a central Z-axis axle
	addMotorAction(wheel1, m_world);

		////////////////////////////
		// SHAFT
		////////////////////////////
	hkVector4 size(8,8,2);
	size.set(shaftHalfLength * 2.0f,2,4);
	hkVector4 shaftpos(0,-5,shaftShift);
	hkpRigidBody *shaft = getShaft(shaftHalfLength, shaftpos);
	shaft->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_KEYFRAME );
	m_world->addEntity( shaft );
	shaft->removeReference();


		// Add constraints to join shaft to wheel
	if(1)
	{
		hkVector4 axis(0,0,1);
		hkVector4 pos = shaft->getPosition();
		pos(0) -= shaftHalfLength;
		{
			hkpHingeConstraintData* bas = new hkpHingeConstraintData(); 
			bas->setInWorldSpace(shaft->getTransform(), wheel0->getTransform(), pos, axis );
			m_world->createAndAddConstraintInstance( shaft, wheel0, bas )->removeReference(); 
			bas->removeReference();
		}
	}

		// Add constraints to join shaft to wheel
	if(1)
	{
		hkVector4 axis(0,0,1);
		hkVector4 pos = shaft->getPosition();
		pos(0) += shaftHalfLength;
		{
			hkpHingeConstraintData* bas = new hkpHingeConstraintData(); 
			bas->setInWorldSpace(shaft->getTransform(), wheel1->getTransform(), pos, axis );
			m_world->createAndAddConstraintInstance( shaft, wheel1, bas )->removeReference(); 
			bas->removeReference();
		}

	}
	
	




		////////////////////////////
		// CONVEYOR BASE (3 SEGMENTS)
		////////////////////////////
	hkReal baseHeight = 5.4f;
	hkReal baseWidth = 1.3f;

		// Note: Base friction is high (to get blocks to stop, not slide), but block friction is low
		// to avoid "stickiness" when the base block of a tall pile is pushed out.
	hkReal baseFriction = 3.0f;

	{
		hkVector4 basePos(0, baseHeight, shaftShift);
		size.set(2*shaftHalfLength +5 +6, 0.5f, baseWidth);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		base0->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_STATIC );
		m_world->addEntity( base0 );
		base0->removeReference();
		base0->getMaterial().setFriction( baseFriction );
	}

	{
		hkVector4 basePos(0, baseHeight, shaftShift-2);
		size.set(2*shaftHalfLength +5+6, 0.5f, baseWidth);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		base0->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_STATIC );
		m_world->addEntity( base0 );
		base0->removeReference();
		base0->getMaterial().setFriction( baseFriction );
	}


	{
		hkVector4 basePos(0, baseHeight, shaftShift+2);
		size.set(2*shaftHalfLength +5+6, 0.5f, baseWidth);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		base0->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_STATIC );
		m_world->addEntity( base0 );
		base0->removeReference();
		base0->getMaterial().setFriction( baseFriction );
	}


		////////////////////////////
		// BLOCKS
		////////////////////////////
	hkArray<hkpRigidBody*> blocks;
	{
		for(int i = 0; i < 8; i++)
		{
			hkVector4 blockPos(-7.0f , 7 +i*2.1f, shaftShift);
			size.set(1.5f, 2, 4);
			hkpRigidBody *block = GameUtils::createBox(size, 10, blockPos);
			m_world->addEntity( block );
			block->setCollisionFilterInfo( hkpGroupFilterSetup::LAYER_DYNAMIC );

			block->removeReference();

			block->getMaterial().setFriction( 0.2f );

			blocks.pushBack(block);

		}
	}

	
		////////////////////////////
		// HOLDER 
		////////////////////////////

		// Left
	{
		hkVector4 basePos(-8.05f, baseHeight + 7.8f + 10, shaftShift);
		size.set(0.5f, 30, 4.0f);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
	}

		
		// Right
	{
		hkVector4 basePos(-5.95f, baseHeight + 7.8f + 10, shaftShift);
		size.set(0.5f, 30, 4.0f);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
	}

		// Back
	{
		hkVector4 basePos(-7.1f, baseHeight + 7.8f + 10, shaftShift -2.3f);
		size.set(0.3f, 30, 0.3f);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
	}

		
		// Front
	{
		hkVector4 basePos(-7.1f, baseHeight + 7.8f + 10, shaftShift +2.3f);
		size.set(0.3f, 30, 0.3f);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
	}






		////////////////////////////
		// BASES 
		////////////////////////////

		// Small one at end of conveyor
	{
		hkVector4 basePos(20, -4.5f, shaftShift+2);
		size.set(15,1,10);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
		base0->getMaterial().setFriction( 4.1f );
	}

		// Big one (floor)
	
	{
		hkVector4 basePos(0, -12.f, shaftShift+2);
		size.set(50,1,30);
		hkpRigidBody *base0 = GameUtils::createBox(size, 0, basePos);
		m_world->addEntity( base0 );
		base0->removeReference();
		base0->getMaterial().setFriction( 4.1f );
	}

		// Set gravity to (arbitrary) value so blocks seem to fall OK. Should be bigger, but
		// no frame-locking => sim would be way to fast!
	hkVector4 gravity(0, -30.0f, 0);
	m_world->setGravity(gravity);

	m_world->unlock();
}


// This helper function creates a compound body which will be attached to the wheels
// and which conveys the blocks using its "pins"
hkpRigidBody*  ArmConveyorDemo::getShaft(hkReal shaftHalfLength, hkVector4& position)
{
	
	hkVector4 stripSize(shaftHalfLength * 2.0f + 1.f, 1, .5f);
	hkVector4 pinSize(.5f,1.9f,.5f);
	hkVector4 joinerSize(.4f, .4f, 3.f);
	hkReal mass = 10.0f;

	
	//
	hkVector4 halfExtents(stripSize(0) * 0.5f, stripSize(1) * 0.5f, stripSize(2) * 0.5f);
	hkpBoxShape* strip = new hkpBoxShape(halfExtents, 0 );	// Note: We use HALF-extents for boxes
	
	halfExtents.set(pinSize(0) * 0.5f, pinSize(1) * 0.5f, pinSize(2) * 0.5f);
	hkpBoxShape* pin = new hkpBoxShape(halfExtents, 0 );	// Note: We use HALF-extents for boxes

	halfExtents.set(joinerSize(0) * 0.5f, joinerSize(1) * 0.5f, joinerSize(2) * 0.5f);
	hkpBoxShape* joiner = new hkpBoxShape(halfExtents, 0 );	// Note: We use HALF-extents for boxes

	//
	// Create a rigid body construction template and start filling it out
	//
	hkpRigidBodyCinfo compoundInfo;


	// We'll basically have to create a 'List' Shape  (ie. a hkpListShape) in order to have many 
	// shapes in the same body. Each element of the list will be a (transformed) hkpShape, ie.
	// a hkpTransformShape (which basically is a (geometry,transformation) pair).
	// The hkpListShape constructor needs a pointer to an array of hkShapes, so we create an array here, and push
	// back the hkTransformShapes as we create them.
	hkArray<hkpShape*> shapeArray;

	/////////////////// STRIPS /////////////////////////

	hkReal zpos = 1;
		// Create body

	int num = 4;

	
	{
		hkReal xpos = 0;
		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, 0, -zpos);
			t.setTranslation( trans );

			hkpTransformShape* stripTrans = new hkpTransformShape( strip, t );
			shapeArray.pushBack(stripTrans);	
		}

			// Create body
		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, 0, zpos);
			t.setTranslation( trans );
			
			hkpTransformShape* stripTrans = new hkpTransformShape( strip, t );			
			shapeArray.pushBack(stripTrans);	
		}
	}


	strip->removeReference();



		/////////////////// PINS /////////////////////////
		// Create body
	hkReal offset = 0.0f;
	hkReal pinheight = 1.4f;
	for(int i=0; i<num; i++)
	{
		hkReal xpos =  2.92f*i-5 + offset;
		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, pinheight, -zpos);
			t.setTranslation( trans );

			hkpTransformShape* pinTrans = new hkpTransformShape( pin, t );
			shapeArray.pushBack(pinTrans);	
		}

		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, pinheight, zpos);
			t.setTranslation( trans );
			
			hkpTransformShape* pinTrans = new hkpTransformShape( pin, t );
			shapeArray.pushBack(pinTrans);	
		}
		offset += 0.75f;
		
	}


	pin->removeReference();

	
	/////////////////// JOINERS //////////////////////

	{
		hkReal xpos = -shaftHalfLength;
		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, 0, -2.8f);
			t.setTranslation( trans );

			hkpTransformShape* joinerTrans = new hkpTransformShape( joiner, t );
			shapeArray.pushBack(joinerTrans);	
		}

			// Create body
		xpos = shaftHalfLength;
		{
			hkTransform t;
			t.setIdentity();
			hkVector4 trans = hkVector4(xpos, 0, -2.8f);
			t.setTranslation( trans );
			
			hkpTransformShape* joinerTrans = new hkpTransformShape( joiner, t );
			shapeArray.pushBack(joinerTrans);	
		}
	}

	joiner->removeReference();






	
	// Now we can create the compund body as a hkpListShape
	
	hkpListShape* listShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());
	compoundInfo.m_shape = listShape;

	{
		// remove references to child shapes
		for ( int i = 0; i < shapeArray.getSize(); ++i )
		{
			shapeArray[i]->removeReference();
		}
	}

	//
	// Create the rigid body 
	//

	hkMatrix3 m;
		// Fake an inertia ensor using a box
	hkVector4 halfCube( 1*0.5f, 5 * 0.5f, 5 *0.5f);
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(listShape, mass, compoundInfo);
	compoundInfo.m_position = position;
	compoundInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			// Finally create the body
	hkpRigidBody* compoundRigidBody = new hkpRigidBody(compoundInfo);

	listShape->removeReference();

	compoundRigidBody->setAngularDamping(0.0f);
	compoundRigidBody->setLinearDamping(0.0f);

	compoundRigidBody->getMaterial().setFriction( 0.4f );

	return compoundRigidBody;
}


// This helper function creates a compound body which is "wheel-like", ie is an extruded regular n-gon
hkpRigidBody*  ArmConveyorDemo::createDisc(hkReal radius, hkReal mass, hkVector4& position)
{
	int numSides = 20;
	

	hkArray<hkVector4> vertices;

	//
	// Create the vertices array
	//
	for(int i = 0 ; i < numSides; i++)
	{
		hkTransform t;
		t.setIdentity();
		hkVector4 trans = hkVector4(radius, 0, 0);

		hkReal angle = HK_REAL_PI * 2 * i / (hkReal) numSides;
		hkVector4 axis(0, 0, 1);
		hkQuaternion q(axis, angle);
		trans.setRotatedDir(q, trans);

		hkVector4 v = trans;
		v(2) = -1.0f;
		vertices.pushBack(v);
		v(2) = 1.0f;
		vertices.pushBack(v);
		
	}

	hkpConvexVerticesShape* shape;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(vertices[0](0));
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}

	hkpRigidBodyCinfo compoundInfo;
	compoundInfo.m_shape = shape;

	//
	// Create the rigid body 
	//
		// Fake an inertia tensor using a box. There's no very good reason for this, but this is the
		// original code for this demos.
	hkVector4 halfCube( 0.5f, radius * 0.5f, radius * 0.5f);
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(shape, mass, compoundInfo);
	compoundInfo.m_position = position;
	compoundInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;


			// Finally create the body
	hkpRigidBody* compoundRigidBody = new hkpRigidBody(compoundInfo);


	shape->removeReference();

	return compoundRigidBody;
}

// Add motor (and remove damping, which fights against motor)
hkpMotorAction* ArmConveyorDemo::addMotorAction(hkpRigidBody *body, hkpWorld* world)
{
	body->setAngularDamping(0.0f);
	body->setLinearDamping(0.0f);

	hkVector4 axis(0,0,1);
	hkReal angVel(-1.0f);
	hkReal gain(50.0f);

	hkpMotorAction* motorAction = new hkpMotorAction(body, axis, angVel, gain);
	world->addAction(motorAction);
	motorAction->removeReference();

	return motorAction;
}

// Constraint with axle through z-axis (using "fixed" rigid body to constrain to.
void ArmConveyorDemo::constrainCog(hkpRigidBody *body, hkpWorld* world)
{
	hkpRigidBody* fixedRigidBody = world->getFixedRigidBody();
	hkVector4 axis(0,0,1);
	hkVector4 pos = body->getPosition();
	{
		hkpHingeConstraintData* bas = new hkpHingeConstraintData(); 
	    bas->setInWorldSpace(body->getTransform(), fixedRigidBody->getTransform(), pos, axis );
		m_world->createAndAddConstraintInstance( body, fixedRigidBody, bas )->removeReference(); 
		bas->removeReference();
	}
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A motorized arm conveyor";

HK_DECLARE_DEMO(ArmConveyorDemo, HK_DEMO_TYPE_PRIME, "Arm Conveyor", helpString);

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

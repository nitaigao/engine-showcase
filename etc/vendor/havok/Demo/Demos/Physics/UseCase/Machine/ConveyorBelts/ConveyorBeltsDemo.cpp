/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Physics/Utilities/Collide/ContactModifiers/SurfaceVelocity/hkpSurfaceVelocityUtil.h>

#include <Demos/Physics/UseCase/Machine/ConveyorBelts/ConveyorBeltsDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Physics/Utilities/Collide/hkpShapeGenerator.h>

DEMO_OPTIONS_DEFINE(ConveyorBeltsDemo,Options);

void ConveyorBeltsDemo::createConveyorBelt( const ConveyorBeltBlueprint& bp )
{

	hkVector4 position; 	position.setInterpolate4( bp.m_startPos, bp.m_endPos, 0.5f );

	hkQuaternion rotation;

		// The transform of the conveyor base
	hkTransform transform;

	//
	// calculate the rotation
	//
	hkReal length; 
	{
		hkVector4 x; x.setSub4( bp.m_endPos, bp.m_startPos );
		length = x.normalizeWithLength3();

		hkVector4 up( 0.0f, 1.0f, 0.0f);
		hkVector4 z; z.setCross( x, up ); z.normalize3();
		hkVector4 y; y.setCross( z, x );  y.normalize3();

		transform.getRotation().setCols(x,y,z);
		transform.setTranslation( position );

		rotation.set( transform.getRotation() );
	}



	//
	// create the base box
	//
	{
		hkVector4 baseSize( length * 0.5f, bp.m_thickness, bp.m_width * 0.5f);
		hkpShape* shape = new hkpBoxShape( baseSize , 0 );

		hkpRigidBodyCinfo rbci;
		rbci.m_motionType = hkpMotion::MOTION_FIXED;
		rbci.m_shape = shape;
		rbci.m_position = position;
		rbci.m_rotation = rotation;
		rbci.m_friction = bp.m_friction;

		hkpRigidBody* body = new hkpRigidBody( rbci );


		//
		// calculate the surface velocity for the conveyor belt, and add a surface velocity util
		//
		hkVector4 conveyorVelocity = transform.getRotation().getColumn( 0 );
		conveyorVelocity.mul4( bp.m_speed );

		hkpSurfaceVelocityUtil* su = new hkpSurfaceVelocityUtil( body, conveyorVelocity );


		m_world->addEntity( body );

		su->removeReference();
		body->removeReference();
		shape->removeReference();
	}

	//
	// create the bounderies
	//
	{
		hkVector4 boundarySize( length * 0.5f, bp.m_height * 0.5f, bp.m_thickness * 0.5f );
		hkpShape* shape = new hkpBoxShape( boundarySize , 0 );

		hkpRigidBodyCinfo rbci;
		rbci.m_motionType = hkpMotion::MOTION_FIXED;
		rbci.m_shape = shape;
		rbci.m_friction = 0.0f;

		hkVector4 x(1.0f,0.0f,0.0f);

		{ // first
			hkQuaternion localRot; localRot.setAxisAngle( x, bp.m_sideAngle );
			hkVector4 localPos( 0.0f, bp.m_height * 0.5f, bp.m_width * 0.5f );

			rbci.m_position.setTransformedPos( transform, localPos );
			rbci.m_rotation.setMul( rotation, localRot );

			hkpRigidBody* body = new hkpRigidBody( rbci );
			m_world->addEntity( body );
			body->removeReference();
		}
		{ // second
			hkQuaternion localRot; localRot.setAxisAngle( x, -bp.m_sideAngle );
			hkVector4 localPos( 0.0f, bp.m_height * 0.5f, -bp.m_width * 0.5f );

			rbci.m_position.setTransformedPos( transform, localPos );
			rbci.m_rotation.setMul( rotation, localRot );

			hkpRigidBody* body = new hkpRigidBody( rbci );
			m_world->addEntity( body );
			body->removeReference();
		}

		shape->removeReference();
	}
}

ConveyorBeltsDemo::ConveyorBeltsDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
//	environment->m_drawWireframe = false;


	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 6.0f, 20.0f);
		hkVector4 to  ( 0.0f, 0.0f,  0.0f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.81f, 0.0f);
		info.setBroadPhaseWorldSize( 200.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_HARD);
		info.m_collisionTolerance = 0.05f;

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}



	//
	// Create Conveyor Belts
	//
	{
		const hkReal speedFactor = 2.0f;
		const hkReal xa = 3.0f;

		ConveyorBeltBlueprint bp;
		bp.m_friction = 10.0f;

		bp.m_height = 1.0f;
		bp.m_thickness = 0.3f;
		bp.m_width = 1.8f;
		bp.m_sideAngle = 0.6f;


		bp.m_speed = 0.3f * speedFactor;
		bp.m_width = 1.8f;
		bp.m_thickness = 0.4f;
		bp.m_startPos.set(-4.0f, -3.5f, 0.0f );		bp.m_endPos.set  (  3.8f + xa, -3.5f, 0 );		createConveyorBelt( bp );


		bp.m_speed = 1.7f * speedFactor;
		bp.m_width = 2.2f;
		bp.m_height = 1.4f;
		bp.m_thickness = 0.3f;
		bp.m_sideAngle = 0.6f;
		bp.m_startPos.set( 5+xa, -4.5f, -2.9f );		bp.m_endPos.set  (  5.0f+xa, -5.0f, -1.0f );		createConveyorBelt( bp );
		bp.m_startPos.set( 5+xa, -5.0f, -1.0f );		bp.m_endPos.set  (  5.0f+xa, 0.0f, 20.0f );		createConveyorBelt( bp );

		bp.m_speed = 2.7f * speedFactor;
		bp.m_width = 2.1f;
		bp.m_startPos.set(  5+xa, -.2f, 20 );		bp.m_endPos.set  (  5+xa, 0.0f, 29.0f );		createConveyorBelt( bp );

		
		bp.m_speed = 0.5f * speedFactor;
		bp.m_width = 4.1f;
		bp.m_height = 1.0f;
		bp.m_startPos.set(  8+xa, -.8f, 31 );		bp.m_endPos.set  (  1.0f, -.8f, 31.0f );		createConveyorBelt( bp );

		bp.m_speed = 1.5f * speedFactor;
		bp.m_width = 2.2f;
		bp.m_height = 1.0f;
		bp.m_startPos.set(  0, -1.7f, 34 );		bp.m_endPos.set  (  0.0f, -1.7f, 28.0f );		createConveyorBelt( bp );

		bp.m_startPos.set(  0, -1.9f, 28 );		bp.m_endPos.set  (  0.0f, 1.0f, 15.0f );		createConveyorBelt( bp );

		bp.m_speed = 2.7f * speedFactor;
		bp.m_width = 2.2f;
		bp.m_height = 1.0f;
		bp.m_startPos.set(  0, .8f, 15 );		bp.m_endPos.set  (  0.0f, 3.0f, 5.0f );		createConveyorBelt( bp );

	}


	// 
	// Create the funnel
	//
	{
		{
			hkVector4 boxSize( 3.0f, .6f , 3.3f );
			hkpShape *boxShape = new hkpBoxShape( boxSize , 0 );

			hkpRigidBodyCinfo boxInfo;
			boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
			boxInfo.m_shape = boxShape;

			const hkReal dist = 3.1f;
			{
				boxInfo.m_rotation = hkQuaternion( hkVector4( 1.0f,0.0f,0.0f ), 0.95f ); 
				boxInfo.m_position = hkVector4( 0.0f,0.0f,-dist );
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				m_world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}
			{
				boxInfo.m_rotation = hkQuaternion( hkVector4( 1.0f,0.0f,0.0f ), -0.95f ); 
				boxInfo.m_position = hkVector4( 0.0f,0.0f,dist );
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				m_world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}
			boxShape->removeReference();
		}
		{
			hkVector4 box2Size( 2.0f, .3f , 4.0f );
			hkpShape *box2Shape = new hkpBoxShape( box2Size , 0 );

			hkpRigidBodyCinfo boxInfo;
			boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
			boxInfo.m_shape = box2Shape;

			const hkReal dist2 = 2.7f;
			{
				boxInfo.m_rotation = hkQuaternion( hkVector4( 0.0f,0.0f,1.0f ), -1.51f ); 
				boxInfo.m_position = hkVector4( -dist2,-1.0f,0.0f );
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				m_world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}
			{
				boxInfo.m_rotation = hkQuaternion( hkVector4( 0.0f,0.0f,1.0f ), +1.51f ); 
				boxInfo.m_position = hkVector4( dist2,0.0f,0.0f );
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				m_world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}
			box2Shape->removeReference();
		}
	}


	// 
	// Create ground box
	//

	{
		hkVector4 baseRadii( 24.0f, 0.5f, 24.0f);
		hkpBoxShape* baseShape = new hkpBoxShape(baseRadii, 0 );
		
		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_shape = baseShape;
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_position = hkVector4( 0.0f, -6.0f, 12.0f );
		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);

		m_world->addEntity( baseRigidBody );
		baseRigidBody->removeReference();
		baseShape->removeReference();
	}


	//
	// Create a few different shapes
	//
	hkPseudoRandomGenerator random(100);

	const int numShapes = 10;
	hkpShape* shapes[numShapes];
	{

		for (int i = 0; i < numShapes; i++)
		{
			hkVector4 boxSize( random.getRandRange( .1f, .4f), random.getRandRange( .1f, .4f), random.getRandRange( .1f, .6f) );
			hkVector4 negSize; negSize.setMul4( -m_options.m_shapeScale, boxSize );
			boxSize.mul4( m_options.m_shapeScale );

			switch( m_options.m_shapeType )
			{
				default:
				case Options::BOX:
					shapes[i] = new hkpBoxShape( boxSize , 0 );
					break;
				case Options::CONVEX_VERTICES:
					shapes[i] = hkpShapeGenerator::createRandomConvexVerticesShape(	negSize, boxSize, 20, &random);
					break;
			}
		}
	}

	// 
	// Create falling rigid bodies
	//
	{
		hkVector4 initialPosition(  0.0f, 0.0f, 0.0f );
		for (int i = 0; i < m_options.m_numBodies; i++ )
		{
			hkpRigidBody* boxRigidBody;

			hkpRigidBodyCinfo boxInfo;
			boxInfo.m_shape = shapes[ int(random.getRandRange( 0.0f, numShapes-1))];
			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			boxInfo.m_friction = 0.1f;

			initialPosition(1) += .02f;

			hkVector4 r( random.getRandRange( -2.0f,2.0f ), random.getRandRange( -2.0f,2.0f ),random.getRandRange( -2.0f,2.0f ));
			boxInfo.m_position.setAdd4( initialPosition, r );

			hkpInertiaTensorComputer::setShapeVolumeMassProperties( boxInfo.m_shape, 1.0f, boxInfo );

			boxInfo.m_inertiaTensor.setDiagonal( 0.05f, 0.05f, 0.05f );

			boxRigidBody = new hkpRigidBody(boxInfo);

			m_world->addEntity( boxRigidBody );

			boxRigidBody->removeReference();
		}
	}

	//
	// remove shape reference
	//
	{
		for (int i = 0; i < numShapes; i++)
		{
			shapes[i]->removeReference();
		}
	}

	m_world->unlock();
}



ConveyorBeltsDemo::~ConveyorBeltsDemo()
{
//	environment->m_drawWireframe = true;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how the hkpSurfaceVelocityUtil can be used to " \
"change the apparent velocity of an underlying surface";

HK_DECLARE_DEMO(ConveyorBeltsDemo, HK_DEMO_TYPE_PRIME, "Belt", helpString);

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

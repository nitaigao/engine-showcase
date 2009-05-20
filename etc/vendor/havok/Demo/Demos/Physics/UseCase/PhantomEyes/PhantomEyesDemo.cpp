/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/PhantomEyes/PhantomEyesDemo.h>

#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>

#include <Physics/Dynamics/Action/hkpUnaryAction.h>




/// Create a table with 4 boundary boxes.
/// A small moving box gets pushed around on the table, see 
/// MyPhantomAction for details
PhantomEyesDemo::PhantomEyesDemo(hkDemoEnvironment* env):hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 20.0f, 30.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up );
	}

	//
	// Create world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(100.0f);
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	// Register ALL agents (though some may not be necessary)
	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

	// 
	// Create the table
	//
	{
		const float xdim = 15.0f;
		const float zdim = 15.0f;

		createStaticBox( m_world, 0.0f,  -1.0f, 0.0f, xdim, 1.0f, zdim );
		createStaticBox( m_world, -xdim, 1.0f, 0.0f, 1.0f, 1.0f, zdim );
		createStaticBox( m_world, xdim,  1.0f, 0.0f, 1.0f, 1.0f, zdim );
		createStaticBox( m_world, 0.0f,  1.0f, zdim, xdim,1.0f, 1.0f );
		createStaticBox( m_world, 0.0f,  1.0f,-zdim, xdim,1.0f, 1.0f );
	}


	/* 
	// Create our 'special' shape (A box with two triangle phantoms attached:
	//	
	//         /|   |\        //
	//        / |   | \       //
	//       /__|   |__\      //
	//
	//			|---|
	//          | x |
	//	        |___| 
	//
	// 
	// The shape tree will look like
	//
	//         -hkpListShape
	//                - hkpBoxShape
	//
	//                - hkpBvShape
	//                     - hkpTriangleShape as BoundingVolumeShape 
	//                     - hkpPhantomCallbackShape  as ChildShape
	//
	//                - hkpBvShape
	//                     - hkpTriangleShape as BoundingVolumeShape 
	//                     - hkpPhantomCallbackShape  as ChildShape
	//
	*/

	hkpListShape*	shape;
	MyPhantomCallbackShape* leftEyeShape;
	MyPhantomCallbackShape* rightEyeShape;

	{
		hkpShape* shapes[3];
		
		// create the box
		{
			hkVector4 radii(1.0f, 0.5f, 1.0f);
			shapes[0] = new hkpBoxShape( radii , 0 );
		}

		// define the eye size
		const float s = 4.0f;

		// create the left eye
		{
			
			hkVector4 v0( -1.0f,   0.0f, 1.0f );
			hkVector4 v1( -1.0f-s, 0.0f, 1.0f );
			hkVector4 v2( -1.0f,   0.0f, 1.0f+s );

			hkpTriangleShape* triangle = new hkpTriangleShape( v0, v1, v2 );
			leftEyeShape = new MyPhantomCallbackShape();
			shapes[1] = new hkpBvShape( triangle, leftEyeShape );
			

			leftEyeShape->removeReference();
			triangle->removeReference();
		}

		// create the right eye
		{
			hkVector4 v0(  1.0f,   0.0f, 1.0f );
			hkVector4 v1(  1.0f,   0.0f, 1.0f+s );
			hkVector4 v2(  1.0f+s, 0.0f, 1.0f );

			hkpTriangleShape* triangle = new hkpTriangleShape( v0, v1, v2 );
			rightEyeShape = new MyPhantomCallbackShape();

			shapes[2] = new hkpBvShape( triangle, rightEyeShape );

			rightEyeShape->removeReference();
			triangle->removeReference();
		}

		shape = new hkpListShape( shapes, 3 );

		shapes[0]->removeReference();
		shapes[1]->removeReference();
		shapes[2]->removeReference();
	}
	
	
	
	// 
	// Create a compound body from this list
	//
	hkpRigidBody* boxRigidBody;

	{

		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = shape;

		boxInfo.m_position.set( 0.0f, 1.0f, 0.0f );
		boxInfo.m_inertiaTensor.setDiagonal( 10.0f, 10.0f, 10.0f );
		boxInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		boxInfo.m_mass = 1.0f;

		boxRigidBody = new hkpRigidBody(boxInfo);

		hkVector4 angVel( 0.0f, 1.0f, 0.0f );
		boxRigidBody->setAngularVelocity( angVel );

		m_world->addEntity( boxRigidBody );
		boxRigidBody->removeReference();

		shape->removeReference();
	}

	//
	//	Create our special Phantom Action
	//
	{
		MyPhantomAction* cle = new MyPhantomAction(boxRigidBody, leftEyeShape, rightEyeShape );
		m_world->addAction( cle );
		cle->removeReference();
	}


	m_world->unlock();
}

void PhantomEyesDemo::createStaticBox( hkpWorld* world, float centerX, float centerY, float centerZ, float radiusX, float radiusY, float radiusZ )
{
	hkVector4 radii ( radiusX, radiusY, radiusZ );

	hkpShape* boxShape = new hkpBoxShape( radii , 0 );

	hkpRigidBodyCinfo boxInfo;

	boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
	boxInfo.m_shape = boxShape;
	boxInfo.m_position.set( centerX, centerY, centerZ );

	hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
	world->addEntity( boxRigidBody );
	boxRigidBody->removeReference();
	boxShape->removeReference();
}



MyPhantomAction::MyPhantomAction( hkpRigidBody* body, MyPhantomCallbackShape* leftEye, MyPhantomCallbackShape* rightEye ) : hkpUnaryAction(body)
{
	m_leftEye = leftEye;
	m_rightEye = rightEye;
	m_leftActive  = 0;
	m_rightActive = 0;
}




/// Just push the box forward, except when some eye sees (penetrates) something
/// If only onew does, turn in the opposite direction.
/// If both do, reverse for a while.
/// => Very basic AI.
void MyPhantomAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* body = static_cast<hkpRigidBody*>( m_entity );

	const int phantomHitDuration = 10;

	if ( m_leftEye->m_penetratingShapesCount )
	{
		m_leftActive = phantomHitDuration;
	}

	if ( m_rightEye->m_penetratingShapesCount )
	{
		m_rightActive = phantomHitDuration;
	}

	hkVector4 forwardLocal( 0.0f, 0.0f, 7.0f );
	hkVector4 rotateLocal( 0.0f, 0.0f, 0.0f );

	if ( m_leftActive )
	{
		m_leftActive--;
		if ( m_rightActive )
		{
			m_rightActive--;
			// if both eyes penetrate, go backwards 
			forwardLocal.setNeg4( forwardLocal );
		}
		else
		{
			// left eye, that means rotate to the right
			forwardLocal.setZero4();
			rotateLocal.set(0.0f, 7.5f, 0.0f);
		}
	}
	else
	{
		if ( m_rightActive )
		{
			m_rightActive--;
			// right eye, that means rotate to the left
			forwardLocal.setZero4();
			rotateLocal.set(0.0f, -7.5f, 0.0f);
		}
	}

		// Convert from Local to World (as both applyForce() and applyTorque() need themin World space)
	hkVector4 forwardWorld; 
	forwardWorld.setRotatedDir( body->getTransform().getRotation(), forwardLocal );
	hkVector4 rotateWorld; 
	rotateWorld.setRotatedDir( body->getTransform().getRotation(), rotateLocal );
	body->applyForce( stepInfo.m_deltaTime, forwardWorld );
	body->applyTorque( stepInfo.m_deltaTime, rotateWorld );
}


MyPhantomCallbackShape:: MyPhantomCallbackShape() : hkpPhantomCallbackShape()
{
	m_penetratingShapesCount = 0;
}

void MyPhantomCallbackShape::phantomEnterEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl, const hkpCollisionInput& env )
{
	m_penetratingShapesCount++;
	// we should really use a thread safe implementation below
	//hkInterlockedIncrement( m_penetratingShapesCount );
}

void MyPhantomCallbackShape::phantomLeaveEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl )
{
	m_penetratingShapesCount--;
	//hkInterlockedDecrement( m_penetratingShapesCount );
}

//////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of phantoms as eyes for an AI controlled character. " \
"Usually we think of phantoms and entities as distinct. In this demo " \
"the phantom triangles are part of the shape hierarchy for the bot. When " \
"the bot moves the phantom automatically moves with it.";

HK_DECLARE_DEMO(PhantomEyesDemo, HK_DEMO_TYPE_PRIME, "Phantom Demo", helpString);

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

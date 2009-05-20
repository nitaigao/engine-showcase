/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/CollisionCallbacks/UserCollisionResponse/UserCollisionResponseDemo.h>

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

#include <Physics/Utilities/Dynamics/EntityContactCollector/hkpEntityContactCollector.h>
//#include <stdio.h>


class MyCollisionResolutionAction: public hkpUnaryAction
{
public:
	MyCollisionResolutionAction( hkpRigidBody* body ) :
	  hkpUnaryAction(body){}

	  ~MyCollisionResolutionAction()
	  {
	  }

	  /// The hkpAction interface implementation
	  virtual void applyAction( const hkStepInfo& stepInfo );

	  virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	  hkpEntityContactCollector& getContactCollector(){ return m_contactCollector; }

protected:

	hkpEntityContactCollector		m_contactCollector;

};



// Just apply a constant force in the direction of the normal ("push" the body away from the
// contact point"). This is not a very robust form of collision resolution (not an accurate model)
// but illustrates how the user can implement their own in special cases, if desired (for example
// for "controllers" for characters/cars, other "semi-physical" objects.
void MyCollisionResolutionAction::applyAction( const hkStepInfo& stepInfo )
{
	hkpRigidBody* body = static_cast<hkpRigidBody*>( m_entity );

	m_contactCollector.flipContactPoints( body );

	const hkArray<hkpEntityContactCollector::ContactPoint>& cpts = m_contactCollector.getContactPoints();
	for (int i = 0; i < cpts.getSize(); i++)
	{
		const hkpEntityContactCollector::ContactPoint& ccp = cpts[i];

		hkVector4 force; force.setMul4( 1000.0f * stepInfo.m_deltaTime, ccp.m_point.getNormal() );
		body->applyForce( stepInfo.m_deltaTime, force );
	}

	m_contactCollector.reset();
}


// This example shows how to redirect the information flow from the collision detection into our
// own controller (e.g. character controller, car sdk, ... ).\n
//		Steps to be done:
//			- Set the collision response in hkpEntity to hkpMaterial::HK_REPORTING_RESPONSE
//			- Set m_processContactCallbackDelay in hkpEntity to 0, (or another value if you require less information)
//			- add an hkContactCollecter as a hkpCollisionListener to the rigid body
//			- process the contacts in the contact collector each frame (preferably in an action).

UserCollisionResponseDemo::UserCollisionResponseDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 20.0f, 30.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Setup the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.8f, 0.0f);
		info.setBroadPhaseWorldSize(100.0f);
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// 
	// Create the table
	//
	{
		const float xdim = 10.0f;
		const float zdim = 10.0f;

		createStaticBox( m_world, 0.0f,     -1.0f,0.0f,		xdim,1.0f,zdim );
		createStaticBox( m_world, -xdim,  1.0f,0.0f,		   1.0f,1,zdim );
		createStaticBox( m_world,  xdim,  1.0f,0.0f,		   1.0f,1,zdim );
		createStaticBox( m_world,     0.0f,  1.0f, zdim,	xdim,1.0f,   1.0f );
		createStaticBox( m_world,     0.0f,  1.0f,-zdim,	xdim,1.0f,   1.0f );
	}

	// 
	// Create a body with a different collision response
	//
	{
		hkpConvexShape* shape = new hkpSphereShape( 0.8f );

		hkpRigidBodyCinfo sphereInfo;
		sphereInfo.m_shape = shape;
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		sphereInfo.m_position.set( 0.0f, 1.0f, 0.0f );
		sphereInfo.m_inertiaTensor.setDiagonal( 1.0f,1.0f,1.0f );
		sphereInfo.m_mass = 1.0f;
		sphereInfo.m_collisionResponse = hkpMaterial::RESPONSE_REPORTING;
		sphereInfo.m_processContactCallbackDelay = 0;

		hkpRigidBody* sphereRigidBody = new hkpRigidBody(sphereInfo);

		shape->removeReference();

		//
		//	Create our special (Collision Resolution) Action
		//
		MyCollisionResolutionAction* cle = new MyCollisionResolutionAction(sphereRigidBody );

		//
		//  Add our hkpEntityContactCollector to the rigid body
		//	(before it's added to the world, just to be sure to capture all collision callbacks,
		//	 even the ones which will be raised when this object was added.
		//   Note: This is not necessary in this example, however always a good practice. )
		// 

		cle->getContactCollector().addToEntity( sphereRigidBody );

		// Add our rigid body to the world 
		m_world->addEntity( sphereRigidBody );

		// Add our action to the world (after the rigid body is added)
		m_world->addAction( cle );
		cle->removeReference();

		sphereRigidBody->removeReference();

		// Let's start it off with a little horizontal velocity
		hkVector4 velocity(3.0f, 0.0f , -2.0f);
		sphereRigidBody->setLinearVelocity(velocity);

	}

	m_world->unlock();
}



void UserCollisionResponseDemo::createStaticBox( hkpWorld* world, float centerX, float centerY, float centerZ, float radiusX, float radiusY, float radiusZ )
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


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = \
"We set up an action which applies a constant force in the direction of the normal (push the body away from the " \
"contact point). This is not a very robust form of collision resolution (not an accurate model) " \
"but illustrates how the user can implement their own in special cases, if desired (for example " \
"for controllers for characters/cars, other semi-physical objects. ";

HK_DECLARE_DEMO(UserCollisionResponseDemo, HK_DEMO_TYPE_PRIME, "You can implement your own collision response", helpString);

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

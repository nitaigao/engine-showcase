/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Phantoms/PhantomEvents/PhantomEventsDemo.h>

// We will need these shapes
#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>
#include <Physics/Collide/Shape/Misc/Bv/hkpBvShape.h>


// We need to display some collision based info
//#include <hkvisualize/hkDisplayHandler.h>
#include <Common/Visualize/hkDebugDisplay.h>


// This is where all of the 'magic' for this demo takes place and it all centres around our custom
// implementations of the two pure virtual methods in the base class, namely phantomEnterEvent(...)
// and phantomLeaveEvent(...). These methods notify us of an entry or exit event for the phantom
// volume and most importantly provide a reference to the collidable that has penetrated the phantom.
// The code for this class is given below:


class MyPhantomShape: public hkpPhantomCallbackShape
{
	public:

		MyPhantomShape(){ }

		
		// hkpPhantom interface implementation
		virtual void phantomEnterEvent( const hkpCollidable* collidableA, const hkpCollidable* collidableB, const hkpCollisionInput& env )
		{
			// the color can only be changed once the entity has been added to the world
			hkpRigidBody* owner = hkGetRigidBody(collidableB);

			// the "Collidables" here are "faked" so it's necessary to get the owner first in order
			// to get the "real" collidable!
			HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), hkColor::rgbFromChars(255, 0, 0));
			//HK_SET_OBJECT_COLOR((int)&collidableB, hkColor::rgbFromChars(255, 0, 0));
		}

		// hkpPhantom interface implementation
		virtual void phantomLeaveEvent( const hkpCollidable* collidableA, const hkpCollidable* collidableB )
		{
			// the color can only be changed once the entity has been added to the world
			hkpRigidBody* owner = hkGetRigidBody(collidableB);

			// the "Collidables" here are "faked" so it's necessary to get the owner first in order
			// to get the "real" collidable!
			HK_SET_OBJECT_COLOR((hkUlong)owner->getCollidable(), hkColor::rgbFromChars(0, 255, 0));

			// If moving out AND falling down, apply impulse to fire it towards "wall"
			if(owner->getLinearVelocity()(1) < 0.0f)
			{
				hkVector4 impulse(-50.0f, 220.0f, 0.0f);
				owner->applyLinearImpulseAsCriticalOperation(impulse);
			}
		}
		
};


// While the entry event simply colours the ball object, the exit event also applies an impulse to the ball to make
// it bounce upwards back towards the wall. The reason why we call getOwner() on the original collidable
// only to later call getCollidable() on the owner is that the collidable passed by reference may be a 'sub' collidable
// of the rigid body, perhaps a hkpTriangleShape belonging to a larger hkpMoppBvTreeShape.

PhantomEventsDemo::PhantomEventsDemo( hkDemoEnvironment* env )
	:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(13.0f, 10.0f, 13.0f);
		hkVector4 to   (0.0f, 0.0f, 0.0f);
		hkVector4 up   (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the collision agents
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}


	// In this demo we have three different objects; the wall, the sphere and a phantom volume. Both the wall, which is simply a box,
	// and the sphere are created in the usual manner using a hkpRigidBodyCinfo 'blueprint' and are added to the world.

	//
	// Create the wall box
	//
	{
		hkpRigidBodyCinfo info;
		hkVector4 fixedBoxSize( 0.5f, 5.0f , 5.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );
		info.m_shape = fixedBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position.set(-2.0f, 0.0f ,0.0f);

		// Add some bounce.
		info.m_restitution = 0.9f;

		// Create fixed box
		hkpRigidBody* floor = new hkpRigidBody(info);
		m_world->addEntity(floor);
		floor->removeReference();

		fixedBoxShape->removeReference();
	}

	//
	// Create a moving sphere
	//
	{
		hkReal radius = 0.5f;
		hkpConvexShape* sphereShape = new hkpSphereShape(radius);


		// To illustrate using the shape, create a rigid body.
		hkpRigidBodyCinfo sphereInfo;

		sphereInfo.m_shape = sphereShape;
		sphereInfo.m_position.set(2.0f, 0.0f, 0.0f);
		sphereInfo.m_restitution = 0.9f;
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

		// If we need to compute mass properties, we'll do this using the hkpInertiaTensorComputer.
		if (sphereInfo.m_motionType != hkpMotion::MOTION_FIXED)
		{
			sphereInfo.m_mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereInfo.m_mass, massProperties);

			sphereInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			sphereInfo.m_centerOfMass = massProperties.m_centerOfMass;
			sphereInfo.m_mass = massProperties.m_mass;	
		}
		
			
		// Create a rigid body (using the template above)
		hkpRigidBody* sphereRigidBody = new hkpRigidBody(sphereInfo);

		// Remove reference since the body now "owns" the Shape
		sphereShape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(sphereRigidBody);
		sphereRigidBody->removeReference();
	}


	// Given below is the construction code for the phantom volume:

	//
	// Create a PHANTOM floor
	//
	{
		hkpRigidBodyCinfo boxInfo;
		hkVector4 boxSize( 4.0f, 1.5f , 2.0f );
		hkpShape* boxShape = new hkpBoxShape( boxSize , 0 );
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;

		boxInfo.m_position.set(2.0f, -4.0f, 0.0f);
		
		MyPhantomShape* myPhantomShape = new MyPhantomShape();
		hkpBvShape* bvShape = new hkpBvShape( boxShape, myPhantomShape );
		boxShape->removeReference();
		myPhantomShape->removeReference();

		boxInfo.m_shape = bvShape;
	

		hkpRigidBody* boxRigidBody = new hkpRigidBody( boxInfo );
		bvShape->removeReference();

		m_world->addEntity( boxRigidBody );
		
		// the color can only be changed after the entity has been added to the world
		HK_SET_OBJECT_COLOR((hkUlong)boxRigidBody->getCollidable(), hkColor::rgbFromChars(255, 255, 255, 50));

		boxRigidBody->removeReference();
		
	}

	// The phantom volume is created using a hkpBvShape using a hkpBoxShape as the bounding volume and a 
	// hkpPhantomCallbackShape as the child shape. The volume is set to be fixed in space and coloured with a slight alpha blend.
	//
	// Once the simulation starts the ball drops into the phantom volume, upon notification of this event we colour the ball
	// RED and wait for an exit event. As soon as this event is raised we colour the ball GREEN and apply an impulse upwards
	// back towards the wall and the simulation repeats.

	m_world->unlock();
}



#if defined( HK_COMPILER_MWERKS )
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of Phantom events to react to a ball when it passes through a Phantom  " \
"'box' shape. While the ball is inside the phantom shape it is displayed in RED otherwise " \
"it is shown in GREEN.";

HK_DECLARE_DEMO( PhantomEventsDemo, HK_DEMO_TYPE_PRIME, "Using a Phantom Shape to monitor entry/exit of an arbitrary region", helpString);

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/AsynchronousQueries/ClosestPoints/ClosestPointsDemo.h>
#include <Physics/Collide/Agent/hkpProcessCollisionOutput.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpGskBaseAgent.h>

// For access to display context
#include <Graphics/Common/Window/hkgWindow.h>

// We need to display some collision based info
#include <Common/Visualize/hkDebugDisplay.h>

// for debugging only
#include <Physics/Collide/Agent/ConvexAgent/Gjk/hkpGskfAgent.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>


struct ClosestPointsDemoVariant
{
	const char*	m_name;
	bool		m_useProcessFunc;
	const char*	m_details;
};



static const ClosestPointsDemoVariant g_ClosestPointsDemoVariants[] =
{
	{ "Using getClosestPoint", false, "" },
	{ "Using processCollision", true, "" },
};


/////////////////////////////////////////////////////////
////////////////// DEMO DEFINITIONS /////////////////////
/////////////////////////////////////////////////////////

ClosestPointsDemo::ClosestPointsDemo(hkDemoEnvironment* env, bool setupWorldFlag) 
	: hkDefaultPhysicsDemo(env) 
{
	if ( setupWorldFlag )
	{
		setupWorld(env, false);
	}
}


ClosestPointsDemo::~ClosestPointsDemo()
{
	m_world->lock();

	const ClosestPointsDemoVariant& variant =  g_ClosestPointsDemoVariants[this->m_variantId];
	if ( variant.m_useProcessFunc )
	{
		m_world->removeCollisionListener( this );
	}

	m_world->unlock();
}


void ClosestPointsDemo::setupWorld(hkDemoEnvironment* env, bool spuSupportedShapesOnly)
{
	// Disable face culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);

	// Get current variant
	const ClosestPointsDemoVariant& variant =  g_ClosestPointsDemoVariants[this->m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(3.0f, 4.0f, 8.0f);
		hkVector4 to(0.0f, 1.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.setZero4();
		info.m_enableDeactivation = false;
		info.m_contactPointGeneration = info.CONTACT_POINT_ACCEPT_ALWAYS;
		info.m_collisionTolerance = 0.0f;

		if ( variant.m_useProcessFunc )
		{
			info.m_collisionTolerance = 100.0f;
		}

		m_world = new hkpWorld( info );
		m_world->lock();

		if ( variant.m_useProcessFunc )
		{
			m_world->addCollisionListener( this );
		}

		setupGraphics();
	}

	//
	// Register the collision agents
	//
	{
		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	//	Create master tmplt for some of our special shapes
	//
	hkpRigidBodyCinfo tmplt;
	tmplt.m_linearDamping = 1.0f;
	tmplt.m_angularDamping = 1.0f;

	//
	// Create a box
	//
	{
		hkVector4 pos; pos.set(0,0,-3);
		hkVector4 size; size.set(1,1,1);
		hkpRigidBody* body = GameUtils::createBox(size, 1, pos);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	//
	// Create a sphere
	//
	{
		hkVector4 pos; pos.set(0,3,0);
		hkpRigidBody* body = GameUtils::createSphere(1.0f, 1.0f, pos);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	//
	// Create a sphere
	//
	{
		hkVector4 pos; pos.set(0,-3,0);
		hkpRigidBody* body = GameUtils::createSphere(1.0f, 1.0f, pos);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	//
	// Create a triangle
	//
	{
		hkVector4 pos; pos.set(3,0,0);
		hkpRigidBody* body = createTriangle( tmplt, pos, 1.0f);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	//
	// Create a triangle
	//
	{
		hkVector4 pos; pos.set(-3,0,0);
		hkpRigidBody* body = createTriangle( tmplt, pos, 1.0f);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	// We use several different shape types in this demo, including hkBoxShapes, hkSphereShapes and hkCapsuleShapes.
	// For the purposes of this tutorial we'll only outline the creation process for the capsule shape. This is
	// given below:

	//
	//	Create Capsule
	//
	{
		// End points for the capsule
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);

		// Radius for the capsule
		hkReal radius = 1.0f;

		hkpCapsuleShape* shape = new hkpCapsuleShape(A, B, radius);

		// Set up construction info for this rigid body
		hkpRigidBodyCinfo ci = tmplt;

		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_inertiaTensor.setDiagonal( 2.f, 2.f, 2.f );

		ci.m_mass = 1.0f;
		ci.m_shape = shape;
		ci.m_position.set(-3,0,3);

		hkpRigidBody* rigidBody = new hkpRigidBody(ci);
		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);

		// Remove our reference to the shape
		shape->removeReference();
	}



	//
	// Create a convex shape
	//
	{
		hkVector4 pos; pos.set(3,0,-3);
		hkPseudoRandomGenerator generator(12);
		hkpRigidBody* body = GameUtils::createRandomConvexGeometric(1.0f, 1.0f, pos, 7, &generator);
		m_world->addEntity(body);
		body->removeReference();
		m_bodies.pushBack(body);
	}

	//
	// Create a list shape made of 3 spheres
	//
	{
		hkpShape* listShape;
		{
			hkVector4 pos0( 0,0,.5f);
			hkVector4 pos1( 0,0,-.5f);
			hkVector4 pos2( 0,0.5f,0);
			hkpSphereShape* sphere = new hkpSphereShape( 0.3f );
			hkpConvexTranslateShape* cts0 = new hkpConvexTranslateShape( sphere, pos0);
			hkpConvexTranslateShape* cts1 = new hkpConvexTranslateShape( sphere, pos1);
			hkpConvexTranslateShape* cts2 = new hkpConvexTranslateShape( sphere, pos2);

			const hkpShape* shapes[3] = { cts0, cts1, cts2 };
			listShape = new hkpListShape( &shapes[0], 3 );

			cts2->removeReference();
			cts1->removeReference();
			cts0->removeReference();
			sphere->removeReference();
		}

		hkpRigidBodyCinfo ci = tmplt;

		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_inertiaTensor.setDiagonal( 2.f, 2.f, 2.f );

		ci.m_mass = 1.0f;
		ci.m_shape = listShape;
 		ci.m_position.set(-3,3,3);

		hkpRigidBody* rigidBody = new hkpRigidBody(ci);
		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);

		listShape->removeReference();
	}

	//
	//	Create a Cylinder
	//
	{
		// End points for the capsule
		hkVector4 A( 0.f, 1.2f, 0.f);
		hkVector4 B( 0.f,-1.2f, 0.f);

		// Radius for the capsule
		hkReal radius = 1.0f;

		hkpCylinderShape* shape = new hkpCylinderShape(A, B, radius);

		// Set up construction info for this rigid body
		hkpRigidBodyCinfo ci = tmplt;

		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		ci.m_inertiaTensor.setDiagonal( 2.f, 2.f, 2.f );

		ci.m_mass = 1.0f;
		ci.m_shape = shape;
		ci.m_position.set(-3,0,-3);

		hkpRigidBody* rigidBody = new hkpRigidBody(ci);
		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);

		// Remove our reference to the shape
		shape->removeReference();
	}

	//
	// Create a Mopp
	//
	{
		hkpRigidBodyCinfo rigidBodyInfo;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED; // Needs to be fixed so that it can run on SPU
		rigidBodyInfo.m_position.set( -4, -4, 3 );

		hkpMoppBvTreeShape* shape = GameUtils::createMoppShape(2);
		rigidBodyInfo.m_shape = shape;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
		m_world->addEntity(rigidBody)->removeReference();
		m_bodies.pushBack(rigidBody);
		shape->removeReference();
	}

	m_world->unlock();
}


hkDemo::Result ClosestPointsDemo::stepDemo()
{
	m_world->lock();

	// To find the closest points between our shape pairs we'll make use of the getClosestPoints(...) method of the agent for those shapes.
	// There is, however, a catch and that is the getClosestPoints(...) method will only return results that are inside tolerance and since
	// the closest points between our shape pairs are likely to lie beyond the default for this distance we must increase the tolerance:

	hkpCollisionInput input = *m_world->getCollisionInput();
	input.setTolerance( 100.f );

	//
	// Query each pair
	//
	const ClosestPointsDemoVariant& variant =  g_ClosestPointsDemoVariants[this->m_variantId];

	// We now create a loop to iterate across every body to every other body (ignoring any self body iterations) and ask the collision dispatcher
	// for the most suitable getClosestPoints(...) method to deal with our shape pair. Once we have this we simply pass through our collidables,
	// our input structure and a 'collector' class to gather the results of the query. To visualize the results we make use of a macro defined in
	// "hkDebugDisplay.h" called HK_DISPLAY_LINE. The macro takes three parameters: a start point, an end point and the line colour. We connect the
	// shape pairs with a GREEN line if a specialized agent was used to determine the distance. Otherwise we use a RED line which indicates that
	// GJK (or perhaps the more optimized GSK) was used to calculate the distance.
	if ( !variant.m_useProcessFunc )
	{
		for (int i=0; i < m_bodies.getSize(); i++)
		{
			for (int j=0; j < m_bodies.getSize(); j++)
			{

				// Ignore any self-self iterations.
				if ( i == j)
				{
					continue;
				}

				// Use a hkpClosestCdPointCollector class to gather the results of our query.
				hkpClosestCdPointCollector collector;

				// Get the shape type of each shape (this is used to figure out the most appropriate
				// getClosestPoints(...) method to use)
				hkpShapeType a = m_bodies[i]->getCollidable()->getShape()->getType();
				hkpShapeType b = m_bodies[j]->getCollidable()->getShape()->getType();

				// Ask the collision dispatcher to locate a suitable getClosestPoints(...) method
				hkpCollisionDispatcher::GetClosestPointsFunc getClosestPoints = m_world->getCollisionDispatcher()->getGetClosestPointsFunc( a, b );
				getClosestPoints( *m_bodies[i]->getCollidable(), *m_bodies[j]->getCollidable(), input, collector );


				// Display the results of our query. We connect the shape pairs with a GREEN line if a specialized
				// agent was used to determine the distance. Otherwise we use a RED line which indicates that
				// GJK (or perhaps the more optimized GSK) was used to calculate the distance.

				if ( collector.hasHit() )
				{
					const hkContactPoint& pt = collector.getHitContact();
					hkBool isGjk = (getClosestPoints == hkpGskBaseAgent::staticGetClosestPoints); 
					drawContactPoint(pt, isGjk);
				}
			}
		}
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


// We now create a callback function, which should draw our contact points between all pairs.
// The engine will call this function for all contact points between two objects, which have
// an overlapping aabb. We created the world with a collision tolerance of 100.0f, so all aabbs of
// all objects should overlap

void ClosestPointsDemo::contactProcessCallback( hkpContactProcessEvent& event)
{
	hkpProcessCollisionData& result = *event.m_collisionData;

	int size = result.getNumContactPoints();
	for (int i = 0; i < size; i++ )
	{
		const hkContactPoint& cp = result.m_contactPoints[i].m_contact;
		const hkVector4& pos    = cp.getPosition();
		const hkVector4& normal = cp.getNormal();
		hkVector4 pos2; pos2.setAddMul4( pos, normal, cp.getDistanceSimdReal() );
		HK_DISPLAY_LINE( pos, pos2, hkColor::GREEN );
		//HK_DISPLAY_ARROW( pos, normal, hkColor::GREEN );
	}
}


// This function is called for all new contact points.
// We are setting the process callback delay to zero, so that we receive the 
// contactProcessCallback every frame
void ClosestPointsDemo::contactPointAddedCallback( hkpContactPointAddedEvent& event)
{
	hkpEntity* entityA = hkGetRigidBody( event.m_bodyA->getRootCollidable() );
	entityA->setProcessContactCallbackDelay(0);
}


void ClosestPointsDemo::contactPointRemovedCallback( hkpContactPointRemovedEvent& event)
{

}


void ClosestPointsDemo::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{

}


// Create the shape and a rigid body to view it.
hkpRigidBody* ClosestPointsDemo::createTriangle(const hkpRigidBodyCinfo& tmplt, const hkVector4& pos, hkReal mass)
{
	int numVertices = 3;
	int stride = 4;

	float vertices[] = {
		-0.5f, -0.5f,  0.0f, 0.0f, // v0
		 0.5f, -0.5f,  0.0f, 0.0f, // v1
		 0.0f,  0.5f,  0.0f, 0.0f, // v2
	};

	/////////////////// SHAPE CONSTRUCTION ////////////////
	hkpTriangleShape* shape = new hkpTriangleShape();

	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		shape->setVertex(i, hkVector4(vertices[index], vertices[index + 1], vertices[index + 2]));
		index = index + stride;
	}
	///////////////////////////////////////////////////////


	// To illustrate using the shape, create a rigid body by first defining a tmplt.
	hkpRigidBodyCinfo rigidBodyInfo = tmplt;

	rigidBodyInfo.m_shape = shape;
	rigidBodyInfo.m_position = pos;

	// If we set this to true, the body is fixed, and no mass properties need to be computed.
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	stride *= sizeof(float); // convert to a byte stride
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeVertexCloudMassProperties(vertices, stride, numVertices, mass, massProperties);

	rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
	rigidBodyInfo.m_mass = massProperties.m_mass;			
		
	 
	// Create a rigid body (using the template above).
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
	// Remove references since the body now "owns" the Shapes.
	shape->removeReference();

	return rigidBody;
}


void ClosestPointsDemo::drawContactPoint(const hkContactPoint& contactPoint, hkBool isGjk)
{
	// getPosition() returns a point on B by convention
	const hkVector4 pointOnBInWorld   = contactPoint.getPosition();

	// normal goes from B to A by convention
	const hkVector4 normalBtoAInWorld = contactPoint.getNormal();

	// pointOnA = pointOnB + dist * normalBToA
	hkVector4 pointOnAInWorld;
	pointOnAInWorld.setAddMul4(pointOnBInWorld, normalBtoAInWorld, contactPoint.getDistance());

	//
	// Which color? GREEN for optimized agent, RED for GJK/GSK
	//
	hkUint32 color = (isGjk)? hkColor::RED : hkColor::GREEN;

	// Draw the line from A to B and the normal on B
	HK_DISPLAY_LINE(  pointOnAInWorld, pointOnBInWorld, color  );
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demonstrates how you can find the closest points between a given pair of objects. "	\
"In addition to this we also indicate whether or not a specially optimized agent exists for "	\
"this pair of shapes. If one does then we connect the closest points for this pair with "		\
"a GREEN line. Otherwise the pair are connected by a RED line to indicate that GSK was used " \
"to compute the points. ";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ClosestPointsDemo, HK_DEMO_TYPE_PRIME, ClosestPointsDemoVariant, g_ClosestPointsDemoVariants, helpString );

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

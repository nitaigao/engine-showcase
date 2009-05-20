/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Dynamics/Phantoms/PhantomObject/PhantomObjectDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

// We need to create a phantom
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// We will need these collision agents for registering 
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>
#include <Physics/Collide/Agent/MiscAgent/Phantom/hkpPhantomAgent.h>

// We need to display the Phantom in some way, so we can see where it is!
#include <Common/Visualize/hkDebugDisplay.h>


class hkMovingRBCollectorPhantom : public hkpAabbPhantom
{
	public:

		hkMovingRBCollectorPhantom( const hkAabb& aabb, hkUint32 collisionFilterInfo = 0 );

		// Callback implementation 
		virtual void addOverlappingCollidable( hkpCollidable* handle );

		// Callback implementation 
		virtual void removeOverlappingCollidable( hkpCollidable* handle );
};

DEMO_OPTIONS_DEFINE(PhantomObjectDemo,Options);

PhantomObjectDemo::PhantomObjectDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE), m_time(0)
{

	//
	// Set up the camera
	//
	{
		hkVector4 from(0.0f, 30.0f, 40.0f);
		hkVector4 to(0.0f, 3.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
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
	// Register the agents required, both box-box and phantom-anything.
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	// This demo has three distinct sets of objects; a box shape floor object, a phantom and a collection
	// of box shapes to be scattered about by the phantom. All three are created in the standard fashion each using
	// a construction info (cinfo).

	//
	// Create the fixed box
	//
	{
		hkpRigidBodyCinfo info;

		hkVector4 fixedBoxSize(20.0f, .5f , 20.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );
		info.m_shape = fixedBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		
		info.m_position.set(0.0f,0.0f,0.0f);

		// create fixed box
		hkpRigidBody* box = new hkpRigidBody(info);
		fixedBoxShape->removeReference();

		m_world->addEntity(box);
		box->removeReference();
	}

	//
	// Create many falling rigid bodies
	//
	{
		// We create a single shape, and resuse it for all bodies.
		hkVector4 boxSize( .5f, .5f ,.5f );
		hkpShape* boxShape = new hkpBoxShape( boxSize , 0 );

		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = boxShape;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Compute the box inertia tensor
		hkReal boxMass = 1.0f;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(boxSize, boxMass, massProperties);
		boxInfo.m_mass = boxMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;

		const int numBodies = m_options.m_numberOfBoxes;

		hkPseudoRandomGenerator generator(1342);

		for ( int i = 0; i < numBodies; i++ )
		{			
			boxInfo.m_position.set( generator.getRandRange(-10.0f, 10.0f), 1.0f, generator.getRandRange(-10.0f, 10.0f) );

			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}

		boxShape->removeReference();
	}
	
	// The code to create the phantom is presented below:

	{
		hkAabb infoAABB;
		hkReal halfWidth = 0.5f * m_options.m_aabbWidth;
		infoAABB.m_min.set( -halfWidth, 0.0f, -halfWidth );
		infoAABB.m_max.set( halfWidth, 20.0f, halfWidth ) ;
		m_phantom = new hkMovingRBCollectorPhantom( infoAABB );
		m_world->addPhantom( m_phantom );
	}

	m_world->unlock();
}



PhantomObjectDemo::~PhantomObjectDemo()
{
	m_world->lock();
	m_phantom->removeReference();
	m_world->unlock();
}


// Stepping the game consists of two parts; moving the phantom and applying forces to the overlapping collidables.
// We generate a circular motion for the phantom using sine and cosine function calls. However, since the phantom
// is just an AABB we must explicitly call the setAabb(...) method to set it's new position. The phantom will
// automatically review it's internal list of overlapping collidables and add or remove entries as necessary.

hkDemo::Result PhantomObjectDemo::stepDemo()
{
	m_world->lock();

	const hkReal radius = 6.0f;
	m_options.m_aabbWidth = hkMath::max2( 1, m_options.m_aabbWidth );
	const hkReal phantomSide = hkReal(m_options.m_aabbWidth);
		
	m_time += m_timestep;

	//
	//	Move the phantom
	//
	{
		
		hkReal circleParam = m_time * 0.3f;
		hkReal si = hkMath::sin( circleParam ) * radius;
		hkReal co = hkMath::cos( circleParam ) * radius;
		hkAabb aabb;
		aabb.m_min = hkVector4( -phantomSide * 0.5f + si, 0.6f, -phantomSide * 0.5f + co);
		aabb.m_max = hkVector4(  phantomSide * 0.5f + si,  20.0f,  phantomSide * 0.5f + co );

		// To "move" a Phantom since it is (in the current API) just an AABB, we use the
		// setAabb() method to explicitly set its new location in the World. It
		// automtically updates its own list of entities which it currently overlaps.
		m_phantom->setAabb( aabb );
		

		// (The phantom outline is displayed automatically by the demo framework.)
	}

	// The second thing we need to do is to apply an upward force to any overlapping rigid bodies. The code to do this is as follows:
	
	for (int i = 0; i < m_phantom->getOverlappingCollidables().getSize(); i++ )
	{
		// Note: it is OK to directly cast here, because we filter out all but moving rigid bodies in the add and remove collidable calls (above)
		hkpRigidBody* rb = hkGetRigidBody(m_phantom->getOverlappingCollidables()[i]);

		rb->activate();
		hkReal height = rb->getPosition()(1);
		hkVector4 force( 0.0f, 5.0f*(10.0f - height) ,0.0f );
		hkVector4 impulse; impulse.setMul4( m_timestep, force );
		rb->applyLinearImpulse( impulse );
	}

	// Which simply iterates through the phantom's internal list of overlapping collidables and applies an upward force to it.
	// A subtlety to note here is the call to activate(). This is done to ensure the object reacts to the applied force
	// immediately, otherwise, assuming the rigid body had deactivated, the effect would not be noticeable until later reactivation.

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



// This class centers around the implementations of the two virtual methods in the base class, namely
// addOverlappingCollidable(...) and removeOverlappingCollidable(...). The code for these methods is given below.
// Note: This (very simple) demo works fine without implementing either of these functions. Deriving an entirely
// new hkpAabbPhantom class is not strictly necessary either...

hkMovingRBCollectorPhantom::hkMovingRBCollectorPhantom( const hkAabb& aabb, hkUint32 collisionFilterInfo  )
: hkpAabbPhantom(aabb, collisionFilterInfo)
{
}


void hkMovingRBCollectorPhantom::addOverlappingCollidable( hkpCollidable* c )
{
	hkpRigidBody* rb = hkGetRigidBody(c);

	// Ignore other phantoms and fixed rigid bodies.
	if ( (rb != HK_NULL) && !rb->isFixed() )
	{
		hkpAabbPhantom::addOverlappingCollidable( c );
	} 
}

void hkMovingRBCollectorPhantom::removeOverlappingCollidable( hkpCollidable* c )
{
	hkpRigidBody* rb = hkGetRigidBody(c);

	// hkpAabbPhantom::removeOverlappingCollidable() protects its m_overlappingCollidables member,
	// regardless of whether collidables are present in it or not. However this check needs to be
	// symmetric with the one above, if we want symmetry in callbacks fired within 
	// hkpAabbPhantom::removeOverlappingCollidable() and hkpAabbPhantom::addOverlappingCollidable().
	if ( (rb != HK_NULL) && !rb->isFixed() )
	{
		hkpAabbPhantom::removeOverlappingCollidable( c );
	}
}


// As you can see these methods simply check that we have a rigid body which is not fixed (i.e. the floor) 
// and if so, either add and remove collidables from the phantom's internal list of overlapping collidables.
// Since the phantom maintains this list, we can at any point query the phantom for all objects overlapping
// with it. This is exactly what we do in the stepDemo(...) method.


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of a Phantom Object. A phantom is an unintegrated object that maintains " \
"a list of other objects whose AABBs overlap with it (including other phantoms). " \
"We illustrate which other objects (in this case rigid bodies) are overlapping with the Phantom by " \
"applying an upward force to them as the phantom volume sweeps over. " \
"The phantom itself is just an AABB and has no physical presence in the simulation. ";

HK_DECLARE_DEMO(PhantomObjectDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Using a broadphase Phantom Entity for fast AABB region queries", helpString);

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

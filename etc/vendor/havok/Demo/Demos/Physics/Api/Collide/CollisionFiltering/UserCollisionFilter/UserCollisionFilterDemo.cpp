/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



#include <Demos/demos.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>

#include <Demos/Physics/Api/Collide/CollisionFiltering/UserCollisionFilter/UserCollisionFilterDemo.h>


// We will need this collision agent (we are not using hkpAgentRegisterUtil)
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>


// In order to allow certain bodies to interact but not others we must create a collision filter and add it to our world.
// Thus when two bodies come in contact Havok can decide (based on the collision filter rules) whether or not
// to allow them to collide. In our case the rule is rather simple: if collidable A and B are both in the same collision 'group'
// then allow the collision, otherwise ignore them.
// To implement this rule we derive our own MyCollisionFilter class from the hkpCollisionFilter base class and customise
// each of the 4 pure virtual isCollisionEnabled(...) methods to extract the relevant hkUint32 filter infos. The logic of this
// custom filter is given below:

hkBool MyCollisionFilter::isCollisionEnabled(hkUint32 infoA, hkUint32 infoB) const
{
	// Examine the collisionfilterinfo of each. This is "user" information which can be interpreted
	// however the filter sees fit. Another example would be to let every body have a unique
	// collisionfilterinfo value, and allow/disallow collisions based on a pairwise lookup.
	// We use a much simpler rule here for ease of illustration.
	// Let's say that bodies with collisionfilters X and Y are allowed to collide if and only if X == Y,
	// ie. if they are in the same "group"

	if( infoA == infoB )
	{
		return true;
	}

	return false;
}

// This decides if two collidables (overlapping in the broadphase for example) should collide and is inherited from
// hkpCollidableCollidableFilter
hkBool MyCollisionFilter::isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const
{
	return isCollisionEnabled( a.getCollisionFilterInfo(), b.getCollisionFilterInfo() );
}

// This decides if an element of a hkpShapeCollection should collide against another hkpShape
// and is inherited from hkpShapeCollectionFilter. 
// N.B. There are two possible 'gotchas' associated with implementing this method, both related to the logic
// of what should happen when a collection collides against another collection: Please read the comments in hkpGroupFilter
// very carefully to see how it deals with these problems. 
// 1. If you allow 'per child shape vs child shape' level filtering you must handle the case where 'a' is *also* a child of 
// a shape collection by marching back up the hierarchy to find a suitable parent collection from which 'a's filterinfo can
// be extracted.
// 2. If you allow 'per child shape vs child shape' level filtering you are likely to run into consistency problems which 
// can be very hard to debug. For example if we had two hierachies like this (say two hkListShapes, each with two children)
// with corresponding filterinfos:
/*
  1
 / \
2   2

colliding against

  3
 / \
4   4

*/
// And suppose we were to enable/disable collisions like this:
// 1 & 3 collide
// 1 & 4 collide
// 2 & 4 collide
// but 2 & 3 *don't* collide,
// Then we'd either get collisions between the leaves, or we wouldn't, *depending on the order of the filter calls*, ie depending
// which body is 'a and which is 'b'. In one case it will test 2 against 3, and in one case it won't. (Again, see the comments in hkpGroupFilter)
// This call order depends on the agent types and on the order of addition of bodies to the world. Clearly this is highly dangerous. 
hkBool MyCollisionFilter::isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bCollection, hkpShapeKey bKey  ) const
{
	hkUint32 infoB = bCollection.getCollisionFilterInfo( bKey );
	// We need a corresponding filter info for 'a'. Whether we should get this from a parent/grandparent/etc... of 'a' in the case that
	// 'a' is part of a shape collection depends on how we decide to handle the 'collection vs collection' case.
	// Here we just assume that we do not have collections colliding against collections, and use the filter info of the root collidable of 'a'
	return isCollisionEnabled( a.getRootCollidable()->getCollisionFilterInfo(), infoB );
}

hkBool MyCollisionFilter::isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const
{
	hkUint32 infoA = containerShapeA.getCollisionFilterInfo( keyA );
	hkUint32 infoB = containerShapeB.getCollisionFilterInfo( keyB );
	return isCollisionEnabled( infoA, infoB );
}

hkBool MyCollisionFilter::isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const 
{
	hkUint32 infoB = bContainer.getCollisionFilterInfo( bKey );
	return isCollisionEnabled( aInput.m_filterInfo, infoB );
}

hkBool MyCollisionFilter::isCollisionEnabled( const hkpWorldRayCastInput& aInput, const hkpCollidable& collidableB ) const
{
	return isCollisionEnabled( aInput.m_filterInfo, collidableB.getCollisionFilterInfo() );
}


// Our filter rule logic simply gets the collision filter info (a hkUint32) for both entities and checks to
// see if they are equal; if they are the collision is allowed, otherwise we inform Havok to ignore them.

UserCollisionFilterDemo::UserCollisionFilterDemo( hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 10.0f, 15.0f);
		hkVector4 to(0.0f, 3.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}


	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();
	}

	//
	// Register the box-box collision agent
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	/// To add the filter to the world we simply instantiate a new filter and set it to be active:

	//
	//	Create the my collision filter
	//
	{
		MyCollisionFilter* filter = new MyCollisionFilter();
	
		m_world->setCollisionFilter( filter );
		filter->removeReference();
	}

	// NOTE: You must set the collision filter prior to adding any entities to the world otherwise the pre-filter
	// added entities could cause undesirable behaviour before the filter is active.

	// All three objects are hkpBoxShape shapes, two of which are fixed and the third dynamic. In order to allow
	// our filters work correctly we must assign each of the objects a collision 'group'. Both the lower floor
	// and the dynamic box have their collision 'group' set to zero: 

	//
	// Create the floor with collisioninfo = 0
	//
	{
		hkpRigidBodyCinfo info;
		hkVector4 fixedBoxSize(5.0f, .5f , 5.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );
		info.m_shape = fixedBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position.set(0.0f, -1.0f, 0.0f);

		// Create fixed box
		hkpRigidBody* floor = new hkpRigidBody(info);
		floor->setCollisionFilterInfo(0);

		m_world->addEntity(floor);
		floor->removeReference();
		fixedBoxShape->removeReference();
	}

	// Whereas the upper floor belongs to the 'one' group:

	//
	// Create the floor with collisioninfo = 1
	//
	{
		hkpRigidBodyCinfo info;
		hkVector4 fixedBoxSize(3.0f, .5f , 3.0f );
		hkpBoxShape* fixedBoxShape = new hkpBoxShape( fixedBoxSize , 0 );
		info.m_shape = fixedBoxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_position.set(0.0f,4.0f,0.0f);

		// Create fixed box
		hkpRigidBody* floor = new hkpRigidBody(info);
		fixedBoxShape->removeReference();

		floor->setCollisionFilterInfo(1);

		m_world->addEntity(floor);
		floor->removeReference();
	}

	// In this way only the lower floor and the dynamic box will interact based on the rule we defined in
	// MyCollisionFilter::isCollisionEnabled(...).

	//
	// Create a moving box with collisioninfo = 0, so it will collide with only the "bottom" floor
	//
	{
		hkpRigidBodyCinfo boxInfo;
		hkVector4 boxSize( .5f, .5f ,.5f );
		hkpShape* boxShape = new hkpBoxShape( boxSize , 0 );
		boxInfo.m_shape = boxShape;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		// Compute the box inertia tensor
		hkReal boxMass = 1.0f;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties( boxSize, boxMass, massProperties );
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		boxInfo.m_mass = boxMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		boxInfo.m_position.set(0.0f, 7.0f, 0.0f);

		hkpRigidBody* boxRigidBody = new hkpRigidBody( boxInfo );
		boxShape->removeReference();

		boxRigidBody->setCollisionFilterInfo(0);	// HERE WE SET THE COLLISION FILTER INFO
		m_world->addEntity( boxRigidBody );
		boxRigidBody->removeReference();
	}

	m_world->unlock();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A demo which shows use of a very basic filter to disallow collisions between certain bodies. " \
"The scene consists of 3 boxes, two of which are fixed and the third dynamic. Initially the " \
"dynamic box is placed above the other two and dropped. However, due to the nature of the " \
"collision filter we have created, the box will pass through the first fixed box and only " \
"collide with the second lower one. ";

HK_DECLARE_DEMO( UserCollisionFilterDemo, HK_DEMO_TYPE_PRIME, "A user-defined collision filter", helpString);

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

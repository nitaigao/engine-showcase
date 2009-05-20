/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDemoUtils.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Phantom/hkpPhantom.h>

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>


void HK_CALL KdTreeDemoUtils::createRandomBodies( hkpWorld* world, const hkAabb& worldAabb, int numBodies, hkpMotion::MotionType motionType, class hkPseudoRandomGenerator* rand,  hkArray<const hkpCollidable*>& collidablesOut )
{
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1, 1);

	hkArray<hkpEntity*> bodyArray;
	bodyArray.reserve(numBodies);

	hkpShape* shape;
	{
		hkVector4 halfExtents(0.5f, 0.5f, 0.5f);
		shape = new hkpBoxShape( halfExtents, 0.0f );
	}

	hkAabb shrunkenAabb = worldAabb;
	shrunkenAabb.m_min.mul4(.9f);
	shrunkenAabb.m_max.mul4(.9f);

	for( int i = 0; i < numBodies; i++)
	{
		// All bodies created below are movable
		rigidBodyInfo.m_motionType = motionType;

		// A collection of many rigid bodies is randomly created using a hkpBoxShape
		rigidBodyInfo.m_shape = shape;

		// As usual we fill out the hkpRigidBodyCinfo 'blueprint' for the rigidbody, with the code above specifying
		// the necessary information for the 'm_shape' member. To create a hkpConvexVerticesShape we need a set of vertices and
		// we must generate a set of plane equations from these points. As you can see from the code this is all performed 
		// prior to instantiating the shape.

		// Fake Inertia tensor for simplicity, assume it's a unit cube
		{
			hkReal mass = 10.0f;
			hkReal d = mass * 0.5f;
			rigidBodyInfo.m_inertiaTensor.setDiagonal( d,d,d );
			rigidBodyInfo.m_mass = mass;			
		}	

		// The object is then assigned a random position, orientation and angular velocity and added to the world:
		rand->getRandomVectorRange(shrunkenAabb.m_min, shrunkenAabb.m_max, rigidBodyInfo.m_position);
		rand->getRandomRotation( rigidBodyInfo.m_rotation );

		rigidBodyInfo.m_collisionFilterInfo = hkpGroupFilterSetup::LAYER_DEBRIS;

		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Give them an intial velocity
		if (!rigidBody->isFixed())
		{
			hkVector4 angularVel(rand->getRandRange(-1.0f, 1.0f), rand->getRandRange(-1.0f, 1.0f), rand->getRandRange(-1.0f, 1.0f));
			rigidBody->setAngularVelocity(angularVel);
			rigidBody->setAngularDamping(0.0f);
		}

		bodyArray.pushBack( rigidBody );
		collidablesOut.pushBack(rigidBody->getCollidable());

		// There is no gravity vector for this world and so the bodies will appear to float in space.
	}
	shape->removeReference();

	// Batch add all bodies to the system and defragment the broadphase
	world->addEntityBatch( bodyArray.begin(), bodyArray.getSize() );
	world->getBroadPhase()->defragment();

	//
	//	Remove all references to bodies. They are now referenced by m_world
	//
	{
		for ( int i = 0; i < bodyArray.getSize(); i++ )
		{
			bodyArray[i]->removeReference();
		}
	}
}

void HK_CALL KdTreeDemoUtils::makeBundleFromInputs(hkpWorldRayCastInput* inputs, hkKdTreeUtils::RayBundle& fromBundle, hkKdTreeUtils::RayBundle& toBundle)
{
	hkVector4 tempFrom, tempTo;
	fromBundle.m_vec[0] = inputs[0].m_from;
	toBundle.m_vec[0] = inputs[0].m_to;

	fromBundle.m_vec[1] = inputs[1].m_from;
	toBundle.m_vec[1] = inputs[1].m_to;

	fromBundle.m_vec[2] = inputs[2].m_from;
	toBundle.m_vec[2] = inputs[2].m_to;

	tempFrom = inputs[3].m_from;
	tempTo = inputs[3].m_to;

	HK_TRANSPOSE4(fromBundle.m_vec[0], fromBundle.m_vec[1], fromBundle.m_vec[2], tempFrom);
	HK_TRANSPOSE4(toBundle.m_vec[0], toBundle.m_vec[1], toBundle.m_vec[2], tempTo);
}

// Generates a valid set of rays for a bundle
// We keep the starts points close, and ensure that the signs of the direction components are all the same
void HK_CALL KdTreeDemoUtils::makeRaysForBundle(hkVector4& startIn, hkVector4& endIn, hkVector4* startsOut, hkVector4* endsOut, hkReal variance, hkPseudoRandomGenerator& rand )
{
	startsOut[0] = startIn;
	endsOut[0] = endIn;

	hkVector4 dir; dir.setSub4(endIn, startIn);

	for(int i=1; i<4; i++)
	{
		hkVector4 tempDir = dir;
		hkVector4 deltaDir; rand.getRandomVector01(deltaDir);

		rand.getRandomVector01(deltaDir);
		deltaDir.mul4(variance);
		const hkVector4 one(1,1,1);
		deltaDir.add4(one);

		tempDir.mul4(deltaDir);

		hkVector4 deltaStart; rand.getRandomVector11(deltaStart);
		deltaStart.mul4(5.0f*variance);
		startsOut[i].setAdd4(startIn, deltaStart);
		endsOut[i].setAdd4(startsOut[i], tempDir);

		if (variance == 0.0f)
		{
			startsOut[i] = startIn;
			endsOut[i] = endIn;
		}	
	}
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

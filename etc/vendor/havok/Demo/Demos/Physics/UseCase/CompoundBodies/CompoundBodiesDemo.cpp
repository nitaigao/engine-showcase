/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>


#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>


#include <Demos/Physics/UseCase/CompoundBodies/CompoundBodiesDemo.h>

#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>



CompoundBodiesDemo::CompoundBodiesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 30.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	// Create the base
	{
		hkVector4 baseSize( 20.0f, 1.0f, 20.0f);
		hkVector4 basePos(   0.0f,-0.5f,  0.0f);
		hkpRigidBody* baseRigidBody = GameUtils::createBox(baseSize, 0.0f, basePos);
		m_world->addEntity( baseRigidBody );

		baseRigidBody->removeReference();
	}



	hkPseudoRandomGenerator generator(747);
		// Create 10 wibblies
	for(int i = 0; i < 10; i++)
	{

		hkVector4 pos( generator.getRandRange(-3.0f,3.0f) , i * 3.0f + 1.5f, generator.getRandRange(-3.0f,3.0f));
		hkpRigidBody* wibbly = createWibbly(pos, &generator);
		m_world->addEntity( wibbly );

		wibbly->removeReference();

		hkVector4 vel(generator.getRandRange(-2.0f,2.0f), generator.getRandRange(-2.0f,2.0f), generator.getRandRange(-2.0f,2.0f));
		wibbly->setAngularVelocity(vel);
	}

	m_world->unlock();
}


// A "wibbly" is a sphere-man with his center of mass brought low inside the body so he wobbles easily.
// Note that we do not share shapes. We could share the spheres and boxes used since they're of fixed size
// but we create new ones each when time using this method.
hkpRigidBody* CompoundBodiesDemo::createWibbly(hkVector4& position, hkPseudoRandomGenerator* generator)
{
	// We build a wibbly from 4 bodies:
	// 1. A big sphere
	// 2,3 Two arms
	// 4 A head

		// These parameters specify the wibbly size. The main (body) sphere has the radius defined
		// below. The arms have size 'boxSize'.
	hkReal radius = 1.0f;
	hkVector4 boxSize( 1.0f, 0.5f, 0.5f);
	hkReal mass = 10.0f;


	//
	// Create the shapes (we could share these between wibblies of the same size, but we don't here)
	//

	hkpSphereShape* sphere = new hkpSphereShape(radius);

	hkpSphereShape* sphere2 = new hkpSphereShape(radius * 0.3f);

	hkVector4 halfExtents(boxSize(0) * 0.5f, boxSize(1) * 0.5f, boxSize(2) * 0.5f);

	hkpBoxShape* cube = new hkpBoxShape(halfExtents, 0 );	

	//
	// Create a rigid body construction template and start filling it out
	//
	hkpRigidBodyCinfo compoundInfo;


	// We'll basically have to create a 'List' Shape  (ie. a hkpListShape) in order to have many
	// shapes in the same body. Each element of the list will be a (transformed) hkpShape, ie.
	// a hkpTransformShape (which basically is a (geometry,transformation) pair).
	// The hkpListShape constructor needs a pointer to an array of hkShapes, so we create an array here, and push
	// back the hkTransformShapes as we create them.
	hkInplaceArray<hkpShape*,4> shapeArray;

	// Create body
	{
		sphere->addReference();
		shapeArray.pushBack(sphere);
	}

	// Create head
	{
		hkVector4 offset(0.0f, 1.1f, 0.0f);
		hkpConvexTranslateShape* sphere2Trans = new hkpConvexTranslateShape( sphere2, offset );
		shapeArray.pushBack(sphere2Trans);
	}


	// Create right arm
	{

		hkTransform t;
		hkVector4 v(0.0f,0.0f,1.0f);
		hkQuaternion r(v, 0.4f);
		t.setRotation(r);
		t.getTranslation().set(0.9f, .7f, 0.0f);
		hkpConvexTransformShape* cubeTrans = new hkpConvexTransformShape( cube,t );
		shapeArray.pushBack(cubeTrans);
	}


	// Create left arm
	{
		hkTransform t;
		hkVector4 v(0.0f,0.0f,1.0f);
		hkQuaternion r(v, -0.4f);
		t.setRotation(r);
		t.getTranslation().set(-0.9f, .7f, 0.0f);
		hkpConvexTransformShape* cubeTrans = new hkpConvexTransformShape( cube,t );
		shapeArray.pushBack(cubeTrans);
	}



	// Now we can create the compound body as a hkpListShape

	hkpListShape* listShape;
	{
		listShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());
		sphere->removeReference();
		sphere2->removeReference();
		cube->removeReference();
		for (int i = 0; i < shapeArray.getSize(); ++i)
		{
			shapeArray[i]->removeReference();
		}
	}
	compoundInfo.m_shape = listShape;


	//
	// Create the rigid body
	//

	compoundInfo.m_mass = mass;
	
		// Fake an inertia tensor using a cube of side 'radius'
	hkVector4 halfCube(radius *0.5f, radius *0.5f, radius *0.5f);
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfCube, mass, massProperties);
	compoundInfo.m_inertiaTensor = massProperties.m_inertiaTensor;

	// Now (and here's the wibbly bit) set the center of mass to be near the bottom of the
	// body sphere.
	compoundInfo.m_centerOfMass.set(0.0f,-0.8f,0.0f);
	compoundInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	compoundInfo.m_position = position;
		// Use "random" initial orientation
	hkVector4 axis(generator->getRandRange(-1.0f,1.0f),generator->getRandRange(-1.0f,1.0f),generator->getRandRange(-1.0f,1.0f));
	axis.normalize3();
	hkQuaternion q(axis, generator->getRandRange(0,HK_REAL_PI));
	compoundInfo.m_rotation = q;


		// Finally create the body
	hkpRigidBody* compoundRigidBody = new hkpRigidBody(compoundInfo);

	listShape->removeReference();

	return compoundRigidBody;

}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates constructing compound bodies from several (convex) pieces, " \
"as well as how to shift the center of mass of objects. " \
"We create a crowd of wibblies, and drop them on a base box, where they wibble away to their " \
"hearts' content.";

HK_DECLARE_DEMO(CompoundBodiesDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "CompoundBodies", helpString);

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Landscape/Mopp/MoppDemo.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>


MoppDemo::MoppDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	// Setup the camera.
	{
		hkVector4 from(0.0f, 5.0f, 10.0f);
		hkVector4 to  (0.0f, 0.0f,  0.0f);
		hkVector4 up  (0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		
		// Set gravity to zero so body floats.
		info.m_gravity.set(0.0f, 0.0f, 0.0f);	
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();
	}

	//
	// Create the MOPP shape
	//
	hkVector4 halfExtents1 = hkVector4(2.0f, 0.5f, 0.5f);
	hkVector4 halfExtents2 = hkVector4(0.5f, 2.0f, 0.5f);
	hkpBoxShape* boxShape1 = new hkpBoxShape(halfExtents1, 0 );
	hkpBoxShape* boxShape2 = new hkpBoxShape(halfExtents2, 0 );

	hkArray<hkpShape*> shapeArray;
	shapeArray.pushBack(boxShape1);
	shapeArray.pushBack(boxShape2);


	/////////////////// SHAPE CONSTRUCTION ////////////////
	hkpListShape* listShape = new hkpListShape(shapeArray.begin(), shapeArray.getSize());

	// Remove reference since the listShape now "owns" the boxShapes.
	boxShape1->removeReference();
	boxShape2->removeReference();

	hkpMoppCompilerInput moppInput;
	moppInput.setAbsoluteFitToleranceOfTriangles(0.1f);
	hkpMoppCode* code = hkpMoppUtility::buildCode(listShape, moppInput);		

	hkpMoppBvTreeShape* shape = new hkpMoppBvTreeShape(listShape, code);

	// Remove references since the MoppBvTreeShape now "owns" the listShape and code
	code->removeReference();
	listShape->removeReference();
	///////////////////////////////////////////////////////


	// To illustrate using the shape, first define a rigid body template.
	hkpRigidBodyCinfo rigidBodyInfo;
	rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	rigidBodyInfo.m_angularDamping = 0.0f;
	rigidBodyInfo.m_linearDamping = 0.0f;
	rigidBodyInfo.m_shape = shape;

	// Compute the rigid body inertia.
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties( rigidBodyInfo.m_shape, 100.0f, rigidBodyInfo );
		
	// Create a rigid body (using the template above).
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

	// Remove reference since the body now "owns" the Shape.
	shape->removeReference();

	// Finally add body so we can see it, and remove reference since the world now "owns" it.
	m_world->addEntity(rigidBody);
	rigidBody->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"A simple MoppBvTreeShape - a list shape with bounding volume info for fast collision detection";

HK_DECLARE_DEMO(MoppDemo, HK_DEMO_TYPE_PRIME, "MOPP shape construction", helpString ); 

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

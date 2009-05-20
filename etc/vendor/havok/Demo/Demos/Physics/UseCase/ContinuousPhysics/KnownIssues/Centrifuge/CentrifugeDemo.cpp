/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */



#include <Demos/demos.h>
#include <Demos/Physics/UseCase/ContinuousPhysics/KnownIssues/Centrifuge/CentrifugeDemo.h>


#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>

struct CentrifugeVariant
{
	const char*	 m_name;
	bool   m_usePredictive;
	hkReal m_allowsPenetration;
	const char* m_details;
};


static const CentrifugeVariant g_variants[] =
{
	{ "discrete",								false, 0.01f,	"Discrete simulation fails quickly" },
	{ "continuous, allowed penetration=0.2f",	true,  0.2f,	"Continuous simulation using default allowed penetration is not much better" },
	{ "continuous, allowed penetration=0.01",	true,  0.01f,	"Decreasing penetration does help" },
	{ "continuous, allowed penetration=0.0001",	true,  0.0001f,	"Further decreasing allowed penetration helps some more" },
};



hkpRigidBody* CentrifugeDemo::createHollowBox()
{
	// Create the shape and a rigid body to view it.
	hkArray<hkpShape*> shapes;
	{
		hkVector4 carDim(1.0f, 2.0f, 1.0f);
		carDim.mul4(0.5f);
		hkReal thick = 0.01f * 0.5f;
		hkVector4 thickness(thick, thick, thick);

		for (int i = 0; i < 3; i++)
		{
			hkVector4 halfExt; halfExt.setZero4();
			halfExt(i) = carDim(i);
			halfExt((i+1)%3) = carDim((i+1)%3);
			halfExt.add4(thickness);

			hkpBoxShape* box = new hkpBoxShape(halfExt, 0.01f);

			hkVector4 shift; shift.setZero4();
			shift((i+2)%3) = carDim((i+2)%3);

			hkVector4 transl[2]; 
			transl[0].setAdd4(carDim, shift);
			transl[1].setSub4(carDim, shift);
			
			shapes.pushBack(new hkpConvexTranslateShape(box, transl[0]));
			shapes.pushBack(new hkpConvexTranslateShape(box, transl[1]));
			box->removeReference();
		}
	}

	hkpListShape* listShape = new hkpListShape(shapes.begin(), shapes.getSize());

	for (int s = 0; s < shapes.getSize(); s++)
	{
		shapes[s]->removeReference();
	}

	///////////////////////////////////////

	
	// Construct MOPP
	hkpMoppCompilerInput moppInput;
	moppInput.setAbsoluteFitToleranceOfTriangles(0.01f);
	hkpMoppCode* code = hkpMoppUtility::buildCode(listShape, moppInput);		

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(listShape, code);
		// Remove references since the MoppBvTreeShape now "owns" the listShape and code
	code->removeReference();


	// To illustrate using the shape, create a rigid body by first defining a template.
	hkpRigidBodyCinfo rigidBodyInfo;

	rigidBodyInfo.m_shape = listShape;
	rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
	rigidBodyInfo.m_angularDamping = 0.0f;
	rigidBodyInfo.m_linearDamping = 0.0f;

	// If we set this to true, the body is fixed, and no mass properties need to be computed.
	rigidBodyInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

	// If we need to compute mass properties, we'll do this using the hkpInertiaTensorComputer.
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(listShape, 1000.0f, rigidBodyInfo);
	rigidBodyInfo.m_friction = 1.0f;
	rigidBodyInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		
	
	// Create a rigid body (using the template above).
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

	// disable rotation around the other axis
	rigidBody->getMotion()->m_inertiaAndMassInv(0) = 0.0f;
	rigidBody->getMotion()->m_inertiaAndMassInv(1) = 0.0f;

	// Remove reference since the body now "owns" the Shape.
	moppShape->removeReference();
	listShape->removeReference();
	return rigidBody;
}



hkpRigidBody* CentrifugeDemo::createABall()
{
	hkpShape* shape = new hkpSphereShape(0.15f);
	hkpRigidBodyCinfo info;
	info.m_shape = shape;
	info.m_motionType = hkpMotion::MOTION_DYNAMIC;
	info.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;

	hkpInertiaTensorComputer::setShapeVolumeMassProperties( shape, 1.1f, info );
	info.m_allowedPenetrationDepth = 0.0001f;
	info.m_position = hkVector4(0.6f, 0.5f, 0.5f);

	hkpRigidBody* body = new hkpRigidBody(info);
	shape->removeReference();
	return body;
}


CentrifugeDemo::CentrifugeDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	enableDisplayingToiInformation(true);

	const CentrifugeVariant& variant =  g_variants[m_variantId];

	//
	//	Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, 0.0f, -10.0f );
		if ( variant.m_usePredictive )
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS;
		}
		else
		{
			info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
		}
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}


	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 5.0f, 3.5f);
		hkVector4 to(0.0f, 0.0f, 1.0f);
		hkVector4 up(0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );

		setupGraphics();
	}

	m_centrifuge  = createHollowBox();
	m_centrifuge->setAllowedPenetrationDepth( variant.m_allowsPenetration );
	m_world->addEntity(m_centrifuge );
	HK_SET_OBJECT_COLOR((hkUlong)m_centrifuge ->getCollidable(), hkColor::rgbFromChars(255, 255, 255, 50));


	m_sphere = createABall();
	m_sphere->setAllowedPenetrationDepth( variant.m_allowsPenetration );
	m_world->addEntity(m_sphere);

	m_world->unlock();
}




CentrifugeDemo::~CentrifugeDemo()
{
	m_world->lock();

	m_centrifuge->removeReference();
	m_sphere->removeReference();
	m_world->unlock();
}



hkDemo::Result CentrifugeDemo::stepDemo()
{
	hkReal physicsDeltaTime = m_timestep;

	{
		m_world->lock();

		hkReal vel = m_world->getGravity()(2) * -physicsDeltaTime;

		m_centrifuge->getRigidMotion()->m_linearVelocity(2) = vel;
		m_centrifuge->getRigidMotion()->m_angularVelocity(0) = 0.0f;
		m_centrifuge->getRigidMotion()->m_angularVelocity(1) = 0.0f;

		hkVector4 angularImp(0,0,0.02f * m_centrifuge->getMass());
		m_centrifuge->applyAngularImpulse( angularImp );

		m_world->unlock();
	}

	hkDefaultPhysicsDemo::stepDemo( );

	{
		m_world->lock();

		bool failed = true;
		// check the distance between centrifuge and ball
		{
			hkVector4 diff; diff.setSub4( m_centrifuge->getPosition(), m_sphere->getPosition());
			hkReal dist = diff.length3();
			if ( dist < 10.0f )
			{
				failed = false;
				m_angularVelocityBallInside = m_centrifuge->getAngularVelocity()(2);
			}
		}

		char buffer[256];
		if ( failed )
		{
			hkString::sprintf( buffer, "Centrifuge failed at angular velocity:%f", m_angularVelocityBallInside );
		}
		else
		{
			hkString::sprintf( buffer, "Centrifuge works at angular velocity:%f", m_angularVelocityBallInside );
		}

		m_env->m_textDisplay->outputText( buffer, 100,100, 0xffffff00 );

		m_world->unlock();
	}

	return DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( CentrifugeDemo, HK_DEMO_TYPE_PRIME, CentrifugeVariant, g_variants, HK_NULL); 

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

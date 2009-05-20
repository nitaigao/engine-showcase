/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/ContinuousPhysics/RagdollVsMopp/RagdollVsMoppDemo.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpRigidBodyInertiaViewer.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>


#include <Graphics/Common/Window/hkgViewport.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Common/Visualize/hkDebugDisplay.h>


static ragdollVsMoppDemo::Parameter Variants[] =
{
#define DISC hkpWorldCinfo::SIMULATION_TYPE_DISCRETE
#define CONT  hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS
#define MED4 hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM
#define MED8 hkpWorldCinfo::SOLVER_TYPE_8ITERS_MEDIUM
	//
	//	**************************************  SimType, frameHz,physicsHz, solverIterations,				InertiaFactor
	//
	{   "60Hz Discrete RealTime", DISC,  60, 60,   MED4, 1.f,  "A ragdoll hitting the room simulated at 60Hz" },
	{   "30Hz Discrete RealTime", DISC,  60, 30,   MED8, 1.5f,	"At 30Hz the ragdoll still works well\nWe also increased the inertia of all limbs " \
																"However using the mouse you can get limbs tunnel through the wall" },
	{   "15Hz Discrete RealTime", DISC,  60, 15,   MED8, 3.f,	"At 15Hz the ragdoll stops working properly without backstepping" },
	{   "10Hz Discrete RealTime", DISC,  60, 10,   MED8, 3.f,  "10Hz is worse than 15 Hz" },
	{   "15Hz Discrete 4xSlower", DISC, 240, 15,   MED8, 3.f,  "The 15Hz version in 4x slow motion" },
	{   "30Hz Continuous RealTime",CONT,   60, 30,  MED8, 1.5f,	"30Hz continuous is fine (1.5 * increased inertia for the limbs" },
	{   "15Hz Continuous RealTime",CONT,   60, 15,  MED8, 3.f,  "15Hz continuous is ok (5 * increased inertia for the limbs" },
	{   "10Hz Continuous RealTime",CONT,   60, 10,  MED8, 5.f,	"10Hz serious artefacts, but doesn't break  (5 * increased inertia for the limbs" },
	{   "7 Hz Continuous RealTime", CONT,   60, 7,  MED8, 6.f,	"7Hz serious artefacts, but doesn't break  (6 * increased inertia for the limbs" },
	{   "5 Hz Continuous RealTime", CONT,   60, 5,  MED8, 8.f, 	"5Hz serious artefacts, but doesn't break  (8 * increased inertia for the limbs" },
};

ragdollVsMoppDemo::ragdollVsMoppDemo(hkDemoEnvironment* env): hkDefaultPhysicsDemo( env )
{
	// Disable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, false); //'This utility is intended primarily for Havok demo use. If you wish to step the world asynchronously,...'

	//
	//	Create the world
	//
	ragdollVsMoppDemo::Parameter& params = Variants[ env->m_variantId ];
	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, -10.0f, 0.0f );
		info.m_simulationType = params.m_simulationType;
		info.m_collisionTolerance = 0.1f;
		info.m_contactPointGeneration = info.CONTACT_POINT_REJECT_MANY;
		info.setupSolverInfo( params.m_solverType );

		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Collision Filter
	//
	{ 
		m_filter = new hkpGroupFilter();
		hkpGroupFilterSetup::setupGroupFilter( m_filter );
		m_world->setCollisionFilter(m_filter);
	}

	///[createragdoll_text]
	/// The ragdoll in this demo are created using the createRagdoll function from the 'GameUtils'
	/// utility class. This function takes a height and constructs rigid bodies and constraints
	/// representing limbs and joints in proportion for a standard humanoid biped of that size.
	{
		m_ragdollType = GameUtils::RPT_CONVEX;

		hkVector4		position = hkVector4::getZero();
		hkQuaternion	rotation; rotation.setAxisAngle( hkVector4( 1, 0, 0 ), -0.5f * HK_REAL_PI );

		int systemGroup = m_filter->getNewSystemGroup();
		m_ragdoll = GameUtils::createRagdoll( 2.0f, position, rotation, systemGroup, GameUtils::RagdollPartType(m_ragdollType) );

		hkpGroupFilterUtil::disableCollisionsBetweenConstraintBodies( m_ragdoll->getConstraints().begin(), m_ragdoll->getConstraints().getSize(), systemGroup );

		addRagdoll( m_world, m_ragdoll, params.m_inertiaFactor );
	}

	// Create MOPP room
	{
		hkVector4 scale(1.0f, 1.0f, 1.0f);
		hkpShape* moppShape = GameUtils::loadTK2MOPP("Resources/Physics/Tk/RagdollVsMopp/room.tk" ,scale, 0.015f);
		{
			hkpRigidBodyCinfo rinfo;
			rinfo.m_motionType = hkpMotion::MOTION_FIXED;

			rinfo.m_shape = moppShape;
			hkpRigidBody* room = new hkpRigidBody(rinfo);
			m_world->addEntity(room);
			room->removeReference();
		}
		moppShape->removeReference();
	}

	//
    // create boxes/tables
	//
	if( 0 )
	{
		hkVector4 size(.25f,.25f,.25f);
		hkpShape* shape = new hkpBoxShape(size, 0.01f );


		hkpRigidBodyCinfo rinfo;
		rinfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		rinfo.m_shape = shape;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties( shape, 20.0f, rinfo );
		rinfo.m_position.set(-3.0f, -1.0f,0.0f);
		rinfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
		rinfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( hkpGroupFilterSetup::LAYER_DYNAMIC );
		rinfo.m_allowedPenetrationDepth = 0.1f;

		for( int i = 0; i < 3; i++ )
		{
			hkpRigidBody* box = new hkpRigidBody(rinfo);
			m_world->addEntity(box)->removeReference();
			rinfo.m_position(1) += 0.5f;
		}
		shape->removeReference();
	}

	//
	// Setup the camera
	//
	{
		hkVector4 from(-5.0f, 2.0f, 4.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up, 1.f, 1000.0f );
		setupGraphics();
	}

	setTransformAndVelocityFromCamera( m_ragdoll );

	m_world->unlock();
}


ragdollVsMoppDemo::~ragdollVsMoppDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0xafe97523, true); 

	m_world->lock();

	if( m_ragdoll )
	{
		m_ragdoll->removeReference();
		m_ragdoll = HK_NULL;
	}
	m_filter->removeReference();

	m_world->unlock();
}


void ragdollVsMoppDemo::setTransformAndVelocity( const hkTransform& tform, const hkVector4& velocity, hkpPhysicsSystem* ragdoll )
{
	// re-position ragdoll
	// set collision information
	{
		hkVector4 ragdollPosition = tform.getTranslation();
		hkQuaternion ragdollRotation = hkQuaternion( tform.getRotation() );
		for( int i = ragdoll->getRigidBodies().getSize()-1; i >= 0; i-- )
		{
			hkpRigidBody* rbRoot	= ragdoll->getRigidBodies()[0];
			hkpRigidBody* rb		= ragdoll->getRigidBodies()[i];
	
            hkVector4 position;
			position.setSub4( rb->getPosition(), rbRoot->getPosition() );
			position.setRotatedDir( ragdollRotation, position );
			position.add4( ragdollPosition );

			rb->setPosition( position );

			hkQuaternion rotation;
			rotation.setMul( ragdollRotation, rb->getRotation() );
			rb->setRotation( rotation );

			rb->setLinearVelocity( velocity );
		}
	}
}

void ragdollVsMoppDemo::setTransformAndVelocityFromCamera(hkpPhysicsSystem* ragdoll)
{
	hkgViewport* v1 = m_env->m_window->getViewport(0);
    hkVector4 to;
    hkVector4 from;
 	v1->getCamera()->getFrom( &from(0) );
	v1->getCamera()->getTo( &to(0) );

    hkVector4 dir;
	dir.setSub4(to,from);
	dir.normalize3();

	// add a little extra up component
	dir.addMul4( -0.02f, m_world->getGravity() );
	dir.normalize3();


	hkVector4 offset = dir;
	offset.mul4(2.0f);
	dir.mul4(8.0f);
	from.add4(offset);

	hkQuaternion	rotation; rotation.setIdentity();

	hkTransform tform;
    tform.setIdentity();
	tform.setTranslation(from);
	tform.setRotation( rotation );

	setTransformAndVelocity( tform, dir, ragdoll );
}


hkDemo::Result ragdollVsMoppDemo::stepDemo()
{
	m_world->lock();

	const hkgPad* pad = m_env->m_gamePad;

	if( pad->wasButtonPressed( HKG_PAD_BUTTON_1 ) )
	{
		hkTransform tform;
	    hkVector4 position( -5.5f, 4.5f, -3.75f );
	    tform.setIdentity();
	    tform.setTranslation( position );
		setTransformAndVelocity( tform, hkVector4::getZero(), m_ragdoll );
	}

	if( pad->wasButtonPressed( HKG_PAD_BUTTON_2 ) )
	{
        hkTransform tform;
	    hkVector4 position( -0.5f, 4.5f, 1.5f );
	    tform.setIdentity();
	    tform.setTranslation( position );
		setTransformAndVelocity( tform, hkVector4::getZero(), m_ragdoll );
	}

	if( pad->wasButtonPressed( HKG_PAD_BUTTON_3 ) )
	{
        hkTransform tform;
	    hkVector4 position( -4.75f, 2.75f, 1.5f );
	    tform.setIdentity();
	    tform.setTranslation( position );
		setTransformAndVelocity( tform, hkVector4::getZero(), m_ragdoll );
	}

	const	int	numRagdollTypes = GameUtils::RPT_COUNT;
	hkBool		recreateRagdoll = false;

	if( pad->wasButtonPressed( HKG_PAD_DPAD_UP ) )
	{
		setTransformAndVelocityFromCamera( m_ragdoll );
	}

	if( pad->wasButtonPressed( HKG_PAD_DPAD_DOWN ) )
	{
		recreateRagdoll = true;
	}

	if( pad->wasButtonPressed( HKG_PAD_DPAD_LEFT ) )
	{
		recreateRagdoll = true;
		m_ragdollType = (--m_ragdollType + numRagdollTypes) % numRagdollTypes;
	}

	if( pad->wasButtonPressed( HKG_PAD_DPAD_RIGHT ) )
	{
		recreateRagdoll = true;
		m_ragdollType = (++m_ragdollType + numRagdollTypes) % numRagdollTypes;
	}

	ragdollVsMoppDemo::Parameter& params = Variants[ m_env->m_variantId ];

	if( recreateRagdoll )
	{
		removeRagdoll(); 
		m_ragdoll->removeReference();
		hkVector4		position = hkVector4::getZero();
		hkQuaternion	rotation; rotation.setAxisAngle( hkVector4( 1, 0, 0 ), -0.5f * HK_REAL_PI );
		int systemGroup = m_filter->getNewSystemGroup();
		m_ragdoll = GameUtils::createRagdoll( 2.0f, position, rotation, systemGroup, GameUtils::RagdollPartType(m_ragdollType) );
		hkpGroupFilterUtil::disableCollisionsBetweenConstraintBodies( m_ragdoll->getConstraints().begin(), m_ragdoll->getConstraints().getSize(), systemGroup );
		addRagdoll(m_world, m_ragdoll, params.m_inertiaFactor);
		setTransformAndVelocityFromCamera( m_ragdoll );
	}

	m_world->unlock();

	hkDefaultPhysicsDemo::stepAsynchronously(m_world, 1.0f / params.m_frameFrequency, 1.0f/params.m_physicsFrequency );
	return DEMO_OK;
}

void ragdollVsMoppDemo::addRagdoll( hkpWorld* world, hkpPhysicsSystem* ragdoll, hkReal inertiaFactor)
{
	///[addragdoll_text]
	/// Rigid bodies and constraints stored within an hkRagdoll object
	/// can be added to Havok world as follows:
	/// We also increase the relative inertia tensor by decreasing the mass
	///
	const hkArray<hkpRigidBody*>& rigidbodies = ragdoll->getRigidBodies();
	for( int iRB = 0; iRB < rigidbodies.getSize(); iRB++ )
	{
		hkpRigidBody* rb = rigidbodies[iRB];
		rb->setQualityType( HK_COLLIDABLE_QUALITY_CRITICAL );
		hkReal massInv = rb->getMassInv();
		rb->setMassInv( massInv * inertiaFactor );
	}
	const hkArray<hkpConstraintInstance*>& constraints = ragdoll->getConstraints();
	for( int iC = 0; iC < constraints.getSize(); iC++ )
	{
		constraints[iC]->setPriority( hkpConstraintInstance::PRIORITY_TOI );
	}
	///>
	/// Note that the entities must be added before the constraints.

	world->addPhysicsSystem(ragdoll);
}


//
// removes the bodies of the ragdoll from world
//
void ragdollVsMoppDemo::removeRagdoll()
{
	///[removeragdoll_text]
	/// Rigid bodies and constraints stored within an hkRagdoll object
	/// can be removed from a Havok world as follows:
	///
	m_world->removePhysicsSystem(m_ragdoll);
	
	///>
	/// Note that we do not need to explicitly remove the constraints.
	/// They will be removed from the world automatically when any of their
	/// associated rigid bodies are removed.
}




#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows a single 11 bone ragdoll colliding again a room made " \
"from a concave mopp geometry. ";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ragdollVsMoppDemo, HK_DEMO_TYPE_PHYSICS, ragdollVsMoppDemo::Parameter, Variants, HK_NULL );

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

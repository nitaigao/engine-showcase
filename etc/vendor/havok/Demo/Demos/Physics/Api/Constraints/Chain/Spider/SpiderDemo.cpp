/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Common/Base/Math/Vector/hkVector4Util.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Physics/Dynamics/Constraint/Chain/Powered/hkpPoweredChainData.h>
#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/Api/Constraints/Chain/Spider/SpiderDemo.h>


struct SpiderVariant
{
	const char*	 m_name;
	hkInt32 m_numLegs;
	const char* m_details;
};


static const SpiderVariant g_variants[] =
{
	{ "6 Leg Spider", 6,	"Use arrow keys to steer" },
	{ "8 Leg Spider", 8,	"Use arrow keys to steer" },
	{ "50 Leg Centipede", 50,	"Use arrow keys to steer" },
};



SpiderDemo::SpiderDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	m_time = 0.0f;
	m_velocity = 1.0f;
	m_leftFactor = 1.0f;
	m_rightFactor = 1.0f;

	const SpiderVariant& variant = g_variants[env->m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 3.0f, -10.0f);
		hkVector4 to  (0.0f, 0.0f,  -0.0f);
		hkVector4 up  (0.0f, 1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up, .5f, 100.0f );
	}

	//
	// Create the world
	//
	hkpGroupFilter* filter;
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_8ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.set(0.0f, -19.81f, 0.0f);

		m_world = new hkpWorld( info );
		m_world->lock();

		m_world->m_wantDeactivation = false;

		filter = new hkpGroupFilter();
		m_world->setCollisionFilter( filter );
		filter->removeReference();


		//
		// Create constraint viewer
		//
		setupGraphics();
	}

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	// floor
	{
		hkpRigidBodyCinfo info;
		info.m_shape = new hkpBoxShape( hkVector4(50.0f, 1.0f, 50.0f) );
		info.m_position.set( 0, -2.0f,.0f );
		info.m_friction = 1.0f;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		hkpRigidBody* body = new hkpRigidBody(info);

		m_world->addEntity(body);

		info.m_shape->removeReference();
		body->removeReference();
	}

	// little spider
	{
		if (variant.m_numLegs == 6)
		{
			m_spiderLayout.m_bodyHalfExtents.set( 0.5f, 0.2f, 0.3f );
			m_spiderLayout.m_legHalfExtents.set( 2.0f, 1.0f, 2.0f );
			m_spiderLayout.m_numLegs = 3;
		}
		else if (variant.m_numLegs == 8)
		{
			m_spiderLayout.m_bodyHalfExtents.set( 0.8f, 0.2f, 0.3f );
			m_spiderLayout.m_legHalfExtents.set( 2.2f, 1.0f, 2.0f );
			m_spiderLayout.m_numLegs = 4;
		}
		else
		{
			m_spiderLayout.m_bodyHalfExtents.set( 5.5f, 0.2f, 0.7f );
			m_spiderLayout.m_legHalfExtents.set(  6.0f, 1.0f, 2.0f );
			m_spiderLayout.m_numLegs = 20;
		}
		m_spiderLayout.m_filterInfo = hkpGroupFilter::calcFilterInfo( 0, filter->getNewSystemGroup() );
		createSpider( m_world, m_spiderLayout );
	}

	m_world->unlock();
}


	// little ik
void SpiderDemo::doLegIk( const SpiderDemo::calcLegMatrizesIn& in, hkVector4& pivotOut, hkRotation& mAOut, hkRotation& mBOut )
{
	hkVector4 dir; dir.setSub4( in.m_to, in.m_from );
	hkReal tdist = dir.length3();
	if ( tdist >= in.m_lenA + in.m_lenB )
	{
		hkVector4Util::buildOrthonormal( dir, in.m_up, mAOut );
		mBOut = mAOut;
		dir.normalize3();
		pivotOut.setAddMul4( in.m_from, dir, in.m_lenA );
		return;
	}
	dir.normalize3();
	hkReal la2 = in.m_lenA*in.m_lenA;
	hkReal lb2 = in.m_lenB*in.m_lenB;
	hkReal adist = 0.5f * (la2- lb2 + tdist*tdist) / tdist;
	hkReal hdist = hkMath::sqrt( la2 - adist*adist );

	hkVector4 cup;
	{
		hkVector4 h; h.setCross( dir, in.m_up );
		cup.setCross( h, dir );
		cup.normalize3();
	}

	{
		pivotOut.setAddMul4( in.m_from, dir, adist );
		pivotOut.addMul4( hdist, cup );
	}

	{
		hkVector4 d; d.setSub4( pivotOut, in.m_from );
		d.normalize3();
		hkVector4Util::buildOrthonormal( d, in.m_up, mAOut );
	}
	{
		hkVector4 d; d.setSub4( in.m_to, pivotOut );
		d.normalize3();
		hkVector4Util::buildOrthonormal( d, in.m_up, mBOut );
	}
}

static hkpRigidBody* createCapsule( hkRotation& rot, const hkVector4& aWs, const hkVector4& bWs, int filterInfo, hkReal radius )
{
	hkVector4 center; center.setInterpolate4( aWs, bWs, 0.5f );
	hkVector4 a; a.setSub4( aWs, center );
	hkVector4 b; b.setSub4( bWs, center );

	a.setRotatedInverseDir( rot, a );
	b.setRotatedInverseDir( rot, b );

	hkpRigidBodyCinfo ci;
	ci.m_shape = new hkpCapsuleShape( a, b, radius );
	ci.m_friction = 2.0f;
	ci.m_restitution = 0.0f;
	ci.m_collisionFilterInfo = filterInfo;
	ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
	ci.m_position = center;
	ci.m_rotation.set( rot );

	hkpInertiaTensorComputer::setShapeVolumeMassProperties( ci.m_shape, 1.0f, ci );

	hkpRigidBody* body = new hkpRigidBody( ci );

	ci.m_shape->removeReference();

	return body;
}


void SpiderDemo::buildLeg( hkpWorld* world, hkpRigidBody* rootBody, const SpiderDemo::calcLegMatrizesIn& in, int filterInfo,  Leg& legOut )
{
	hkVector4 pivot;
	hkRotation mA;
	hkRotation mB;
	doLegIk( in, pivot, mA, mB );

	const hkReal radius = .1f;

	hkpRigidBody* bodyA = createCapsule( mA, in.m_from, pivot, filterInfo, radius );
	hkpRigidBody* bodyB = createCapsule( mB, pivot, in.m_to, filterInfo, radius );

	world->addEntity( bodyA )->removeReference();
	world->addEntity( bodyB )->removeReference();

	legOut.m_lenA = in.m_lenA;
	legOut.m_lenB = in.m_lenB;
	legOut.m_constraint = HK_NULL;

	hkpConstraintChainInstance* chainInstance;
	{
		//CONSTRAINT_6D
		hkpPoweredChainData* chainData = new hkpPoweredChainData();
		chainInstance = new hkpConstraintChainInstance( chainData );

		hkpPositionConstraintMotor* strongMotor = new hkpPositionConstraintMotor();
		{
			strongMotor->m_tau = 1.0f;
			strongMotor->m_maxForce = 150000.0f;
			strongMotor->m_damping = 1.0f;
			strongMotor->m_constantRecoveryVelocity = 1.0f;
			strongMotor->m_proportionalRecoveryVelocity = 1.1f;
		}
		hkpPositionConstraintMotor* weakMotor = new hkpPositionConstraintMotor();
		{
			weakMotor->m_tau = 1.0f;
			weakMotor->m_maxForce = 550.0f;
			weakMotor->m_damping = 1.0f;
			weakMotor->m_constantRecoveryVelocity = 1.0f;
			weakMotor->m_proportionalRecoveryVelocity = 1.1f;
		}

		chainInstance->addEntity( rootBody );

		{
			hkVector4 pivotinA; pivotinA.setTransformedInversePos( rootBody->getTransform(), in.m_from );
			hkVector4 pivotinB; pivotinB.setTransformedInversePos( bodyA->getTransform(),    in.m_from );

			hkQuaternion aTw; aTw.setInverse( rootBody->getRotation() );
			hkQuaternion aTb; aTb.setMul( aTw, bodyA->getRotation() );

			chainData->addConstraintInfoInBodySpace( pivotinA, pivotinB, aTb, strongMotor, strongMotor, weakMotor );
			chainInstance->addEntity( bodyA );
		}
		{
			hkVector4 pivotinA; pivotinA.setTransformedInversePos( bodyA->getTransform(), pivot );
			hkVector4 pivotinB; pivotinB.setTransformedInversePos( bodyB->getTransform(), pivot );

			hkQuaternion aTw; aTw.setInverse( bodyA->getRotation() );
			hkQuaternion aTb; aTb.setMul( aTw, bodyB->getRotation() );

			chainData->addConstraintInfoInBodySpace( pivotinA, pivotinB, aTb, strongMotor, strongMotor, strongMotor );

			chainInstance->addEntity( bodyB );
		}
		strongMotor->removeReference();
		weakMotor->removeReference();
		chainData->removeReference();
	}
	world->addConstraint( chainInstance );
	legOut.m_constraint = chainInstance;
}


void SpiderDemo::createSpider( hkpWorld* world, SpiderLayout& sl )
{
	hkVector4 center( 0, 1, 0 );

	// center body
	hkpRigidBody* rootBody;
	{
		hkpRigidBodyCinfo ci;
		ci.m_shape = new hkpBoxShape( sl.m_bodyHalfExtents, 0.0f );
		ci.m_friction = 2.0f;
		ci.m_restitution = 0.0f;
		ci.m_collisionFilterInfo = sl.m_filterInfo;
		ci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		ci.m_position = center;
		ci.m_angularDamping = 1.0f;

		hkReal mass = 30.0f;
		hkReal inertiaFactor = 1.0f;
		hkpMassProperties mp;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties( sl.m_legHalfExtents, mass * inertiaFactor, mp );
		ci.m_mass = mass;
		ci.m_inertiaTensor = mp.m_inertiaTensor;

		rootBody = new hkpRigidBody( ci );
		world->addEntity( rootBody );
		rootBody->removeReference();
		ci.m_shape->removeReference();
	}

	// legs
	{
		for (int leg = 0; leg < sl.m_numLegs; leg++ )
		{
			hkReal xf = leg/hkReal(sl.m_numLegs-1)*2.0f - 1.0f;
			for (int z = -1; z <= 1; z+=2 )
			{
				SpiderDemo::calcLegMatrizesIn m;
				m.m_from = center;
				m.m_from(0) += xf * sl.m_bodyHalfExtents(0);
				m.m_from(2) += z  * sl.m_bodyHalfExtents(2);

				m.m_to = center;
				m.m_to(0) += xf * sl.m_legHalfExtents(0);
				m.m_to(1) -= sl.m_legHalfExtents(1);
				m.m_to(2) += z * sl.m_legHalfExtents(2);
				m.m_up.set(0,1,0);
				hkVector4 diff; diff.setSub4( m.m_to, m.m_from );
				hkReal len = diff.length3();
				m.m_lenA = 0.6f * len;
				m.m_lenB = 0.8f * len;
				buildLeg( world, rootBody, m, sl.m_filterInfo, m_legs.expandOne() );
			}
		}
	}
}

void SpiderDemo::moveLeg( SpiderLayout& sl, SpiderDemo::Leg& leg, hkReal z, hkReal xf, hkReal time, hkReal vel )
{
	static hkReal xControl[] = {  1.f,  0.6f, -0.6f,  -1.0f, 1.f };
	static hkReal yControl[] = {  1.f,  0.0f,  0.0f,   1.0f, 1.f };
	static hkReal tControl[] = {  0.f,  0.16f, 0.64f,  0.8f, 1.f };

	hkReal yOffset;
	hkReal xOffset;
	{
		time += xf * 0.5f + z * 0.25f;
		hkReal t = time - int(time);
		HK_ASSERT( 0, t >=-1.0 && t <=1.0f );
		if ( t < 0.0f )
		{
			t += 1.0f;
		}

		int i;
		const int num = sizeof(tControl)/sizeof(hkReal);
		for (i=0; i < num-1; i++)
		{
			if ( t < tControl[i])
			{
				break;
			}
		}
		int in = i;
		i--;
		hkReal f = (t - tControl[i])/ (tControl[in]- tControl[i]);
		xOffset = xControl[in] * f + (1-f) * xControl[i];
		yOffset = yControl[in] * f + (1-f) * yControl[i];

		xOffset *= 1.1f * vel;
		yOffset *= 0.3f;
	}

	// this is a copy of create spider
	// we calculate all the relative matrizes in the root object space
	hkVector4 center; center.setZero4();

	SpiderDemo::calcLegMatrizesIn m;
	m.m_from = center;
	m.m_from(0) += xf * sl.m_bodyHalfExtents(0);
	m.m_from(2) += z  * sl.m_bodyHalfExtents(2);

	m.m_to = center;
	m.m_to(0) += xOffset + xf * sl.m_legHalfExtents(0);
	m.m_to(1) += yOffset - sl.m_legHalfExtents(1);
	m.m_to(2) += z * sl.m_legHalfExtents(2);
	m.m_up.set(0,1,0);

	m.m_lenA = leg.m_lenA;
	m.m_lenB = leg.m_lenB;

	hkVector4 pivot;
	hkRotation mA;
	hkRotation mB;
	doLegIk( m, pivot, mA, mB );

	hkpPoweredChainData* data = static_cast<hkpPoweredChainData*>(leg.m_constraint->getData());
	{
		data->m_infos[0].m_aTc.set(mA);
	}
	{
		hkRotation invMa; invMa.setTranspose(mA);
		hkRotation aTb; aTb.setMul( invMa, mB );
		data->m_infos[1].m_aTc.set( aTb );
	}
}

void SpiderDemo::moveSpider( SpiderLayout& sl, hkReal time, hkReal leftVel, hkReal rightVel )
{
	int legId = 0;
	for (int posId = 0; posId < sl.m_numLegs; posId++ )
	{
		hkReal xf = posId/hkReal(sl.m_numLegs-1)*2.0f - 1.0f;
		hkReal vel = leftVel;
		for (int z = -1; z <= 1; z+=2 )
		{
			Leg& leg = m_legs[legId++];
			moveLeg( sl, leg, hkReal(z), xf, time, vel );
			vel = rightVel;
		}
	}
}


SpiderDemo::~SpiderDemo()
{
	m_world->lock();

	for (int i=0; i < m_legs.getSize(); i++ )
	{
		m_legs[i].m_constraint->removeReference();
	}

	m_world->unlock();
}

hkDemo::Result SpiderDemo::stepDemo()
{
	m_world->lock();

	hkReal dt = 0.016f;

	if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_UP) )
	{
		m_velocity += 2.0f * dt;
	}
	if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_DOWN) )
	{
		m_velocity += -2.0f * dt;
	}

	m_leftFactor += dt * 2.0f;
	m_rightFactor += dt * 2.0f;

	if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_RIGHT) )
	{
		m_rightFactor -= dt * 4.0f;
	}
	if( m_env->m_gamePad->isButtonPressed(HKG_PAD_DPAD_LEFT) )
	{
		m_leftFactor -= dt * 4.0f;
	}

	hkReal maxVel = 8.0f;
	m_velocity = hkMath::clamp( m_velocity, -maxVel, maxVel );
	m_leftFactor = hkMath::clamp( m_leftFactor, -0.5f, 1.f );
	m_rightFactor = hkMath::clamp( m_rightFactor, -0.5f, 1.f );

	moveSpider( m_spiderLayout, m_time, m_leftFactor, m_rightFactor );
	m_time += dt * m_velocity;

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}


////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "A fast spider";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( SpiderDemo, HK_DEMO_TYPE_PRIME, SpiderVariant, g_variants, helpString );

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

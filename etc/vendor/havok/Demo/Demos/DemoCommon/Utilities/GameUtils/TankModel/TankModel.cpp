/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiDemo.h>
#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>

// Need this to generate plane equations for hkpConvexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>


#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>

#include <Demos/DemoCommon/Utilities/GameUtils/TankModel/TankModel.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/BallAndSocket/hkpBallAndSocketConstraintData.h>
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>
#include <Physics/Dynamics/Constraint/Motor/Position/hkpPositionConstraintMotor.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Common/Visualize/hkDebugDisplay.h>
 
static hkpShape* createConvexVertsShape( const hkVector4* points, int numPoints, const hkVector4* shift, int numShifts )
{
	hkLocalBuffer<hkVector4> verts( numPoints * numShifts );
	for (int s = 0; s < numShifts; s++ )
	{
		for (int i = 0; i < numPoints; i++)
		{
			verts[ s * numPoints + i ].setAdd4( points[i], shift[s] );
		}
	}
	//
	// SHAPE CONSTRUCTION.
	//
	hkpConvexVerticesShape* shape;
	hkArray<hkVector4> planeEquations;
	hkGeometry geom;
	{
		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = numPoints * numShifts;
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &verts[0](0);
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
	}

	shape->setRadius(0.05f);
	return shape;
}

#define s 0.5f

static void initCamera( hkp1dAngularFollowCam& camera )
{
	hkp1dAngularFollowCamCinfo cinfo;

	cinfo.m_yawSignCorrection = 1.0f; 
	cinfo.m_upDirWS.set(0.0f, 1.0f, 0.0f); 
	cinfo.m_rigidBodyForwardDir.set(1.0f, 0.0f, 0.0f); 

	cinfo.m_set[0].m_velocity = 10.0f;
	cinfo.m_set[1].m_velocity = 50.0f;
	cinfo.m_set[0].m_speedInfluenceOnCameraDirection = 0.0f;
	cinfo.m_set[1].m_speedInfluenceOnCameraDirection = 0.0f;
	cinfo.m_set[0].m_angularRelaxation = 10.5f;
	cinfo.m_set[1].m_angularRelaxation = 10.5f;

	// The two camera positions ("slow" and "fast" rest positions) are both the same here,
	// -6 units behind the chassis, and 2 units above it. Again, this is dependent on 
	// m_chassisCoordinateSystem.
	cinfo.m_set[0].m_positionUS.set( -16.0f, 1.5f, 0.0f); 
	cinfo.m_set[1].m_positionUS.set( -16.0f, 1.5f, 0.0f); 
	cinfo.m_set[0].m_positionUS.mul4(s);
	cinfo.m_set[1].m_positionUS.mul4(s);

	cinfo.m_set[0].m_lookAtUS.set ( 2.0f, 1.0f, 0.0f );
	cinfo.m_set[1].m_lookAtUS.set ( 2.0f, 1.0f, 0.0f );
	cinfo.m_set[0].m_lookAtUS.mul4(s);
	cinfo.m_set[1].m_lookAtUS.mul4(s);

	cinfo.m_set[0].m_fov = 60.0f;
	cinfo.m_set[1].m_fov = 60.0f;

	camera.reinitialize( cinfo );
}

TankModel::TankModel( hkpWorld* world, const hkVector4& position, hkReal mass )
:	m_skiL(HK_NULL),
	m_skiR(HK_NULL),
	m_body(HK_NULL),
	m_tower(HK_NULL),
	m_gun(HK_NULL),
	m_left(HK_NULL),
	m_right(HK_NULL)
{
	m_currentVelocity.setZero4();
	m_directionAngle = 0.0f;
	m_lastAngle = 0.0f;
	m_rotations = 0.0f;
	m_upAngle = 0.0f;

	hkpShape* skiShape;
	{
		const HK_ALIGN16( hkReal vertices[] ) = 
		{
			-0.5f*s, -0.01f*s, 0.0f, 0.0f,
			-2.0f*s, 0.0f*s, 0.0f, 0.0f,
			-4.0f*s, 0.9f*s, 0.0f, 0.0f,
			-3.9f*s, 1.0f*s, 0.0f, 0.0f,
			 3.9f*s, 1.0f*s, 0.0f, 0.0f,
			 4.0f*s, 0.9f*s, 0.0f, 0.0f,
			 2.0f*s, 0.0f*s, 0.0f, 0.0f,
			 0.5f*s, -0.01f*s, 0.0f, 0.0f,
		};

		const hkVector4 shifts[] = {
			hkVector4( 0, 0, -0.5f*s, 0.0f ),
			hkVector4( 0, 0,  0.5f*s, 0.0f )
		};
		skiShape = createConvexVertsShape( (const hkVector4*)vertices, 8, shifts, 2 );
	}

	hkpShape* bodyShape;
	{
		const HK_ALIGN16( hkReal verts[]) = 
		{
			-1.8f*s,  0.1f*s, 0.0f, 0.0f,
			-2.0f*s,  0.0f*s, 0.0f, 0.0f,
			-1.8f*s, -0.1f*s, 0.0f, 0.0f,
			 1.8f*s, -0.1f*s, 0.0f, 0.0f,
			 2.0f*s,  0.0f*s, 0.0f, 0.0f,
			 1.8f*s,  0.1f*s, 0.0f, 0.0f,
		};

		const hkVector4 shifts[] = {
			hkVector4( 0, 0, -2.5f*s, 0.0f ),
			hkVector4( 0, 0,  2.5f*s, 0.0f )
		};
		bodyShape = createConvexVertsShape( (const hkVector4*)verts, 6, shifts, 2 );
	}

	hkpShape* tower;
	{
		hkVector4 a(0,  .3f*s, 0.0f );
		hkVector4 b(0, -.2f*s, 0.0f );
		tower = new hkpCylinderShape( a, b, 1.7f*s, 0.1f );
	}

	hkpShape* gunShape;
	{
		const HK_ALIGN16( hkReal verts[]) = 
		{
			 5.0f*s,  0.2f*s, 0.0f, 0.0f,
			 6.0f*s,  0.2f*s, 0.0f, 0.0f,
			 5.0f*s, -0.2f*s, 0.0f, 0.0f,
			 -1.0f*s,  0.2f*s, 0.0f, 0.0f,
			 -1.0f*s, -0.2f*s, 0.0f, 0.0f,
		};

		const hkVector4 shifts[] = {
			hkVector4( 0, 0, -0.3f*s, 0.0f ),
			hkVector4( 0, 0,  0.3f*s, 0.0f )
		};
		gunShape = createConvexVertsShape( (const hkVector4*)verts, 5, shifts, 2 );
	}

	hkpRigidBodyCinfo info;
	info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 1, 234 );
	info.m_shape = skiShape;
	info.m_rigidBodyDeactivatorType = hkpRigidBodyDeactivator::DEACTIVATOR_NEVER;
	info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(skiShape, mass * 0.3f, info);

	info.m_position.setAdd4( position, hkVector4( 0, 1.0f*s, -2.5f*s ) );
	m_skiL = new hkpRigidBody(info);

	world->addEntity( m_skiL );

	info.m_position.setAdd4( position, hkVector4( 0, 1.0f*s, 2.5f*s ) );
	m_skiR = new hkpRigidBody(info);
	world->addEntity( m_skiR );

	info.m_position.setAdd4( position, hkVector4( 0, 2.0f*s, 0.0f*s ) );
	info.m_shape = bodyShape;
	info.m_restitution = 0.5f;
	info.m_friction = 20.0f;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(bodyShape, mass * 0.3f, info);
	m_body = new hkpRigidBody(info);
	world->addEntity( m_body );


	info.m_position.setAdd4( position, hkVector4( 0, 2.1f*s, 0.0f ) );
	info.m_shape = tower;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(tower, mass * 0.1f, info);
	m_tower = new hkpRigidBody(info);
	world->addEntity( m_tower );

	info.m_position.setAdd4( position, hkVector4( 0, 2.5f*s, 0.9f*s ) );
	info.m_shape = gunShape;
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(gunShape, mass * 0.1f, info);
	m_gun = new hkpRigidBody(info);
	world->addEntity( m_gun );

	hkReal skiAngle = 0.1f;
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();
		lhc->setInWorldSpace( m_body->getTransform(), m_skiL->getTransform(), m_skiL->getPosition(), hkTransform::getIdentity().getColumn(2) );
		lhc->setMinAngularLimit( -skiAngle );
		lhc->setMaxAngularLimit(  skiAngle );
		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor(  );
		motor->m_maxForce = m_body->getMass() * 10.0f;
		motor->m_tau = 0.05f;
		motor->m_proportionalRecoveryVelocity = 1.0f;
		motor->m_constantRecoveryVelocity = 0.5f;
		lhc->setMotor( motor );
		motor->removeReference();

		hkpConstraintInstance* instance = world->createAndAddConstraintInstance( m_body, m_skiL, lhc );
		lhc->setMotorActive( instance, true );
		instance->removeReference();
		lhc->removeReference();
	}
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();
		lhc->setInWorldSpace( m_body->getTransform(), m_skiR->getTransform(), m_skiR->getPosition(), hkTransform::getIdentity().getColumn(2) );
		lhc->setMinAngularLimit( -skiAngle );
		lhc->setMaxAngularLimit(  skiAngle );
		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor(  );
		motor->m_maxForce = m_body->getMass() * 10.1f;
		motor->m_tau = 0.05f;
		motor->m_proportionalRecoveryVelocity = 1.0f;
		motor->m_constantRecoveryVelocity = 0.5f;
		lhc->setMotor( motor );
		motor->removeReference();

		hkpConstraintInstance* instance = world->createAndAddConstraintInstance( m_body, m_skiR, lhc );
		lhc->setMotorActive( instance, true );
		instance->removeReference();
		lhc->removeReference();
	}
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();
		lhc->setInWorldSpace( m_body->getTransform(), m_tower->getTransform(), m_tower->getPosition(), hkTransform::getIdentity().getColumn(1) );
		lhc->disableLimits();
		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor(  );
		motor->m_maxForce = m_body->getMass() * 100.0f;
		motor->m_tau = 0.1f;
		motor->m_proportionalRecoveryVelocity = 0.5f;
		motor->m_constantRecoveryVelocity = 0.2f;
		m_towerMotor = lhc;
		lhc->setMotor( motor );
		motor->removeReference();

		hkpConstraintInstance* instance = world->createAndAddConstraintInstance( m_body, m_tower, lhc );
		lhc->setMotorActive( instance, true );
		instance->removeReference();
		lhc->removeReference();
	}
	{
		hkpLimitedHingeConstraintData* lhc = new hkpLimitedHingeConstraintData();
		lhc->setInWorldSpace( m_tower->getTransform(), m_gun->getTransform(), m_gun->getPosition(), hkTransform::getIdentity().getColumn(2) );
		lhc->setMinAngularLimit( -.5f );
		lhc->setMaxAngularLimit(  0.2f );

		hkpPositionConstraintMotor* motor = new hkpPositionConstraintMotor(  );
		motor->m_maxForce = m_body->getMass() * 100.1f;
		motor->m_tau = 0.1f;
		motor->m_proportionalRecoveryVelocity = 1.0f;
		motor->m_constantRecoveryVelocity = 0.5f;
		m_gunMotor = lhc;
		lhc->setMotor( motor );
		motor->removeReference();

		hkpConstraintInstance* instance = world->createAndAddConstraintInstance( m_tower, m_gun, lhc );
		lhc->setMotorActive( instance, true );
		instance->removeReference();
		lhc->removeReference();
	}

	m_left  = new hkpSurfaceVelocityUtil( m_skiL, hkVector4::getZero() );
	m_right = new hkpSurfaceVelocityUtil( m_skiR, hkVector4::getZero() );
	gunShape->removeReference();
	tower->removeReference();
	skiShape->removeReference();
	bodyShape->removeReference();

	HK_SET_OBJECT_COLOR( hkUlong(m_skiL->getCollidable()), hkColor::GREY25 );
	HK_SET_OBJECT_COLOR( hkUlong(m_skiR->getCollidable()), hkColor::GREY25 );
	HK_SET_OBJECT_COLOR( hkUlong(m_body->getCollidable()), 0xff4040a0 );
	HK_SET_OBJECT_COLOR( hkUlong(m_tower->getCollidable()), hkColor::GREY25 );
	HK_SET_OBJECT_COLOR( hkUlong(m_gun->getCollidable()), 0xff4040a0 );

	initCamera( m_camera );
}

TankModel::~TankModel()
{
	if (m_skiL)
	{
		if ( m_skiL->getWorld() ){ m_skiL->getWorld()->removeEntity(m_skiL); }
		m_skiL->removeReference();
	}
	if (m_skiR)
	{
		if ( m_skiR->getWorld() ){ m_skiR->getWorld()->removeEntity(m_skiR); }
		m_skiR->removeReference();
	}
	if (m_body)
	{
		if ( m_body->getWorld() ){ m_body->getWorld()->removeEntity(m_body); }
		m_body->removeReference();
	}
	if (m_tower)
	{
		if ( m_tower->getWorld() ){ m_tower->getWorld()->removeEntity(m_tower); }
		m_tower->removeReference();
	}
	if (m_gun)
	{
		if ( m_gun->getWorld() ){ m_gun->getWorld()->removeEntity(m_gun); }
		m_gun->removeReference();
	}
	if (m_left)
	{
		m_left->removeReference();
	}
	if (m_right)
	{
		m_right->removeReference();
	}
}


void TankModel::updateCamera( hkDemoEnvironment* environment,  hkReal timestep )
{
	VehicleApiUtils::updateCamera( environment, *m_tower, timestep, m_camera );
}

void TankModel::stepTank( hkDemoEnvironment* environment, hkReal deltaTime )
{
	//const hkVector4& towerUp = m_tower->getTransform().getColumn(1);
	const hkVector4& towerForward = m_tower->getTransform().getColumn(0);
	const hkVector4& towerRight   = m_tower->getTransform().getColumn(2);

	const hkVector4& tankForward = m_body->getTransform().getColumn(0);
	//const hkVector4& tankRight   = m_body->getTransform().getColumn(2);

	{
		int mouseX = environment->m_window->getMouse().getPosX();
		int mouseY = environment->m_window->getMouse().getPosY();
		{
			const hkReal MOUSE_SENSITIVITY_X = 0.1f;
			const hkReal MOUSE_SENSITIVITY_Y = 0.03f;
			{
				int halfWindowWidth = environment->m_window->getWidth() >> 1;
				hkReal inc  = hkReal(mouseX - halfWindowWidth ) / halfWindowWidth;
				m_directionAngle += 3.4f * inc * MOUSE_SENSITIVITY_X;
			}
			{
				int halfWindowHeight = environment->m_window->getHeight() >> 1;
				hkReal inc  = hkReal(halfWindowHeight - mouseY) / halfWindowHeight;
				m_upAngle += -3.4f * inc * MOUSE_SENSITIVITY_Y;
				m_upAngle = hkMath::max2( m_upAngle, -0.2f );
				m_upAngle = hkMath::min2( m_upAngle, 1.0f );
			}

			hkReal currentAngle;
			if(1)
			{
				currentAngle = m_rotations * 2.0f * HK_REAL_PI  + 0.5f * HK_REAL_PI - 
					hkMath::atan2fApproximation( tankForward(0), tankForward(2)  );
				if ( currentAngle - m_lastAngle > HK_REAL_PI )
				{
					currentAngle -= HK_REAL_PI * 2.0f;
					m_rotations -= 1.0f;
				}
				while ( currentAngle - m_lastAngle < -HK_REAL_PI )
				{
					currentAngle += HK_REAL_PI * 2.0f;
					m_rotations += 1.0f;
				}
				m_lastAngle = currentAngle;
			}
			m_towerMotor->setMotorTargetAngle( m_directionAngle - currentAngle );
			m_gunMotor->setMotorTargetAngle( -m_upAngle );
		}


		// Snap mouse back to middle
		environment->m_window->setMousePosition(environment->m_window->getWidth() >> 1, environment->m_window->getHeight() >> 1);
	}

	hkReal speed = m_currentVelocity.length3();
	if ( speed < 5.0f )
	{
		//m_currentVelocity.mul4( 1.0f - deltaTime * 1.5f );
	}
	else
	{
		m_currentVelocity.mul4( 1.0f - deltaTime * 0.7f );
	}


	const hkgPad* pad = environment->m_gamePad;


	hkReal accel = 0.4f;

	if ( pad->getButtonState() & HKG_PAD_DPAD_UP )		{		m_currentVelocity.addMul4( accel, towerForward );	}
	if ( pad->getButtonState() & HKG_PAD_DPAD_DOWN )	{		m_currentVelocity.addMul4( -accel, towerForward );	}
	if ( pad->getButtonState() & HKG_PAD_DPAD_LEFT )	{		m_currentVelocity.addMul4( -accel, towerRight );	}
	if ( pad->getButtonState() & HKG_PAD_DPAD_RIGHT )	{		m_currentVelocity.addMul4(  accel, towerRight );	}

	{
		if (environment->m_window->getKeyboard().getKeyState( 'A')) {		m_currentVelocity.addMul4( -accel, towerRight );	}
		if (environment->m_window->getKeyboard().getKeyState( 'D')) {		m_currentVelocity.addMul4(  accel, towerRight );	}
		if (environment->m_window->getKeyboard().getKeyState( 'W')) {		m_currentVelocity.addMul4( accel, towerForward );	}
		if (environment->m_window->getKeyboard().getKeyState( 'S')) {		m_currentVelocity.addMul4( -accel, towerForward );	}
	}

	hkReal velLeft = m_currentVelocity.dot3( tankForward );
	hkReal velRight = velLeft;

	hkReal rotationGain = 20.0f;
	if ( velLeft * velLeft > 0.01f )
	{
		hkVector4 cd = m_currentVelocity;
		cd.normalize3();
		hkVector4 x; x.setCross( tankForward, cd );
		hkReal a = x(1);

		velLeft -= a * rotationGain;
		velRight += a * rotationGain;
	}

	// apply velocities
	//HK_DISPLAY_ARROW( m_tower->getPosition(), tankForward, hkColor::GREEN );
	hkVector4 to; to.setAdd4( m_tower->getPosition(), m_currentVelocity );
	HK_DISPLAY_LINE( m_tower->getPosition(), to, hkColor::BLUE );
	hkVector4 lv; lv.setMul4( -velLeft,  tankForward );
	hkVector4 rv; rv.setMul4( -velRight, tankForward );
	m_left ->setSurfaceVelocity( lv );
	m_right->setSurfaceVelocity( rv );
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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Utilities/Actions/Motor/hkpMotorAction.h>
#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>
#include <Common/Visualize/hkDebugDisplay.h>

#include <Demos/Physics/Api/Dynamics/Actions/MotorAction/MotorActionDemo.h>

// A hkpBroadPhaseBorder that puts objects back at the origin
class MotorActionDemoBorder : public hkpBroadPhaseBorder
{
public:
	MotorActionDemoBorder( hkpWorld* world )
		: hkpBroadPhaseBorder( world )
	{
	}

	// called back when an object leaves the broadphase
	void maxPositionExceededCallback( hkpEntity* entity )
	{
		hkpRigidBody* body = static_cast<hkpRigidBody*>(entity);
		body->setPositionAndRotation( hkVector4::getZero(), hkQuaternion::getIdentity() );
		body->setLinearVelocity( hkVector4::getZero() );
		body->setAngularVelocity( hkVector4::getZero() );
	}
};

MotorActionDemo::MotorActionDemo(hkDemoEnvironment* env)
	: hkDefaultPhysicsDemo(env), m_elapsedSteps(0)
{
	// Set up the camera.
	{
		hkVector4 from(0.0f, 2.0f, 5.0f);
		hkVector4 to  (0.0f, 0.5f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	// Create the world.
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.m_broadPhaseBorderBehaviour = info.BROADPHASE_BORDER_DO_NOTHING;	// we do our own stuff
		info.setBroadPhaseWorldSize( 30.0f );
		m_world = new hkpWorld( info );
		m_world->lock();

		setupGraphics();

		// Register broadphase border
		MotorActionDemoBorder *border = new MotorActionDemoBorder( m_world );
		m_world->setBroadPhaseBorder(border);
		border->removeReference();

		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	// Create a spinner
	const hkInt32 numSides = 6;
	const hkReal topRadius = 0.5f;
	const hkReal topHeight = 0.75f;
	hkpRigidBody* topBody = HK_NULL;
	{
		hkArray<hkVector4> verts;
		verts.setSize( numSides + 1 );

		// vert at the 'top'
		verts[0].setZero4();

		// verts around 'base'
		for(hkInt32 i = 1; i < verts.getSize(); i++)
		{
			verts[i].set( hkMath::cos( (hkReal)i/((hkReal)verts.getSize()-1) * (HK_REAL_PI * 2.0f) ) * topRadius, 
						topHeight, 
						hkMath::sin( (hkReal)i/((hkReal)verts.getSize()-1) * (HK_REAL_PI * 2.0f) ) * topRadius );
		}

		// make a convex hull for the top
		hkpConvexVerticesShape* topShape = HK_NULL;
		{
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;
				
			hkStridedVertices stridedVerts;
			{
				stridedVerts.m_numVertices = verts.getSize();
				stridedVerts.m_striding = sizeof(hkVector4);
				stridedVerts.m_vertices = reinterpret_cast<const float*>(verts.begin());
			}

			hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

			{
				stridedVerts.m_numVertices = geom.m_vertices.getSize();
				stridedVerts.m_striding = sizeof(hkVector4);
				stridedVerts.m_vertices = &(geom.m_vertices[0](0));
			}

			topShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
		}

		// Add the body
		topBody = GameUtils::createRigidBody( topShape, 1.0f, hkVector4::getZero() );
		topShape->removeReference();

		m_world->addEntity( topBody )->removeReference();

		// Add a fixed box for it to fall on
		m_world->addEntity( GameUtils::createBox( hkVector4(20.0f,1.0f,20.0f), 0.0f, hkVector4(0.0f,-1.0f,0.0f) ) )->removeReference();
	};

	// Finally create and add the action
	{
		const hkVector4 upAxis( 0.0f, 1.0f, 0.0f ); // up axis in body space
		const hkReal spinRate = HK_REAL_PI * 2.0f; // spin rate in rad/s
		const hkReal gain = 10.0f; // gain- larger values here mean the motor will 'pick up' faster

		m_motorAction = new hkpMotorAction( topBody, upAxis, spinRate, gain );
		m_world->addAction( m_motorAction );
		m_motorAction->removeReference();
	}

	m_world->unlock();
}

hkDemo::Result MotorActionDemo::stepDemo()
{
	// reverse direction every 300 steps
	// (this is really just a code coverage thing for the hkpMotorAction 
	// get/set methods, as it serves no other purpose than to make the
	// top fall over. Aww!)
	if( 0 == ++m_elapsedSteps % 300 && m_motorAction->isActive() )
	{
		m_motorAction->setSpinRate( -(const_cast<const hkpMotorAction*>(m_motorAction)->getSpinRate()) );
		m_motorAction->setGain( const_cast<const hkpMotorAction*>(m_motorAction)->getGain() );
		m_motorAction->setAxis( const_cast<const hkpMotorAction*>(m_motorAction)->getAxis() );
	}

	// display the motor axis
	{
		hkVector4 motorAxisStart, motorAxisEnd;
		motorAxisStart = static_cast<hkpRigidBody*>(m_motorAction->getEntity())->getPosition();
		motorAxisEnd.setRotatedDir( static_cast<hkpRigidBody*>(m_motorAction->getEntity())->getRotation(), m_motorAction->getAxis() );

		HK_DISPLAY_ARROW( motorAxisStart, motorAxisEnd, hkColor::YELLOW );
	}

	// and step the world
	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char* helpString = "A simple hkpMotorAction demo. The motor reverses direction every 300 steps.";

HK_DECLARE_DEMO(MotorActionDemo, HK_DEMO_TYPE_PRIME, "Demonstrates the use of a hkpMotorAction\n", helpString);

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

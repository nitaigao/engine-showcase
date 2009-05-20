/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Demos/Physics/UseCase/ContinuousPhysics/PrioritiesAndQualities/ThreeWaySqueeze/ThreeWaySqueezeDemo.h>
//#include <hkutilities/charactercontrol/characterproxy/hkpCharacterProxy.h>

#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

#include <Common/Visualize/hkDebugDisplay.h>


class My3WCollisionFilter: public hkpCollisionFilter
{
	hkBool isCollisionEnabled(const hkpCollidable& a, const hkpCollidable& b) const
	{
		hkpRigidBody* body[2] = { (hkpRigidBody*)a.getOwner(), (hkpRigidBody*)b.getOwner() };
		if (body[0]->isFixedOrKeyframed() && body[1]->isFixedOrKeyframed())
		{
			return false;
		}
		return true;
	}

	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const hkpShapeContainer& bContainer, hkpShapeKey bKey  ) const	{		return true;	}
	hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& shape, const hkpShapeContainer& bContainer, hkpShapeKey bKey ) const	{		return true;	}
	hkBool isCollisionEnabled( const hkpWorldRayCastInput& a, const hkpCollidable& collidableB ) const	{		return true;	}
	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& cA, const hkpCdBody& cB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const
	{
		return true;
	}
};



ThreeWaySqueezeDemo::ThreeWaySqueezeDemo(hkDemoEnvironment* env):	hkDefaultPhysicsDemo(env)
{
	// Disable warning
	hkError::getInstance().setEnabled(0xf0de4356, false);	// 'Your m_contactRestingVelocity seems to be too small'

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 10.0f, 10.0f);
		hkVector4 to  (0.0f,  0.0f,  0.0f);
		hkVector4 up  (0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		info.setBroadPhaseWorldSize( 350.0f );  
		info.m_gravity.set(0,-40,0);
		info.m_collisionTolerance = 0.1f;
		info.m_numToisTillAllowedPenetrationToi = 1.1f;

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
		setupGraphics();
	}

	{
		hkpCollisionFilter* cf = new My3WCollisionFilter();
		m_world->setCollisionFilter(cf);
		cf->removeReference();
	}

	// Build a Base
	hkVector4 baseSize( 50.0f, 1.0f, 50.0f);
	{ 
		hkpRigidBodyCinfo rci;
		rci.m_shape = new hkpBoxShape( baseSize );
		rci.m_position.set(0.0f, -0.5f, 0.0f);
		rci.m_motionType = hkpMotion::MOTION_FIXED;
			
		// Create a rigid body (using the template above).
		hkpRigidBody* base = new hkpRigidBody(rci);

		// Remove reference since the body now "owns" the Shape.
		rci.m_shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity( base )->removeReference();
	} 
	
	
	// Create a circle of keyframed objects 
	// Each of the objects is given a different increasing priority
	// We set the priority as a property on the object and extract this i nthe callback.
	hkVector4 blockerSize(1,3,5);
	hkpShape* blocker = new hkpBoxShape( blockerSize );
	{
		//hkPseudoRandomGenerator ran(100);
		for (int b = 0; b < NUM_OBJECTS; b++ )
		{
			hkVector4 up(0,1,0);
			hkReal angle = hkReal(b) / NUM_OBJECTS * HK_REAL_PI * 2;

			hkpRigidBodyCinfo rci;
			rci.m_position.set(5,0,0);
			rci.m_rotation.setAxisAngle( up, angle );
			rci.m_position.setRotatedDir( rci.m_rotation, rci.m_position );
			rci.m_shape = blocker;
			
			// If we set this to true, the body is fixed, and no mass properties need to be computed.
			rci.m_motionType = hkpMotion::MOTION_KEYFRAMED;
			if (b < 1)
			{
				rci.m_qualityType = HK_COLLIDABLE_QUALITY_KEYFRAMED;
			}
			else
			{
				rci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			}

			m_objects[b] = new hkpRigidBody( rci );
			m_world->addEntity( m_objects[b] );
			m_objects[b]->removeReference();

			int color = rci.m_qualityType == HK_COLLIDABLE_QUALITY_FIXED ?
						hkColor::rgbFromFloats(1.0f, 0.0f, 0.0f, 1.0f) :
						hkColor::rgbFromFloats(0.0f, 1.0f, 0.0f, 1.0f) ;

			HK_SET_OBJECT_COLOR((hkUlong)m_objects[b]->getCollidable(), color );
		}
	}
	blocker->removeReference();

	//
	// Crate middle sphere
	//

	{
		hkpShape* shape = new hkpSphereShape(1.5f);
		hkpRigidBodyCinfo rbInfo;
		rbInfo.m_shape = shape;
		rbInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		hkpRigidBody* body = new hkpRigidBody(rbInfo);
		m_world->addEntity(body);
		body->removeReference();
		shape->removeReference();
	}
	
	m_prevObj = 0;

	// Zero current time at start
	m_currentTime = 0.0f;

	m_world->unlock();
}

ThreeWaySqueezeDemo::~ThreeWaySqueezeDemo()
{
	hkError::getInstance().setEnabled(0xf0de4356, true);
}


/////////////////////////////////////////////////////////////////////

hkDemo::Result ThreeWaySqueezeDemo::stepDemo()
{
	{
		m_world->lock();

		m_currentTime += m_timestep;

		// Move the blockers
		{
			hkReal scaledTime = m_currentTime * 0.2f;

			int currentObject = (int)scaledTime % NUM_OBJECTS;
			if (currentObject != m_prevObj)
			{
				m_prevObj = currentObject;
			}

			for (int b = 0; b < NUM_OBJECTS; b++ )
			{
				hkVector4 pos;
				pos.setZero4();
				if (b == currentObject)
				{
					pos.set(hkMath::cos( scaledTime * HK_REAL_PI * 2 ) + 1.0f, 0, 0);
					pos(0) += 0.1f;
					pos.mul4(3.0f);
					pos(1) = b * 0.01f;
				}
				else
				{
					pos(0) = 2.7f;
					pos(1) = b * 0.01f;
				}

				pos.setRotatedDir( m_objects[b]->getRotation(), pos );
				m_objects[b]->activate();
				hkpKeyFrameUtility::applyHardKeyFrame( pos, m_objects[b]->getRotation(), 1.f / m_timestep, m_objects[b] );
			}
		}

		m_world->unlock();
	}

	// Step the world
	hkDefaultPhysicsDemo::stepDemo();

	return hkDemo::DEMO_OK;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

const char helpString [] =
"This demo shows how the physics engine can use a primitive priority scheme "	\
"to stop moving object from leaving the world. In this example the red objects " \
"are flagged with the highest priority and the green one with a lower priority "	\
"(see hkpRigidBodyCinfo::m_qualityType). \n"											\
"Note: because we use an iterative solver, penetrations might happen if a moving body "	\
"gets squeezed between two other moving bodies of the same priority. In this case " \
"penetration will  happen." \
"For this reason you can only assign the highest priority to fixed objects";

HK_DECLARE_DEMO(ThreeWaySqueezeDemo, HK_DEMO_TYPE_PRIME, "Show different hkCollidabeQualityTypes", helpString);

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

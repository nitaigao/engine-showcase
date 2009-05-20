/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/Doors/DoorsDemo.h>

// For Havok timers
#include <Common/Base/Monitor/hkMonitorStream.h>

// File loading
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

// For player walking
#include <Physics/Utilities/Constraint/Keyframe/hkpKeyFrameUtility.h>

// Collision Groups
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>

// Constraint handling
#include <Physics/Dynamics/Constraint/Bilateral/LimitedHinge/hkpLimitedHingeConstraintData.h>

// For the triggered doors
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>

// Debug display
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpConvexRadiusViewer.h>

DoorSpring::DoorSpring(const hkpConstraintInstance* hinge, const hkReal strength, const hkReal damping)
: hkpUnaryAction(hinge->getEntityA())
, m_hinge(hinge)
, m_strength(strength)
, m_damping(damping)
, m_angVel(0.0f)
, m_torque(0.0f)
{
	// Assume the door is hung vertically
	m_axis = m_entity->getWorld()->getGravity();
	m_axis.normalize3();
	HK_ASSERT2(0x0, !m_entity->isFixed(), "Entity A must be dynamic, only because that is the entity that I pass to the hkpUnaryAction constructor.");
}

void DoorSpring::applyAction(const hkStepInfo& stepInfo)
{
	HK_TIME_CODE_BLOCK("DoorSpring::applyAction", HK_NULL);
	float angle = hkpLimitedHingeConstraintData::getRuntime(m_hinge->getRuntime())->getCurrentPos();

	if(hkMath::fabs(angle) < 0.001f )
	{
		return;
	}

	hkpRigidBody* rb = static_cast<hkpRigidBody*>(m_entity);
	hkVector4 torque = m_axis;
	m_angVel = rb->getAngularVelocity().dot3(torque);
	m_opening = (m_angVel * angle) < 0.0f;
	torque.mul4( (angle * m_strength) - (m_angVel * m_damping ) );
	rb->applyTorque(stepInfo.m_deltaTime, torque);
	m_torque = torque.length3();
}

PlayerWalkAction::PlayerWalkAction (hkpEntity* entity, const hkReal walkSpeed)
: hkpUnaryAction(entity)
, m_walkSpeed(walkSpeed)
{
	m_axis = entity->getWorld()->getGravity();
	m_axis.normalize3();
}

void PlayerWalkAction::applyAction(const hkStepInfo& stepInfo)
{
	HK_TIME_CODE_BLOCK("PlayerWalkAction::applyAction", HK_NULL);

	hkpRigidBody* rb = static_cast<hkpRigidBody*>(m_entity);
	hkQuaternion rotation;
	rotation.setAxisAngle(m_axis, m_walkSpeed * stepInfo.m_deltaTime);
	hkVector4 nextPos; nextPos.setRotatedDir(rotation, rb->getPosition());
	hkpKeyFrameUtility::applyHardKeyFrame(nextPos, rb->getRotation(), stepInfo.m_invDeltaTime, rb);
}
	

DoorsDemo::DoorsDemo(hkDemoEnvironment* env)
: hkDefaultPhysicsDemo(env)
, m_targetAngle(0.0f)
, m_timeSinceExit(0.0f)
, m_doorSpeed(0.5f)
, m_doorDelayBeforeClosing(1.0f)
{
	//
	// Setup the camera
	//
	{
		static const hkVector4 from(7.0f,  4.5f,  5.0f);
		static const hkVector4 to  (0.0f,  0.0f,  0.0f);
		static const hkVector4 up  (0.0f,  0.0f,  1.0f);
		setupDefaultCameras(env, from, to, up );
	}

	hkString file("Resources/Physics/Objects/Doors.xml");
	hkXmlPackfileReader reader;
	hkIstream infile( file.cString() );
	HK_ASSERT( 0x215d080c, infile.isOk() );
	
	reader.loadEntireFile(infile.getStreamReader());

	hkVersionUtil::updateToCurrentVersion(reader, hkVersionRegistry::getInstance());

	m_data = reader.getPackfileData();
	m_data->addReference();

	hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>( reader.getContents( hkRootLevelContainerClass.getName() ) );
	HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );

	hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0xa6451544, physicsData, "Could not find physics data in root level object" );
	HK_ASSERT2(0xa6451535, physicsData->getWorldCinfo() != HK_NULL, "No physics cinfo in loaded file - cannot create a hkpWorld" );

	m_world = new hkpWorld( *physicsData->getWorldCinfo() );
	m_world->lock();

	// Add the convex radius viewer and the constraint viewer
	// so that we can see exactly what is going on.
	m_debugViewerNames.pushBack(hkpConstraintViewer::getName());
	m_debugViewerNames.pushBack(hkpConvexRadiusViewer::getName());
	setupGraphics();
	
	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

	hkpRigidBody* player = HK_NULL;

	// Add all the loaded physics systems to the world.
	for (int sys=0; sys<physicsData->getPhysicsSystems().getSize(); sys++)
	{
		hkpPhysicsSystem* system = physicsData->getPhysicsSystems()[sys];

		// Loop through all the rigid bodies, looking for doors, the player, and fixed
		// objects. Set the collision filter accordingly.
		for (int rb=0; rb<system->getRigidBodies().getSize(); rb++)
		{
			hkpRigidBody* rigidBody = system->getRigidBodies()[rb];
			const char* name = rigidBody->getName();

			// The doors have all been named in 3DS Max.
			if (hkString::strCmp(name, "LeftPoweredDoor")  == 0 ||
				hkString::strCmp(name, "RightPoweredDoor") == 0 ||
				hkString::strCmp(name, "LeftSwingDoor")    == 0 ||
				hkString::strCmp(name, "RightSwingDoor")   == 0)
			{
				rigidBody->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_KEYFRAME));
			}
			// The player rigid body has been named in 3DS Max.
			else if (hkString::strCmp(name, "Player") == 0)
			{
				player = rigidBody;
				rigidBody->setMotionType(hkpMotion::MOTION_KEYFRAMED);
				rigidBody->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_PLAYER));
			}
			else if (rigidBody->isFixed())
			{
				rigidBody->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_STATIC));
			}
		}

		m_world->addPhysicsSystem(system);

		// Loop through all the constraints in the system,
		// looking for the door hinges
		for (int cons=0; cons<system->getConstraints().getSize(); cons++)
		{
			hkpConstraintInstance* constraint = system->getConstraints()[cons];
			if (constraint->getData()->getType() == hkpConstraintData::CONSTRAINT_TYPE_LIMITEDHINGE)
			{
				hkpLimitedHingeConstraintData* data = static_cast<hkpLimitedHingeConstraintData*>(constraint->getDataRw());

				if (data->getMotor()) // the powered doors
				{
					// Store a pointer to these, so that we can access them
					// in the main loop.
					m_poweredHingeDatas.pushBack(data);
					m_poweredHingeInstances.pushBack(constraint);
				}
				else // The swing doors
				{
					// Add the spring action to the spring doors
					DoorSpring* spring = new DoorSpring(constraint, 35.0f, 5.0f);
					m_world->addAction(spring);
					spring->removeReference();
				}
			}
		}
	}

	if (player)
	{
		// Add a custom action to the player rigid
		// body to key frame it around the world.
		PlayerWalkAction* walk = new PlayerWalkAction(player, 0.5f);
		m_world->addAction(walk);
		walk->removeReference();
	}

	// Add an AABB phantom to the world that acts as a trigger
	// volume for the powered door.
	{
		static const hkVector4 minv(2.5f, -1.5f, 0.0f);
		static const hkVector4 maxv(4.5f,  1.5f, 2.0f);
		hkAabb aabb(minv, maxv);
		m_doorTrigger = new hkpAabbPhantom(aabb, hkpGroupFilter::calcFilterInfo(hkpGroupFilterSetup::LAYER_AI));
		m_world->addPhantom(m_doorTrigger);
		
	}

	m_world->unlock();
}

DoorsDemo::~DoorsDemo(void)
{
	// Free the world.
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	// Free up all the data that we still have a pointer to
	m_doorTrigger->removeReference();
	m_data->removeReference();
}

DoorsDemo::Result DoorsDemo::stepDemo( void )
{
	HK_TIMER_BEGIN("updateDoors", HK_NULL);

	m_world->lock();

	// If there are entities overlapping with the trigger volume:
	if (m_doorTrigger->getOverlappingCollidables().getSize())
	{
		for (int h=0; h<m_poweredHingeDatas.getSize(); h++)
		{		
			// Increase the target angle until it reaches the max limit
			m_targetAngle += m_doorSpeed * m_timestep;
			m_targetAngle = hkMath::min2(m_targetAngle , m_poweredHingeDatas[h]->getMaxAngularLimit());
			m_poweredHingeDatas[h]->setMotorTargetAngle(m_targetAngle);

			hkpConstraintInstance* constraintInstance = m_poweredHingeInstances[h];
			constraintInstance->getRigidBodyA()->activate();
			constraintInstance->getRigidBodyB()->activate();

		}
		m_timeSinceExit = 0.0f;
	}
	// Otherwise if there are no overlaps
	else
	{
		for (int h=0; h<m_poweredHingeDatas.getSize(); h++)
		{
			// Keep track of how long has passed since the
			// player left the trigger volume
			if (m_timeSinceExit < m_doorDelayBeforeClosing)
			{
				m_timeSinceExit += m_timestep;
			}
			else
			{
				// If there has been no player in the trigger volume for long enough...
				hkpConstraintInstance* constraintInstance = m_poweredHingeInstances[h];
				float currentAngle = hkpLimitedHingeConstraintData::getRuntime(constraintInstance->getRuntime())->getCurrentPos();

				// If the door is not closed yet activate it (setting the motor
				// angle does not activate the entities attached to the constraint).
				if (currentAngle > m_poweredHingeDatas[h]->getMinAngularLimit())
				{
					constraintInstance->getRigidBodyA()->activate();
					constraintInstance->getRigidBodyB()->activate();
				}

				// Decrease the hinge target angle to zero
				m_targetAngle -= m_doorSpeed * m_timestep;
				m_targetAngle = hkMath::max2(m_targetAngle , m_poweredHingeDatas[h]->getMinAngularLimit());
				m_poweredHingeDatas[h]->setMotorTargetAngle(m_targetAngle);
			}
		}
	}

	m_world->unlock();

	HK_TIMER_NAMED_END("updateDoors");

	return hkDefaultPhysicsDemo::stepDemo();
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =
	"Shows how to create doors that a character can interact with, using  either rigid "
	"body dynamics to push doors open, or trigger volumes to enable powered hinge motors.";

HK_DECLARE_DEMO(DoorsDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "Doors", helpString);

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

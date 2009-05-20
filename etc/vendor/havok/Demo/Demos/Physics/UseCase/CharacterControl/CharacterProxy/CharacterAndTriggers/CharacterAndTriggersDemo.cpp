/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/UseCase/CharacterControl/CharacterProxy/CharacterAndTriggers/CharacterAndTriggersDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

	// Used to generate a convex hull for convexVerticesShape
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

	// Used for character controller code
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Agent/Collidable/hkpCdPoint.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>

#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

	// Used for graphics and I/O

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterUtils.h>

 // Used to create the Mopp 'code' object
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#define HK_OBJECT_IS_TRIGGER 7777


#define RIGIDBODY_TRIGGERCOLOR hkColor::rgbFromChars(255, 0, 0)
#define PHANTOM_TRIGGERCOLOR hkColor::rgbFromChars(255, 0, 0, 128)

////////////////////////////////////////////////////////////////////
///////////////////////// Custom Collector /////////////////////////
////////////////////////////////////////////////////////////////////
class MyTriggerIdentifyingPointCollector: public hkpAllCdPointCollector 
{ 
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, MyTriggerIdentifyingPointCollector);

	MyTriggerIdentifyingPointCollector()
		: hkpAllCdPointCollector()
	{
		m_objectsWeHit.clear();
	}

	virtual void addCdPoint( const hkpCdPoint& event ) 
	{
		//
		// Put your own tests in here if you like. The code currently:
		// 1. Identifies shapes (whose root collidables are) marked as triggers. Culls them for collection
		//		but records that they were 'hit'.
		// 2. Culls phantoms for collection (to allow character to walk through them).


		const hkpCollidable* rootCollidable = event.m_cdBodyB.getRootCollidable();
		hkpWorldObject* wo = hkGetWorldObject(rootCollidable);

		if ( wo->hasProperty(HK_OBJECT_IS_TRIGGER) )
		{
				// If not already collected, since character controller may use several casts in one integrate step.
			if(m_objectsWeHit.indexOf(wo) == -1)
			{
				m_objectsWeHit.pushBack(wo);

					// Push back contact . N.B. The *distance* here is may be an actual (world ) distance,
					// or it may be a parameterization [0,1] of a linear cast, depending on what algorithm is using the collector.
				m_contacts.pushBack(event.m_contact);
			}
			
		}
		else 
		{ 
				// Let rigid bodies be dealt with, let phantoms be ignored
			if(hkGetRigidBody(wo->getCollidable()))
			{
				hkpAllCdPointCollector::addCdPoint(event); 
			}
		} 
	}

	hkArray<hkpWorldObject*> m_objectsWeHit;
	hkArray<hkContactPoint> m_contacts;
}; 


////////////////////////////////////////////////////////////////////
/////////////////////////////// Demo ///////////////////////////////
////////////////////////////////////////////////////////////////////
CharacterAndTriggersDemo::CharacterAndTriggersDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_generator(747)
{
	m_characterVelocity = 10.0f;

	m_wallDistance = 25.0f;	

	m_env = env;

	//
	// Setup the camera
	//
	{
		hkVector4 from(  0.0f, -80.0f, 20.0f);
		hkVector4 to  (  0.0f,  0.0f,   0.0f);
		hkVector4 up  (  0.0f,  0.0f,   1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 500.0f );  
		info.m_gravity.set(0,0,-9.8f);
		info.m_collisionTolerance = 0.1f;		
		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		setupGraphics();
	}

	
	//
	// Add a 'floor
	//
	{
		hkVector4 halfExtents(30.0f, 30.0f, 1.0f);
		hkpBoxShape* shape = new hkpBoxShape(halfExtents);
		
		///////////////////////////////////////////////////////

		hkpRigidBodyCinfo rigidBodyInfo;

		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_position.set(0.0f, 0.0f, 0.0f);
		rigidBodyInfo.m_angularDamping = 0.0f;
		rigidBodyInfo.m_linearDamping = 0.0f;

		// If we set this to true, the body is fixed, and no mass properties need to be computed.
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
			
		 
		// Create a rigid body (using the template above).
		hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);

		// Remove references since the body now "owns" the Shapes.
		shape->removeReference();

		// Finally add body so we can see it, and remove reference since the world now "owns" it.
		m_world->addEntity(rigidBody);
		rigidBody->removeReference();

	}


	//
	//	Create a character proxy object
	//
	{
		// Construct a shape

		hkVector4 vertexA(0,0, .4f);
		hkVector4 vertexB(0,0,-.4f);		

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Create a capsule to represent the character crouching
		vertexA.setZero4();
		m_crouchShape = new hkpCapsuleShape(vertexA, vertexB, .6f);


		// Construct a Shape Phantom
		hkTransform tx; tx.setIdentity();
		m_phantom = new hkpSimpleShapePhantom( m_standShape, tx, hkpGroupFilter::calcFilterInfo(0,2) );
		
		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(0,0,5);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	
		cpci.m_userPlanes = 4;

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}
	

	//
	// Create the Character state machine and context
	//
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		hkpCharacterStateOnGround* gstate = new hkpCharacterStateOnGround();
		gstate->setSpeed(m_characterVelocity);
		manager->registerState( gstate,	HK_CHARACTER_ON_GROUND);
		gstate->removeReference();

		state = new hkpCharacterStateInAir();
		manager->registerState( state,	HK_CHARACTER_IN_AIR);
		state->removeReference();

		state = new hkpCharacterStateJumping();
		manager->registerState( state,	HK_CHARACTER_JUMPING);
		state->removeReference();

		state = new hkpCharacterStateClimbing();
		manager->registerState( state,	HK_CHARACTER_CLIMBING);
		state->removeReference();

		m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		manager->removeReference();
	}

	
	// Current camera angle about up
	m_currentAngle = 0.4f;
	// Snap mouse back to middle
	m_env->m_window->setMousePosition(m_env->m_window->getWidth() >> 1, m_env->m_window->getHeight() >> 1);

	//
	// Add some rigid bodies and mark them as 'triggers'
	//
	int i;
	for(i = 0; i < 15; i++)
	{
		hkpRigidBodyCinfo rci;

		rci.m_shape = getRandomShape();

		// Place at random positon, random rotation
		rci.m_position = getRandomPositionAwayFromCharacter(15.0f, 15.0f, 7.0f);
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		rci.m_rotation.setAxisAngle(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));

		rci.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		rci.m_linearDamping = 1.0f;	// Don't want them rolling away
		rci.m_angularDamping = 1.0f;

		const hkReal mass = 100.0f;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(rci.m_shape, mass, massProperties);
		rci.m_inertiaTensor = massProperties.m_inertiaTensor;
		rci.m_centerOfMass = massProperties.m_centerOfMass;
		rci.m_mass = massProperties.m_mass;			



		hkpRigidBody* trigger = new hkpRigidBody(rci);
		rci.m_shape->removeReference();
		m_world->addEntity(trigger);

		if(m_generator.getRandRange(0.0f, 1.0f) < 0.5f)
		{
			// Now just set a user property on this boyo, and change its color to make it easy to see
			triggerifyWorldObject(trigger, RIGIDBODY_TRIGGERCOLOR);
		}
	
		trigger->removeReference();
	
	}

	//
	// Add some phantoms and mark them as 'triggers'
	//
	for(i = 0; i < 15; i++)
	{
		hkpShape* shape = getRandomShape();

		// Place at random positon, random rotation
		hkTransform randomTx; 
		randomTx.setTranslation( getRandomPositionAwayFromCharacter(15.0f, 0.0f, 15.0f));
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
		randomTx.setRotation(rot);
		
		hkpSimpleShapePhantom* trigger = new hkpSimpleShapePhantom( shape, randomTx );
		shape->removeReference();
		m_world->addPhantom(trigger);

		if(m_generator.getRandRange(0.0f, 1.0f) < 0.5f)
		{
			// Now just set a user property on this boyo, and change its color to make it easy to see
			triggerifyWorldObject(trigger, PHANTOM_TRIGGERCOLOR);
		}
	
		trigger->removeReference();


	}

	m_world->unlock();
}

void CharacterAndTriggersDemo::removeAndReaddObject(hkpWorldObject* wo)
{
	wo->addReference();
	hkpRigidBody* body = hkGetRigidBody(wo->getCollidable());
	if(body)
	{
		// It's a rigid body
		m_world->removeEntity(body);

		// Place at random positon, random rotation
		hkVector4 newpos = getRandomPositionAwayFromCharacter(15.0f, 15.0f, 7.0f);
		body->setPosition(newpos);
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
		body->setRotation(rot);

		body->setAngularVelocity(hkVector4::getZero());
		body->setLinearVelocity(hkVector4::getZero());

		m_world->addEntity(body);

			// Color the trigger so we can see it clearly
		HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), RIGIDBODY_TRIGGERCOLOR);

	}
	else
	{
		// It's phantom, assume a simpleshape phantom 
		hkpSimpleShapePhantom* phantom = static_cast<hkpSimpleShapePhantom*> (hkGetPhantom(wo->getCollidable()));

		// Place at random positon, random rotation
		hkVector4 newpos = getRandomPositionAwayFromCharacter(15.0f, 0.0f, 15.0f);
		hkVector4 axis(m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f), m_generator.getRandRange(-1.0f, 1.0f));
		axis.normalize3();
		hkQuaternion rot(axis, m_generator.getRandRange(0.0f, HK_REAL_PI));
		phantom->setPosition(newpos);
		hkTransform t(rot, newpos);
		phantom->setTransform(t);

		m_world->removePhantom(phantom);

		m_world->addPhantom(phantom);

			// Color the trigger so we can see it clearly
		HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), PHANTOM_TRIGGERCOLOR);

	}


	wo->removeReference();

}

CharacterAndTriggersDemo::~CharacterAndTriggersDemo()
{
	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());

	m_characterProxy->removeReference();
	m_standShape->removeReference();
	m_crouchShape->removeReference();

	m_world->unlock();

	delete m_characterContext;
}



hkDemo::Result CharacterAndTriggersDemo::stepDemo()
{
	hkpCharacterInput input;

	{
		m_world->lock();

		//hkString s("NORMAL rigidbodies are WHITE, normal phantoms are transparent YELLOW\nTRIGGER rigid bodies are RED, trigger phantoms are transparent RED.");
		//m_env->m_textDisplay->outputText(s, 7, 350, 0xffffffff);

		//	Get user input data
		int m_upAxisIndex = 2;
		hkVector4 up; up.setZero4();
		up(m_upAxisIndex) = 1;

		hkQuaternion orient;
		hkReal posX = 0.f;
		hkReal posY = 0.f;
		{
			float deltaAngle = 0.f;
			CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
			m_currentAngle += deltaAngle;
			orient.setAxisAngle(up, m_currentAngle);
		}

		hkpCharacterOutput output;
		{
			input.m_inputLR = posX;
			input.m_inputUD = posY;

			input.m_wantJump = m_env->m_window->getMouse().wasButtonPressed(HKG_MOUSE_LEFT_BUTTON)
				|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_1);
			input.m_atLadder = false;

			input.m_up = up;
			input.m_forward.set(1,0,0);
			input.m_forward.setRotatedDir( orient, input.m_forward );

			input.m_stepInfo.m_deltaTime = m_timestep;
			input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
			input.m_characterGravity.set(0,0,-16);
			input.m_velocity = m_characterProxy->getLinearVelocity();
			input.m_position = m_characterProxy->getPosition();

			hkVector4 down;	down.setNeg4(up);
			hkpSurfaceInfo ground;
			m_characterProxy->checkSupport(down, ground);

			input.m_isSupported = ground.m_supportedState == hkpSurfaceInfo::SUPPORTED;
			input.m_surfaceNormal = ground.m_surfaceNormal;
			input.m_surfaceVelocity = ground.m_surfaceVelocity;

		}


		// Apply the character state machine
		{
			HK_TIMER_BEGIN( "update character state", HK_NULL );

			m_characterContext->update(input, output);

			HK_TIMER_END();
		}

		// Display state
		{
			hkpCharacterStateType state = m_characterContext->getState();
			char * stateStr;

			switch (state)
			{
			case HK_CHARACTER_ON_GROUND:
				stateStr = "On Ground";	break;
			case HK_CHARACTER_JUMPING:
				stateStr = "Jumping"; break;
			case HK_CHARACTER_IN_AIR:
				stateStr = "In Air"; break;
			case HK_CHARACTER_CLIMBING:
				stateStr = "Climbing"; break;
			default:
				stateStr = "Other";	break;
			}
			char buffer[255];
			const int& h = m_env->m_window->getHeight();
			hkString::snprintf(buffer, 255, "State : %s", stateStr);
			m_env->m_textDisplay->outputText(buffer, 20, h-40, 0xffffffff);
		}

		//
		// Handle crouching
		//
		{
			hkBool wantCrouch = (m_env->m_window->getMouse().getButtonState() & HKG_MOUSE_RIGHT_BUTTON)
				|| m_env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2);
			hkBool isCrouching = m_phantom->getCollidable()->getShape() == m_crouchShape;

			// We want to stand
			if (isCrouching && !wantCrouch)
			{
				// Attempt to stand
				swapPhantomShape(m_standShape);
			}

			// We want to crouch
			if (!isCrouching && wantCrouch)
			{
				// Attempt to crouch
				swapPhantomShape(m_crouchShape);
			}
		}

		hkStepInfo si;
		si.m_deltaTime = m_timestep;
		si.m_invDeltaTime = 1.0f / m_timestep;

		MyTriggerIdentifyingPointCollector castCollector;
		MyTriggerIdentifyingPointCollector startPointCollector;

		//Apply the player character controller
		{
			HK_TIMER_BEGIN( "simulate character", HK_NULL );

			// Feed output from state machine into character proxy
			m_characterProxy->setLinearVelocity(output.m_velocity);

			//
			// Here we pass our collectors to pick up 'triggers'
			//
			m_characterProxy->integrateWithCollectors( si, m_world->getGravity(), castCollector, startPointCollector );

			HK_TIMER_END();
		}

		//
		// Here we deal with any 'triggers' we may have encountered.
		//
		{
			// We'll decide now which objects we'd like to consider as being 'hit'
			int i;
			// N.B. If our character were moving fast enough that it might move entirely through a trigger
			// in one step then we should examine what we picked up during the linear casts. This is not necessary
			// unless the character is moving very fast, and and has the disadvantage that it may pick up objects 
			// (during some tentative casts) which the character never actually reached, for example a trigger on 
			// the other side of a very thin wall. We will not use these results here.
			/*
			for(i = 0; i < castCollector.m_objectsWeHit.getSize(); i++)
			{
			removeAndReaddObject(castCollector.m_objectsWeHit[i]);
			}
			*/

			// We'll assume that any body we picked up using the start collector which has a negative distance 
			// was penetrating us during that integrate  - we need to check these here because they aren't picked
			// up during the linear casts.
			for(i = 0; i < startPointCollector.m_objectsWeHit.getSize(); i++)
			{
				if(startPointCollector.m_contacts[i].getDistance() < 0)
				{
					removeAndReaddObject(startPointCollector.m_objectsWeHit[i]);
				}
			}
		}

		m_world->unlock();
	}

	// Do collision detection
	hkDefaultPhysicsDemo::stepDemo();

	{
		m_world->lock();

		// Camera Handling
		{
			const hkReal height = 1.7f;

			hkVector4 from, to;
			to = m_characterProxy->getPosition();
			to.addMul4(height, m_characterProxy->m_up);

			hkVector4 dir;
			dir.setMul4( height, m_characterProxy->m_up );
			dir.addMul4( -8.f, input.m_forward);

			from.setAdd4(to, dir);
			setupDefaultCameras(m_env, from, to, m_characterProxy->m_up, 1.0f);
		}

		m_world->unlock();
	}

	return hkDemo::DEMO_OK;
}


void CharacterAndTriggersDemo::triggerifyWorldObject(hkpWorldObject* wo, int color)
{
		// Add a property so we can identify this as a trigger
	hkpPropertyValue val(1);
	wo->addProperty(HK_OBJECT_IS_TRIGGER, val);

	// Color the trigger so we can see it clearly
	HK_SET_OBJECT_COLOR(static_cast<int>((hkUlong)wo->getCollidable()), color);
}


void CharacterAndTriggersDemo::swapPhantomShape( hkpShape* newShape )
{
	// Remember the current shape
	hkpShape* currentShape = const_cast<hkpShape*>(m_phantom->getCollidable()->getShape());

	// Swap to the new shape.
	// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
	// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
	// may also be present in the world.
	// This also forces the display to be rebuilt, which is necessary for us to see the new shape!
	{
		// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
		// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
		// be accidentally deleted.
		m_world->removePhantom( m_phantom );
		m_phantom->setShape(newShape);
		m_world->addPhantom( m_phantom );
	}

	//
	// We use getClosestPoints to check for penetration
	//
	hkpClosestCdPointCollector collector;
	m_phantom->getClosestPoints( collector );

	// Allow a very slight tolerance (approx 1cm)

	if (collector.hasHit() && collector.getHit().m_contact.getDistance() < .01f)
	{
		// Switch the phantom back to our current shape.
		// N.B. To be safe, we always remove the phantom from the world first, then change the shape,
		// then re-add, in order to refresh the cached agents in any hkCachingShapePhantoms which
		// may also be present in the world.
		// This also forces the display to be rebuilt, which is necessary for us to see the new shape!	
		{
			// Note we do not have to add a reference before removing becasue we hold a hkpCharacterProxy
			// which has a reference to this phantom - hence removal from the world cannot cause this phantom to 
			// be accidentally deleted.
			m_world->removePhantom( m_phantom );
			m_phantom->setShape( currentShape );
			m_world->addPhantom( m_phantom );
		}
	}
}



hkpMoppBvTreeShape* CharacterAndTriggersDemo::getTiledPlanarMoppBvTreeShape(int sideDivisions, hkReal sideLength, hkReal radius)
{
	sideDivisions++;
	//
	//	We use a simple mesh in our example, which copies all data.
	//  If you want to share graphics and physics, use the hkpMeshShape instead
	//
	hkpSimpleMeshShape* meshShape = new hkpSimpleMeshShape;

	{
		for(int i = 0; i < sideDivisions; i++)
		{
			for (int j = 0; j < sideDivisions; j++ )
			{
				hkVector4 vertex ( i * 1.0f - (sideDivisions-1) * 0.5f, j * 1.0f - (sideDivisions-1) * 0.5f, 0.0f );
				vertex.mul4(sideLength/(sideDivisions-1));
				meshShape->m_vertices.pushBack( vertex );
			}
		}
	}

	{
		int corner = 0;
		for(int i = 0; i < sideDivisions - 1; i++)
		{
			for (int j = 0; j < sideDivisions - 1; j++ )
			{
				hkpSimpleMeshShape::Triangle t;
				t.m_a = corner; t.m_b = corner + 1; t.m_c = corner + sideDivisions; t.m_weldingInfo = 0;
				meshShape->m_triangles.pushBack( t );
				t.m_a = corner+1; t.m_b = corner + sideDivisions; t.m_c = corner + sideDivisions + 1;
				meshShape->m_triangles.pushBack( t );
				corner++; 
			}
			corner++; 
		}
	}

	meshShape->m_radius = radius;

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( meshShape ,mci);

	hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape(meshShape, code);
	code->removeReference();
	meshShape->removeReference();

	return moppShape;
}


hkpShape* CharacterAndTriggersDemo::getRandomShape()
{
	int shapeType = (int) (m_generator.getRandRange(0,1) * 6);
	switch(shapeType)
	{
		case 0:
		// Create MOPP body
		{
		
			hkpMoppBvTreeShape* shape = getTiledPlanarMoppBvTreeShape(2, 4.0f);
			return(shape);
		}

		// Create ConvexVertices body
		case 1:
		{
			// Data specific to this shape.
			int numVertices = 4;

				// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
			int stride = 16;

			float vertices[] = { // 4 vertices plus padding
				-2.0f, 1.0f, 1.0f, 0.0f, // v0
				 1.0f, 2.0f, 0.0f, 0.0f, // v1
				 0.0f, 0.0f, 5.0f, 0.0f, // v2
				 1.0f, -1.0f, 0.0f, 0.0f  // v3
			};
			

			hkpConvexVerticesShape* shape;
			hkArray<hkVector4> planeEquations;
			hkGeometry geom;
			{
				hkStridedVertices stridedVerts;
				{
					stridedVerts.m_numVertices = numVertices;
					stridedVerts.m_striding = stride;
					stridedVerts.m_vertices = vertices;
				}

				hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

				{
					stridedVerts.m_numVertices = geom.m_vertices.getSize();
					stridedVerts.m_striding = sizeof(hkVector4);
					stridedVerts.m_vertices = &(geom.m_vertices[0](0));
				}

				shape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
			}

			return(shape);
		}


		// Create Box body
		case 2:
		{
			// Data specific to this shape.
			hkVector4 halfExtents = hkVector4(m_generator.getRandRange(0.5f, 2.0f), m_generator.getRandRange(0.5f, 4.0f), m_generator.getRandRange(0.5f, 2.0f));
			hkpBoxShape* shape = new hkpBoxShape(halfExtents );
			return(shape);
		}

		// Create Sphere body
		case 3:
		{
			hkReal radius = m_generator.getRandRange(1.5f, 4.0f);
			hkpConvexShape* shape = new hkpSphereShape(radius);
			return(shape);
		}

		// Create Triangle body
		case 4:
		{
			hkVector4 a(-m_generator.getRandRange(0.5f, 2.0f), -1.5f,  0.0f);
			hkVector4 b(m_generator.getRandRange(0.5f, 2.0f), -1.5f,  0.0f);
			hkVector4 c(0.0f,  3.5f,  0.0f);

			hkpTriangleShape* shape = new hkpTriangleShape(a, b, c);
			shape->setRadius(0.3f);	// Make fat
			return(shape);
		}

		// Create Capsule body
		case 5:
		{
			hkVector4 a(-1.5f, -2.5f,  0.0f);
			hkVector4 b(1.5f, -2.5f,  0.0f);

			hkpCapsuleShape* shape = new hkpCapsuleShape(a, b, 0.7f);
			return(shape);
		}

	}	// end case


	HK_ASSERT2(0x0, 0, "Failed to build shape.");
	return HK_NULL;

}


hkVector4 CharacterAndTriggersDemo::getRandomPositionAwayFromCharacter(hkReal planerange, hkReal heightrange, hkReal mindist)
{
	hkVector4 pos;
	do
	{
		pos.set(m_generator.getRandRange(-planerange, planerange), m_generator.getRandRange(-planerange, planerange), m_generator.getRandRange(2.0f, heightrange + 2.0f));
		pos.sub4(m_characterProxy->getPosition());
	}while(pos.length3() < mindist);

	pos.add4(m_characterProxy->getPosition());
	return pos;
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This is a test scene for one technique to implement 'phantom' trigger objects " \
"for the Havok Character Controller : Using the custom collectors.\n" \
"Rigid bodies which have been 'tagged' with a 'trigger' property are coloured red. " \
"Phantoms which have been 'tagged' with a 'trigger' property are transparent red. " \
"We remove and re-add when the triggers are 'hit'. Please read class description for more details.";

HK_DECLARE_DEMO(CharacterAndTriggersDemo, HK_DEMO_TYPE_PRIME, "Character 'picking up' objects marked as triggers.", helpString);

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

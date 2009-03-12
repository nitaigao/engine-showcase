/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

// Dynamics includes
#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>				

// Keycode
#include <Common/Base/keycode.cxx>

#if !defined USING_HAVOK_PHYSICS
#error Physics is needed to build this demo. It is included in the common package for reference only.
#endif



// Classlists
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

// Generate a custom list to trim memory requirements
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>

#include <stdio.h>
static void HK_CALL errorReport(const char* msg, void*)
{
	printf("%s", msg);
}

#if defined(HK_PLATFORM_PS3)
#include <Common/Base/Spu/Util/hkSpuUtil.h>
#include <cell/spurs.h>
#endif


//
// Forward declarations
//

void setupPhysics(hkpWorld* physicsWorld);
hkpRigidBody* g_ball;


int HK_CALL main(int argc, const char** argv)
{
	//
	// Do platform specific initialization
	//
#if !defined(HK_PLATFORM_WIN32)
	extern void initPlatform();
	initPlatform();
#endif



	//
	// Initialize the base system including our memory system
	//

	hkPoolMemory* memoryManager = new hkPoolMemory();
	hkThreadMemory* threadMemory = new hkThreadMemory(memoryManager);
	hkBaseSystem::init( memoryManager, threadMemory, errorReport );
	memoryManager->removeReference();

	// We now initialize the stack area to 100k (fast temporary memory to be used by the engine).
	char* stackBuffer;
	{
		int stackSize = 0x100000;
		stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE);
		hkThreadMemory::getInstance().setStackArea( stackBuffer, stackSize);
	}

	{


		//
		// <PHYSICS-ONLY>: Create the physics world.
		// At this point you would initialize any other Havok modules you are using.
		//
		hkpWorld* physicsWorld;
		{
			// The world cinfo contains global simulation parameters, including gravity, solver settings etc.
			hkpWorldCinfo worldInfo;

			// Set the simulation type of the world to multi-threaded.
			worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;

			// Flag objects that fall "out of the world" to be automatically removed - just necessary for this physics scene
			worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

			physicsWorld = new hkpWorld(worldInfo);

			// Disable deactivation, so that you can view timers in the VDB. This should not be done in your game.



			// When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
			// to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
			// must call markForRead / markForWrite before it modifies the world to enable these checks.

		}




		

		//
		// Clean up physics and graphics
		//

		// <PHYSICS-ONLY>: cleanup physics
		{
			physicsWorld->markForWrite();
			physicsWorld->removeReference();
		}



	}

	// Deallocate stack area
	//threadMemory->setStackArea(0, 0);
	//hkDeallocate(stackBuffer);

	//threadMemory->removeReference();

	// Quit base system
	hkBaseSystem::quit();

	return 0;
}



void createBrickWall( hkpWorld* world, int height, int length, const hkVector4& position, hkReal gapWidth, hkpConvexShape* box, hkVector4Parameter halfExtents )
{
	hkVector4 posx = position;
	// do a raycast to place the wall
	{
		hkpWorldRayCastInput ray;
		ray.m_from = posx;
		ray.m_to = posx;

		ray.m_from(1) += 20.0f;
		ray.m_to(1)   -= 20.0f;

		hkpWorldRayCastOutput result;
		world->castRay( ray, result );
		posx.setInterpolate4( ray.m_from, ray.m_to, result.m_hitFraction );
	}
	// move the start point
	posx(0) -= ( gapWidth + 2.0f * halfExtents(0) ) * length * 0.5f;
	posx(1) -= halfExtents(1) + box->getRadius();

	hkArray<hkpEntity*> entitiesToAdd;

	for ( int x = 0; x < length; x ++ )		// along the ground
	{
		hkVector4 pos = posx;
		for( int ii = 0; ii < height; ii++ )
		{
			pos(1) += (halfExtents(1) + box->getRadius())* 2.0f;

			hkpRigidBodyCinfo boxInfo;
			boxInfo.m_mass = 10.0f;
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExtents, boxInfo.m_mass, massProperties);

			boxInfo.m_mass = massProperties.m_mass;
			boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
			boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
			boxInfo.m_solverDeactivation = boxInfo.SOLVER_DEACTIVATION_MEDIUM;
			boxInfo.m_shape = box;
			//boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;
			boxInfo.m_restitution = 0.0f;

			boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

			{
				boxInfo.m_position = pos;
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				world->addEntity( boxRigidBody );
				boxRigidBody->removeReference();
			}

			pos(1) += (halfExtents(1) + box->getRadius())* 2.0f;
			pos(0) += halfExtents(0) * 0.6f;
			{
				boxInfo.m_position = pos;
				hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
				entitiesToAdd.pushBack(boxRigidBody);
			}
			pos(0) -= halfExtents(0) * 0.6f;
		}
		posx(0) += halfExtents(0)* 2.0f + gapWidth;
	}
	world->addEntityBatch( entitiesToAdd.begin(), entitiesToAdd.getSize());

	for (int i=0; i < entitiesToAdd.getSize(); i++){ entitiesToAdd[i]->removeReference(); }
}

void setupPhysics(hkpWorld* physicsWorld)
{
	//
	//  Create the ground box
	//
	{
		hkVector4 groundRadii( 70.0f, 2.0f, 140.0f );
		hkpConvexShape* shape = new hkpBoxShape( groundRadii , 0 );

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = hkVector4( 0.0f, -2.0f, 0.0f );
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

		physicsWorld->addEntity( new hkpRigidBody( ci ) )->removeReference();
		shape->removeReference();
	}

	hkVector4 groundPos( 0.0f, 0.0f, 0.0f );
	hkVector4 posy = groundPos;

	//
	// Create the walls
	//

	int wallHeight = 8;
	int wallWidth  = 8;
	int numWalls = 6;
	hkVector4 boxSize( 1.0f, 0.5f, 0.5f);
	hkpBoxShape* box = new hkpBoxShape( boxSize , 0 );
	box->setRadius( 0.0f );

	hkReal deltaZ = 25.0f;
	posy(2) = -deltaZ * numWalls * 0.5f;

	for ( int y = 0; y < numWalls; y ++ )			// first wall
	{
		createBrickWall( physicsWorld, wallHeight, wallWidth, posy, 0.2f, box, boxSize );
		posy(2) += deltaZ;
	}
	box->removeReference();

	//
	// Create a ball moving towards the walls
	//

	const hkReal radius = 1.5f;
	const hkReal sphereMass = 150.0f;

	hkVector4 relPos( 0.0f,radius + 0.0f, 50.0f );

	hkpRigidBodyCinfo info;
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereMass, massProperties);

	info.m_mass = massProperties.m_mass;
	info.m_centerOfMass  = massProperties.m_centerOfMass;
	info.m_inertiaTensor = massProperties.m_inertiaTensor;
	info.m_shape = new hkpSphereShape( radius );
	info.m_position.setAdd4(posy, relPos );
	info.m_motionType  = hkpMotion::MOTION_BOX_INERTIA;

	info.m_qualityType = HK_COLLIDABLE_QUALITY_BULLET;


	hkpRigidBody* sphereRigidBody = new hkpRigidBody( info );
	g_ball = sphereRigidBody;

	physicsWorld->addEntity( sphereRigidBody );
	sphereRigidBody->removeReference();
	info.m_shape->removeReference();

	hkVector4 vel(  0.0f,4.9f, -100.0f );
	sphereRigidBody->setLinearVelocity( vel );


}

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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

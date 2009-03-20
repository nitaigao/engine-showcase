#include "HavokPhysicsSystem.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

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

#include <Common/Base/keycode.cxx>
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>

#include "../Logging/Logger.h"

ISystemScene* HavokPhysicsSystem::CreateScene()
{
	hkpWorldCinfo info;

	//info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
	info.m_gravity.set( 0, -9.8f, 0 );
	info.m_collisionTolerance = 0.01f; 
	info.m_contactPointGeneration = hkpWorldCinfo::CONTACT_POINT_ACCEPT_ALWAYS;
	info.setBroadPhaseWorldSize( 2050.0f );
	info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );

	HavokPhysicsSystemScene* scene = new HavokPhysicsSystemScene( info );

	_scenes.push_back( scene );

	return scene;
}

void HavokPhysicsSystem::Initialize()
{
	hkPoolMemory* memoryManager = new hkPoolMemory( );
	_threadMemory = new hkThreadMemory( memoryManager );
	hkBaseSystem::init( memoryManager, _threadMemory, errorReportFunction );
	memoryManager->removeReference( );

	int stackSize = 0x100000;
	_stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
	hkThreadMemory::getInstance( ).setStackArea( _stackBuffer, stackSize );
}

void HavokPhysicsSystem::Release()
{
	_threadMemory->setStackArea(0, 0);
	hkDeallocate( _stackBuffer );
	_threadMemory->removeReference();

	hkBaseSystem::quit( );
}

void HavokPhysicsSystem::errorReportFunction( const char* errorMessage, void* errorOutputObject )
{
	Logger::GetInstance( )->Warn( errorMessage );
}
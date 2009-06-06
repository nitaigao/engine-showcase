#include "HavokPhysicsSystemScene_Tests.h"

#include "Physics/HavokPhysicsSystemScene.h"
using namespace Physics;

#include "Logging/Logger.h"
using namespace Logging;

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( HavokPhysicsSystemScene_Tests, Suites::PhysicsSuite( ) );

#include "Physics/HavokPhysicsSystemScene.h"

void HavokPhysicsSystemScene_Tests::setUp()
{
	hkPoolMemory* memoryManager = new hkPoolMemory( );
	_threadMemory = new hkThreadMemory( memoryManager );
	hkBaseSystem::init( memoryManager, _threadMemory, errorReportFunction );
	memoryManager->removeReference( );

	int stackSize = 0x100000;
	_stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
	hkThreadMemory::getInstance( ).setStackArea( _stackBuffer, stackSize );
}

void HavokPhysicsSystemScene_Tests::tearDown()
{
	_threadMemory->setStackArea(0, 0);
	hkDeallocate( _stackBuffer );
	_threadMemory->removeReference();

	hkBaseSystem::quit( );
}

void HavokPhysicsSystemScene_Tests::Should_Return_A_PhysicsSystemComponent_On_CreateComponent()
{
	hkpWorldCinfo info;

	info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
	info.m_gravity.set( 0,-9.8f,0);
	info.m_collisionTolerance = 0.1f; 
	info.setBroadPhaseWorldSize( 150.0f );
	info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );

	HavokPhysicsSystemScene scene( 0 );
	ISystemComponent* component = scene.CreateComponent( "test", "default" );

	CPPUNIT_ASSERT( component != 0 );

	delete component;
}
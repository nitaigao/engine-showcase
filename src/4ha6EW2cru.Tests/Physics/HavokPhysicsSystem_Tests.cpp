#include "HavokPhysicsSystem_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( HavokPhysicsSystem_Tests, Suites::PhysicsSuite( ) );

#include "Physics/HavokPhysicsSystem.h"

void HavokPhysicsSystem_Tests::Should_Initialize_Correctly()
{
	HavokPhysicsSystem system;
	system.Initialize( );
	system.Release( );
}

void HavokPhysicsSystem_Tests::Should_Return_PhysicsScene_On_CreateScene()
{
	HavokPhysicsSystem system;
	system.Initialize( );

	ISystemScene* scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	delete scene;

	scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	delete scene;

	system.Release( );
}
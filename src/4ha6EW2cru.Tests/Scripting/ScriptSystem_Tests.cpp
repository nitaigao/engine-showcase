#include "ScriptSystem_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScriptSystem_Tests, Suites::ScriptingSuite( ) );

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Scripting/ScriptSystem.h"
using namespace Script;

void ScriptSystem_Tests::setUp()
{
	Management::Initialize( );
}

void ScriptSystem_Tests::tearDown()
{
	Management::Release( );
}

void ScriptSystem_Tests::Should_Initialize_Correctly()
{
	ScriptSystem system;
	system.Initialize( 0 );
	system.Release( );
}

void ScriptSystem_Tests::Should_Return_Scene_On_CreateScene()
{
	ScriptSystem system;
	system.Initialize( 0 );

	ISystemScene* scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	scene->Destroy( );

	delete scene;

	system.Release( );
}

void ScriptSystem_Tests::Should_Return_ScriptSystemType_On_GetType()
{
	ScriptSystem system;
	system.Initialize( 0 );

	CPPUNIT_ASSERT( system.GetType( ) == System::Types::SCRIPT );

	system.Release( );
}
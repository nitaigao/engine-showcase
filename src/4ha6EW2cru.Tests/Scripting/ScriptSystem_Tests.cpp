#include "ScriptSystem_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScriptSystem_Tests, Suites::ScriptingSuite( ) );

#include "Logging/Logger.h"
#include "System/Management.h"

#include "Scripting/ScriptSystem.h"

void ScriptSystem_Tests::setUp()
{
	Logger::Initialize( );
	Management::Initialize( );
}

void ScriptSystem_Tests::tearDown()
{
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void ScriptSystem_Tests::Should_Initialize_Correctly()
{
	ScriptSystem system( 0 );
	system.Initialize( );
}

void ScriptSystem_Tests::Should_Return_Scene_On_CreateScene()
{
	ScriptSystem system( 0 );
	system.Initialize( );

	ISystemScene* scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	delete scene;
}

void ScriptSystem_Tests::Should_Return_ScriptSystemType_On_GetType()
{
	ScriptSystem system( 0 );
	system.Initialize( );

	CPPUNIT_ASSERT( system.GetType( ) == ScriptSystemType );
}
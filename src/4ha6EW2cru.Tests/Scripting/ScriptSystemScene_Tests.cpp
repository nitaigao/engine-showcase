#include "ScriptSystemScene_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScriptSystemScene_Tests, Suites::ScriptingSuite( ) );

#include "Logging/Logger.h"
#include "System/Management.h"

#include "Scripting/ScriptSystemScene.h"

void ScriptSystemScene_Tests::setUp()
{
	Logger::Initialize( );
	Management::Initialize( );
}

void ScriptSystemScene_Tests::tearDown()
{
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void ScriptSystemScene_Tests::Should_Initialize_Correctly()
{
	ScriptSystemScene scene( 0 );
	scene.Initialize( );
}
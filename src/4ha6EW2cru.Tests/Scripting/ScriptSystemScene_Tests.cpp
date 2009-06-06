#include "ScriptSystemScene_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScriptSystemScene_Tests, Suites::ScriptingSuite( ) );

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Scripting/ScriptSystemScene.h"
using namespace Script;

void ScriptSystemScene_Tests::setUp()
{
	Management::Initialize( );
}

void ScriptSystemScene_Tests::tearDown()
{
	Management::Release( );
}

void ScriptSystemScene_Tests::Should_Initialize_Correctly()
{
	ScriptSystemScene scene( 0 );
	scene.Initialize( );
	scene.Destroy( );
}
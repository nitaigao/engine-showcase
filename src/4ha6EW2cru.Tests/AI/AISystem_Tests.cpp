#include "AISystem_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AISystem_Tests, Suites::AISuite( ) );

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "AI/AISystem.h"
using namespace AI;

#include "Scripting/ScriptSystem.h"
using namespace Script;

void AISystem_Tests::setUp()
{
	Management::Initialize( );
}

void AISystem_Tests::tearDown()
{
	Management::GetInstance( )->Release( );
}

void AISystem_Tests::Should_Initialize_Correctly()
{
	AISystem system;
	system.Initialize( );
}

void AISystem_Tests::Should_Return_Scene_On_CreateScene()
{
	Management::GetInstance( )->GetSystemManager( )->RegisterSystem( new ScriptSystem( 0 ) );
	Management::GetInstance( )->GetSystemManager( )->InitializeAllSystems( );

	AISystem system;

	ISystemScene* scene = system.CreateScene( );

	CPPUNIT_ASSERT( scene != 0 );

	delete scene;
}
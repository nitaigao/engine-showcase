#include "AISystemScene_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AISystemScene_Tests, Suites::AISuite( ) );

#include "Logging/Logger.h"
#include "System/Management.h"

#include "AI/AISystemScene.h"

#include "Scripting/ScriptSystem.h"

void AISystemScene_Tests::setUp()
{
	Logger::Initialize( );
	Management::Initialize( );

	Management::GetInstance( )->GetSystemManager( )->AddSystem( new ScriptSystem( 0 ) );
}

void AISystemScene_Tests::tearDown()
{
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void AISystemScene_Tests::Should_Return_Component_On_CreateComponent()
{
	AISystemScene scene;
	scene.Initialize( );
 	ISystemComponent* component = scene.CreateComponent( "test", "default" );

	CPPUNIT_ASSERT( component != 0 );

	delete component;
}

void AISystemScene_Tests::Should_Create_ScriptScene_On_Initialize()
{
	AISystemScene scene;
	scene.Initialize( );

	CPPUNIT_ASSERT( scene.GetScriptScene( ) != 0 );;
}

void AISystemScene_Tests::Should_Delete_Component_On_DestroyComponent()
{
	AISystemScene scene;
	scene.Initialize( );
	ISystemComponent* component = scene.CreateComponent( "test", "default" );
	scene.DestroyComponent( component );

	//CPPUNIT_ASSERT( component == 0 );
}
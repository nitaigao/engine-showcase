#include "InputSystemScene_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InputSystemScene_Tests, Suites::InputSuite( ) );

#include "System/ISystemComponent.hpp"
#include "Input/InputSystemScene.h"

void InputSystemScene_Tests::setUp()
{
	Logger::Initialize( );
	Management::Initialize( );
	Management::GetInstance( )->GetPlatformManager( )->CreateInteractiveWindow( "Test Window", 1, 1, false );
}

void InputSystemScene_Tests::tearDown()
{
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void InputSystemScene_Tests::Should_Return_An_InputSystemComponent_On_CreateComponent()
{
	InputSystemScene scene( 0, 0 );

	ISystemComponent* component = scene.CreateComponent( "Test Component", "default" );

	CPPUNIT_ASSERT( component != 0 );

	delete component;
}

void InputSystemScene_Tests::Should_Destroy_Component_On_DestroyComponent()
{
	InputSystemScene scene( 0, 0 );

	ISystemComponent* component = scene.CreateComponent( "Test Component", "default" );

	scene.DestroyComponent( component );
}


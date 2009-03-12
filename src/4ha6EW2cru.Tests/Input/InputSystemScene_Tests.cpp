#include "InputSystemScene_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InputSystemScene_Tests, Suites::InputSuite( ) );

#include "Input/InputSystemScene.h"

void InputSystemScene_Tests::Should_Return_An_InputSystemComponent_On_CreateComponent()
{
	InputSystemScene scene;

	ISystemComponent* component = scene.CreateComponent( "Test Component" );

	CPPUNIT_ASSERT( component != 0 );

	delete component;
}

void InputSystemScene_Tests::Should_Destroy_Component_On_DestroyComponent()
{
	InputSystemScene scene;

	ISystemComponent* component = scene.CreateComponent( "Test Component" );

	scene.DestroyComponent( component );
}
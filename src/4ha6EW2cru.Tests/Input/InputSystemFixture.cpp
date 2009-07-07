#include "InputSystemFixture.h"

#include "Input/InputSystem.h"
using namespace Input;

#include "Logging/Logger.h"
using namespace Logging;

#include "IO/FileSystem.h"
#include "Events/EventManager.h"
#include "Management/Management.h"

#include "Exceptions/IntializeFailedException.hpp"
#include "Exceptions/OutOfRangeException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InputSystemFixture, Suites::InputSuite( ) );

void InputSystemFixture::setUp( )
{
	Management::Initialize( );
}

void InputSystemFixture::tearDown( )
{
	Management::Release( );
}

void InputSystemFixture::Should_Initialize_Properly( )
{
	InputSystem* inputSystem = new InputSystem( );
	inputSystem->Initialize( 0 );

	delete inputSystem;
}

void InputSystemFixture::Should_Throw_Given_Already_Initialized( )
{
	InputSystem* inputSystem = new InputSystem( );
	inputSystem->Initialize( 0 );
	CPPUNIT_ASSERT_THROW( inputSystem->Initialize( 0 ), AlreadyInitializedException );

	delete inputSystem;
}

void InputSystemFixture::Should_Not_Initialize_With_NULL_HWND( )
{	
	InputSystem* inputSystem = new InputSystem( );
	CPPUNIT_ASSERT_THROW( inputSystem->Initialize( 0 ), IntializeFailedException );

	delete inputSystem;
}
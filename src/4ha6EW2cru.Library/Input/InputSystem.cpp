#include "InputSystem.h"

#include <sstream>
#include "../Logging/Logger.h"

#include "InputSystemScene.h"

#include "../Exceptions/IntializeFailedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

void InputSystem::Initialize( ) 
{ 	
	Logger::GetInstance( )->Info( "Initializing Input System" );
}

InputSystem::~InputSystem( )
{

}

ISystemScene* InputSystem::CreateScene()
{
	InputSystemScene* inputScene = new InputSystemScene(
		_configuration->Find< int >( "Graphics", "width" ),
		_configuration->Find< int >( "Graphics", "height" )
		);

	return inputScene;
}
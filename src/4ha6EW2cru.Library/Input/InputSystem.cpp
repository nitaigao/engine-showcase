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

	_systemScenes.push_back( inputScene );

	return inputScene;
}

void InputSystem::SetProperties( PropertyMap properties )
{
	for ( PropertyMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		SystemProperty property = ( *i ).second;
		
		if ( property.GetName( ) == "inputAllowed" )
		{
			for( SystemSceneList::iterator j = _systemScenes.begin( ); j != _systemScenes.end( ); ++j )
			{
				InputSystemScene* inputScene = static_cast< InputSystemScene* >( ( *j ) );

				inputScene->SetInputAllowed( property.GetValue< bool >( ) );
			}
		}
	}

	
}
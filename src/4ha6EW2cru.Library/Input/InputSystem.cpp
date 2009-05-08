#include "InputSystem.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Management/Management.h"

#include "../Exceptions/IntializeFailedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

using namespace OIS;

namespace Input
{
	InputSystem::~InputSystem( )
	{
		_inputManager->destroyInputSystem( _inputManager );
	}
	
	void InputSystem::Initialize( ) 
	{ 	
		Logger::Info( "Initializing Input System" );
	
		_inputManager = OIS::InputManager::createInputSystem( Management::GetInstance( )->GetPlatformManager( )->GetWindowId( ) );
	
		_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
		_keyboard->setEventCallback( this );
	
		_mouse = static_cast< OIS::Mouse* >( _inputManager->createInputObject( OIS::OISMouse, true ) );
		_mouse->setEventCallback( this );
	
		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );

		_mouse->getMouseState( ).width = _configuration->Find( "Graphics", "width" ).GetValue< int >( );
		_mouse->getMouseState( ).height = _configuration->Find( "Graphics", "height" ).GetValue< int >( );
	}
	
	ISystemScene* InputSystem::CreateScene( )
	{
		InputSystemScene* inputScene = new InputSystemScene( _mouse, _keyboard );
	
		_inputScenes.push_back( inputScene );
	
		return inputScene;
	}
	
	void InputSystem::SetProperty( const std::string& name, AnyValue value )
	{
		if ( name == "inputAllowed" )
		{
			for( InputSystemSceneList::iterator j = _inputScenes.begin( ); j != _inputScenes.end( ); ++j )
			{
				( *j )->SetInputAllowed( value.GetValue< bool >( ) );
			}
		}
	}
	
	
	void InputSystem::Update( const float& deltaMilliseconds )
	{
		_mouse->capture( );
		_keyboard->capture( );
	}
	
	bool InputSystem::keyPressed( const KeyEvent &arg )
	{
		for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
		{
			( *i )->KeyPressed( arg );
		}
	
		return true;
	}
	
	bool InputSystem::keyReleased( const KeyEvent &arg )
	{
		for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
		{
			( *i )->KeyReleased( arg );
		}
	
		return true;
	}
	
	/* Fired when the user moves the mouse */
	bool InputSystem::mouseMoved( const MouseEvent &arg )
	{
		for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
		{
			( *i )->MouseMoved( arg );
		}
	
		return true;
	}
	
	/* Fired when the user presses a button on the mouse */
	bool InputSystem::mousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
		{
			( *i )->MousePressed( arg, id );
		}
	
		return true;
	}
	
	/* Fired when the user releases a button on the mouse */
	bool InputSystem::mouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
		{
			( *i )->MouseReleased( arg, id );
		}
	
		return true;
	}
	
	AnyValue::AnyValueMap InputSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;
	
		if ( actionName == "setInputAllowed" )
		{
			for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
			{
				( *i )->SetInputAllowed( parameters[ "inputAllowed" ].GetValue< bool >( ) );
			}
		}
	
		return results;
	}
}
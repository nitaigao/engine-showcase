#include "InputSystem.h"

#include <sstream>
#include "../Logging/Logger.h"

#include "../Exceptions/IntializeFailedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
#include "../Scripting/ScriptEvent.hpp"

using namespace OIS;

InputSystem::~InputSystem( )
{
	_inputManager->destroyInputSystem( _inputManager );
}

void InputSystem::Initialize( ) 
{ 	
	Logger::GetInstance( )->Info( "Initializing Input System" );

	_inputManager = OIS::InputManager::createInputSystem( Management::GetInstance( )->GetPlatformManager( )->GetHwnd( ) );

	_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
	_keyboard->setEventCallback( this );

	_mouse = static_cast< OIS::Mouse* >( _inputManager->createInputObject( OIS::OISMouse, true ) );
	_mouse->setEventCallback( this );
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


void InputSystem::Update( float deltaMilliseconds )
{
	_mouse->capture( );
	_keyboard->capture( );

	_mouse->getMouseState( ).width = _configuration->Find< int >( "Graphics", "width" );
	_mouse->getMouseState( ).height = _configuration->Find< int >( "Graphics", "height" );
}

bool InputSystem::keyPressed( const KeyEvent &arg )
{
	for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
	{
		( *i )->keyPressed( arg );
	}

	if ( arg.key != OIS::KC_GRAVE )
	{
		Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key, _keyboard->getAsString( arg.key ) ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );
	}

	return true;
}

bool InputSystem::keyReleased( const KeyEvent &arg )
{
	for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
	{
		( *i )->keyReleased( arg );
	}

	if ( arg.key == OIS::KC_GRAVE )
	{
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( new ScriptEvent( "UI_CONSOLE" ) );
	}
	else
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_KEY_UP", arg.key, _keyboard->getAsString( arg.key ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );

		IEvent* event = new Event( INPUT_KEY_UP, new KeyEventData( arg.key, _keyboard->getAsString( arg.key ) ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );
	}

	return true;
}

/* Fired when the user moves the mouse */
bool InputSystem::mouseMoved( const MouseEvent &arg )
{
	for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
	{
		( *i )->mouseMoved( arg );
	}

	Event* event = new Event( INPUT_MOUSE_MOVED, new MouseEventData( arg.state, OIS::MB_Left ) );
	Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

	return true;
}

/* Fired when the user presses a button on the mouse */
bool InputSystem::mousePressed( const MouseEvent &arg, MouseButtonID id )
{
	for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
	{
		( *i )->mousePressed( arg, id );
	}

	Event* event = new Event( INPUT_MOUSE_PRESSED, new MouseEventData( arg.state, id ) );
	Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

	return true;
}

/* Fired when the user releases a button on the mouse */
bool InputSystem::mouseReleased( const MouseEvent &arg, MouseButtonID id )
{
	for( InputSystemSceneList::iterator i = _inputScenes.begin( ); i != _inputScenes.end( ); ++i )
	{
		( *i )->mouseReleased( arg, id );
	}

	Event* event = new Event( INPUT_MOUSE_RELEASED, new MouseEventData( arg.state, id ) );
	Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

	return true;
}
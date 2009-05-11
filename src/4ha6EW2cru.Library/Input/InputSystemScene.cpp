#include "InputSystemScene.h"

using namespace OIS;

#include "../Management/Management.h"

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
using namespace Events;

#include "InputSystemComponent.h"
#include "KeyboardInputComponent.h"
#include "MouseInputComponent.h"

namespace Input
{
	ISystemComponent* InputSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		ISystemComponent* inputComponent = 0;

		if ( type == "keyboard" )
		{
			inputComponent = new KeyboardInputComponent( name, _keyboard );
		}
		else if ( type == "mouse" )
		{
			inputComponent = new MouseInputComponent( name, _mouse );
		}
		else
		{
			inputComponent = new InputSystemComponent( name );
		}

		_inputComponents.push_back( inputComponent );

		return inputComponent;
	}

	void InputSystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
			if ( component->GetName( ) == ( *i )->GetName( ) )
			{
				_inputComponents.erase( i );
				break;
			}
		}

		delete component;
		component = 0;
	}

	void InputSystemScene::Update( const float& deltaMilliseconds )
	{
		if ( _inputAllowed )
		{
			for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
			{
				( *i )->Update( deltaMilliseconds );
			}

			const_cast< MouseState& >( _mouse->getMouseState( ) ).X.abs = _mouse->getMouseState( ).width / 2;
			const_cast< MouseState& >( _mouse->getMouseState( ) ).Y.abs = _mouse->getMouseState( ).height / 2;
		}
	}

	bool InputSystemScene::KeyPressed( const KeyEvent &arg )
	{
		if ( arg.key != OIS::KC_GRAVE )
		{
			Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key, _keyboard->getAsString( arg.key ) ) );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );
		}

		if ( _inputAllowed )
		{
			unsigned int changes = 0;



			/*if ( changes > 0 )
			{
			for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
			{
			( *i )->PushChanges( changes );
			}
			}*/
		}

		return true;
	}

	bool InputSystemScene::KeyReleased( const KeyEvent &arg )
	{
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

		/*if ( _inputAllowed )
		{
		for( InputSystemComponentList::iterator i = _inputComponents.begin( ); i != _inputComponents.end( ); ++i  )
		{
		( *i )->KeyUp( arg.key, _keyboard->getAsString( arg.key ) );
		}
		}*/

		return true;
	}

	/* Fired when the user moves the mouse */
	bool InputSystemScene::MouseMoved( const MouseEvent &arg )
	{
		Event* event = new Event( INPUT_MOUSE_MOVED, new MouseEventData( arg.state, OIS::MB_Left ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

	/* Fired when the user presses a button on the mouse */
	bool InputSystemScene::MousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );

		Event* event = new Event( INPUT_MOUSE_PRESSED, new MouseEventData( arg.state, id ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

	/* Fired when the user releases a button on the mouse */
	bool InputSystemScene::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{	
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_RELEASED", id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );

		Event* event = new Event( INPUT_MOUSE_RELEASED, new MouseEventData( arg.state, id ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}
}
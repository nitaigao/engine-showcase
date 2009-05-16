#include "InputSystemScene.h"

using namespace OIS;

#include "../Management/Management.h"

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
using namespace Events;

#include "InputSystemComponent.h"

namespace Input
{
	ISystemComponent* InputSystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		IInputSystemComponent* inputComponent = new InputSystemComponent( name, m_mouse, m_keyboard );
		m_inputComponents.push_back( inputComponent );

		return inputComponent;
	}

	void InputSystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
		{
			if ( component->GetName( ) == ( *i )->GetName( ) )
			{
				m_inputComponents.erase( i );
				break;
			}
		}

		delete component;
		component = 0;
	}

	void InputSystemScene::Update( const float& deltaMilliseconds )
	{
		if ( m_inputAllowed )
		{
			for( InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->Update( deltaMilliseconds );
			}

			const_cast< MouseState& >( m_mouse->getMouseState( ) ).X.abs = m_mouse->getMouseState( ).width / 2;
			const_cast< MouseState& >( m_mouse->getMouseState( ) ).Y.abs = m_mouse->getMouseState( ).height / 2;
		}
	}

	bool InputSystemScene::KeyPressed( const KeyEvent &arg )
	{
		if ( arg.key != OIS::KC_GRAVE )
		{
			Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key, m_keyboard->getAsString( arg.key ) ) );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );
		}

		return true;
	}

	bool InputSystemScene::KeyReleased( const KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_GRAVE )
		{
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( new ScriptEvent( "UI_CONSOLE" ) );
		}
		else if ( arg.key == OIS::KC_F12 )
		{
			IService* renderService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );
			renderService->Execute( "screenShot", AnyValue::AnyValueMap( ) );
		}
		else
		{
			IEvent* scriptEvent = new ScriptEvent( "INPUT_KEY_UP", arg.key, m_keyboard->getAsString( arg.key ) );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );

			IEvent* event = new Event( INPUT_KEY_UP, new KeyEventData( arg.key, m_keyboard->getAsString( arg.key ) ) );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );
		}

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
		if ( m_inputAllowed )
		{
			for( InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->MouseReleased( arg, id );
			}
		}

		Event* event = new Event( INPUT_MOUSE_RELEASED, new MouseEventData( arg.state, id ) );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}
}
#include "InputSystemScene.h"

using namespace OIS;

#include "../Service/IService.hpp"
using namespace Services;

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
		IInputSystemComponent* component = new InputSystemComponent( );

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::INPUT );
		component->SetAttribute( System::Attributes::Parent, static_cast< IInputSystemScene* >( this ) );

		m_inputComponents.push_back( component );

		return component;
	}

	void InputSystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
		{
			if ( component == ( *i ) )
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
			for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->SetAttribute( System::Parameters::InvertYAxis, m_configuration->Find( System::ConfigSections::Input, "inverty" ).As< bool >( ) );
				( *i )->SetAttribute( System::Parameters::SmoothMouse, m_configuration->Find( System::ConfigSections::Input, "smoothmouse" ).As< bool >( ) );
				( *i )->SetAttribute( System::Parameters::MouseSensitivity, m_configuration->Find( System::ConfigSections::Input, "mousesmooth_amount" ).As< int >( ) );
				( *i )->Update( deltaMilliseconds );
			}

			const_cast< MouseState& >( m_system->GetMouse( )->getMouseState( ) ).X.abs = m_system->GetMouse( )->getMouseState( ).width / 2;
			const_cast< MouseState& >( m_system->GetMouse( )->getMouseState( ) ).Y.abs = m_system->GetMouse( )->getMouseState( ).height / 2;
		}
	}

	bool InputSystemScene::KeyPressed( const KeyEvent &arg )
	{
		if ( arg.key != OIS::KC_GRAVE )
		{
			Event* event = new Event( INPUT_KEY_DOWN, new KeyEventData( arg.key, m_system->GetKeyboard( )->getAsString( arg.key ) ) );
			Management::Get( )->GetEventManager( )->TriggerEvent( event );

			if ( m_inputAllowed )
			{
				for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
				{
					( *i )->KeyPressed( arg );
				}
			}
		}

		return true;
	}

	bool InputSystemScene::KeyReleased( const KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_GRAVE )
		{
			Management::Get( )->GetEventManager( )->TriggerEvent( new ScriptEvent( "UI_CONSOLE" ) );
		}
		else if ( arg.key == OIS::KC_F12 )
		{
			IService* renderService = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );
			renderService->Execute( "screenShot", AnyType::AnyTypeMap( ) );
		}
		else
		{
			IEvent* scriptEvent = new ScriptEvent( "INPUT_KEY_UP", arg.key, m_system->GetKeyboard( )->getAsString( arg.key ) );
			Management::Get( )->GetEventManager( )->TriggerEvent( scriptEvent );

			IEvent* event = new Event( INPUT_KEY_UP, new KeyEventData( arg.key, m_system->GetKeyboard( )->getAsString( arg.key ) ) );
			Management::Get( )->GetEventManager( )->TriggerEvent( event );

			if ( m_inputAllowed )
			{
				for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
				{
					( *i )->KeyReleased( arg );
				}
			}
		}

		return true;
	}

	bool InputSystemScene::MouseMoved( const MouseEvent &arg )
	{
		Event* event = new Event( INPUT_MOUSE_MOVED, new MouseEventData( arg.state, OIS::MB_Left ) );
		Management::Get( )->GetEventManager( )->TriggerEvent( event );

		if ( m_inputAllowed )
		{
			for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->MouseMoved( arg );
			}
		}

		return true;
	}

	bool InputSystemScene::MousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		if ( m_inputAllowed )
		{
			for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->MousePressed( arg, id );
			}
		}

		Event* event = new Event( INPUT_MOUSE_PRESSED, new MouseEventData( arg.state, id ) );
		Management::Get( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}

	bool InputSystemScene::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{	
		if ( m_inputAllowed )
		{
			for( IInputSystemComponent::InputSystemComponentList::iterator i = m_inputComponents.begin( ); i != m_inputComponents.end( ); ++i  )
			{
				( *i )->MouseReleased( arg, id );
			}
		}

		Event* event = new Event( INPUT_MOUSE_RELEASED, new MouseEventData( arg.state, id ) );
		Management::Get( )->GetEventManager( )->TriggerEvent( event );

		return true;
	}
}
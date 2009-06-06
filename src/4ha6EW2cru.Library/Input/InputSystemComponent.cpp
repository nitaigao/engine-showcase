#include "InputSystemComponent.h"

using namespace OIS;

#include "../Maths/MathQuaternion.hpp"
using namespace Maths;

#include "../Events/IEvent.hpp"
using namespace Events;

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Management/Management.h"

#include "IInputSystemScene.hpp"
#include "IInputSystem.hpp"

#include <OIS/OISKeyboard.h>

namespace Input
{	
	void InputSystemComponent::Initialize()
	{
		for( int i = 0; i < m_historySize; i++ )
		{
			m_xHistory.push_front( 0.0f );
			m_yHistory.push_front( 0.0f );
		}
	}

	AnyType InputSystemComponent::PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		for( ObserverList::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			( *i )->Message( message, parameters );
		}

		return AnyType( );
	}

	AnyType InputSystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].As< MathVector3 >( );
		}

		if ( message == System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );
		}

		return AnyType( );
	}

	void InputSystemComponent::Update( const float& deltaMilliseconds )
	{
		OIS::Mouse* mouse = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( )->GetMouse( );

		// Mouse Movement

		MouseState mouseState = mouse->getMouseState( );

		float mouseY = 0;

		if ( m_attributes[ System::Parameters::InvertYAxis ].As< bool >( ) )
		{
			mouseY = -mouseState.Y.rel;
		}
		else
		{
			mouseY = mouseState.Y.rel;
		}

		m_xHistory.pop_back( );
		m_xHistory.push_front( mouseState.X.rel );

		m_yHistory.pop_back( );
		m_yHistory.push_front( mouseY );

		m_attributes[ System::Parameters::DeltaX ] = this->AverageInputHistory( m_xHistory );
		m_attributes[ System::Parameters::DeltaY ] = this->AverageInputHistory( m_yHistory );

		this->PushMessage( System::Messages::Mouse_Moved, m_attributes );
	}

	void InputSystemComponent::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_RELEASED", m_attributes[ System::Attributes::Name ].As< std::string >( ), id );
		Management::GetEventManager( )->TriggerEvent( scriptEvent );

		for( InputMessageBinding::InputMessageBindingList::iterator i = m_mouseUpMessages.begin( ); i != m_mouseUpMessages.end( ); )
		{
			if ( ( *i ).GetType( ) == BINDING_MOUSE && id == static_cast< OIS::MouseButtonID >( ( *i ).GetCode( ) ) )
			{
				std::string newMessage = ( *i ).GetMessage( ).replace( ( *i ).GetMessage( ).find_first_of( "+" ), 1, "-" );
				this->PushMessage( newMessage, m_attributes );
				i = m_mouseUpMessages.erase( i );
			}
			else
			{
				++i;
			}
		}
	}

	void InputSystemComponent::MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", m_attributes[ System::Attributes::Name ].As< std::string >( ), id );
		Management::GetEventManager( )->TriggerEvent( scriptEvent );

		IInputSystem* inputSystem = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( );
		InputMessageBinding::InputMessageBindingList messageBindings = inputSystem->GetBindings( );

		for( InputMessageBinding::InputMessageBindingList::iterator i = messageBindings.begin( ); i != messageBindings.end( ); ++i )
		{
			if ( ( *i ).GetType( ) == BINDING_MOUSE )
			{
				if ( id == static_cast< OIS::MouseButtonID >( ( *i ).GetCode( ) ) )
				{
					if ( ( *i ).GetMessage( ).find( "+" ) != std::string::npos )
					{
						m_mouseUpMessages.push_back( ( *i ) );
					}

					this->PushMessage( ( *i ).GetMessage( ), m_attributes );
				}
			}
		}
	}

	float InputSystemComponent::AverageInputHistory( const InputHistory& inputHistory )
	{
		int index = 0;
		float sum = 0.0f;

		float modifier = static_cast< float >( m_attributes[ System::Parameters::MouseSensitivity ].As< int >( ) ) / 100.0f;

		for ( InputHistory::const_iterator i = inputHistory.begin( ); i != inputHistory.end( ); ++i )
		{
			sum += ( *i ) * pow( modifier, index++ );

			if ( m_attributes[ System::Parameters::SmoothMouse ].As< bool >( ) )
			{
				break;
			}
		}

		return sum / m_historySize;
	}

	void InputSystemComponent::KeyReleased( const KeyEvent &arg )
	{
		for( InputMessageBinding::InputMessageBindingList::iterator i = m_keyUpMessages.begin( ); i != m_keyUpMessages.end( ); )
		{
			if ( ( *i ).GetType( ) == BINDING_KEYBOARD )
			{
				if ( arg.key == static_cast< OIS::KeyCode >( ( *i ).GetCode( ) ) )
				{
					std::string newMessage = ( *i ).GetMessage( ).replace( ( *i ).GetMessage( ).find_first_of( "+" ), 1, "-" );
					this->PushMessage( newMessage, m_attributes );
					i = m_keyUpMessages.erase( i );
					return;
				}
			}

			++i;
		}
	}

	void InputSystemComponent::KeyPressed( const OIS::KeyEvent &arg )
	{
		IInputSystem* inputSystem = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( );
		InputMessageBinding::InputMessageBindingList messageBindings = inputSystem->GetBindings( );

		for( InputMessageBinding::InputMessageBindingList::iterator i = messageBindings.begin( ); i != messageBindings.end( ); ++i )
		{
			if ( ( *i ).GetType( ) == BINDING_KEYBOARD )
			{
				if ( arg.key == static_cast< OIS::KeyCode >( ( *i ).GetCode( ) ) )
				{
					if ( ( *i ).GetMessage( ).find( "+" ) != std::string::npos )
					{
						m_keyUpMessages.push_back( ( *i ) );
					}

					this->PushMessage( ( *i ).GetMessage( ), m_attributes );
				}
			}
		}
	}
}
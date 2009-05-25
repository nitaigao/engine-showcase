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
		OIS::Keyboard* keyboard = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( )->GetKeyboard( );
		OIS::Mouse* mouse = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( )->GetMouse( );

		System::Message moveMessage;
		System::Message attackMessage;

		IInputSystem* inputSystem = m_attributes[ System::Attributes::Parent ].As< IInputSystemScene* >( )->GetSystem( );
		InputMessageBinding::InputMessageBindingList messageBindings = inputSystem->GetBindings( );

		for( InputMessageBinding::InputMessageBindingList::iterator i = messageBindings.begin( ); i != messageBindings.end( ); ++i )
		{
			switch( ( *i ).GetType( ) )
			{

			case BINDING_KEYBOARD: 

				if ( keyboard->isKeyDown( static_cast< OIS::KeyCode >( ( *i ).GetCode( ) ) ) )
				{
					moveMessage = ( *i ).GetMessage( );
					this->PushMessage( ( *i ).GetMessage( ), m_attributes );
				}

				break;

			case BINDING_MOUSE:

				if ( mouse->getMouseState( ).buttonDown( static_cast< OIS::MouseButtonID >( ( *i ).GetCode( ) ) ) )
				{
					attackMessage = ( *i ).GetMessage( );
					this->PushMessage( ( *i ).GetMessage( ), m_attributes );
				}

				break;
			}
		}

		if ( moveMessage.empty( ) )
		{
			this->PushMessage( System::Messages::Move_Idle, m_attributes );
		}

		if ( attackMessage.empty( ) )
		{
			this->PushMessage( System::Messages::Attack_Idle, m_attributes );
		}

		// Mouse Movement

		MouseState mouseState = mouse->getMouseState( );

		float mouseY = 0;

		if ( m_attributes[ System::Attributes::InvertYAxis ].As< bool >( ) )
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

		m_attributes[ System::Attributes::DeltaX ] = this->AverageInputHistory( m_xHistory );
		m_attributes[ System::Attributes::DeltaY ] = this->AverageInputHistory( m_yHistory );

		this->PushMessage( System::Messages::Mouse_Moved, m_attributes );
	}

	void InputSystemComponent::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_RELEASED", m_attributes[ System::Attributes::Name ].As< std::string >( ), id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
	}

	void InputSystemComponent::MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", m_attributes[ System::Attributes::Name ].As< std::string >( ), id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
	}

	float InputSystemComponent::AverageInputHistory( const InputHistory& inputHistory )
	{
		int index = 0;
		float sum = 0.0f;

		float modifier = static_cast< float >( m_attributes[ System::Attributes::MouseSensitivity ].As< int >( ) ) / 100.0f;

		for ( InputHistory::const_iterator i = inputHistory.begin( ); i != inputHistory.end( ); ++i )
		{
			sum += ( *i ) * pow( modifier, index++ );

			if ( m_attributes[ System::Attributes::SmoothMouse ].As< bool >( ) )
			{
				break;
			}
		}

		return sum / m_historySize;
	}
}
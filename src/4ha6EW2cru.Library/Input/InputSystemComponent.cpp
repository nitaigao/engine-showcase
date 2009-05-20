#include "InputSystemComponent.h"

using namespace OIS;

#include "../Maths/MathQuaternion.hpp"
using namespace Maths;

#include "../Events/IEvent.hpp"
using namespace Events;

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Management/Management.h"

namespace Input
{	
	AnyValue InputSystemComponent::PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		for( ObserverList::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			( *i )->Message( messageId, parameters );
		}

		return AnyValue( );
	}

	AnyValue InputSystemComponent::Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters )
	{
		if ( messageId & System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].GetValue< MathVector3 >( );
		}

		if ( messageId & System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].GetValue< MathQuaternion >( );
		}

		return AnyValue( );
	}

	void InputSystemComponent::Update( const float& deltaMilliseconds )
	{
		unsigned int moveMessage = 0;

		// Keyboard

		if ( m_keyboard->isKeyDown( OIS::KC_W ) )
		{
			moveMessage |= System::Messages::Move_Forward;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_S ) )
		{
			moveMessage |= System::Messages::Move_Backward;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_A ) )
		{
			moveMessage |= System::Messages::Strafe_Left;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_D ) )
		{
			moveMessage |= System::Messages::Strafe_Right;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_ESCAPE ) )
		{
			moveMessage |= System::Messages::Pause_Game;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_SPACE ) )
		{
			moveMessage |= System::Messages::Jump;
		}

		if ( moveMessage == 0 )
		{
			moveMessage |= System::Messages::Move_Idle;
		}

		// Mouse

		MouseState mouseState = m_mouse->getMouseState( );

		m_attributes[ System::Attributes::DeltaX ] = mouseState.X.rel;
		m_attributes[ System::Attributes::DeltaY ] = mouseState.Y.rel;

		moveMessage |= System::Messages::Mouse_Moved;

		unsigned int attackMessage = 0;

		if ( m_mouse->getMouseState( ).buttonDown( MB_Left ) )
		{
			attackMessage |= System::Messages::Fire;
		}
		else
		{
			attackMessage |= System::Messages::Attack_Idle;
		}

		this->PushMessage( moveMessage |= attackMessage, m_attributes );
	}

	void InputSystemComponent::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_RELEASED", m_name, id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
	}

	void InputSystemComponent::MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", m_name, id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
	}
}
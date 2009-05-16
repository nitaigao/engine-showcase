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
	void InputSystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		for( ObserverList::iterator i = m_observers.begin( ); i != m_observers.end( ); ++i )
		{
			( *i )->Observe( this, systemChanges );
		}
	}

	void InputSystemComponent::Update( const float& deltaMilliseconds )
	{
		unsigned int changes = 0;

		// Keyboard

		if ( m_keyboard->isKeyDown( OIS::KC_W ) )
		{
			changes |= System::Changes::Input::Move_Forward;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_S ) )
		{
			changes |= System::Changes::Input::Move_Backward;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_A ) )
		{
			changes |= System::Changes::Input::Strafe_Left;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_D ) )
		{
			changes |= System::Changes::Input::Strafe_Right;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_ESCAPE ) )
		{
			changes |= System::Changes::Input::Pause_Game;
		}

		if ( m_keyboard->isKeyDown( OIS::KC_SPACE ) )
		{
			changes |= System::Changes::Input::Jump;
		}

		// Mouse

		MouseState mouseState = m_mouse->getMouseState( );

		m_attributes[ "yDelta" ] = mouseState.Y.rel;
		m_attributes[ "xDelta" ] = mouseState.X.rel;

		changes |= System::Changes::Input::Mouse_Moved;

		if ( m_mouse->getMouseState( ).buttonDown( MB_Left ) )
		{
			changes |= System::Changes::Input::Fire;

			IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_PRESSED", MB_Left );
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
		}

		this->PushChanges( changes );
	}

	void InputSystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* compoenent = static_cast< ISystemComponent* >( subject );

		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			m_orientation = compoenent->GetOrientation( );
		}

		if ( System::Changes::Geometry::Position & systemChanges )
		{
			m_position = compoenent->GetPosition( );
		}
	}

	void InputSystemComponent::MouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		IEvent* scriptEvent = new ScriptEvent( "INPUT_MOUSE_RELEASED", id );
		Management::GetInstance( )->GetEventManager( )->TriggerEvent( scriptEvent );
	}
}
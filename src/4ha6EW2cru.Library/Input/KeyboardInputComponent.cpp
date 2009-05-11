#include "KeyboardInputComponent.h"

namespace Input
{
	void Input::KeyboardInputComponent::Update( const float& deltaMilliseconds )
	{
		unsigned int changes = 0;

		if ( _keyboard->isKeyDown( OIS::KC_W ) )
		{
			changes |= System::Changes::Input::Move_Forward;
		}

		if ( _keyboard->isKeyDown( OIS::KC_S ) )
		{
			changes |= System::Changes::Input::Move_Backward;
		}

		if ( _keyboard->isKeyDown( OIS::KC_A ) )
		{
			changes |= System::Changes::Input::Strafe_Left;
		}

		if ( _keyboard->isKeyDown( OIS::KC_D ) )
		{
			changes |= System::Changes::Input::Strafe_Right;
		}

		if ( _keyboard->isKeyDown( OIS::KC_ESCAPE ) )
		{
			changes |= System::Changes::Input::Pause_Game;
		}

		if ( _keyboard->isKeyDown( OIS::KC_SPACE ) )
		{
			changes |= System::Changes::Input::Jump;
		}

		this->PushChanges( changes );
	}

	void KeyboardInputComponent::PushChanges( const unsigned int& systemChanges )
	{
		for( ObserverList::iterator i = _observers.begin( ); i != _observers.end( ); ++i )
		{
			( *i )->Observe( this, systemChanges );
		}
	}
}
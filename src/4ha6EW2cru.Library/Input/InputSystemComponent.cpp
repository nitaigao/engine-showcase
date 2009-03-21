#include "InputSystemComponent.h"

void InputSystemComponent::KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText )
{
	switch( keyCode )
	{

	case OIS::KC_W:

		this->PushChanges( System::Changes::Input::Move_Forward );

		break;

	case OIS::KC_S:

		this->PushChanges( System::Changes::Input::Move_Backward );

		break;

	case OIS::KC_A:

		this->PushChanges( System::Changes::Input::Strafe_Left );

		break;

	case OIS::KC_D:

		this->PushChanges( System::Changes::Input::Strafe_Right );

		break;

	case OIS::KC_ESCAPE:

		this->PushChanges( System::Changes::Input::Pause_Game );

		break;

	}
}

void InputSystemComponent::KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText )
{

}

void InputSystemComponent::PushChanges( unsigned int systemChanges )
{
	_observer->Observe( this, systemChanges );
}
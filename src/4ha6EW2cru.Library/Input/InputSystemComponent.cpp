#include "InputSystemComponent.h"

void InputSystemComponent::KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText )
{
	/* So basically we want to change the position, scale and orientation of thsi object and post the changes
	*  as geomertry changes, this will mean updating the ISystemObject interface to have the Get methods
	*  for each of these properties and simply calling push changes after weve made them */

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

void InputSystemComponent::PushChanges( unsigned int systemChanges )
{
	_observer->Observe( this, systemChanges );
}
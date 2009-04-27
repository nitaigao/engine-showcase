#include "InputSystemComponent.h"

namespace Input
{
	void InputSystemComponent::KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText )
	{
	
	}
	
	void InputSystemComponent::KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText )
	{
	
	}
	
	void InputSystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		_observer->Observe( this, systemChanges );
	}
}
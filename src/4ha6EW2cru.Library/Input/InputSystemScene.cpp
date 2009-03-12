#include "InputSystemScene.h"

#include "InputSystemComponent.h"

ISystemComponent* InputSystemScene::CreateComponent( const std::string& name )
{
	return new InputSystemComponent( name );
}

void InputSystemScene::DestroyComponent( ISystemComponent* component )
{
	delete component;
	component = 0;
}
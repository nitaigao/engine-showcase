#include "GeometrySystemScene.h"

#include "GeometrySystemComponent.h"

ISystemComponent* GeometrySystemScene::CreateComponent( const std::string& name )
{
	return new GeometrySystemComponent( name );
}

void GeometrySystemScene::DestroyComponent( ISystemComponent* component )
{
	delete component;
}
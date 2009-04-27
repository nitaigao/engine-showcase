#include "GeometrySystemScene.h"

#include "GeometrySystemComponent.h"

namespace Geometry
{
	ISystemComponent* GeometrySystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		return new GeometrySystemComponent( name );
	}

	void GeometrySystemScene::DestroyComponent( ISystemComponent* component )
	{
		delete component;
	}
}
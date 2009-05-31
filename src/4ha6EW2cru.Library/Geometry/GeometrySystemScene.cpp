#include "GeometrySystemScene.h"

#include "GeometrySystemComponent.h"

namespace Geometry
{
	ISystemComponent* GeometrySystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		ISystemComponent* component = new GeometrySystemComponent( name );
		
		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::GEOMETRY );
		component->SetAttribute( System::Attributes::Parent, this );

		return component;
	}

	void GeometrySystemScene::DestroyComponent( ISystemComponent* component )
	{
		delete component;
	}
}
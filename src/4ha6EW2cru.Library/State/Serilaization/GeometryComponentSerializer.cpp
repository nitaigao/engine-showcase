#include "GeometryComponentSerializer.h"

#include <yaml.h>
#include "../../Maths/MathVector3.hpp"
#include "../../Maths/MathQuaternion.hpp"
using namespace Maths;

namespace Serialization
{
	ISystemComponent* GeometryComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		ISystemScene::SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::GEOMETRY );

		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	
		float x, y, z, w;
	
		componentNode[ "position" ][ "x" ] >> x;
		componentNode[ "position" ][ "y" ] >> y;
		componentNode[ "position" ][ "z" ] >> z;

		systemComponent->SetAttribute( System::Attributes::Position, MathVector3( x, y, z ) );


		componentNode[ "scale" ][ "x" ] >> x;
		componentNode[ "scale" ][ "y" ] >> y;
		componentNode[ "scale" ][ "z" ] >> z;

		systemComponent->SetAttribute( System::Attributes::Scale, MathVector3( x, y, z ) );


		componentNode[ "orientation" ][ "w" ] >> w;
		componentNode[ "orientation" ][ "x" ] >> x;
		componentNode[ "orientation" ][ "y" ] >> y;
		componentNode[ "orientation" ][ "z" ] >> z;
		systemComponent->SetAttribute( System::Attributes::Orientation, MathQuaternion( x, y, z, w ) );

		systemComponent->Initialize( );
	
		return systemComponent;
	}
}

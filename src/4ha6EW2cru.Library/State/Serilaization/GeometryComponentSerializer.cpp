#include "GeometryComponentSerializer.h"

#include <yaml.h>
#include "../../Maths/MathVector3.hpp"
#include "../../Maths/MathQuaternion.hpp"
using namespace Maths;

namespace Serialization
{
	ISystemComponent* GeometryComponentSerializer::DeSerialize( const std::string& entityName, const YAML::Node& componentNode, const ISystemScene::SystemSceneMap& systemScenes )
	{
		AnyValue::AnyValueMap properties;
	
		float x, y, z, w;
	
		componentNode[ "position" ][ "x" ] >> x;
		componentNode[ "position" ][ "y" ] >> y;
		componentNode[ "position" ][ "z" ] >> z;
		properties.insert( std::make_pair( "position", MathVector3( x, y, z ) ) );
	
		componentNode[ "scale" ][ "x" ] >> x;
		componentNode[ "scale" ][ "y" ] >> y;
		componentNode[ "scale" ][ "z" ] >> z;
		properties.insert( std::make_pair( "scale", MathVector3( x, y, z ) ) );
	
		componentNode[ "orientation" ][ "w" ] >> w;
		componentNode[ "orientation" ][ "x" ] >> x;
		componentNode[ "orientation" ][ "y" ] >> y;
		componentNode[ "orientation" ][ "z" ] >> z;
		properties.insert( std::make_pair( "orientation", MathQuaternion( x, y, z, w ) ) );
	
		SystemSceneMap::const_iterator systemScene = systemScenes.find( System::Types::GEOMETRY );
	
		ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
		systemComponent->Initialize( properties );
	
		return systemComponent;
	}
}

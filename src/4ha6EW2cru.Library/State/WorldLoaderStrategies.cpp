#include "WorldLoaderStrategies.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

IWorldLoader_ComponentStrategy* WorldLoader_ComponentStrategy_Factory::Create( const std::string& componentType )
{
	IWorldLoader_ComponentStrategy* strategy = 0;

	if( componentType == "graphics" )
	{
		strategy = new WorldLoader_GraphicsComponentStrategy( );
	} 
	else if ( componentType == "geometry" )
	{
		strategy = new WorldLoader_GeometryComponentStrategy( );
	}
	else if ( componentType == "physics" )
	{
		strategy = new WorldLoader_PhysicsComponentStrategy( );
	}

	return strategy;
}

ISystemComponent* WorldLoader_GraphicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList&  systemScenes )
{
	SystemPropertyList properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.push_back( SystemProperty( propertyKey, propertyValue ) );
	}

	SystemSceneList::const_iterator systemScene = systemScenes.find( RenderSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_GeometryComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList& systemScenes )
{
	SystemPropertyList properties;

	float x, y, z, w;

	componentNode[ "position" ][ "x" ] >> x;
	componentNode[ "position" ][ "y" ] >> y;
	componentNode[ "position" ][ "z" ] >> z;
	properties.push_back( SystemProperty( "position", MathVector3( x, y, z ) ) );

	componentNode[ "scale" ][ "x" ] >> x;
	componentNode[ "scale" ][ "y" ] >> y;
	componentNode[ "scale" ][ "z" ] >> z;
	properties.push_back( SystemProperty( "scale", MathVector3( x, y, z ) ) );

	componentNode[ "orientation" ][ "w" ] >> w;
	componentNode[ "orientation" ][ "x" ] >> x;
	componentNode[ "orientation" ][ "y" ] >> y;
	componentNode[ "orientation" ][ "z" ] >> z;
	properties.push_back( SystemProperty( "orientation", MathQuaternion( x, y, z, w ) ) );

	SystemSceneList::const_iterator systemScene = systemScenes.find( GeometrySystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_PhysicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList& systemScenes )
{
	SystemPropertyList properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.push_back( SystemProperty( propertyKey, propertyValue ) );
	}

	SystemSceneList::const_iterator systemScene = systemScenes.find( PhysicsSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName );
	systemComponent->Initialize( properties );

	return systemComponent;
}
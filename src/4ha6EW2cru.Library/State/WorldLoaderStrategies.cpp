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
	else if ( componentType == "input" )
	{
		strategy = new WorldLoader_InputComponentStrategy( );
	}

	return strategy;
}

ISystemComponent* WorldLoader_GraphicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap&  systemScenes )
{
	SystemPropertyList properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, SystemProperty( propertyKey, propertyValue ) ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( RenderSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_GeometryComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	SystemPropertyList properties;

	float x, y, z, w;

	componentNode[ "position" ][ "x" ] >> x;
	componentNode[ "position" ][ "y" ] >> y;
	componentNode[ "position" ][ "z" ] >> z;
	properties.insert( std::make_pair( "position", SystemProperty( "position", MathVector3( x, y, z ) ) ) );

	componentNode[ "scale" ][ "x" ] >> x;
	componentNode[ "scale" ][ "y" ] >> y;
	componentNode[ "scale" ][ "z" ] >> z;
	properties.insert( std::make_pair( "scale", SystemProperty( "scale", MathVector3( x, y, z ) ) ) );

	componentNode[ "orientation" ][ "w" ] >> w;
	componentNode[ "orientation" ][ "x" ] >> x;
	componentNode[ "orientation" ][ "y" ] >> y;
	componentNode[ "orientation" ][ "z" ] >> z;
	properties.insert( std::make_pair( "orientation", SystemProperty( "orientation", MathQuaternion( x, y, z, w ) ) ) );

	SystemSceneMap::const_iterator systemScene = systemScenes.find( GeometrySystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_PhysicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	SystemPropertyList properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, SystemProperty( propertyKey, propertyValue ) ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( PhysicsSystemType );

	std::string type = properties[ "type" ].GetValue< std::string >( );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "character" ) ? type : "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_InputComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	SystemPropertyList properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, SystemProperty( propertyKey, propertyValue ) ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( InputSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}
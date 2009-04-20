#include "WorldLoaderStrategies.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

#include "../Scripting/ScriptComponent.h"

IWorldLoader_ComponentStrategy* WorldLoader_ComponentStrategy_Factory::Create( const SystemType& systemType )
{
	IWorldLoader_ComponentStrategy* strategy = 0;

	if( systemType == RenderSystemType )
	{
		strategy = new WorldLoader_GraphicsComponentStrategy( );
	} 
	else if ( systemType == GeometrySystemType )
	{
		strategy = new WorldLoader_GeometryComponentStrategy( );
	}
	else if ( systemType == PhysicsSystemType )
	{
		strategy = new WorldLoader_PhysicsComponentStrategy( );
	}
	else if ( systemType == InputSystemType )
	{
		strategy = new WorldLoader_InputComponentStrategy( );
	}
	else if ( systemType == AISystemType )
	{
		strategy = new WorldLoader_AIComponentStrategy( );
	}
	else if ( systemType == ScriptSystemType )
	{
		strategy = new WorldLoader_ScriptComponentStrategy( );
	}

	return strategy;
}

ISystemComponent* WorldLoader_GraphicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap&  systemScenes )
{
	AnyValueMap properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, propertyValue ) );
	}

	std::string type = properties[ "type" ].GetValue< std::string >( );

	SystemSceneMap::const_iterator systemScene = systemScenes.find( RenderSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "camera" ) ? type : "default" );;
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_GeometryComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	AnyValueMap properties;

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

	SystemSceneMap::const_iterator systemScene = systemScenes.find( GeometrySystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_PhysicsComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	AnyValueMap properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, propertyValue ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( PhysicsSystemType );

	std::string type = properties[ "type" ].GetValue< std::string >( );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, ( type == "character" ) ? type : "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_InputComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	AnyValueMap properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, propertyValue ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( InputSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_AIComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{ 
	AnyValueMap properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, propertyValue ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( AISystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	systemComponent->Initialize( properties );

	return systemComponent;
}

ISystemComponent* WorldLoader_ScriptComponentStrategy::CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes )
{
	AnyValueMap properties;

	for( YAML::Iterator componentProperty = componentNode.begin( ); componentProperty != componentNode.end( ); ++componentProperty ) 
	{
		std::string propertyKey, propertyValue;

		componentProperty.first( ) >> propertyKey;
		componentProperty.second( ) >> propertyValue;

		properties.insert( std::make_pair( propertyKey, propertyValue ) );
	}

	SystemSceneMap::const_iterator systemScene = systemScenes.find( ScriptSystemType );

	ISystemComponent* systemComponent = ( *systemScene ).second->CreateComponent( entityName, "default" );
	ScriptComponent* scriptComponent = static_cast< ScriptComponent* >( systemComponent );
	scriptComponent->Initialize( properties );
	scriptComponent->Execute( );

	return scriptComponent;
}
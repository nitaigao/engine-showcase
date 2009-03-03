#include "WorldLoader.h"

#include <sstream>
#include "../System/Management.h"

#include "../Maths/Vector3.hpp"

void WorldLoader::Load( const std::string& levelPath )
{
	FileBuffer* levelBuffer = _fileManager->GetFile( levelPath );

	std::istringstream inputStream( levelBuffer->fileBytes );
	YAML::Parser parser( inputStream );
	parser.Load( inputStream );

	while( parser ) 
	{
		YAML::Node doc;
		parser.GetNextDocument( doc );
		
		for( YAML::Iterator documentRoot = doc.begin( ); documentRoot != doc.end( ); ++documentRoot ) 
		{
			const YAML::Node& rootNode = ( * documentRoot );

			for( YAML::Iterator entityNode = rootNode.begin( ); entityNode != rootNode.end( ); ++entityNode ) 
			{
				this->LoadEntity( entityNode.second( ) );
			}
		}
	} 

	delete levelBuffer;
}

void WorldLoader::LoadEntity( const YAML::Node& node )
{ 
	std::string entityName;
	node[ "name" ] >> entityName;
	IEntity* entity = _world->CreateEntity( entityName );

	for( YAML::Iterator entityNode = node.begin( ); entityNode != node.end( ); ++entityNode ) 
	{
		std::string key;
		entityNode.first( ) >> key;

		if ( key == "components" )
		{
			this->LoadEntityComponents( entityNode.second( ), entity );
		}
	}
}

void WorldLoader::LoadEntityComponents( const YAML::Node& node, IEntity* entity )
{
	for( YAML::Iterator components = node.begin( ); components != node.end( ); ++components ) 
	{
		const YAML::Node& componentNode = ( *components );
		
		for( YAML::Iterator component = componentNode.begin( ); component != componentNode.end( ); ++component ) 
		{
			const YAML::Node& componentDetail = component.second( );

			std::string key;
			componentDetail[ "system" ] >> key;

			SystemPropertyList properties;
			SystemSceneList systemScenes = _world->GetSystemScenes( );

			if ( key == "graphics" )
			{
				std::string modelProperty = "model";

				std::string model;
				componentDetail[ modelProperty ] >> model;
				properties.push_back( SystemProperty( modelProperty, model ) );

				ISystemComponent* systemComponent = systemScenes[ RenderSystemType ]->CreateComponent( entity->GetName( ) );
				systemComponent->Initialize( properties );

				entity->AddComponent( systemComponent );
			}

			if ( key == "geometry" )
			{
				float x, y, z;

				componentDetail[ "position" ][ 0 ] >> x;
				componentDetail[ "position" ][ 1 ] >> y;
				componentDetail[ "position" ][ 2 ] >> z;

				Vector3 positionVector( x, y, z );
				properties.push_back( SystemProperty( "positon", positionVector ) );

				ISystemComponent* systemComponent = systemScenes[ GeometrySystemType ]->CreateComponent( entity->GetName( ) );
				systemComponent->Initialize( properties );

				entity->AddComponent( systemComponent );
			}
		}		
	}
}
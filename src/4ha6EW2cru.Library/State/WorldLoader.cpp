#include "WorldLoader.h"

#include <sstream>
#include "../System/Management.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

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
			const YAML::Node& rootNode = ( *documentRoot );

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
				ISystemComponent* systemComponent = systemScenes[ RenderSystemType ]->CreateComponent( entity->GetName( ) );

				for( YAML::Iterator componentProperties = componentDetail.begin( ); componentProperties != componentDetail.end( ); ++componentProperties ) 
				{
					std::string propertyKey, propertyValue;

					componentProperties.first( ) >> propertyKey;
					componentProperties.second( ) >> propertyValue;

					properties.push_back( SystemProperty( propertyKey, propertyValue ) );
				}
				
				systemComponent->Initialize( properties );
				entity->AddComponent( systemComponent );
			}

			if ( key == "geometry" )
			{
				float x, y, z, w;

				componentDetail[ "position" ][ 0 ] >> x;
				componentDetail[ "position" ][ 1 ] >> y;
				componentDetail[ "position" ][ 2 ] >> z;
				properties.push_back( SystemProperty( "position", MathVector3( x, y, z ) ) );

				componentDetail[ "scale" ][ 0 ] >> x;
				componentDetail[ "scale" ][ 1 ] >> y;
				componentDetail[ "scale" ][ 2 ] >> z;
				properties.push_back( SystemProperty( "scale", MathVector3( x, y, z ) ) );

				componentDetail[ "orientation" ][ 0 ] >> x;
				componentDetail[ "orientation" ][ 1 ] >> y;
				componentDetail[ "orientation" ][ 2 ] >> z;
				componentDetail[ "orientation" ][ 3 ] >> w;
				properties.push_back( SystemProperty( "orientation", MathQuaternion( x, y, z, w ) ) );

				ISystemComponent* systemComponent = systemScenes[ GeometrySystemType ]->CreateComponent( entity->GetName( ) );
				entity->AddComponent( systemComponent );

				systemComponent->Initialize( properties );
			}
		}		
	}
}
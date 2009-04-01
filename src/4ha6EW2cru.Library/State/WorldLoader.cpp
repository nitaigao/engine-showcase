#include "WorldLoader.h"

#include <sstream>
#include "../System/Management.h"

#include "WorldLoaderStrategies.h"

#include "../Scripting/ScriptEvent.hpp"

void WorldLoader::Load( const std::string& levelPath )
{
	_activeNodeIndex = 0;
	_loadProgress = 0;

	Management::GetInstance( )->GetEventManager( )->TriggerEvent( new ScriptEvent( "WORLD_LOADING_STARTED" ) );

	FileBuffer* levelBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( levelPath, false );

	std::istringstream inputStream( levelBuffer->fileBytes );
	YAML::Parser parser( inputStream );

	parser.Load( inputStream );

	while( parser )
	{
		YAML::Node* node = new YAML::Node( );
		parser.GetNextDocument( *node );
		_loadTotal += node->size( );
		_loadQueue.push( node );
	}

	delete levelBuffer;
}

void WorldLoader::LoadEntity( const YAML::Node& entityNode )
{ 
	YAML::CONTENT_TYPE nodeType = entityNode.GetType( );

	std::string name;
	entityNode[ "name" ] >> name;

	IEntity* entity = _world->CreateEntity( name );

	for( YAML::Iterator entityProperty = entityNode.begin( ); entityProperty != entityNode.end( ); ++entityProperty ) 
	{
		YAML::CONTENT_TYPE nodeType = ( entityProperty.second( ) ).GetType( );
		
		if( nodeType == YAML::CT_SEQUENCE )
		{
			std::string key;
			entityProperty.first( ) >> key;

			if ( key == "components" )
			{
				this->LoadEntityComponents( entityProperty.second( ), entity );
			}
		}
	}
}

void WorldLoader::LoadEntityComponents( const YAML::Node& node, IEntity* entity )
{
	ISystemComponent* geometryComponent = 0;

	for( YAML::Iterator components = node.begin( ); components != node.end( ); ++components ) 
	{
		const YAML::Node& componentNode = ( *components );

		std::string system;
		componentNode[ "system" ] >> system;

		IWorldLoader_ComponentStrategy* componentStrategy = WorldLoader_ComponentStrategy_Factory::Create( system );
		ISystemComponent* entityComponent = componentStrategy->CreateComponent( entity->GetName( ), componentNode, _world->GetSystemScenes( ) );

		entity->AddComponent( entityComponent );

		if ( entityComponent->GetType( ) == GeometrySystemType )
		{
			geometryComponent = entityComponent;
		}

		delete componentStrategy;
	}

	if ( geometryComponent != 0 )
	{
		geometryComponent->PushChanges( System::Changes::Geometry::All );
	}
}

void WorldLoader::Update( float deltaMilliseconds )
{
	if ( !_loadQueue.empty( ) )
	{
		YAML::Node* loadSource = _loadQueue.front( );

		if( _activeNodeIndex < loadSource->size( ) )
		{

			const YAML::Node& documentNode = loadSource[ 0 ];
			const YAML::Node& entityNode = documentNode[ _activeNodeIndex ]; 

			this->LoadEntity( entityNode );

			int progressPercent = ( ( float ) ++_loadProgress / ( float ) _loadTotal ) * 100.0f;
			Management::GetInstance( )->GetEventManager( )->TriggerEvent( new ScriptEvent( "WORLD_LOADING_PROGRESS", progressPercent ) );

			if ( ++_activeNodeIndex == loadSource->size( ) )
			{
				_activeNodeIndex = 0;
				delete loadSource;
				_loadQueue.pop( );
			}

			if ( _loadProgress >= _loadTotal )
			{
				Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_FINISHED" ) );
			}
		}
	}
}
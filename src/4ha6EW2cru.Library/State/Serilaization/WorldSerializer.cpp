#include "WorldSerializer.h"

using namespace State;

#include "../../IO/IResource.hpp"
using namespace Resources;

#include "../../System/SystemTypeMapper.hpp"
#include "../../Management/Management.h"

#include "../../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../../Renderer/Color.hpp"
using namespace Renderer;

#include "ComponentSerializerFactory.h"
#include "AIComponentSerializer.h"
#include "GeometryComponentSerializer.h"
#include "GraphicsComponentSerializer.h"
#include "InputComponentSerializer.h"
#include "PhysicsComponentSerializer.h"
#include "ScriptComponentSerializer.h"

namespace Serialization
{
	
	void WorldSerializer::Load( const std::string& levelPath )
	{
		_activeNodeIndex = 0;
		_loadProgress = 0;
		_loadTotal = 0;
	
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_STARTED", levelPath ) );
	
		IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( levelPath );
	
		std::istringstream inputStream( resource->GetFileBuffer( )->fileBytes );
		YAML::Parser parser( inputStream );
	
		parser.Load( inputStream );
	
		while( parser )
		{
			YAML::Node* node = new YAML::Node( );
			parser.GetNextDocument( *node );
			_loadTotal += node->size( );
			_loadQueue.push( node );
		}
	}
	
	void WorldSerializer::LoadNode( const YAML::Node& node )
	{
		std::string type;
		node[ "type" ] >> type;
	
		if ( type == "entity" )
		{
			this->LoadEntity( node );
		}
		else if ( type == "color" )
		{
			this->LoadColor( node );
		}
		else if ( type == "skybox" )
		{
			this->LoadSkyBox( node );
		}
		else if ( type == "link" )
		{
			this->LoadLink( node );
		}
	}
	
	void WorldSerializer::LoadLink( const YAML::Node& node )
	{ 
		std::string entityName, entitySystem, observerName, observerSystem;
	
		node[ "entityName" ] >> entityName;
		node[ "entitySystem" ] >> entitySystem;
		node[ "observerName" ] >> observerName;
		node[ "observerSystem" ] >> observerSystem;
	
		System::Types::Type entitySystemType = System::TypeMapper::StringToType( entitySystem );
		System::Types::Type observerSystemType = System::TypeMapper::StringToType( observerSystem );
	
		IWorldEntity* entity = _world->FindEntity( entityName );
		IWorldEntity* observer = _world->FindEntity( observerName );
	
		ISystemComponent* entityComponent = entity->FindComponent( entitySystemType );
		ISystemComponent* observerComponent = observer->FindComponent( observerSystemType );
	
		entityComponent->AddObserver( observerComponent );
	
	}
	
	void WorldSerializer::LoadSkyBox( const YAML::Node& node )
	{
		std::string materialName;
		node[ "material" ] >> materialName;
	
		ISystem* renderer = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
		renderer->SetProperty( "skyBox", materialName );
	}
	
	void WorldSerializer::LoadColor( const YAML::Node& environmentNode )
	{
		std::string name;
		environmentNode[ "name" ] >> name;
	
		AnyValue::AnyValueMap properties;
	
		float red = 0;
		float green = 0;
		float blue = 0;
	
		environmentNode[ "r" ] >> red;
		environmentNode[ "g" ] >> green;
		environmentNode[ "b" ] >> blue;
	
		ISystem* graphicsSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
	
		if ( name == "ambient" )
		{
			graphicsSystem->SetProperty( "ambientColor", Color( red, green, blue ) );
		}
	
		if ( name == "background" )
		{
			graphicsSystem->SetProperty( "backgroundColor", Color( red, green, blue ) );
		}
	}
	
	void WorldSerializer::LoadEntity( const YAML::Node& entityNode )
	{ 
		YAML::CONTENT_TYPE nodeType = entityNode.GetType( );
	
		std::string name;
		entityNode[ "name" ] >> name;
	
		IWorldEntity* entity = _world->CreateEntity( name );
	
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
	
	void WorldSerializer::LoadEntityComponents( const YAML::Node& node, IWorldEntity* entity )
	{
		ISystemComponent* geometryComponent = 0;
	
		for( YAML::Iterator components = node.begin( ); components != node.end( ); ++components ) 
		{
			const YAML::Node& componentNode = ( *components );
	
			std::string system;
			componentNode[ "system" ] >> system;
	
			System::Types::Type systemType = System::TypeMapper::StringToType( system );
			IComponentSerializer* serializer = ComponentSerializerFactory::Create( systemType );
			ISystemComponent* entityComponent = serializer->DeSerialize( entity->GetName( ), componentNode, _world->GetSystemScenes( ) );
	
			entity->AddComponent( entityComponent );
	
			if ( entityComponent->GetType( ) == System::Types::GEOMETRY )
			{
				geometryComponent = entityComponent;
			}
	
			delete serializer;
		}
	
		if ( geometryComponent != 0 )
		{
			geometryComponent->PushChanges( System::Changes::Geometry::All );
		}
	}
	
	void WorldSerializer::Update( const float& deltaMilliseconds )
	{
		if ( !_loadQueue.empty( ) )
		{
			YAML::Node* loadSource = _loadQueue.front( );
	
			if( _activeNodeIndex < loadSource->size( ) )
			{
				const YAML::Node& documentNode = loadSource[ 0 ];
				const YAML::Node& node = documentNode[ _activeNodeIndex ];
	
				this->LoadNode( node );
		
				float progressPercent = ( ( float ) ++_loadProgress / ( float ) _loadTotal ) * 100.0f;
				Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_PROGRESS", static_cast< int >( progressPercent ) ) );
	
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
}
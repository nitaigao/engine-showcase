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
		else if( type == "fog" )
		{
			this->LoadFog( node );
		}
		else if ( type == "skybox" )
		{
			this->LoadSkyBox( node );
		}
		else if ( type == "entityLink" )
		{
			this->LoadLink( node );
		}
	}
	
	void WorldSerializer::LoadLink( const YAML::Node& node )
	{ 
		std::string subjectName, subjectSystem, observerName, observerSystem;
	
		node[ "subjectName" ] >> subjectName;
		node[ "subjectSystem" ] >> subjectSystem;
		node[ "observerName" ] >> observerName;
		node[ "observerSystem" ] >> observerSystem;
	
		System::Types::Type subjectSystemType = System::TypeMapper::StringToType( subjectSystem );
		System::Types::Type observerSystemType = System::TypeMapper::StringToType( observerSystem );

		if ( 
			Management::GetInstance( )->GetSystemManager( )->HasSystem( subjectSystemType ) && 
			Management::GetInstance( )->GetSystemManager( )->HasSystem( observerSystemType ) 
			)
		{
			IWorldEntity* subject = _world->FindEntity( subjectName );
			IWorldEntity* observer = _world->FindEntity( observerName );

			subject->AddObserver( System::Messages::SetPlayerPosition, observer );
		}
	}
	
	void WorldSerializer::LoadSkyBox( const YAML::Node& node )
	{
		if ( Management::GetInstance( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
		{
			std::string materialName;
			node[ "material" ] >> materialName;

			float distance;
			node[ "distance" ] >> distance;

			AnyValue::AnyValueMap parameters;
			parameters[ "material" ] = materialName;
			parameters[ "distance" ] = distance;
		
			ISystem* renderer = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
			renderer->SetAttribute( "skyBox", parameters );
		}
	}

	void WorldSerializer::LoadFog( const YAML::Node& node )
	{
		if ( Management::GetInstance( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
		{
			float linearEnd, linearStart;
			node[ "linearEnd" ] >> linearEnd;
			node[ "linearStart" ] >> linearStart;

			float r, g, b;
			node[ "r" ] >> r;
			node[ "g" ] >> g;
			node[ "b" ] >> b;

			AnyValue::AnyValueMap parameters;
			parameters[ "linearEnd" ] = linearEnd;
			parameters[ "linearStart" ] = linearStart;
			parameters[ "color" ] = Renderer::Color( r, g, b );

			ISystem* renderer = Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
			renderer->SetAttribute( "fog", parameters );
		}
	}
	
	void WorldSerializer::LoadColor( const YAML::Node& environmentNode )
	{
		if ( Management::GetInstance( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
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
				graphicsSystem->SetAttribute( "ambientColor", Color( red, green, blue ) );
			}
		
			if ( name == "background" )
			{
				graphicsSystem->SetAttribute( "backgroundColor", Color( red, green, blue ) );
			}
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
		for( YAML::Iterator component = node.begin( ); component != node.end( ); ++component ) 
		{
			const YAML::Node& componentNode = ( *component );
	
			std::string system;
			componentNode[ "system" ] >> system;
	
			System::Types::Type systemType = System::TypeMapper::StringToType( system );

			if ( Management::GetInstance( )->GetSystemManager( )->HasSystem( systemType ) )
			{
				IComponentSerializer* serializer = ComponentSerializerFactory::Create( systemType );
				ISystemComponent* entityComponent = serializer->DeSerialize( entity->GetName( ), componentNode, _world->GetSystemScenes( ) );
		
				entity->AddComponent( entityComponent );
		
				delete serializer;
			}
		}
	}
	
	void WorldSerializer::Update( const float& deltaMilliseconds )
	{
		if ( !_loadQueue.empty( ) )
		{
			if( _activeNodeIndex < _loadQueue.front( )->size( ) )
			{
				const YAML::Node& documentNode = _loadQueue.front( )[ 0 ];
				const YAML::Node& node = documentNode[ _activeNodeIndex ];
	
				this->LoadNode( node );
		
				float progressPercent = ( ( float ) ++_loadProgress / ( float ) _loadTotal ) * 100.0f;
				Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_PROGRESS", static_cast< int >( progressPercent ) ) );
				_activeNodeIndex++;
			}
			else
			{
				_activeNodeIndex = 0;
				delete _loadQueue.front( );
				_loadQueue.pop( );

				if ( _loadQueue.empty( ) )
				{
					_loadProgress = _loadTotal;
					Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_FINISHED" ) );
				}
			}
		}
	}

	WorldSerializer::~WorldSerializer()
	{
		while( _loadQueue.size( ) > 0 )
		{
			delete _loadQueue.front( );
			_loadQueue.pop( );
		}
	}
}
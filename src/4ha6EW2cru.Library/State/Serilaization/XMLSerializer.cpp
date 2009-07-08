#include "XMLSerializer.h"

using namespace State;

#include "../../Logging/Logger.h"
using namespace Logging;

#include "../../IO/IResource.hpp"
using namespace Resources;

#include "../../System/SystemTypeMapper.hpp"
#include "../../Management/Management.h"

#include "../../Events/ScriptEvent.hpp"
using namespace Events;

#include "ComponentSerializerFactory.h"
#include "AIComponentSerializer.h"
#include "GeometryComponentSerializer.h"
#include "GraphicsComponentSerializer.h"
#include "InputComponentSerializer.h"
#include "PhysicsComponentSerializer.h"
#include "ScriptComponentSerializer.h"
#include "..\IWorldEntity.hpp"

using namespace ticpp;

namespace Serialization
{
	
	void XMLSerializer::Load( const std::string& levelPath )
	{
		if ( !Management::Get( )->GetFileManager( )->FileExists( levelPath ) )
		{
			std::stringstream logMessage;
			logMessage << "Unable to locate level file at path: " << levelPath;
			Logger::Get( )->Warn( logMessage.str( ) );
			return;
		}

		_loadProgress = 0;
		_loadTotal = 0;
	
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_STARTED", levelPath ) );
	
		IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( levelPath );
		Document levelFile( resource->GetFileBuffer()->fileBytes );

		this->LoadElement( levelFile.FirstChildElement( ) );
	}

	void XMLSerializer::LoadElement( ticpp::Element* element )
	{
		for( Iterator< Element > child = element->FirstChildElement( false ); child != child.end( ); child++ )
		{
			std::string elementName;
			( *child ).GetValue( &elementName );

			if ( elementName == "color" || elementName == "entity" )
			{
				_loadQueueEl.push( ( *child ).ClonePtr( ) );
				_loadTotal++;
			}

			this->LoadElement( &( *child ) );
		}
	}

	void XMLSerializer::DeserializeElement( ticpp::Element* element )
	{
		std::string elementName;
		element->GetValue( &elementName );

		if ( elementName == "entity" )
		{
			this->LoadEntity( element );
		}
		else if ( elementName == "color" )
		{
			this->LoadColor( element );
		}
		/*else if( elementName == "fog" )
		{
			this->LoadFog( element );
		}
		else if ( elementName == "skybox" )
		{
			this->LoadSkyBox( element );
		}*/
	}
	
	/*void XMLSerializer::LoadLink( const YAML::Node& node )
	{ 
		std::string subjectName, subjectSystem, observerName, observerSystem;
	
		node[ "subjectName" ] >> subjectName;
		node[ "subjectSystem" ] >> subjectSystem;
		node[ "observerName" ] >> observerName;
		node[ "observerSystem" ] >> observerSystem;
	
		System::Types::Type subjectSystemType = System::SystemTypeMapper::StringToType( subjectSystem );
		System::Types::Type observerSystemType = System::SystemTypeMapper::StringToType( observerSystem );

		if ( 
			Management::Get( )->GetSystemManager( )->HasSystem( subjectSystemType ) && 
			Management::Get( )->GetSystemManager( )->HasSystem( observerSystemType ) 
			)
		{
			IWorldEntity* subject = _world->FindEntity( subjectName );
			IWorldEntity* observer = _world->FindEntity( observerName );

			subject->AddObserver( System::Messages::SetPlayerPosition, observer );
		}
	}*/
	
	/*void XMLSerializer::LoadSkyBox( const YAML::Node& node )
	{
		if ( Management::Get( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
		{
			std::string materialName;
			node[ "material" ] >> materialName;

			float distance;
			node[ "distance" ] >> distance;

			AnyType::AnyTypeMap parameters;
			parameters[ "material" ] = materialName;
			parameters[ "distance" ] = distance;
		
			ISystem* renderer = Management::Get( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
			renderer->SetAttribute( "skyBox", parameters );
		}
	}*/

	/*void XMLSerializer::LoadFog( const YAML::Node& node )
	{
		if ( Management::Get( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
		{
			float linearEnd, linearStart;
			node[ "linearEnd" ] >> linearEnd;
			node[ "linearStart" ] >> linearStart;

			float r, g, b;
			node[ "r" ] >> r;
			node[ "g" ] >> g;
			node[ "b" ] >> b;

			AnyType::AnyTypeMap parameters;
			parameters[ "linearEnd" ] = linearEnd;
			parameters[ "linearStart" ] = linearStart;
			parameters[ "color" ] = Renderer::Color( r, g, b );

			ISystem* renderer = Management::Get( )->GetSystemManager( )->GetSystem( System::Types::RENDER );
			renderer->SetAttribute( "fog", parameters );
		}
	}*/
	
	void XMLSerializer::LoadColor( ticpp::Element* element )
	{
		if ( Management::Get( )->GetSystemManager( )->HasSystem( System::Types::RENDER ) )
		{
			std::string key;

			float red = 0;
			float green = 0;
			float blue = 0;

			element->GetAttribute( "type", &key );
			element->GetAttribute( "r", &red );
			element->GetAttribute( "g", &green );
			element->GetAttribute( "b", &blue );

			AnyType::AnyTypeMap parameters;
			parameters[ "r" ] = red;
			parameters[ "g" ] = green;
			parameters[ "b" ] = blue;

			ISystem* graphicsSystem = Management::Get( )->GetSystemManager( )->GetSystem( System::Types::RENDER );

			graphicsSystem->SetAttribute( key, parameters );
		}
	}
	
	void XMLSerializer::LoadEntity( ticpp::Element* element )
	{ 	
		std::string name;
		element->GetAttribute( "name", &name );

		IWorldEntity* entity = _world->CreateEntity( name );

		std::string src;
		element->GetAttribute( "src", &src, false );

		NodePtrMap components;

		if ( !src.empty( ) )
		{
			IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( src );
			Document externalFile( resource->GetFileBuffer( )->fileBytes );

			for( Iterator< Element > child = externalFile.FirstChildElement( false ); child != child.end( ); child++ )
			{
				this->LoadEntityComponents( &*child, components );
			}
		}

		this->LoadEntityComponents( element, components );

		for( NodePtrMap::iterator i = components.begin( ); i != components.end( ); ++i )
		{
			if ( Management::Get( )->GetSystemManager( )->HasSystem( ( *i ).first ) ) 
			{
				IComponentSerializer* serializer = ComponentSerializerFactory::Create( ( *i ).first );
				ISystemComponent* component = serializer->DeSerialize( name, ( *i ).second->ToElement( ), _world->GetSystemScenes( ) );
				entity->AddComponent( component );

				delete serializer;
			}

			delete ( *i ).second;
		}

		entity->Initialize( );
	}

	void XMLSerializer::LoadEntityComponents( ticpp::Element* element, NodePtrMap& components )
	{
		for( Iterator< Element > child = element->FirstChildElement( false ); child != child.end( ); child++ )
		{
			std::string elementName;
			child->GetValue( &elementName );

			if ( elementName == "components" )
			{
				for( Iterator< Element > component = ( *child ).FirstChildElement( false ); component != component.end( ); component++ )
				{
					std::string system;
					( *component ).GetAttribute( "system", &system );

					System::Types::Type systemType = System::SystemTypeMapper::StringToType( system );

					if ( components.find( systemType ) != components.end( ) )
					{
						delete components[ systemType ];
					}

					components[ systemType ] = ( *component ).ClonePtr( );
				}
			}
		}
	}
	
	void XMLSerializer::Update( const float& deltaMilliseconds )
	{
		if ( !_loadQueueEl.empty( ) )
		{
			this->DeserializeElement( _loadQueueEl.front( )->ToElement( ) );

			delete _loadQueueEl.front( );

			_loadQueueEl.pop( );

			float progressPercent = ( ( float ) ++_loadProgress / ( float ) _loadTotal ) * 100.0f;
			Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_PROGRESS", static_cast< int >( progressPercent ) ) );

			if ( _loadQueueEl.empty( ) )
			{
				_loadProgress = _loadTotal;
				Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WORLD_LOADING_FINISHED" ) );
			}
		}
	}

	XMLSerializer::~XMLSerializer()
	{
		while( _loadQueueEl.size( ) > 0 )
		{
			delete _loadQueueEl.front( );
			_loadQueueEl.pop( );
		}
	}
}
#include "AISystemScene.h"

#include "AIScriptComponent.h"
#include "AIWaypointComponent.h"
#include "AINavigationMeshComponent.h"

#include "../Management/Management.h"

#include "../Scripting/ScriptComponent.h"
#include "../Scripting/ScriptSystemScene.h"
using namespace Script;

#include "../Maths/MathVector3.hpp"
using namespace Maths;

#include <luabind/luabind.hpp>
#include <luabind/table_policy.hpp>
using namespace luabind;

#include "../Logging/Logger.h"
using namespace Logging;

namespace AI
{
	ISystemComponent* AISystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		IAISystemComponent* component = 0;

		if ( type == "waypoint" )
		{
			component = new AIWaypointComponent( name );
		}
		else if ( type == "navmesh" )
		{
			component = new AINavigationMeshComponent( name );
		}
		else
		{
			component = new AIScriptComponent( name );
		}

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::AI );
		component->SetAttribute( System::Attributes::ComponentType, type );
		component->SetAttribute( System::Attributes::Parent, static_cast< IAISystemScene* >( this ) );

		m_componentsByName.insert( std::make_pair( name, component ) );
		m_componentsByType.insert( std::make_pair( type, component ) );

		return component;
	}

	void AISystemScene::DestroyComponent( ISystemComponent* component )
	{
		std::string componentName = component->GetAttributes( ) [ System::Attributes::Name ].As< std::string >( );
		m_componentsByName.erase( componentName );

		std::string componentType = component->GetAttributes( )[ System::Attributes::ComponentType ].As< std::string >( );
		ISystemComponent::SystemComponentMultiMap::iterator waypoints = m_componentsByType.find( componentType );

		while( waypoints != m_componentsByType.end( ) )
		{
			if ( ( *waypoints ).second->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) == componentName )
			{
				m_componentsByType.erase( waypoints );
				break;
			}

			++waypoints;
		}

		component->Destroy( );
		delete component;
	}

	void AISystemScene::Initialize()
	{
		IService* scriptService = Management::GetServiceManager( )->FindService( System::Types::SCRIPT );
		lua_State* state = scriptService->Execute( "getMasterState", AnyType::AnyTypeMap( ) )[ "masterState" ].As< lua_State* >( );

		module( state )
		[
			class_< AIScriptComponent >( "AISystemComponent" )
				.def( "getName", &AIScriptComponent::GetName )
				.def( "walkForward", &AIScriptComponent::WalkForward )
				.def( "walkBackward", &AIScriptComponent::WalkBackward )
				.def( "facePlayer", &AIScriptComponent::FacePosition )
				.def( "getName", &AIScriptComponent::GetName )
				.def( "getPlayerDistance", &AIScriptComponent::GetPlayerDistance )
				.def( "getPlayerPosition", &AIScriptComponent::GetPlayerPosition )
				.def( "fireWeapon", &AIScriptComponent::FireWeapon )
				.def( "playAnimation", &AIScriptComponent::PlayAnimation )
				.def( "findRandomWaypoint", &AIScriptComponent::FindRandomWaypoint )
				.def( "getWaypointCount", &AIScriptComponent::GetWaypointCount )
				.def( "navigateTo", &AIScriptComponent::NavigateTo )
				.def( "getPathTo", &AIScriptComponent::GetPathTo, copy_table( result ) )
				.def( "inLineOfSight", &AIScriptComponent::InLineOfSight )
		];
	}

	ISystemComponent::SystemComponentList AISystemScene::GetWaypoints( )
	{
		ISystemComponent::SystemComponentMultiMap::iterator waypoints = m_componentsByType.find( "waypoint" );
		ISystemComponent::SystemComponentList results;
		
		while( waypoints != m_componentsByType.end( ) )
		{
			results.push_back( ( *waypoints ).second );
			++waypoints;
		}

		return results;
	}

	void AISystemScene::Update( const float& deltaMilliseconds )
	{
		for( ISystemComponent::SystemComponentMap::iterator i = m_componentsByName.begin( ); i != m_componentsByName.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}
}
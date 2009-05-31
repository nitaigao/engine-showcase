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
using namespace luabind;

#include "NavigationMesh.h"

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
			m_waypoints.push_back( component );
		}
		else if ( type == "navmesh" )
		{
			component = new AINavigationMeshComponent( name );
			m_navigationMesh = component;
		}
		else
		{
			component = new AIScriptComponent( name );
		}

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::SystemType, System::Types::AI );
		component->SetAttribute( System::Attributes::Parent, static_cast< IAISystemScene* >( this ) );

		m_components[ name ] = component;

		return component;
	}

	void AISystemScene::DestroyComponent( ISystemComponent* component )
	{
		IAISystemComponent* aiComponent = static_cast< IAISystemComponent* >( component );

		ISystemComponent::SystemComponentMap::iterator i = m_components.find( aiComponent->GetAttributes( )[ System::Attributes::Name ].As< std::string >( ) );

		while ( i != m_components.end( ) )
		{
			if ( ( *i ).second == component )
			{
				m_components.erase( i );
				break;
			}

			++i;
		}

		aiComponent->Destroy( );
		delete aiComponent;
	}

	void AISystemScene::Initialize()
	{
		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
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
				.def( "fireWeapon", &AIScriptComponent::FireWeapon )
				.def( "playAnimation", &AIScriptComponent::PlayAnimation )
				.def( "findRandomWaypoint", &AIScriptComponent::FindRandomWaypoint )
				.def( "getWaypointCount", &AIScriptComponent::GetWaypointCount )
				.def( "navigateTo", &AIScriptComponent::NavigateTo )
		];
	}

	void AISystemScene::Update( const float& deltaMilliseconds )
	{
		for ( ISystemComponent::SystemComponentMap::iterator i = m_components.begin( ); i != m_components.end( ); ++i )
		{
			( *i ).second->Update( deltaMilliseconds );
		}
	}
}
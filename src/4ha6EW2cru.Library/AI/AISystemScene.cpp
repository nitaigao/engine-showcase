#include "AISystemScene.h"

#include "AISystemComponent.h"

#include "../Management/Management.h"

#include "../Scripting/ScriptComponent.h"
#include "../Scripting/ScriptSystemScene.h"
using namespace Script;

#include <luabind/luabind.hpp>
using namespace luabind;

namespace AI
{
	ISystemComponent* AISystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		m_lastFrameAssignment = ( m_lastFrameAssignment == 5 ) ? 0 : m_lastFrameAssignment + 1;

		IAISystemComponent* component = new AISystemComponent( name, m_lastFrameAssignment );

		component->SetAttribute( System::Attributes::Name, name );
		component->SetAttribute( System::Attributes::Type, System::Types::AI );
		component->SetAttribute( System::Attributes::Parent, this );

		m_components.insert( std::make_pair( component->GetFrameAssignment( ), component ) );
		return component;
	}

	void AISystemScene::DestroyComponent( ISystemComponent* component )
	{
		IAISystemComponent* aiComponent = static_cast< IAISystemComponent* >( component );

		AISystemComponentMap::iterator i = m_components.find( aiComponent->GetFrameAssignment( ) );

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
		lua_State* state = scriptService->Execute( "getMasterState", AnyValue::AnyValueMap( ) )[ "masterState" ].GetValue< lua_State* >( );

		module( state )
		[
			class_< AISystemComponent >( "AISystemComponent" )
				.def( "getName", &AISystemComponent::GetName )
				.def( "walkForward", &AISystemComponent::WalkForward )
				.def( "walkBackward", &AISystemComponent::WalkBackward )
				.def( "facePlayer", &AISystemComponent::FacePlayer )
				.def( "getName", &AISystemComponent::GetName )
				.def( "getPlayerDistance", &AISystemComponent::GetPlayerDistance )
				.def( "fireWeapon", &AISystemComponent::FireWeapon )
				.def( "playAnimation", &AISystemComponent::PlayAnimation )
		];
	}

	void AISystemScene::Update( const float& deltaMilliseconds )
	{
		m_frameNumber = ( m_frameNumber == 20 ) ? 0 : m_frameNumber + 1;

		AISystemComponentMap::iterator i = m_components.find( m_frameNumber );

		while ( i != m_components.end( ) )
		{
			( *i ).second->Update( deltaMilliseconds );
			++i;
		}
	}
}
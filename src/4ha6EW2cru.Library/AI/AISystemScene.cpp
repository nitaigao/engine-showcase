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
		_lastFrameAssignment = ( _lastFrameAssignment == 20 ) ? 0 : _lastFrameAssignment + 1;

		IAISystemComponent* component = new AISystemComponent( name, _lastFrameAssignment );
		_components.insert( std::make_pair( component->GetFrameAssignment( ), component ) );

		return component;
	}

	void AISystemScene::DestroyComponent( ISystemComponent* component )
	{
		IAISystemComponent* aiComponent = static_cast< IAISystemComponent* >( component );

		AISystemComponentMap::iterator i = _components.find( aiComponent->GetFrameAssignment( ) );

		while ( i != _components.end( ) )
		{
			if ( ( *i ).second->GetName( ) == component->GetName( ) )
			{
				_components.erase( i );
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
				.def( "walkForward", &AISystemComponent::WalkForward )
				.def( "walkBackward", &AISystemComponent::WalkBackward )
				.def( "facePlayer", &AISystemComponent::FacePlayer )
				.def( "setBehavior", &AISystemComponent::SetBehavior )
				.def( "getName", &AISystemComponent::GetName )
				.def( "getId", &AISystemComponent::GetId )
				.def( "getPlayerDistance", &AISystemComponent::GetPlayerDistance )
				.def( "fireWeapon", &AISystemComponent::FireWeapon )
				.def( "playAnimation", &AISystemComponent::PlayAnimation )
		];
	}

	void AISystemScene::Update( const float& deltaMilliseconds )
	{
		_frameNumber = ( _frameNumber == 20 ) ? 0 : _frameNumber + 1;

		AISystemComponentMap::iterator i = _components.find( _frameNumber );

		while ( i != _components.end( ) )
		{
			( *i ).second->Update( deltaMilliseconds );
			++i;
		}
	}
}
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
	AISystemScene::~AISystemScene( )
	{
		delete _scriptScene;
	}

	AISystemScene::AISystemScene( )
	{
		_scriptScene = static_cast< ScriptSystemScene* >( Management::GetInstance( )->GetSystemManager( )->GetSystem( System::Types::SCRIPT )->CreateScene( ) );
	}

	void AISystemScene::Initialize( )
	{
		module( _scriptScene->GetState( ) )
		[
			class_< AISystemComponent >( "AISystemComponent" )
				.def( "walkForward", &AISystemComponent::WalkForward )
				.def( "walkBackward", &AISystemComponent::WalkBackward )
				.def( "facePlayer", &AISystemComponent::FacePlayer )
				.def( "setBehavior", &AISystemComponent::SetBehavior )
				.def( "getName", &AISystemComponent::GetName )
				.def( "getPlayerDistance", &AISystemComponent::GetPlayerDistance )
				.def( "fireWeapon", &AISystemComponent::FireWeapon )
		];
	}

	ISystemComponent* AISystemScene::CreateComponent( const std::string& name, const std::string& type )
	{
		ScriptComponent* scriptComponent = static_cast< ScriptComponent* >( _scriptScene->CreateComponent( name, type ) );
		IAISystemComponent* aiComponent = new AISystemComponent( name, scriptComponent );

		_components.push_back( aiComponent );

		return aiComponent;
	}

	void AISystemScene::DestroyComponent( ISystemComponent* component )
	{
		for( AISystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			if ( ( *i )->GetName( ) == component->GetName( ) )
			{
				_scriptScene->DestroyComponent( ( *i )->GetScriptComponent( ) );
				_components.erase( i );
				delete component;
				component = 0;
				return;
			}
		}
	}

	void AISystemScene::Update( const float& deltaMilliseconds )
	{
		for( AISystemComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
		{
			 ( *i )->Update( deltaMilliseconds );
		}
	}
}
#include "AISystemComponent.h"

#include "../io/FileBuffer.hpp"
#include "../System/Management.h"

#include "../Scripting/ScriptComponent.h"
#include "../Geometry/GeometrySystemComponent.h"

#include <luabind/luabind.hpp>
using namespace luabind;

void AISystemComponent::Initialize( AnyValueMap properties )
{
	ScriptComponent* scriptComponent = static_cast< ScriptComponent* >( _scriptComponent );

	scriptComponent->Initialize( properties );

	luabind::globals( scriptComponent->GetState( ) )[ "me" ] = this;

	lua_pcall( scriptComponent->GetState( ), 0, 0, 0 );
}

void AISystemComponent::Update( float deltaMilliseconds )
{
	ScriptComponent* scriptComponent = static_cast< ScriptComponent* >( _scriptComponent );
	luabind::call_function< void >( scriptComponent->GetState( ), "update" );
}

void AISystemComponent::WalkForward()
{
	this->PushChanges( System::Changes::Input::Move_Forward );
}

void AISystemComponent::WalkBackward()
{
	this->PushChanges( System::Changes::Input::Move_Backward );
}

void AISystemComponent::PushChanges( unsigned int systemChanges )
{
	if ( _observer != 0 )
	{
		_observer->Observe( this, systemChanges );
	}
}

void AISystemComponent::SetBehavior( const std::string& behavior )
{
	_behavior = behavior;
	this->PushChanges( System::Changes::AI::Behavior );
}

void AISystemComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	if ( component->GetType( ) == GeometrySystemType )
	{
		GeometrySystemComponent* geometryComponent = static_cast< GeometrySystemComponent* >( subject );

		_position = geometryComponent->GetPosition( );
		_orientation = geometryComponent->GetOrientation( );
	}
}
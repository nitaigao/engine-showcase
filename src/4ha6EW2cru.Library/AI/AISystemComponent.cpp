#include "AISystemComponent.h"

#include "../IO/FileBuffer.hpp"
#include "../Management/Management.h"

#include "../Scripting/ScriptComponent.h"
#include "../Geometry/GeometrySystemComponent.h"

#include "../Maths/MathUnits.hpp"

#include "../Scripting/ScriptEvent.hpp"

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../Scripting/ScriptSystemScene.h"

namespace AI
{
	void AISystemComponent::Initialize( AnyValue::AnyValueMap properties )
	{
		ScriptComponent* scriptComponent = static_cast< ScriptComponent* >( _scriptComponent );

		scriptComponent->Initialize( properties );

		luabind::globals( scriptComponent->GetState( ) )[ "me" ] = this;

		scriptComponent->Execute( );
	}

	void AISystemComponent::Update( float deltaMilliseconds )
	{
		_scriptComponent->Update( deltaMilliseconds );
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

		if ( systemChanges & System::Changes::Geometry::All )
		{
			if ( component->GetName( ) != this->GetName( ) )
			{
				_playerPosition = component->GetPosition( );
				_playerOrientation = component->GetOrientation( );
			}
			else
			{
				_position = component->GetPosition( );
				_orientation = component->GetOrientation( );
			}
		}
	}

	void AISystemComponent::FacePlayer( )
	{
		// Angle from Origin to Player

		MathVector3 aiToPlayer = ( _playerPosition - _position ).Normalize( );
		MathVector3 aiToOrigin = ( MathVector3::Zero( ) - _position ).Normalize( );

		float aiToPlayerDot = aiToOrigin.DotProduct( aiToPlayer );
		float aiToPlayerAngle = acos( Maths::Clamp( aiToPlayerDot, -1.0f, 1.0f ) );

		MathVector3 aiToPlayerCross = aiToOrigin.CrossProduct( aiToPlayer );

		if( aiToPlayerCross.GetY( ) < 0 )
		{
			float delta = Maths::PI - aiToPlayerAngle;
			aiToPlayerAngle = Maths::PI + delta;
		}

		// Angle from Forward to Origin

		MathVector3 aiForward = _position + MathVector3( 0.0f, 0.0f, -1.0f );
		MathVector3 aiToForward = ( aiForward - _position ).Normalize( );

		float aiForwardToOrigin = aiToForward.DotProduct( aiToOrigin );
		float aiForwardToOriginAngle = acos( Maths::Clamp( aiForwardToOrigin, -1.0f, 1.0f ) );

		MathVector3 aiForwardToOriginCross = aiToForward.CrossProduct( aiToOrigin );

		if( aiForwardToOriginCross.GetY( ) < 0 )
		{
			float delta = Maths::PI - aiForwardToOriginAngle;
			aiForwardToOriginAngle = Maths::PI + delta;
		}

		// rotate to origin - then rotate to player from origin

		_orientation = MathQuaternion( MathVector3::Up( ), aiForwardToOriginAngle ) * MathQuaternion( MathVector3::Up( ), aiToPlayerAngle );
		this->PushChanges( System::Changes::Geometry::Orientation );
	}

	float AISystemComponent::GetPlayerDistance()
	{
		return ( ( _playerPosition - _position ) ).Length( );
	}

	void AISystemComponent::AddObserver( IObserver* observer )
	{
		_observer = observer;

		// update the new observers with our current behavior
		if ( !_behavior.empty( ) )
		{
			this->SetBehavior( _behavior );
		}
	}

	void AISystemComponent::FireWeapon()
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "AI_WEAPON_FIRED", _name ) );
	}
}
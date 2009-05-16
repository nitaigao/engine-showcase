#include "AISystemComponent.h"

using namespace Maths;

#include "../IO/FileBuffer.hpp"
#include "../Management/Management.h"

#include "../Maths/MathUnits.hpp"

#include "../Scripting/ScriptComponent.h"
#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../Scripting/ScriptSystemScene.h"

namespace AI
{
	void AISystemComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		AnyValue::AnyValueMap parameters;
		parameters[ "scriptPath" ] = properties[ "scriptPath" ].GetValue< std::string >( );
		parameters[ "name" ] = m_name + "_ai";

		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
		m_scriptState = scriptService->Execute( "loadScript", parameters )[ "state" ].GetValue< lua_State* >( );

		luabind::globals( m_scriptState )[ "ai" ] = this;
		lua_resume( m_scriptState, 0 );
	}

	void AISystemComponent::Destroy()
	{
		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		AnyValue::AnyValueMap parameters;
		parameters[ "name" ] = m_name + "_ai";
		scriptService->Execute( "unloadComponent", parameters );
	}

	void AISystemComponent::WalkForward()
	{
		this->PushChanges( System::Changes::Input::Move_Forward );
	}

	void AISystemComponent::WalkBackward()
	{
		this->PushChanges( System::Changes::Input::Move_Backward );
	}

	void AISystemComponent::PushChanges( const unsigned int& systemChanges )
	{
		if ( m_observer != 0 )
		{
			m_observer->Observe( this, systemChanges );
		}
	}

	void AISystemComponent::SetBehavior( const std::string& behavior )
	{
		m_behavior = behavior;
		this->PushChanges( System::Changes::AI::Behavior );
	}

	void AISystemComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		if ( systemChanges & System::Changes::Geometry::All )
		{
			if ( component->GetId( ) != this->GetId( ) )
			{
				m_playerPosition = component->GetPosition( );
				m_playerOrientation = component->GetOrientation( );
			}
			else
			{
				m_position = component->GetPosition( );
				m_orientation = component->GetOrientation( );
			}
		}
	}

	void AISystemComponent::FacePlayer( )
	{
		// Angle from Origin to Player

		MathVector3 aiToPlayer = ( m_playerPosition - m_position ).Normalize( );
		MathVector3 aiToOrigin = ( MathVector3::Zero( ) - m_position ).Normalize( );

		float aiToPlayerDot = aiToOrigin.DotProduct( aiToPlayer );
		float aiToPlayerAngle = acos( Maths::Clamp( aiToPlayerDot, -1.0f, 1.0f ) );

		MathVector3 aiToPlayerCross = aiToOrigin.CrossProduct( aiToPlayer );

		if( aiToPlayerCross.Y < 0 )
		{
			float delta = Maths::PI - aiToPlayerAngle;
			aiToPlayerAngle = Maths::PI + delta;
		}

		// Angle from Forward to Origin

		MathVector3 aiForward = m_position + MathVector3( 0.0f, 0.0f, -1.0f );
		MathVector3 aiToForward = ( aiForward - m_position ).Normalize( );

		float aiForwardToOrigin = aiToForward.DotProduct( aiToOrigin );
		float aiForwardToOriginAngle = acos( Maths::Clamp( aiForwardToOrigin, -1.0f, 1.0f ) );

		MathVector3 aiForwardToOriginCross = aiToForward.CrossProduct( aiToOrigin );

		if( aiForwardToOriginCross.Y < 0 )
		{
			float delta = Maths::PI - aiForwardToOriginAngle;
			aiForwardToOriginAngle = Maths::PI + delta;
		}

		// rotate to origin - then rotate to player from origin

		m_orientation = MathQuaternion( MathVector3::Up( ), aiForwardToOriginAngle ) * MathQuaternion( MathVector3::Up( ), aiToPlayerAngle );
		this->PushChanges( System::Changes::Geometry::Orientation );
	}

	void AISystemComponent::AddObserver( IObserver* observer )
	{
		m_observer = observer;

		// update the new observers with our current behavior
		if ( !m_behavior.empty( ) )
		{
			this->SetBehavior( m_behavior );
		}
	}

	void AISystemComponent::FireWeapon()
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "AI_WEAPON_FIRED", m_name ) );
	}

	void AISystemComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyValue::AnyValueMap parameters;

		parameters[ "entityName" ] = m_name;
		parameters[ "animationName" ] = animationName;
		parameters[ "loopAnimation" ] = loopAnimation;

		service->Execute( "playAnimation", parameters );
	}

	void AISystemComponent::Update( const float& deltaMilliseconds )
	{
		m_playerDistance = ( ( m_playerPosition - m_position ) ).Length( );

		if ( m_playerDistance < 50 )
		{
			luabind::call_function< void >( m_scriptState, "update" );
		}
	}
}
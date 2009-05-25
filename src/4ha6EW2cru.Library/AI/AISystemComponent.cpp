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

#include "../Logging/Logger.h"
using namespace Logging;

namespace AI
{
	void AISystemComponent::Initialize( )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ "scriptPath" ] = m_attributes[ System::Attributes::ScriptPath ];
		parameters[ "name" ] = m_name + "_ai";

		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
		m_scriptState = scriptService->Execute( "loadScript", parameters )[ "state" ].As< lua_State* >( );

		luabind::globals( m_scriptState )[ "ai" ] = this;
		lua_resume( m_scriptState, 0 );
	}

	void AISystemComponent::Destroy()
	{
		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		AnyType::AnyTypeMap parameters;
		parameters[ "name" ] = m_name + "_ai";
		scriptService->Execute( "unloadComponent", parameters );
	}

	void AISystemComponent::WalkForward( )
	{
		this->PushMessage( System::Messages::Move_Forward, m_attributes );
	}

	void AISystemComponent::WalkBackward( )
	{
		this->PushMessage( System::Messages::Move_Backward, m_attributes );
	}

	AnyType AISystemComponent::PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		return m_observer->Message( message, parameters );
	}

	AnyType AISystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
 		if ( message == System::Messages::SetPlayerPosition )
		{
			MathVector3 playerPosition = parameters[ System::Attributes::Position ].As< MathVector3 >( );
			
			m_attributes[ System::Attributes::PlayerPosition ] = playerPosition;
			m_attributes[ System::Attributes::PlayerOrientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );

			m_playerDistance = ( ( playerPosition - m_attributes[ System::Attributes::Position ].As< MathVector3 >( ) ) ).Length( );
		}

		if ( message == System::Messages::SetPosition )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].As< MathVector3 >( );
		}

		if ( message == System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );
		}

		return AnyType( );
	}

	void AISystemComponent::FacePlayer( )
	{
		// Angle from Origin to Player

		MathVector3 position = m_attributes[ System::Attributes::Position ].As< MathVector3 >( );
		MathVector3 playerPosition = m_attributes[ System::Attributes::PlayerPosition ].As< MathVector3 >( );

		MathVector3 aiToPlayer = ( playerPosition - position ).Normalize( );
		MathVector3 aiToOrigin = ( MathVector3::Zero( ) - position ).Normalize( );

		float aiToPlayerDot = aiToOrigin.DotProduct( aiToPlayer );
		float aiToPlayerAngle = acos( Maths::Clamp( aiToPlayerDot, -1.0f, 1.0f ) );

		MathVector3 aiToPlayerCross = aiToOrigin.CrossProduct( aiToPlayer );

		if( aiToPlayerCross.Y < 0 )
		{
			float delta = Maths::PI - aiToPlayerAngle;
			aiToPlayerAngle = Maths::PI + delta;
		}

		// Angle from Forward to Origin

		MathVector3 aiForward = position + MathVector3( 0.0f, 0.0f, -1.0f );
		MathVector3 aiToForward = ( aiForward - position ).Normalize( );

		float aiForwardToOrigin = aiToForward.DotProduct( aiToOrigin );
		float aiForwardToOriginAngle = acos( Maths::Clamp( aiForwardToOrigin, -1.0f, 1.0f ) );

		MathVector3 aiForwardToOriginCross = aiToForward.CrossProduct( aiToOrigin );

		if( aiForwardToOriginCross.Y < 0 )
		{
			float delta = Maths::PI - aiForwardToOriginAngle;
			aiForwardToOriginAngle = Maths::PI + delta;
		}

		// rotate to origin - then rotate to player from origin

		m_attributes[ System::Attributes::Orientation ] = MathQuaternion( MathVector3::Up( ), aiForwardToOriginAngle ) * MathQuaternion( MathVector3::Up( ), aiToPlayerAngle );
		this->PushMessage( System::Messages::SetOrientation, m_attributes );
	}

	void AISystemComponent::FireWeapon()
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "AI_WEAPON_FIRED", m_name ) );
	}

	void AISystemComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyType::AnyTypeMap parameters;

		parameters[ "entityName" ] = m_name;
		parameters[ "animationName" ] = animationName;
		parameters[ "loopAnimation" ] = loopAnimation;

		service->Execute( "playAnimation", parameters );
	}

	void AISystemComponent::Update( const float& deltaMilliseconds )
	{
		if ( m_playerDistance < 50 )
		{
			luabind::call_function< void >( m_scriptState, "update" );

			MathVector3 position = m_attributes[ System::Attributes::Position ].As< MathVector3 >( );
			MathQuaternion orientation = m_attributes[ System::Attributes::Orientation ].As< MathQuaternion >( );

			Ogre::Matrix3 rotation;
			orientation.AsOgreQuaternion( ).ToRotationMatrix( rotation );

			MathVector3 forward = MathVector3::Forward( ).AsOgreVector3( ) * rotation.Inverse( );

			m_attributes[ System::Attributes::LookAt ] = position + forward;
			this->PushMessage( System::Messages::SetLookAt, m_attributes );
		}
	}
}
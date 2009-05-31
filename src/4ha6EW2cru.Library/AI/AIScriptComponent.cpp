#include "AIScriptComponent.h"

#include "IAISystemScene.hpp"

#include "../Maths/MathUnits.hpp"
using namespace Maths;

#include "../IO/FileBuffer.hpp"
#include "../Management/Management.h"

#include "../Scripting/ScriptComponent.h"
#include "../Scripting/ScriptEvent.hpp"
#include "../Scripting/ScriptSystemScene.h"
using namespace Script;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../Logging/Logger.h"
using namespace Logging;

namespace AI
{
	void AIScriptComponent::Initialize( )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ "scriptPath" ] = m_attributes[ System::Attributes::FilePath ];
		parameters[ "name" ] = m_name + "_ai";

		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );
		m_scriptState = scriptService->Execute( "loadScript", parameters )[ "state" ].As< lua_State* >( );

		luabind::globals( m_scriptState )[ "ai" ] = this;
		lua_resume( m_scriptState, 0 );
	}

	void AIScriptComponent::Destroy()
	{
		IService* scriptService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		AnyType::AnyTypeMap parameters;
		parameters[ "name" ] = m_name + "_ai";
		scriptService->Execute( "unloadComponent", parameters );
	}

	void AIScriptComponent::WalkForward( )
	{
		this->PushMessage( System::Messages::Move_Forward, m_attributes );
	}

	void AIScriptComponent::WalkBackward( )
	{
		this->PushMessage( System::Messages::Move_Backward, m_attributes );
	}

	void AIScriptComponent::FacePosition( const Maths::MathVector3& target )
	{
		// Angle from Origin to Player

		MathVector3 aiPosition = m_attributes[ System::Attributes::Position ].As< MathVector3 >( );
		MathVector3 aiToPlayer = ( target - aiPosition ).Normalize( );
		MathVector3 aiToOrigin = ( MathVector3::Zero( ) - aiPosition ).Normalize( );

		float aiToTargetDot = aiToOrigin.DotProduct( aiToPlayer );
		float aiToTargetAngle = acos( Maths::Clamp( aiToTargetDot, -1.0f, 1.0f ) );

		MathVector3 aiToTargetCross = aiToOrigin.CrossProduct( aiToPlayer );

		if( aiToTargetCross.Y < 0 )
		{
			float delta = Maths::PI - aiToTargetAngle;
			aiToTargetAngle = Maths::PI + delta;
		}

		// Angle from Forward to Origin

		MathVector3 aiForward = aiPosition + MathVector3( 0.0f, 0.0f, -1.0f );
		MathVector3 aiToForward = ( aiForward - aiPosition ).Normalize( );

		float aiForwardToOrigin = aiToForward.DotProduct( aiToOrigin );
		float aiForwardToOriginAngle = acos( Maths::Clamp( aiForwardToOrigin, -1.0f, 1.0f ) );

		MathVector3 aiForwardToOriginCross = aiToForward.CrossProduct( aiToOrigin );

		if( aiForwardToOriginCross.Y < 0 )
		{
			float delta = Maths::PI - aiForwardToOriginAngle;
			aiForwardToOriginAngle = Maths::PI + delta;
		}

		// rotate to origin - then rotate to player from origin

		m_attributes[ System::Attributes::Orientation ] = MathQuaternion( MathVector3::Up( ), aiForwardToOriginAngle ) * MathQuaternion( MathVector3::Up( ), aiToTargetAngle );
		this->PushMessage( System::Messages::SetOrientation, m_attributes );
	}

	void AIScriptComponent::FireWeapon( )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "AI_WEAPON_FIRED", m_name ) );
	}

	void AIScriptComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyType::AnyTypeMap parameters;

		parameters[ "entityName" ] = m_name;
		parameters[ "animationName" ] = animationName;
		parameters[ "loopAnimation" ] = loopAnimation;

		service->Execute( "playAnimation", parameters );
	}

	void AIScriptComponent::Update( const float& deltaMilliseconds )
	{
		MathVector3 position = m_attributes[ System::Attributes::Position ].As< MathVector3 >( );

		if ( !m_activeWaypoints.empty( ) )
		{
			if ( m_activeWaypoints.front( ).Round( ) == MathVector3( position.X, 0.0f, position.Z ).Round( ) )
			{
				m_activeWaypoints.pop_front( );
			}
			else
			{
				this->FacePosition( m_activeWaypoints.front( ) );

				//m_attributes[ System::Attributes::LookAt ] = m_activeWaypoints.front( );
				//this->PushMessage( System::Messages::SetLookAt, m_attributes );

				this->WalkForward( );
			}
		}
	}

	Maths::MathVector3 AIScriptComponent::FindRandomWaypoint( )
	{
		IAISystemScene* scene = m_attributes[ System::Attributes::Parent ].As< IAISystemScene* >( );

		ISystemComponent::SystemComponentList waypoints = scene->GetWaypoints( );

		/*Logging::Logger::Debug( "available waypoints" );

		for( ISystemComponent::SystemComponentList::iterator i = waypoints.begin( ); i != waypoints.end( ); ++i )
		{
			MathVector3 position = ( *i )->GetAttributes( )[ System::Attributes::Position ].As< MathVector3 >( );

			std::stringstream message;
			message << position.X << " " << position.Y << " " << position.Z;
			Logging::Logger::Debug( message.str( ) );
		}*/

		MathVector3 waypoint = waypoints[ rand( ) % waypoints.size( ) ]->GetAttributes( )[ System::Attributes::Position ].As< MathVector3 >( );

		/*Logging::Logger::Debug( "found waypoint" );

		std::stringstream message;
		message << waypoint.X << " " << waypoint.Y << " " << waypoint.Z;
		Logging::Logger::Debug( message.str( ) );*/

		return waypoint;
	}

	void AIScriptComponent::NavigateTo( const Maths::MathVector3& destination )
	{
		IAISystemScene* scene = m_attributes[ System::Attributes::Parent ].As< IAISystemScene* >( );
		INavigationMesh* navMesh = scene->GetNavigationMesh( )->GetAttributes( )[ System::Attributes::NavigationMesh ].As< INavigationMesh* >( );
		m_activeWaypoints = navMesh->FindPath( m_attributes[ System::Attributes::Position ].As< MathVector3 >( ), destination );
	}
}
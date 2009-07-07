#include "AIScriptComponent.h"

#include "IAISystemScene.hpp"

#include "Maths/MathUnits.hpp"
using namespace Maths;

#include "Service/IService.hpp"
using namespace Services;

#include "IO/FileBuffer.hpp"
#include "Management/Management.h"

#include "Scripting/ScriptComponent.h"
#include "Scripting/ScriptEvent.hpp"
#include "Scripting/ScriptSystemScene.h"
using namespace Script;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "Logging/Logger.h"
using namespace Logging;

namespace AI
{
	void AIScriptComponent::Initialize( )
	{
		IService* scriptService = Management::Get( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Name ] = m_name + "_ai";
		parameters[ System::Parameters::ScriptPath ] = m_attributes[ System::Parameters::ScriptPath ];

		ISystemComponent* scriptComponent = scriptService->Execute( System::Messages::LoadScript, parameters )[ "component" ].As< ISystemComponent* >( );

		lua_State* scriptState = scriptComponent->Message( System::Messages::GetState, AnyType::AnyTypeMap( ) ).As< lua_State* >( );
		globals( scriptState )[ "ai" ] = this;

		scriptComponent->Message( System::Messages::RunScript, AnyType::AnyTypeMap( ) );
	}

	void AIScriptComponent::Destroy()
	{
		IService* scriptService = Management::Get( )->GetServiceManager( )->FindService( System::Types::SCRIPT );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Attributes::Name ] = m_name + "_ai";
		scriptService->Execute( "unloadComponent", parameters );
	}

	void AIScriptComponent::WalkForward( )
	{
		this->PushMessage( System::Messages::Move_Forward_Pressed, m_attributes );
	}

	void AIScriptComponent::WalkBackward( )
	{
		this->PushMessage( System::Messages::Move_Backward_Pressed, m_attributes );
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

		//m_attributes[ System::Attributes::LookAt ] = m_activeWaypoints.front( );
		//this->PushMessage( System::Messages::SetLookAt, m_attributes );
	}

	void AIScriptComponent::FireWeapon( )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( "WEAPON_FIRED", m_name ) );
	}

	void AIScriptComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );

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

				this->WalkForward( );
			}
		}
		else
		{
			//assert( 0 && "" );
			//this->PushMessage( System::Messages::Move_Idle, AnyType::AnyTypeMap( ) );
		}
	}

	Maths::MathVector3 AIScriptComponent::FindRandomWaypoint( )
	{
		IAISystemScene* scene = m_attributes[ System::Attributes::Parent ].As< IAISystemScene* >( );
		ISystemComponent::SystemComponentList waypoints = scene->GetWaypoints( );
		return waypoints[ rand( ) % waypoints.size( ) ]->GetAttributes( )[ System::Attributes::Position ].As< MathVector3 >( );
	}

	void AIScriptComponent::NavigateTo( const Maths::MathVector3& destination )
	{
		m_activeWaypoints.push_back( destination );
	}

	bool AIScriptComponent::InLineOfSight( const Maths::MathVector3& position )
	{
		IService* physicsService = Management::Get( )->GetServiceManager( )->FindService( System::Types::PHYSICS );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Origin ] = m_attributes[ System::Attributes::Position ].As< MathVector3 >( ) + MathVector3( 0.0f, 1.0f, 0.0f );
		parameters[ System::Parameters::Destination ] = position + MathVector3( 0.0f, 1.0f, 0.0f );
		parameters[ System::Parameters::SortByyDistance ] = true;
		parameters[ System::Parameters::MaxResults ] = 1;

		std::vector< std::string > results = physicsService->Execute( System::Messages::CastRay, parameters )[ "hits" ].As< std::vector< std::string > >( );

		return results.size( ) > 0;
	}

	MathVector3::MathVector3List AIScriptComponent::GetPathTo( const MathVector3& position )
	{
		IAISystemScene* scene = m_attributes[ System::Attributes::Parent ].As< IAISystemScene* >( );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Origin ] = m_attributes[ System::Attributes::Position ].As< MathVector3 >( );
		parameters[ System::Parameters::Destination ] = position;

		return scene->GetNavigationMesh( )->Message( System::Messages::FindPath, parameters ).As< MathVector3::MathVector3List >( );
	}
}
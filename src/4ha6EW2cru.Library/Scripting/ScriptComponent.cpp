#include "ScriptComponent.h"

#include "../Maths/MathMatrix.h"
using namespace Maths;

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Exceptions/ScriptException.hpp"

#include "../IO/IResource.hpp"
using namespace Resources;

#include "../Management/Management.h"

#include "../Events/Event.h"
using namespace Events;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "ScriptEvent.hpp"
#include "ScriptFunctionHandler.hpp"

#include "SoundController.h"

namespace Script
{
	void ScriptComponent::Initialize( AnyValue::AnyValueMap& properties )
	{
		Management::GetInstance( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

		for ( AnyValue::AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
		{
			if ( ( *i ).first == "scriptPath" )
			{
				std::string scriptPath = ( *i ).second.GetValue< std::string >( );
				this->LoadScript( scriptPath );
			}
		}


		SoundController* soundController = new SoundController( this );
		luabind::globals( m_state )[ "sfx" ] = soundController;
		m_controllers.push_back( soundController );
	}

	void ScriptComponent::Destroy()
	{
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

		for ( FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); ++i )	
		{
			delete ( *i );
		}

		for ( FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); ++i )
		{
			delete ( *i );
		}

		for( ScriptControllerList::iterator i = m_controllers.begin( ); i != m_controllers.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void ScriptComponent::LoadScript( const std::string& scriptPath )
	{
		IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( scriptPath );

		int result = luaL_loadbuffer( m_state, resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength, resource->GetFileBuffer( )->filePath.c_str( ) );

		if ( LUA_ERRSYNTAX == result )
		{
			std::stringstream errorMessage;
			errorMessage << lua_tostring( m_state, -1 );
			Logger::Warn( errorMessage.str( ) );
			lua_pop( m_state, 1 );
		}

		if ( LUA_ERRMEM == result )
		{
			ScriptException memE( "Script::Initialize - There is memory allocation error within the Script" );
			Logger::Fatal( memE.what( ) );
			throw memE;
		}
	}

	void ScriptComponent::Execute( )
	{
		lua_pcall( m_state, 0, 0, 0 );
	}

	void ScriptComponent::IncludeScript( const std::string& scriptPath )
	{
		this->LoadScript( scriptPath );
		this->Execute( );
	}


	void ScriptComponent::RegisterEvent( const luabind::object& function )
	{
		m_eventHandlers.push_back( new ScriptFunctionHandler( function ) );
	}

	void ScriptComponent::UnRegisterEvent( const luabind::object& function )
	{
		for ( FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); ++i )
		{
			if ( ( *i )->GetFunction( ) == function )
			{
				( *i )->MarkForDeletion( ); 
			}
		}
	}

	void ScriptComponent::RegisterUpdate( const luabind::object& function )
	{
		m_updateHandlers.push_back( new ScriptFunctionHandler( function ) );
	}

	void ScriptComponent::UnRegisterUpdate( const luabind::object& function )
	{
		for ( FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); ++i )
		{
			if ( ( *i )->GetFunction( ) == function )
			{
				( *i )->MarkForDeletion( ); 
			}
		}
	}

	void ScriptComponent::OnEvent( const IEvent* event )
	{
		for ( FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); ++i )
		{
			EventType eventType = event->GetEventType( );
			
			if ( event->GetEventType( ) == ALL_EVENTS )
			{
				ScriptEvent* scriptEvent = ( ScriptEvent* ) event;

				// this is for speed, _not_ elegance!
				switch( scriptEvent->GetParamType( ) )
				{

				case ScriptEvent::PARAMCOMBO_NONE:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ) );

					break;

				case ScriptEvent::PARAMCOMBO_INT:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsInt( ) );

					break;

				case ScriptEvent::PARAMCOMBO_STRING:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsString( ) );

					break;

				case ScriptEvent::PARAMCOMBO_STRING_STRING:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsString( ), scriptEvent->GetValue2AsString( ) );

					break;

				case ScriptEvent::PARAMCOMBO_STRING_INT:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsString( ), scriptEvent->GetValue2AsInt( ) );

					break;

				case ScriptEvent::PARAMCOMBO_INT_STRING:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsInt( ), scriptEvent->GetValue2AsString( ) );

					break;

				case ScriptEvent::PARAMCOMBO_INT_INT:

					call_function< void >( ( *i )->GetFunction( ), scriptEvent->GetEventName( ), scriptEvent->GetValue1AsInt( ), scriptEvent->GetValue2AsInt( ) );

					break;

				}
			}
		}
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1, const std::string& var2 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1, const int& var2 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1, const std::string& var2 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1, const int& var2 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName ) ); 
	}

	void ScriptComponent::PushChanges( const unsigned int& systemChanges )
	{
		if ( m_observer != 0 )
		{
			m_observer->Observe( this, systemChanges );
		}	
	}

	std::vector< std::string > ScriptComponent::RayQuery( MathVector3 origin, MathVector3 direction, const float& length, const bool& sortByDistance, const unsigned int& maxResults )
	{	
		AnyValue::AnyValueMap parameters;

		MathVector3 originToDestination = direction - origin;
		MathVector3 destination = origin + originToDestination * length;

		parameters[ "origin" ] = origin;
		parameters[ "destination" ] = destination;
		parameters[ "sortByDistance" ] = sortByDistance;
		parameters[ "maxResults" ] = maxResults;

		/*AnyValue::AnyValueMap debugParameters;
		debugParameters[ "origin" ] = origin;
		debugParameters[ "destination" ] = destination;

		IService* renderService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );
		renderService->Execute( "drawLine", debugParameters );*/

		IService* rayService = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::PHYSICS );
		return rayService->Execute( "rayQuery", parameters ) [ "hits" ].GetValue< std::vector< std::string > >( );
	}

	void ScriptComponent::Observe( ISubject* subject, const unsigned int& systemChanges )
	{
		ISystemComponent* component = static_cast< ISystemComponent* >( subject );

		if ( System::Changes::Geometry::Position & systemChanges )
		{
			m_position = component->GetPosition( );
		}

		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			m_orientation = component->GetOrientation( );
		}

		if ( System::Changes::POI::LookAt & systemChanges )
		{
			m_lookAt = component->GetAttributes( )[ "lookAt" ].GetValue< MathVector3 >( );
		}

		if ( System::Changes::Input::Move_Forward & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_MOVE_FORWARD", m_name ) );
		}

		if ( System::Changes::Input::Move_Backward & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_MOVE_BACKWARD", m_name ) );
		}

		if ( System::Changes::Input::Strafe_Right & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_STRAFE_RIGHT", m_name ) );
		}

		if ( System::Changes::Input::Strafe_Left & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_STRAFE_LEFT", m_name ) );
		}

		if( System::Changes::Input::Fire & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_FIRED", m_name ) );
		}
	}

	void ScriptComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyValue::AnyValueMap parameters;

		parameters[ "entityName" ] = m_name;
		parameters[ "animationName" ] = animationName;
		parameters[ "loopAnimation" ] = loopAnimation;

		service->Execute( "playAnimation", parameters );
	}

	void ScriptComponent::Update( const float& deltaMilliseconds )
	{
		for ( FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); ++i )	
		{
			call_function< void >( ( *i )->GetFunction( ), deltaMilliseconds );
		}

		for ( FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); )	
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = m_updateHandlers.erase( i );
			}
			else
			{
				++i;
			}
		}

		for ( FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); )
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = m_eventHandlers.erase( i );
			}
			else
			{
				++i;
			}
		}
	}

	void ScriptComponent::ExecuteString( const std::string& input )
	{
		luaL_dostring( m_state, input.c_str( ) );
	}

	float ScriptComponent::GetTime( ) const
	{
		return Management::GetInstance( )->GetPlatformManager( )->GetTime( );
	}

	AnyValue ScriptComponent::Message( const std::string& message, AnyValue::AnyValueMap parameters )
	{
		return AnyValue( );
	}
}
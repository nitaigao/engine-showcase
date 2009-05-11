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
	}

	void ScriptComponent::Destroy()
	{
		Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

		for ( FunctionList::iterator i = _updateHandlers.begin( ); i != _updateHandlers.end( ); ++i )	
		{
			delete ( *i );
		}

		for ( FunctionList::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
		{
			delete ( *i );
		}
	}

	void ScriptComponent::LoadScript( const std::string& scriptPath )
	{
		IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( scriptPath );

		int result = luaL_loadbuffer( _state, resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength, resource->GetFileBuffer( )->filePath.c_str( ) );

		if ( LUA_ERRSYNTAX == result )
		{
			std::stringstream errorMessage;
			errorMessage << lua_tostring( _state, -1 );
			Logger::Warn( errorMessage.str( ) );
			lua_pop( _state, 1 );
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
		//lua_resume( _state, 0 );
		lua_pcall( _state, 0, 0, 0 );
	}

	void ScriptComponent::IncludeScript( const std::string& scriptPath )
	{
		this->LoadScript( scriptPath );
		this->Execute( );
	}


	void ScriptComponent::RegisterEvent( const luabind::object& function )
	{
		_eventHandlers.push_back( new ScriptFunctionHandler( function ) );
	}

	void ScriptComponent::UnRegisterEvent( const luabind::object& function )
	{
		for ( FunctionList::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
		{
			if ( ( *i )->GetFunction( ) == function )
			{
				( *i )->MarkForDeletion( ); 
			}
		}
	}

	void ScriptComponent::RegisterUpdate( const luabind::object& function )
	{
		_updateHandlers.push_back( new ScriptFunctionHandler( function ) );
	}

	void ScriptComponent::UnRegisterUpdate( const luabind::object& function )
	{
		for ( FunctionList::iterator i = _updateHandlers.begin( ); i != _updateHandlers.end( ); ++i )
		{
			if ( ( *i )->GetFunction( ) == function )
			{
				( *i )->MarkForDeletion( ); 
			}
		}
	}

	void ScriptComponent::OnEvent( const IEvent* event )
	{
		for ( FunctionList::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
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
		if ( _observer != 0 )
		{
			_observer->Observe( this, systemChanges );
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
			_position = component->GetPosition( );
		}

		if ( System::Changes::Geometry::Orientation & systemChanges )
		{
			_orientation = component->GetOrientation( );
		}

		if ( System::Changes::POI::LookAt & systemChanges )
		{
			_lookAt = component->GetProperties( )[ "lookAt" ].GetValue< MathVector3 >( );
		}

		if ( System::Changes::Input::Move_Forward & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_MOVE_FORWARD", _name ) );
		}

		if ( System::Changes::Input::Move_Backward & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_MOVE_BACKWARD", _name ) );
		}

		if ( System::Changes::Input::Strafe_Right & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_STRAFE_RIGHT", _name ) );
		}

		if ( System::Changes::Input::Strafe_Left & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_STRAFE_LEFT", _name ) );
		}

		if( System::Changes::Input::Fire & systemChanges )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "ACTOR_FIRED", _name ) );
		}
	}

	void ScriptComponent::PlayAnimation( const std::string& animationName, const bool& loopAnimation )
	{
		IService* service = Management::GetInstance( )->GetServiceManager( )->FindService( System::Types::RENDER );

		AnyValue::AnyValueMap parameters;

		parameters[ "entityName" ] = _name;
		parameters[ "animationName" ] = animationName;
		parameters[ "loopAnimation" ] = loopAnimation;

		service->Execute( "playAnimation", parameters );
	}

	void ScriptComponent::Update( const float& deltaMilliseconds )
	{
		for ( FunctionList::iterator i = _updateHandlers.begin( ); i != _updateHandlers.end( ); ++i )	
		{
			call_function< void >( ( *i )->GetFunction( ), deltaMilliseconds );
		}

		for ( FunctionList::iterator i = _updateHandlers.begin( ); i != _updateHandlers.end( ); )	
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = _updateHandlers.erase( i );
			}
			else
			{
				++i;
			}
		}

		for ( FunctionList::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); )
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = _eventHandlers.erase( i );
			}
			else
			{
				++i;
			}
		}
	}

	void ScriptComponent::ExecuteString( const std::string& input )
	{
		luaL_dostring( _state, input.c_str( ) );
	}

	float ScriptComponent::GetTime( ) const
	{
		return Management::GetInstance( )->GetPlatformManager( )->GetTime( );
	}
}
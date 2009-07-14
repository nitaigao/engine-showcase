#include "ScriptComponent.h"

#include "Maths/MathMatrix.h"
using namespace Maths;

#include "Logging/Logger.h"
using namespace Logging;

#include "Exceptions/ScriptException.hpp"

#include "IO/IResource.hpp"
using namespace Resources;

#include "Management/Management.h"

#include "Events/Event.h"
#include "Events/ScriptEvent.hpp"
using namespace Events;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "Service/IService.hpp"
using namespace Services;

#include "ScriptFunctionHandler.hpp"

#include "SoundFacade.h"
#include "InstrumentationFacade.h"
#include "AnimationFacade.h"
#include "NetworkFacade.h"
#include "InputFacade.h"

namespace Script
{
	void ScriptComponent::Initialize( )
	{
		Management::Get( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

		this->LoadScript( m_attributes[ System::Parameters::ScriptPath ].As< std::string >( ) );

		SoundFacade* soundFacade = new SoundFacade( this );
		luabind::globals( m_state )[ "sfx" ] = soundFacade;
		m_facades.push_back( soundFacade );

		InstrumentationFacade* instrumentationFacade = new InstrumentationFacade( );
		luabind::globals( m_state )[ "instrumentation" ] = instrumentationFacade;
		m_facades.push_back( instrumentationFacade );

		AnimationFacade* animationFacade = new AnimationFacade( this );
		luabind::globals( m_state ) [ "animation" ] = animationFacade;
		m_facades.push_back( animationFacade );

		NetworkFacade* networkFacade = new NetworkFacade( );
		luabind::globals( m_state ) [ "network" ] = networkFacade;
		m_facades.push_back( networkFacade );

		InputFacade* inputFacade = new InputFacade( );
		luabind::globals( m_state )[ "input" ] = inputFacade;
		m_facades.push_back( inputFacade );
	}

	void ScriptComponent::Destroy( )
	{
		Management::Get( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

		for ( IScriptFunctionHandler::FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); )	
		{
			delete ( *i );
			i = m_updateHandlers.erase( i );
		}

		for ( IScriptFunctionHandler::FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); )
		{
			delete ( *i );
			i = m_eventHandlers.erase( i );
		}

		for( IScriptFacade::ScriptControllerList::iterator i = m_facades.begin( ); i != m_facades.end( ); )
		{
			delete ( *i );
			i = m_facades.erase( i );
		}
	}

	void ScriptComponent::LoadScript( const std::string& scriptPath )
	{
		IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( scriptPath );

		int result = luaL_loadbuffer( m_state, resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength, resource->GetFileBuffer( )->filePath.c_str( ) );

		if ( LUA_ERRSYNTAX == result )
		{
			std::stringstream errorMessage;
			errorMessage << lua_tostring( m_state, -1 );
			Logger::Get( )->Warn( errorMessage.str( ) );
			lua_pop( m_state, 1 );
		}

		if ( LUA_ERRMEM == result )
		{
			ScriptException memE( "Script::Initialize - There is memory allocation error within the Script" );
			Logger::Get( )->Fatal( memE.what( ) );
			throw memE;
		}
	}

	void ScriptComponent::RunScript( )
	{
		try
		{
			resume< void >( m_state );
		}
		catch( error& e )
		{
			object error_msg( from_stack( e.state( ) , -1) );
			std::stringstream logMessage;
			logMessage << error_msg;
			Logger::Get( )->Warn( logMessage.str( ) );
		}
	}

	void ScriptComponent::IncludeScript( const std::string& scriptPath )
	{
		this->LoadScript( scriptPath );
		
		if ( lua_pcall( m_state, 0, 0, 0 ) )
		{
			std::stringstream errorMessage;
			errorMessage << lua_tostring( m_state, -1 );
			Logger::Get( )->Warn( errorMessage.str( ) );
			lua_pop( m_state, 1 );
		}
	}


	void ScriptComponent::ExecuteString( const std::string& input )
	{
		Logger::Get( )->Info( input );

		if ( luaL_dostring( m_state, input.c_str( ) ) )
		{
			std::stringstream errorMessage;
			errorMessage << lua_tostring( m_state, -1 );
			Logger::Get( )->Warn( errorMessage.str( ) );
			lua_pop( m_state, 1 );
		}
	}

	void ScriptComponent::RegisterEvent( const luabind::object& function )
	{
		m_eventHandlers.push_back( new ScriptFunctionHandler( function ) );
	}

	void ScriptComponent::UnRegisterEvent( const luabind::object& function )
	{
		for ( IScriptFunctionHandler::FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); ++i )
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
		for ( IScriptFunctionHandler::FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); ++i )
		{
			if ( ( *i )->GetFunction( ) == function )
			{
				( *i )->MarkForDeletion( ); 
			}
		}
	}

	void ScriptComponent::OnEvent( const IEvent* event )
	{
		for ( IScriptFunctionHandler::FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); ++i )
		{
			EventType eventType = event->GetEventType( );
			
			if ( event->GetEventType( ) == ALL_EVENTS )
			{
				ScriptEvent* scriptEvent = ( ScriptEvent* ) event;

				try
				{
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
				catch( error& e )
				{
					object error_msg( from_stack( e.state( ) , -1) );
					std::stringstream logMessage;
					logMessage << error_msg;
					Logger::Get( )->Warn( logMessage.str( ) );
				}
			}
		}
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1, const std::string& var2 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const std::string& var1, const int& var2 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1, const std::string& var2 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName, const int& var1, const int& var2 )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName, var1, var2 ) ); 
	}

	void ScriptComponent::BroadcastEvent( const std::string& eventName )
	{
		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( eventName ) ); 
	}

	std::vector< std::string > ScriptComponent::RayQuery( MathVector3 origin, MathVector3 direction, const float& length, const bool& sortByDistance, const int& maxResults )
	{	
		AnyType::AnyTypeMap parameters;

		MathVector3 originToDestination = direction - origin;
		MathVector3 destination = origin + originToDestination * length;

		parameters[ System::Parameters::Origin ] = origin;
		parameters[ System::Parameters::Destination ] = destination;
		parameters[ System::Parameters::SortByyDistance ] = sortByDistance;
		parameters[ System::Parameters::MaxResults ] = maxResults;

		/*AnyType::AnyTypeMap debugParameters;
		debugParameters[ "origin" ] = origin;
		debugParameters[ "destination" ] = destination;

		IService* renderService = Management::Get( )->GetServiceManager( )->FindService( System::Types::RENDER );
		renderService->Execute( "drawLine", debugParameters );*/

		IService* rayService = Management::Get( )->GetServiceManager( )->FindService( System::Types::PHYSICS );
		return rayService->Execute( System::Messages::CastRay, parameters ) [ "hits" ].As< std::vector< std::string > >( );
	}

	AnyType ScriptComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		AnyType result;

		if ( message == System::Messages::SetPosition  )
		{
			m_attributes[ System::Attributes::Position ] = parameters[ System::Attributes::Position ].As< MathVector3 >( );
		}

		if ( message ==  System::Messages::SetOrientation )
		{
			m_attributes[ System::Attributes::Orientation ] = parameters[ System::Attributes::Orientation ].As< MathQuaternion >( );
		}

		if ( message == System::Messages::SetLookAt )
		{
			m_lookAt = parameters[ System::Attributes::LookAt ].As< MathVector3 >( );
		}

		if ( message == System::Messages::RunScript )
		{
			this->RunScript( );
		}

		if ( message == System::Messages::PostInitialize )
		{
			this->RunScript( );
		}

		if( message == System::Messages::GetState )
		{
			result = m_state;
		}

		Management::Get( )->GetEventManager( )->QueueEvent( new ScriptEvent( message, m_attributes[ System::Attributes::Name ].As< std::string >( ) ) );

		return result;
	}

	void ScriptComponent::Update( const float& deltaMilliseconds )
	{
		for ( IScriptFunctionHandler::FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); ++i )	
		{
			try
			{
				call_function< void >( ( *i )->GetFunction( ), deltaMilliseconds );
			}
			catch( error& e )
			{
				object error_msg( from_stack( e.state( ) , -1) );
				std::stringstream logMessage;
				logMessage << error_msg;
				Logger::Get( )->Warn( logMessage.str( ) );
			}
		}

		for ( IScriptFunctionHandler::FunctionList::iterator i = m_updateHandlers.begin( ); i != m_updateHandlers.end( ); )	
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

		for ( IScriptFunctionHandler::FunctionList::iterator i = m_eventHandlers.begin( ); i != m_eventHandlers.end( ); )
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

	float ScriptComponent::GetTime( ) const
	{
		return Management::Get( )->GetPlatformManager( )->GetClock( ).GetTime( );
	}
}
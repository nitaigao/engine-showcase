#include "ScriptComponent.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ScriptException.hpp"
#include "../System/Management.h"
#include "../System/IServiceManager.h"

#include "../Events/IEvent.hpp"
#include "../Events/Event.h"

#include "ScriptEvent.hpp"

#include "../Maths/MathMatrix.hpp"

extern "C" 
{
#	include <lualib.h>
#	include <lauxlib.h>
}

#include <luabind/luabind.hpp>
using namespace luabind;

ScriptComponent::~ScriptComponent()
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

	delete _eventHandlers;
	_eventHandlers = 0;
}

void ScriptComponent::Initialize( AnyValueMap properties )
{
	_eventHandlers = new EventHandlerList( );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( ALL_EVENTS, this, &ScriptComponent::OnEvent );

	for ( AnyValueMap::iterator i = properties.begin( ); i != properties.end( ); ++i )
	{
		if ( ( *i ).first == "scriptPath" )
		{
			std::string scriptPath = ( *i ).second.GetValue< std::string >( );
			this->LoadScript( scriptPath );
		}
	}
}

void ScriptComponent::LoadScript( const std::string& scriptPath )
{
	FileBuffer* scriptBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( scriptPath, false );

	int result = luaL_loadbuffer( _state, scriptBuffer->fileBytes, scriptBuffer->fileLength, scriptBuffer->filePath.c_str( ) );

	if ( LUA_ERRSYNTAX == result )
	{
		ScriptException syntaxE( "Script::Initialize - There is a syntax error within the Script" );
		Logger::GetInstance( )->Fatal( syntaxE.what( ) );
		throw syntaxE;
	}

	if ( LUA_ERRMEM == result )
	{
		ScriptException memE( "Script::Initialize - There is memory allocation error within the Script" );
		Logger::GetInstance( )->Fatal( memE.what( ) );
		throw memE;
	}

	delete scriptBuffer;
}

void ScriptComponent::Execute( )
{
	lua_pcall( _state, 0, 0, 0 );
}

void ScriptComponent::IncludeScript( const std::string& scriptPath )
{
	this->LoadScript( scriptPath );
	this->Execute( );
}


void ScriptComponent::RegisterEvent( luabind::object function )
{
	_eventHandlers->push_back( function );
}

void ScriptComponent::OnEvent( const IEvent* event )
{
	EventHandlerList handlers( *_eventHandlers );

	for ( EventHandlerList::iterator i = handlers.begin( ); i != handlers.end( ); ++i )
	{
		EventType eventType = event->GetEventType( );
		
		if ( event->GetEventType( ) == ALL_EVENTS )
		{
			ScriptEvent* scriptEvent = ( ScriptEvent* ) event;
			call_function< void >( ( *i ), scriptEvent->GetEventName( ), scriptEvent->GetValue1( ), scriptEvent->GetValue2( ) );
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

void ScriptComponent::PushChanges( unsigned int systemChanges )
{
	if ( _observer != 0 )
	{
		_observer->Observe( this, systemChanges );
	}	
}

std::vector< std::string > ScriptComponent::RayQuery( bool sortByDistance, int maxResults )
{	
	Ogre::Vector3 axis;
	Ogre::Degree angle;

	_orientation.AsOgreQuaternion( ).ToAngleAxis( angle, axis );

	std::stringstream angleS;
	angleS << Ogre::Degree( Ogre::Radian( angle ) ).valueDegrees( );
	Logger::GetInstance( )->Debug( angleS.str( ) );

	Ogre::Matrix3 rotation;
	rotation.FromAxisAngle( axis * Ogre::Vector3::NEGATIVE_UNIT_Y, angle );

	Ogre::Vector3 facing = Ogre::Vector3::NEGATIVE_UNIT_Z * rotation;
	Ogre::Vector3 destination = ( facing * 100.0f ) + _position.AsOgreVector3( );

	AnyValue::AnyValueMap parameters;
	parameters[ "sortByDistance" ] = sortByDistance;
	parameters[ "origin" ] = _position;
	parameters[ "destination" ] = MathVector3( destination );

	IService* rayService = Management::GetInstance( )->GetServiceManager( )->FindService( PhysicsSystemType );
	return rayService->Execute( "rayQuery", parameters ) [ "hits" ].GetValue< std::vector< std::string > >( );
}

void ScriptComponent::Observe( ISubject* subject, unsigned int systemChanges )
{
	ISystemComponent* component = static_cast< ISystemComponent* >( subject );

	if ( System::Changes::Geometry::Orientation & systemChanges || System::Changes::Geometry::Position & systemChanges )
	{
		_position = component->GetPosition( );
		_orientation = component->GetOrientation( );
	}
}
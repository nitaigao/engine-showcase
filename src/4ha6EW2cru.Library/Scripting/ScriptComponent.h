#ifndef __SCRIPTCOMPONENT_H
#define __SCRIPTCOMPONENT_H

#include "IScriptComponent.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventType.hpp"
#include "../Events/EventData.hpp"

#ifdef WIN32 
#	include <mmsystem.h>
#endif

extern "C" 
{
#	include <lua.h>
}

#include <luabind/luabind.hpp>

class ScriptComponent : public IScriptComponent
{
	typedef std::vector< luabind::object > FunctionList;

public:

	virtual ~ScriptComponent( );

	ScriptComponent( const std::string& name, lua_State* state )
		: _name( name )
		, _state( state )
		, _observer( 0 )
		, _eventHandlers( 0 )
	{

	};

	void Initialize( AnyValue::AnyValueMap properties );
	void Update( float deltaMilliseconds );
	void Destroy( ) { };

	inline void AddObserver( IObserver* observer ) { _observer = observer; };
	void Observe( ISubject* subject, unsigned int systemChanges );
	void PushChanges( unsigned int systemChanges );
	
	inline unsigned int GetRequestedChanges( )
	{
		return 
			System::Changes::Geometry::All |
			System::Changes::Input::All;
	};

	inline const std::string& GetName( ) { return _name; };
	inline System::Types::Type GetType( ) { return System::Types::SCRIPT; };

	AnyValue::AnyValueMap GetProperties( ) { return AnyValue::AnyValueMap( ); };
	void SetProperties( AnyValue::AnyValueMap systemProperties ) { };

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return _scale; };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

	inline lua_State* GetState( ) { return _state; };
	void OnEvent( const Events::IEvent* event );

	/* Script Handlers */

	void Execute( );
	void LoadScript( const std::string& scriptPath );
	void IncludeScript( const std::string& scriptPath );

	void RegisterEvent( luabind::object function );
	void RegisterUpdate( luabind::object function );

	void BroadcastEvent( const std::string& eventName, const std::string& var1 );
	void BroadcastEvent( const std::string& eventName, const int& var1 );
	void BroadcastEvent( const std::string& eventName, const std::string& var1, const std::string& var2 );
	void BroadcastEvent( const std::string& eventName, const std::string& var1, const int& var2 );
	void BroadcastEvent( const std::string& eventName, const int& var1, const std::string& var2 );
	void BroadcastEvent( const std::string& eventName, const int& var1, const int& var2 );

	std::vector< std::string > RayQuery( bool sortByDistance, int maxResults );
	void PlayAnimation( const std::string& animationName, bool loopAnimation );
	
	float GetTime( )
	{ 
#ifdef WIN32
		return timeGetTime( );
#endif
	};

private:

	lua_State* _state;
	std::string _name;

	FunctionList* _eventHandlers;
	FunctionList* _updateHandlers;

	IObserver* _observer;

	MathVector3 _position;
	MathVector3 _scale;
	MathQuaternion _orientation;

	ScriptComponent( const ScriptComponent & copy ) { };
	ScriptComponent & operator = ( const ScriptComponent & copy ) { return *this; };

};

#endif
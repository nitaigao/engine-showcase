#ifndef __SCRIPTCOMPONENT_H
#define __SCRIPTCOMPONENT_H

#include "../System/ISystemComponent.hpp"

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

class ScriptComponent : public ISystemComponent
{
	typedef std::vector< luabind::object > EventHandlerList;

public:

	virtual ~ScriptComponent( );

	ScriptComponent( const std::string& name, lua_State* state )
		: _name( name )
		, _state( state )
		, _observer( 0 )
		, _eventHandlers( 0 )
	{

	};

	void Initialize( AnyValueMap properties );
	void Destroy( ) { };

	inline void AddObserver( IObserver* observer ) { _observer = observer; };
	void PushChanges( unsigned int systemChanges );
	
	inline unsigned int GetRequestedChanges( )
	{
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All;
	};

	void Observe( ISubject* subject, unsigned int systemChanges );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return ScriptSystemType; };

	inline lua_State* GetState( ) { return _state; };

	inline MathVector3 GetPosition( ) { return _position; };
	inline MathVector3 GetScale( ) { return _scale; };
	inline MathQuaternion GetOrientation( ) { return _orientation; };

	void OnEvent( const IEvent* event );

	void Execute( );
	void LoadScript( const std::string& scriptPath );
	void IncludeScript( const std::string& scriptPath );
	void RegisterEvent( luabind::object function );
	void BroadcastEvent( const std::string& eventName, const std::string& var1 );
	void BroadcastEvent( const std::string& eventName, const int& var1 );
	void BroadcastEvent( const std::string& eventName, const std::string& var1, const std::string& var2 );
	void BroadcastEvent( const std::string& eventName, const std::string& var1, const int& var2 );
	void BroadcastEvent( const std::string& eventName, const int& var1, const std::string& var2 );
	void BroadcastEvent( const std::string& eventName, const int& var1, const int& var2 );
	
	float GetTime( )
	{ 
#ifdef WIN32
		return timeGetTime( );
#endif
	};

	std::vector< std::string > RayQuery( bool sortByDistance, int maxResults );

private:

	lua_State* _state;
	std::string _name;

	EventHandlerList* _eventHandlers;
	IObserver* _observer;

	MathVector3 _position;
	MathVector3 _scale;
	MathQuaternion _orientation;

	ScriptComponent( const ScriptComponent & copy ) { };
	ScriptComponent & operator = ( const ScriptComponent & copy ) { return *this; };

};

#endif
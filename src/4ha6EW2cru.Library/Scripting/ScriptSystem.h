#ifndef __SCRIPTSYSTEM_H
#define __SCRIPTSYSTEM_H

#include "../System/ISystem.hpp"
#include "ScriptObject.h"

#include "../IO/IFileManager.hpp"
#include "../System/IConfiguration.hpp"

extern "C" 
{
#	include <lua.h>
#	include <lualib.h>
}

#include <luabind/luabind.hpp>
using namespace luabind;

class ScriptSystem : public ISystem
{
	typedef std::vector< ScriptObject* > ScriptObjectList;

public:

	ScriptSystem( IConfiguration* configuration );
	ScriptSystem( IConfiguration* configuration, IFileManager* fileManager );

	virtual ~ScriptSystem( );

	void Initialize( );
	void Update( float deltaMilliseconds ) { };
	void Release( ) { };

	inline SystemType GetSystemType( ) { return ScriptSystemType; };

	ISystemObject* CreateObject( const std::string& name, SystemType systemType );

private:

	ScriptSystem( const ScriptSystem & copy ) { };
	ScriptSystem & operator = ( const ScriptSystem & copy ) { return *this; };

	void Constructor( IConfiguration* configuration, IFileManager* fileManager )
	{
		_fileManager = fileManager;

		_configuration = configuration;
		_state = lua_open( );
	}

	IConfiguration* _configuration;
	IFileManager* _fileManager;

	lua_State* _state;
	ScriptObjectList _scriptObjects;

};

#endif
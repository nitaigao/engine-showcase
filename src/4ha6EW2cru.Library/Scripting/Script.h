#ifndef __SCRIPT_H
#define __SCRIPT_H

#include <map>

extern "C" 
{
#	include <lua.h>
}

#include "../Logging/Logger.h"

#include "../Events/EventType.hpp"
#include "../Events/IEvent.hpp"
#include "../IO/FileBuffer.hpp"

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
using namespace luabind;

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"

/*!
	An interface for loading LUA scripts
*/
class Script
{

	typedef std::map< EventType, object > EventHandlers;
	
public:

	Script( lua_State* parentState, FileBuffer* fileBuffer );
	~Script( );

	/*! Initializes the Script from a FileBuffer */
	void Initialize( );

	/*! Returns the Script state */
	inline lua_State* GetState( ) const { return _luaState; };

	/*! Loads a Script into the current state */
	void Include( std::string scriptPath );
	
	template< class P1, class P2, class P3 >
	void CallFunction( std::string functionName, const P1 &p1, const P2 &p2, const P3 &p3 )
	{
		if ( !_isInitialized )
		{
			UnInitializedException unE( "Script::CallFunction - Script is not Initialized" );
			Logger::GetInstance( )->Fatal( unE.what( ) );
			throw unE;
		}

		if( functionName.empty( ) )
		{
			OutOfRangeException outE( "Script::CallFunction - The given function name is empty" ); 
			Logger::GetInstance( )->Fatal( outE.what( ) );
			throw outE;
		}
		
		call_function< int >( _luaState, functionName.c_str( ), p1, p2, p3 );
	}
	
	template< class P1, class P2 >
	void CallFunction( std::string functionName, const P1 &p1, const P2 &p2 )
	{
		this->CallFunction( functionName, p1, p2, 0 );
	}
	
	template< class P1 >
	void CallFunction( std::string functionName, const P1 &p1 )
	{
		this->CallFunction( functionName, p1, 0 );
	}
	
	void CallFunction( std::string functionName )
	{
		this->CallFunction( functionName, 0, 0, 0 );
	}

private:

	Script( );

	Script( const Script & copy ) { };
	Script & operator = ( const Script & copy ) { return *this; }

	lua_State* _parentState;
	lua_State* _luaState;
	EventHandlers _eventHandlers;
	FileBuffer* _fileBuffer;
	bool _isInitialized;
	
};
	
#endif
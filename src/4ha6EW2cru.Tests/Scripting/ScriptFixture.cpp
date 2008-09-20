#include "ScriptFixture.h"

#include "Events/Event.h"
#include "Events/IEventListener.hpp"
#include "Events/EventManager.h"
#include "Logging/AppenderFactory.h"
#include "Logging/Logger.h"
#include "IO/FileManager.h"

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/OutOfRangeException.hpp"

#include "../Mocks/MockEventData.hpp"
#include "../Mocks/MockScriptBinder.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( ScriptFixture );

void ScriptFixture::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( AppenderFactory::CreateAppender( CONSOLEAPPENDER ) );
	FileManager::Initialize( );
	EventManager::Initialize( );

	MockScriptBinder::handle_count = 0;

	FileManager::GetInstance( )->AddFileStore( "../game/test" );
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( "testscript.lua" );
	_script = Script::CreateFromFileBuffer( scriptBuffer );
}

void ScriptFixture::tearDown( )
{
	delete _script;

	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );

	MockScriptBinder::handle_count = 0;
}

void ScriptFixture::Should_Initialize_Given_Valid_Script( )
{
	_script->Initialize( );
}

void ScriptFixture::Should_Throw_On_Initialize_Given_Invalid_Script( )
{
	FileManager::GetInstance( )->AddFileStore( "../game/test" );
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( "garbage.lua" );
	Script* script = Script::CreateFromFileBuffer( scriptBuffer );
	CPPUNIT_ASSERT_THROW( script->Initialize( ), ScriptException );
}

void ScriptFixture::Should_Throw_On_Initialize_Given_Already_Initialized( )
{
	_script->Initialize( );
	CPPUNIT_ASSERT_THROW( _script->Initialize( ), AlreadyInitializedException );
}

void ScriptFixture::Should_Throw_On_CreateFromFileBuffer_Given_NULL_FileBuffer( )
{
	CPPUNIT_ASSERT_THROW( Script::CreateFromFileBuffer( 0 ), NullReferenceException );
}

void ScriptFixture::Should_Throw_On_CreateFromFileBuffer_Given_Invalid_FileBuffer( )
{
	FileBuffer* fileBuffer = new FileBuffer( );
	CPPUNIT_ASSERT_THROW( Script::CreateFromFileBuffer( fileBuffer ), UnInitializedException );
}

void ScriptFixture::Should_CreateFromFileBuffer_Given_Valid_FileBuffer( )
{
	FileManager::GetInstance( )->AddFileStore( "../game/test" );
	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( "testscript.lua" );
	Script* script = Script::CreateFromFileBuffer( scriptBuffer );

	delete script;
}

void ScriptFixture::Should_Throw_On_CallFunction_Given_Not_Initialized( )
{
	CPPUNIT_ASSERT_THROW( _script->CallFunction( "testFunction" ), UnInitializedException );
}

void ScriptFixture::Should_Throw_On_CallFunction_Given_Empty_FunctionName( )
{
	_script->Initialize( );

	CPPUNIT_ASSERT_THROW( _script->CallFunction( "" ), OutOfRangeException );
}

void ScriptFixture::Should_Throw_On_CallFunction_Given_Non_Existing_FunctionName( )
{
	_script->Initialize( );

	CPPUNIT_ASSERT_THROW( _script->CallFunction( "randomFunction" ), ScriptException );
}

void ScriptFixture::Should_Throw_On_CallFunction_Given_Lua_Error( )
{
	_script->Initialize( );

	CPPUNIT_ASSERT_THROW( _script->CallFunction( "killerFunction" ), ScriptException );
}

void ScriptFixture::Should_CallFunction_Given_Valid_FunctionName( )
{
	_script->Initialize( );

	_script->CallFunction( "testFunction" );
}

void ScriptFixture::Should_Thow_On_GetState_Given_Not_Initialized( )
{
	CPPUNIT_ASSERT_THROW( _script->GetState( ), UnInitializedException );
}

void ScriptFixture::Should_GetState_Given_Intialized( )
{
	_script->Initialize( );

	CPPUNIT_ASSERT( _script->GetState( ) != 0 );
}
#include "ScriptFixture.h"

#include "Events/Event.h"
#include "Events/IEventListener.hpp"
#include "Events/EventManager.h"
#include "Logging/AppenderFactory.h"
#include "Logging/Logger.h"
#include "Scripting/Script.h"
#include "IO/FileManager.h"
#include "Exceptions/ScriptException.hpp"

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
}

void ScriptFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );

	MockScriptBinder::handle_count = 0;
}

void ScriptFixture::Should_Not_Initialize_NonExistant_Script( )
{
	FileBuffer* fileBuffer = new FileBuffer( );
	Script* script = Script::CreateFromFileBuffer( fileBuffer );

	bool result = script->Initialize( );
	CPPUNIT_ASSERT( !result );

	delete script;
}

void ScriptFixture::Should_Load_Script_Successfully( )
{
	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "test/testscript.lua" );
	CPPUNIT_ASSERT( fileBuffer != 0 );

	Script* script = Script::CreateFromFileBuffer( fileBuffer );
	bool result = script->Initialize( );

	CPPUNIT_ASSERT( result );
	CPPUNIT_ASSERT( script->GetState( ) != 0 );

	delete script;
}

void ScriptFixture::Should_Recieve_Event( )
{
	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "test/testscript.lua" );
	
	Script* script = Script::CreateFromFileBuffer( fileBuffer );
	script->Initialize( );

	module( script->GetState( ) )
	[
		def( "handledEvent",  &MockScriptBinder::HandleEvent )
	];

	script->CallFunction( "Should_Recieve_Event_Setup" );
	EventManager::GetInstance( )->QueueEvent( new Event( TEST_EVENT ) );
	EventManager::GetInstance( )->Update( );

	CPPUNIT_ASSERT( MockScriptBinder::handle_count == 1 );

	delete script;
}

void ScriptFixture::Should_Call_Function( )
{
	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "test/testscript.lua" );
	
	Script* script = Script::CreateFromFileBuffer( fileBuffer );
	script->Initialize( );

	module( script->GetState( ) )
	[
		def( "shouldCallFunction",  &MockScriptBinder::HandleEvent )
	];

	script->CallFunction( "Should_Call_Function" );
	CPPUNIT_ASSERT( MockScriptBinder::handle_count == 1 );

	delete script;
}

void ScriptFixture::Should_Throw_On_Script_Error( )
{
	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "test/testscript.lua" );
	
	Script* script = Script::CreateFromFileBuffer( fileBuffer );
	script->Initialize( );

	CPPUNIT_ASSERT_THROW( script->CallFunction( "Random_Function" ), ScriptException );

	delete script;
}
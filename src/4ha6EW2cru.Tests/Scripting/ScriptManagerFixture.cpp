#include "ScriptManagerFixture.h"

#include "Scripting/ScriptManager.h"
#include "Exceptions/ScriptException.hpp"

#include "Logging/Logger.h"

#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/NullReferenceException.hpp"

#include "IO/FileManager.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScriptManagerFixture, Suites::ScriptingSuite( ) );

void ScriptManagerFixture::setUp( )
{
	Logger::Initialize( );
}

void ScriptManagerFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Initialize_And_Release_Correctly( )
{
	ScriptManager::Initialize( 0 );
	ScriptManager::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	ScriptManager::Initialize( 0 );
	CPPUNIT_ASSERT_THROW( ScriptManager::Initialize( 0 ), AlreadyInitializedException );
	ScriptManager::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Throw_On_Uninitialized_Release( )
{
	CPPUNIT_ASSERT_THROW( ScriptManager::GetInstance( )->Release( ), UnInitializedException );
}

void ScriptManagerFixture::Should_Throw_On_UnInitialized_GetInstance( )
{
	CPPUNIT_ASSERT_THROW( ScriptManager::GetInstance( )->Release( ), UnInitializedException );
}

void ScriptManagerFixture::Should_GetInstance_Correctly_Given_Intiailized( )
{
	ScriptManager::Initialize( 0 );
	ScriptManager::GetInstance( );
	ScriptManager::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Create_A_New_Script( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/test", "/" );

	ScriptManager::Initialize( 0 );
	Script* script = ScriptManager::GetInstance( )->CreateScript( "testscript.lua" );

	CPPUNIT_ASSERT( 0 != script );
	delete script;

	FileManager::GetInstance( )->Release( );

	ScriptManager::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Destroy_Script( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/test", "/" );

	ScriptManager::Initialize( 0 );
	Script* script = ScriptManager::GetInstance( )->CreateScript( "testscript.lua" );

	ScriptManager::GetInstance( )->DestroyScript( script );

	FileManager::GetInstance( )->Release( );
	ScriptManager::GetInstance( )->Release( );
}

void ScriptManagerFixture::Should_Throw_On_DestroyScript_Given_NULL_Script( )
{
	ScriptManager::Initialize( 0 );
	CPPUNIT_ASSERT_THROW(  ScriptManager::GetInstance( )->DestroyScript( 0 ), NullReferenceException );
	ScriptManager::GetInstance( )->Release( );
}
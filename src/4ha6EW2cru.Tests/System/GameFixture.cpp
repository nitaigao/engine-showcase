#include "GameFixture.h"

#include "System/Game.h"
#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GameFixture, Suites::SystemSuite( ) );

void GameFixture::Should_Initialize( )
{
	Game game;
	game.Initialize( );
	game.Release( );
}

void GameFixture::Should_Throw_On_Initialize_Given_Already_Initialized( )
{
	Game game;
	game.Initialize( );
	CPPUNIT_ASSERT_THROW( game.Initialize( ), AlreadyInitializedException );
	game.Release( );
}

void GameFixture::Should_Loop_Once( )
{
	Game game;
	game.Initialize( );
	game.StartLoop( true );
	game.Release( );
}

void GameFixture::Should_Release_Given_Initialized( )
{
	Game game;
	game.Initialize( );
	game.Release( );
}

void GameFixture::Should_Throw_On_Release_Given_Not_Initialized( )
{
	Game game;
	CPPUNIT_ASSERT_THROW( game.Release( ), UnInitializedException );
}

void GameFixture::Should_Throw_On_StartLoop_Given_Not_Initialized( )
{
	Game game;
	CPPUNIT_ASSERT_THROW( game.StartLoop( true ), UnInitializedException );
}
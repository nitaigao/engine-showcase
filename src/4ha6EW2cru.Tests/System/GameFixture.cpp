#include "GameFixture.h"

#include "System/Game.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GameFixture );

void GameFixture::Should_SuccessFully_Initialize_And_Release( )
{
	Game game;
	bool result = game.Initialize( );

	CPPUNIT_ASSERT( result );

	game.Release( );
}

void GameFixture::Should_Loop_Once_And_Release( )
{
	Game game;
	game.Initialize( );
	game.StartLoop( true );
	game.Release( );
}
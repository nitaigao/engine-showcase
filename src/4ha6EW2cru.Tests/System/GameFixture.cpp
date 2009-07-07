#include "GameFixture.h"

#include "Exceptions/AlreadyInitializedException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GameFixture, Suites::SystemSuite( ) );

#include "../Mocks/Mock_System.hpp"

void GameFixture::setUp( )
{

}

void GameFixture::tearDown( )
{

}

void GameFixture::Should_Initialize( )
{
	/*Game game;
	game.Initialize( );
	game.Release( );*/
}

void GameFixture::Should_Throw_On_Initialize_Given_Already_Initialized( )
{
	/*Game game( _configuration );
	game.Initialize( );
	CPPUNIT_ASSERT_THROW( game.Initialize( ), AlreadyInitializedException );
	game.Release( );*/
}

void GameFixture::Should_Call_Release_On_All_Systems_Given_Initialized( )
{
	/*Mock_System* mockSystem = new Mock_System( );
	mockSystem->release_count.setExpected( 1 );

	Game game( _configuration );
	game.AddSystem( mockSystem );
	game.Initialize( );
	game.Release( );
	mockSystem->verify( );

	delete mockSystem;*/
}

void GameFixture::Should_Throw_On_Release_Given_Not_Initialized( )
{
	/*Game game( _configuration );
	CPPUNIT_ASSERT_THROW( game.Release( ), UnInitializedException );*/
}

void GameFixture::Should_Throw_On_Update_Given_Not_Initialized( )
{
	/*Game game( _configuration );
	CPPUNIT_ASSERT_THROW( game.Update( 0 ), UnInitializedException );*/
}

void GameFixture::Should_Call_Update_On_All_Systems_On_Update( )
{
	/*Mock_System* mockSystem = new Mock_System( );
	mockSystem->update_count.setExpected( 1 );

	Game game( _configuration );
	game.AddSystem( mockSystem );
	game.Initialize( );
	game.Update( 0 );

	mockSystem->verify( );

	game.Release( );

	delete mockSystem;*/
}

void GameFixture::Should_Call_Initialize_On_All_Systems_On_Intialize( )
{
	/*Mock_System* mockSystem = new Mock_System( );
	mockSystem->initialize_count.setExpected( 1 );

	Game game( _configuration );
	game.AddSystem( mockSystem );
	game.Initialize( );

	mockSystem->verify( );

	game.Release( );

	delete mockSystem;*/
}
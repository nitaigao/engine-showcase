#include "GameState_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GameState_Tests, Suites::LogicSuite( ) );

#include "../Mocks/Mock_LevelLoader.hpp"
#include "../Mocks/Mock_World.hpp"

#include "Logic/GameState.h"

#include "Events/Event.h"
#include "Events/EventData.hpp"

void GameState_Tests::setUp( )
{

}

void GameState_Tests::tearDown( )
{

}

void GameState_Tests::Should_Call_The_Level_Loader_On_GAME_LEVEL_CHANGED_Events( )
{
	Mock_LevelLoader* levelLoader = new Mock_LevelLoader( );
	levelLoader->loadLevel_count.setExpected( 1 );

	Mock_World* world = new Mock_World( );

	GameState gameState( levelLoader, world );

	IEventData* eventData = new LevelChangedEventData( "level0" );
	IEvent* event = new Event( GAME_LEVEL_CHANGED, eventData );

	gameState.OnLevelChanged( event );

	levelLoader->loadLevel_count.verify( );

	delete event;
}
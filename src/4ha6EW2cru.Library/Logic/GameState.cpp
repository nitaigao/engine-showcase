#include "GameState.h"

#include "../Events/EventData.hpp"

GameState::~GameState( )
{
	delete _levelLoader;
	delete _world;
}

void GameState::OnLevelChanged( const IEvent* event )
{
	LevelChangedEventData* eventData = static_cast< LevelChangedEventData* >( event->GetEventData( ) );
	_levelLoader->LoadLevel( eventData->GetLevelName( ), _world );
}
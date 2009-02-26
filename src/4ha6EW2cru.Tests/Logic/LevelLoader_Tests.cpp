#include "LevelLoader_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( LevelLoader_Tests, Suites::LogicSuite( ) );

#include "IO/FileManager.h"
#include "Logging/Logger.h"

#include "Logic/LevelLoader.h"
#include "../Mocks/Mock_World.hpp"

void LevelLoader_Tests::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/levels.bad", "data/" );
}

void LevelLoader_Tests::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void LevelLoader_Tests::Should_Call_World_AddEntity_On_LoadLevel( )
{
	Mock_World* world = new Mock_World( );

	LevelLoader levelLoader;
	levelLoader.LoadLevel( "level0", world );

	delete world;

}
#include "WorldLoader_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( WorldLoader_Tests, Suites::StateSuite( ) );

#include "../Mocks/Mock_FileManager.hpp"
#include "../Mocks/Mock_World.hpp"
#include "../Mocks/Mock_System.hpp"

#include "State/WorldLoader.h"
#include "IO/FileManager.h"
#include "Logging/Logger.h"

void WorldLoader_Tests::Should_Call_FileManager_On_Load_To_Load_Level_File( )
{
	/*Mock_FileManager fileManager;
	fileManager.getFile_count.setExpected( 1 );

	Mock_World world;

	WorldLoader loader( &world, &fileManager );
	loader.Load( "/path/to/level/file" );

	fileManager.verify( );*/
}

void WorldLoader_Tests::Should_Load_And_Parse_A_YAML_File( )
{
	Logger::Initialize( );
	FileManager fileManager;
	fileManager.MountFileStore( "../game/data/levels.bad", "data/" );

	Mock_World world;

	Mock_System system1( RenderSystemType );
	world.RegisterSystem( &system1 );

	Mock_System system2( GeometrySystemType );
	world.RegisterSystem( &system2 );

	Mock_System system3( PhysicsSystemType );
	world.RegisterSystem( &system3 );

	WorldLoader loader( &world, &fileManager );
	loader.Load( "/data/levels/level0.yaml" );

	fileManager.Release( );
	Logger::GetInstance( )->Release( );
}
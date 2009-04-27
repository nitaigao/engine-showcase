#include "World_Tests.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( World_Tests, Suites::StateSuite( ) );

#include "../Mocks/Mock_System.hpp"
#include "State/IWorldEntity.hpp"
#include "State/World.h"
using namespace State;

#include "Management/Management.h"

void World_Tests::Entity_Should_Be_Created_on_CreateEntity()
{
	World world;
	IWorldEntity* entity = world.CreateEntity( "test" );

	CPPUNIT_ASSERT( entity != 0 );
}

void World_Tests::GetSystemScenes_Should_Return_All_Extended_Scenes()
{
	World world;

	Mock_System system1( System::Types::RENDER );
	world.AddSystemScene( system1.CreateScene( ) );

	CPPUNIT_ASSERT( !world.GetSystemScenes( ).empty( ) );
}

void World_Tests::Update_Should_Call_Update_On_All_Attached_SystemScenes()
{
	World world;

	Mock_SystemScene* scene = new Mock_SystemScene( System::Types::TEST );
	scene->update_count.setExpected( 1 );

	world.AddSystemScene( scene );

	world.Update( 1.0f / 100.0f );

	scene->verify( );
}

void World_Tests::setUp()
{

}

void World_Tests::tearDown()
{

}
#ifndef __WORLD_TESTS_H
#define __WORLD_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class World_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( World_Tests );
	CPPUNIT_TEST( Should_Call_CreateScene_On_The_Given_System_On_Extend );
	CPPUNIT_TEST( Entity_Should_Be_Created_on_CreateEntity );
	CPPUNIT_TEST( GetSystemScenes_Should_Return_All_Extended_Scenes );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Call_CreateScene_On_The_Given_System_On_Extend( );
	void Entity_Should_Be_Created_on_CreateEntity( );
	void GetSystemScenes_Should_Return_All_Extended_Scenes( );

};

#endif
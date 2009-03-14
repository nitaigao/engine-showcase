#ifndef __WORLD_TESTS_H
#define __WORLD_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class World_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( World_Tests );
	CPPUNIT_TEST( Entity_Should_Be_Created_on_CreateEntity );
	CPPUNIT_TEST( GetSystemScenes_Should_Return_All_Extended_Scenes );
	CPPUNIT_TEST( Update_Should_Call_Update_On_All_Attached_SystemScenes );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Entity_Should_Be_Created_on_CreateEntity( );
	void GetSystemScenes_Should_Return_All_Extended_Scenes( );
	void Update_Should_Call_Update_On_All_Attached_SystemScenes( );

};

#endif
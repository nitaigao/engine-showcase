#ifndef __GAMESTATE_TESTS_H
#define __GAMESTATE_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class GameState_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( GameState_Tests );

	CPPUNIT_TEST( Should_Call_The_Level_Loader_On_GAME_LEVEL_CHANGED_Events );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:
	
	void Should_Call_The_Level_Loader_On_GAME_LEVEL_CHANGED_Events( );

};

#endif
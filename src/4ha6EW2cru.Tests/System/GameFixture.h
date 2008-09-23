#ifndef __GAME_FIXTURE_H
#define __GAME_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class GameFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( GameFixture );
	CPPUNIT_TEST( Should_Initialize );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Initialized );
	CPPUNIT_TEST( Should_Loop_Once );
	CPPUNIT_TEST( Should_Throw_On_StartLoop_Given_Not_Initialized );
	CPPUNIT_TEST( Should_Release_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Release_Given_Not_Initialized );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Initialize( );
	void Should_Throw_On_Initialize_Given_Already_Initialized( );
	void Should_Loop_Once( );
	void Should_Throw_On_StartLoop_Given_Not_Initialized( );
	void Should_Release_Given_Initialized( );
	void Should_Throw_On_Release_Given_Not_Initialized( );

};

#endif
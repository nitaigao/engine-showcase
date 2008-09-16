#ifndef __GAME_FIXTURE_H
#define __GAME_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class GameFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( GameFixture );
	CPPUNIT_TEST( Should_SuccessFully_Initialize_And_Release );
	CPPUNIT_TEST( Should_Loop_Once_And_Release );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_SuccessFully_Initialize_And_Release( );
	void Should_Loop_Once_And_Release( );

};

#endif
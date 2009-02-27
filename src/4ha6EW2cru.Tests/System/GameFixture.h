#ifndef __GAME_FIXTURE_H
#define __GAME_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "System/IConfiguration.hpp"

class GameFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( GameFixture );
	CPPUNIT_TEST( Should_Initialize );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Update_Given_Not_Initialized );
	CPPUNIT_TEST( Should_Call_Update_On_All_Systems_On_Update );
	CPPUNIT_TEST( Should_Call_Initialize_On_All_Systems_On_Intialize );
	CPPUNIT_TEST( Should_Call_Release_On_All_Systems_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Release_Given_Not_Initialized );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize( );
	void Should_Throw_On_Initialize_Given_Already_Initialized( );
	void Should_Throw_On_Update_Given_Not_Initialized( );
	void Should_Call_Release_On_All_Systems_Given_Initialized( );
	void Should_Throw_On_Release_Given_Not_Initialized( );
	void Should_Call_Update_On_All_Systems_On_Update( );
	void Should_Call_Initialize_On_All_Systems_On_Intialize( );

private:

	IConfiguration* _configuration;

};

#endif
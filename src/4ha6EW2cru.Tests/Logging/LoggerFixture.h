#ifndef __LOGGER_FIXTURE_H
#define __LOGGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class LoggerFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( LoggerFixture );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
	CPPUNIT_TEST( Should_Release_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Release_Given_ALready_Released );
	CPPUNIT_TEST( Should_GetIntance_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_GetInstance_Given_Not_Initialized );
	CPPUNIT_TEST( Should_Log_On_Debug_Given_Valid_Message );
	CPPUNIT_TEST( Should_Log_On_Info_Given_Valid_Message );
	CPPUNIT_TEST( Should_Log_On_Fatal_Given_Valid_Message );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Throw_On_Initialize_Given_Already_Intialized( );
	void Should_Release_Given_Initialized( );
	void Should_Throw_On_Release_Given_ALready_Released( );
	void Should_GetIntance_Given_Initialized( );
	void Should_Throw_On_GetInstance_Given_Not_Initialized( );
	void Should_Log_On_Debug_Given_Valid_Message( );
	void Should_Log_On_Info_Given_Valid_Message( );
	void Should_Log_On_Fatal_Given_Valid_Message( );

};

#endif
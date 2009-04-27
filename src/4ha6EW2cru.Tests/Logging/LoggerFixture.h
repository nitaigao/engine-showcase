#ifndef __LOGGER_FIXTURE_H
#define __LOGGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class LoggerFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( LoggerFixture );
	CPPUNIT_TEST( Should_Log_On_Debug_Given_Valid_Message );
	CPPUNIT_TEST( Should_Log_On_Info_Given_Valid_Message );
	CPPUNIT_TEST( Should_Log_On_Fatal_Given_Valid_Message );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Log_On_Debug_Given_Valid_Message( );
	void Should_Log_On_Info_Given_Valid_Message( );
	void Should_Log_On_Fatal_Given_Valid_Message( );

};

#endif
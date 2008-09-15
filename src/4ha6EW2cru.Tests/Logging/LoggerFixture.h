#ifndef __LOGGER_FIXTURE_H
#define __LOGGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class LoggerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( LoggerFixture );
  CPPUNIT_TEST( Should_Initialize_And_Release_Correctly );
  CPPUNIT_TEST( Should_Log_To_Appender );
  CPPUNIT_TEST_EXCEPTION( Should_Throw_On_UnInitialized_GetInstance, UnInitializedException );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
  CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Initialize_And_Release_Correctly( );
	void Should_Log_To_Appender( );
	void Should_Throw_On_UnInitialized_GetInstance( );
	void Should_Throw_On_Initialize_Given_Already_Intialized( );

};

#endif
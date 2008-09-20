#ifndef __APPENDERFACTORY_FIXTURE_H
#define __APPENDERFACTORY_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class AppenderFactoryFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( AppenderFactoryFixture );
  CPPUNIT_TEST( Should_Return_ConsoleAppender );
  CPPUNIT_TEST( Should_Return_FileAppender );
  CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Return_ConsoleAppender( );
	void Should_Return_FileAppender( );

};

#endif
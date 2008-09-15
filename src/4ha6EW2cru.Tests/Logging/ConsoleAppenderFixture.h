#ifndef __CONSOLEAPPENDER_H
#define __CONSOLEAPPENDER_H

#include <cppunit/extensions/HelperMacros.h>

class ConsoleAppenderFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( ConsoleAppenderFixture );
  CPPUNIT_TEST( Should_Append_Message );
  CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Append_Message( );

};

#endif
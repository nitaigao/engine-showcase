#ifndef __FILEAPPENDER_FIXTURE_H
#define __FILEAPPENDER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class FileAppenderFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( FileAppenderFixture );
  CPPUNIT_TEST( Should_Not_Initialize_On_File_Create_Error );
  CPPUNIT_TEST( Should_Initialize_Correctly );
  CPPUNIT_TEST( Should_Append_Message );
  CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Not_Initialize_On_File_Create_Error( );
	void Should_Append_Message( );

};

#endif
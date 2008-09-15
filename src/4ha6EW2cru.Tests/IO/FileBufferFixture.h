#ifndef FILEBUFFER_FIXTURE_H
#define FILEBUFFER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class FileBufferFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( FileBufferFixture );
  CPPUNIT_TEST( Should_Contain_Given_Values );
  CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Contain_Given_Values( );

};

#endif
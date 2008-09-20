#ifndef __FILEAPPENDER_FIXTURE_H
#define __FILEAPPENDER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class FileAppenderFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( FileAppenderFixture );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_File_Create_Error );
	CPPUNIT_TEST( Should_Initialize_Correctly_Given_File_Can_Be_Creaed_Or_Appended );
	CPPUNIT_TEST( Should_Throw_On_Intialized_Given_Already_Intialized );
	CPPUNIT_TEST( Should_Append_Message );
	CPPUNIT_TEST( Should_Throw_On_Append_Given_Append_Error );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Throw_On_Initialize_Given_File_Create_Error( );
	void Should_Initialize_Correctly_Given_File_Can_Be_Creaed_Or_Appended( );
	void Should_Throw_On_Intialized_Given_Already_Intialized( );
	void Should_Append_Message( );
	void Should_Throw_On_Append_Given_Append_Error( );

};

#endif
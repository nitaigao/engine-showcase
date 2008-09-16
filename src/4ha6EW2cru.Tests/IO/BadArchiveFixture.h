#ifndef __BADARCHIVE_FIXTURE_H
#define __BADARCHIVE_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class BadArchiveFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( BadArchiveFixture );
	CPPUNIT_TEST( Should_Return_False_From_Exists_Given_NON_Existing_File );
	CPPUNIT_TEST( Should_Return_True_From_Exists_Given_File_Exists );
	CPPUNIT_TEST( Sould_Return_NULL_Given_File_Not_Found );
	CPPUNIT_TEST( Should_Return_Not_NULL_Given_File_Found );
	CPPUNIT_TEST( Should_Find_Some_Files );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Return_False_From_Exists_Given_NON_Existing_File( );
	void Should_Return_True_From_Exists_Given_File_Exists( );
	void Sould_Return_NULL_Given_File_Not_Found( );
	void Should_Return_Not_NULL_Given_File_Found( );
	void Should_Find_Some_Files( );

};

#endif
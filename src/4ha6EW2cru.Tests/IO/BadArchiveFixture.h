#ifndef __BADARCHIVE_FIXTURE_H
#define __BADARCHIVE_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class BadArchiveFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( BadArchiveFixture );
	CPPUNIT_TEST( Should_Return_True_On_Case_Sensitive );

	CPPUNIT_TEST( Should_Return_DataStreamPtr_Given_File_Found );
	CPPUNIT_TEST( Sould_Return_Throw_Given_File_Not_Found );

	CPPUNIT_TEST( Should_Return_False_From_Exists_Given_NON_Existing_File );
	CPPUNIT_TEST( Should_Return_True_From_Exists_Given_File_Exists );
	
	CPPUNIT_TEST( Should_Return_Empty_StringVectorPtr_On_List );
	CPPUNIT_TEST( Should_Return_Empty_FileInfoPtr_On_ListFileInfo );

	CPPUNIT_TEST( Should_Return_Empty_StringVectorPtr_On_Find );
	CPPUNIT_TEST( Should_Return_Empty_FileInfoPtr_On_FindFileInfo );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Return_True_On_Case_Sensitive( );
	void Should_Return_DataStreamPtr_Given_File_Found( );
	void Sould_Return_Throw_Given_File_Not_Found( );
	void Should_Return_False_From_Exists_Given_NON_Existing_File( );
	void Should_Return_True_From_Exists_Given_File_Exists( );
	void Should_Return_Empty_StringVectorPtr_On_List( );
	void Should_Return_Empty_FileInfoPtr_On_ListFileInfo( );
	void Should_Return_Empty_StringVectorPtr_On_Find( );
	void Should_Return_Empty_FileInfoPtr_On_FindFileInfo( );

};

#endif
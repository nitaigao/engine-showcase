#ifndef FILEMANAGER_FIXTURE_H
#define FILEMANAGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class FileManagerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( FileManagerFixture );
  CPPUNIT_TEST( Should_Initialize_Given_Not_Intialized );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
  CPPUNIT_TEST( Should_Release_Given_Initialized );
  CPPUNIT_TEST( Should_Throw_On_Release_Given_ALready_Released );
  CPPUNIT_TEST( Should_GetIntance_Given_Initialized );
  CPPUNIT_TEST( Should_Throw_On_GetInstance_Given_Not_Initialized );
  CPPUNIT_TEST( Should_Initialize_And_Release_Correctly );
  CPPUNIT_TEST( Should_Throw_Adding_NonExistant_FileStore );
  CPPUNIT_TEST( Should_Add_FileStore_Given_Existing_FileStore );
  CPPUNIT_TEST( Should_Throw_On_GetFile_Given_NonExisting_File );
  CPPUNIT_TEST( Should_Return_FileBuffer_On_GetFile_Given_Existing_File );
  CPPUNIT_TEST( Should_Return_True_Given_File_Exists );
  CPPUNIT_TEST( Should_Return_False_Given_File_Doesnt_Exist );
  CPPUNIT_TEST( Should_Return_No_Results_On_Recursive_File_Search_Given_Invalid_File_Pattern );
  CPPUNIT_TEST( Should_Return_Results_On_Recursive_File_Search_Given_Valid_File_Pattern );
  CPPUNIT_TEST( Should_Return_No_Results_On_Non_Recursive_File_Search_Given_Invalid_File_Pattern );
  CPPUNIT_TEST( Should_Return_Results_On_Non_Recursive_File_Search_Given_Valid_File_Pattern );
  CPPUNIT_TEST( Should_Throw_On_Save_Given_File_Not_Writable );
  CPPUNIT_TEST( Should_Throw_On_Save_Given_Path_Non_Existant );
  CPPUNIT_TEST( Should_Save_File_Successfully );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Given_Not_Intialized( );
	void Should_Throw_On_Initialize_Given_Already_Intialized( );
	void Should_Release_Given_Initialized( );
	void Should_Throw_On_Release_Given_ALready_Released( );
	void Should_GetIntance_Given_Initialized( );
	void Should_Throw_On_GetInstance_Given_Not_Initialized( );
	void Should_Initialize_And_Release_Correctly( );
	void Should_Throw_Adding_NonExistant_FileStore( );
	void Should_Add_FileStore_Given_Existing_FileStore( );
	void Should_Throw_On_GetFile_Given_NonExisting_File( );
	void Should_Return_FileBuffer_On_GetFile_Given_Existing_File( );
	void Should_Return_True_Given_File_Exists( );
	void Should_Return_False_Given_File_Doesnt_Exist( );
	void Should_Return_No_Results_On_Recursive_File_Search_Given_Invalid_File_Pattern( );
	void Should_Return_Results_On_Recursive_File_Search_Given_Valid_File_Pattern( );
	void Should_Return_No_Results_On_Non_Recursive_File_Search_Given_Invalid_File_Pattern( );
	void Should_Return_Results_On_Non_Recursive_File_Search_Given_Valid_File_Pattern( );
	void Should_Throw_On_Save_Given_File_Not_Writable( );
	void Should_Throw_On_Save_Given_Path_Non_Existant( );
	void Should_Save_File_Successfully( );
};

#endif
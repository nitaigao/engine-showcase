#ifndef FILEMANAGER_FIXTURE_H
#define FILEMANAGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class FileManagerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( FileManagerFixture );
  CPPUNIT_TEST( Should_Initialize_And_Release_Correctly );
  CPPUNIT_TEST( Should_Return_False_Adding_NonExistant_FileStore );
  CPPUNIT_TEST( Should_Return_True_Adding_Existing_FileStore );
  CPPUNIT_TEST( Should_Return_NULL_FileBuffer_If_On_NonExisting_GetFile );
  CPPUNIT_TEST( Should_Return_FileBuffer_If_Existing_File_Found );
  CPPUNIT_TEST( Should_Return_True_If_File_Exists );
  CPPUNIT_TEST( Should_Return_False_If_File_Doesnt_Exist );
  CPPUNIT_TEST_EXCEPTION( SHould_Throw_Given_UnInitialized_GetInstance, UnInitializedException );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_And_Release_Correctly( );
	void Should_Return_False_Adding_NonExistant_FileStore( );
	void Should_Return_True_Adding_Existing_FileStore( );
	void Should_Return_NULL_FileBuffer_If_On_NonExisting_GetFile( );
	void Should_Return_FileBuffer_If_Existing_File_Found( );
	void Should_Return_True_If_File_Exists( );
	void Should_Return_False_If_File_Doesnt_Exist( );
	void SHould_Throw_Given_UnInitialized_GetInstance( );
	void Should_Throw_On_Initialize_Given_Already_Intialized( );

};

#endif
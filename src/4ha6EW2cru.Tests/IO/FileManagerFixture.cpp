#include "FileManagerFixture.h"

#include "Management/Management.h"
#include "IO/FileSystem.h"
using namespace IO;

#include "Logging/Logger.h"
using namespace Logging;

#include "Exceptions/NullReferenceException.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/FileWriteException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileManagerFixture, Suites::IOSuite( ) );

void FileManagerFixture::setUp( )
{
	Logger::Get( )->SetLogLevel( LEVEL_DEBUG );
}

void FileManagerFixture::tearDown( )
{

}

void FileManagerFixture::Should_Throw_Adding_NonExistant_FileStore( )
{
	FileSystem fileManager;
	CPPUNIT_ASSERT_THROW( fileManager.Mount( "../game/bogus.bad", "data/" ), FileNotFoundException );
}

void FileManagerFixture::Should_Add_FileStore_Given_Existing_FileStore( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );
}

void FileManagerFixture::Should_Throw_On_GetFile_Given_NonExisting_File( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );

	CPPUNIT_ASSERT_THROW( fileManager.GetFile( "bogus.txt" ), FileNotFoundException );
}

void FileManagerFixture::Should_Return_FileBuffer_On_GetFile_Given_Existing_File( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );

	FileBuffer* fileBuffer = fileManager.GetFile( "data/test/test.txt" );
	CPPUNIT_ASSERT( fileBuffer != 0 );
	delete fileBuffer;
}

void FileManagerFixture::Should_Return_True_Given_File_Exists( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test", "/" );
	bool result = fileManager.FileExists( "/test.txt" );

	CPPUNIT_ASSERT( result );
}

void FileManagerFixture::Should_Return_False_Given_File_Doesnt_Exist( )
{
	FileSystem fileManager;
	bool result = fileManager.FileExists( "RandomFile" );

	CPPUNIT_ASSERT( !result );
}

void FileManagerFixture::Should_Return_No_Results_On_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResult::FileSearchResultList* results = fileManager.FileSearch( "/", "blah", true );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_Results_On_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResult::FileSearchResultList* results = fileManager.FileSearch( "/", "*test*", true );

	CPPUNIT_ASSERT( !results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_No_Results_On_Non_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResult::FileSearchResultList* results = fileManager.FileSearch( "/", "blah", false );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_Results_On_Non_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/test.bad", "/" );

	FileSearchResult::FileSearchResultList* results = fileManager.FileSearch( "/", "*test*", false );

	CPPUNIT_ASSERT( !results->empty( ) && results->size( ) == 1 );

	delete results;
}

void FileManagerFixture::Should_Throw_On_Save_Given_Path_Non_Existant_Or_Not_Writable( )
{
	FileSystem fileManager;

	int contentLength = 3;
	char* contentToWrite = new char[ contentLength ];

	FileBuffer fileBuffer( contentToWrite, contentLength, "/tdsada/ds/asd/asd/asdasd" );

	CPPUNIT_ASSERT_THROW( fileManager.SaveFile( fileBuffer ), FileWriteException );
}

void FileManagerFixture::Should_Save_File_Successfully( )
{
	FileSystem fileManager;
	fileManager.Mount( "../../../etc/data/test/", "/" );

	int contentLength = 3;
	char* contentToWrite = new char[ contentLength ];

	FileBuffer fileBuffer( contentToWrite, contentLength, "canwrite.txt" );

	fileManager.SaveFile( fileBuffer );
}
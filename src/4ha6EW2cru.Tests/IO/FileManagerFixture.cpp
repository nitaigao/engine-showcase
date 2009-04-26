#include "FileManagerFixture.h"

#include "Management/Management.h"
#include "IO/FileManager.h"
#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "Exceptions/NullReferenceException.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/FileWriteException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileManagerFixture, Suites::IOSuite( ) );

void FileManagerFixture::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->SetLogLevel( DEBUGA );
	Management::Initialize( );
}

void FileManagerFixture::tearDown( )
{
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_Adding_NonExistant_FileStore( )
{
	FileManager fileManager;
	CPPUNIT_ASSERT_THROW( fileManager.MountFileStore( "../game/bogus.bad", "data/" ), FileNotFoundException );
}

void FileManagerFixture::Should_Add_FileStore_Given_Existing_FileStore( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );
}

void FileManagerFixture::Should_Throw_On_GetFile_Given_NonExisting_File( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );

	CPPUNIT_ASSERT_THROW( fileManager.GetFile( "bogus.txt" ), FileNotFoundException );
}

void FileManagerFixture::Should_Return_FileBuffer_On_GetFile_Given_Existing_File( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );

	FileBuffer* fileBuffer = fileManager.GetFile( "data/test/test.txt" );
	CPPUNIT_ASSERT( fileBuffer != 0 );
	delete fileBuffer;
}

void FileManagerFixture::Should_Return_True_Given_File_Exists( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test", "/" );
	bool result = fileManager.FileExists( "/test.txt" );

	CPPUNIT_ASSERT( result );
}

void FileManagerFixture::Should_Return_False_Given_File_Doesnt_Exist( )
{
	FileManager fileManager;
	bool result = fileManager.FileExists( "RandomFile" );

	CPPUNIT_ASSERT( !result );
}

void FileManagerFixture::Should_Return_No_Results_On_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResultList* results = fileManager.FileSearch( "/", "blah", true );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_Results_On_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResultList* results = fileManager.FileSearch( "/", "*test*", true );

	CPPUNIT_ASSERT( !results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_No_Results_On_Non_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "data/" );

	FileSearchResultList* results = fileManager.FileSearch( "/", "blah", false );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;
}

void FileManagerFixture::Should_Return_Results_On_Non_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/test.bad", "/" );

	FileSearchResultList* results = fileManager.FileSearch( "/", "*test*", false );

	CPPUNIT_ASSERT( !results->empty( ) && results->size( ) == 1 );

	delete results;
}

void FileManagerFixture::Should_Throw_On_Save_Given_Path_Non_Existant_Or_Not_Writable( )
{
	FileManager fileManager;

	int contentLength = 3;
	char* contentToWrite = new char[ contentLength ];

	FileBuffer fileBuffer( contentToWrite, contentLength, "/tdsada/ds/asd/asd/asdasd" );

	CPPUNIT_ASSERT_THROW( fileManager.SaveFile( fileBuffer ), FileWriteException );
}

void FileManagerFixture::Should_Save_File_Successfully( )
{
	FileManager fileManager;
	fileManager.MountFileStore( "../../../etc/data/test/", "/" );

	int contentLength = 3;
	char* contentToWrite = new char[ contentLength ];

	FileBuffer fileBuffer( contentToWrite, contentLength, "canwrite.txt" );

	fileManager.SaveFile( fileBuffer );
}
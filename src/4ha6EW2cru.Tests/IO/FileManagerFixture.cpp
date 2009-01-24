#include "FileManagerFixture.h"

#include "IO/FileManager.h"
#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "Exceptions/NullReferenceException.hpp"
#include "Exceptions/FileNotFoundException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileManagerFixture, Suites::IOSuite( ) );

void FileManagerFixture::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->SetLogLevel( DEBUGA );
	Logger::GetInstance( )->AddAppender( new ConsoleAppender( ) );
}

void FileManagerFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Initialize_Given_Not_Intialized( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	FileManager::Initialize( );

	CPPUNIT_ASSERT_THROW( 
		FileManager::Initialize( ),
		AlreadyInitializedException
		);

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Release_Given_Initialized( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_On_Release_Given_ALready_Released( )
{
	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( )->Release( ), UnInitializedException );
}

void FileManagerFixture::Should_GetIntance_Given_Initialized( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_On_GetInstance_Given_Not_Initialized( )
{
	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( ), UnInitializedException );
}

void FileManagerFixture::Should_Initialize_And_Release_Correctly( )
{
	bool result = FileManager::Initialize( );
	CPPUNIT_ASSERT( result );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_Adding_NonExistant_FileStore( )
{
	FileManager::Initialize( );
	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( )->MountFileStore( "../game/bogus.bad", "data/" ), FileNotFoundException );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Add_FileStore_Given_Existing_FileStore( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_On_GetFile_Given_NonExisting_File( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( )->GetFile( "bogus.txt" ), FileNotFoundException );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_FileBuffer_On_GetFile_Given_Existing_File( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "data/gui/test.txt" );
	CPPUNIT_ASSERT( fileBuffer != 0 );
	delete fileBuffer;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_True_Given_File_Exists( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/", "data/" );
	bool result = FileManager::GetInstance( )->FileExists( "data/gui.bad" );

	CPPUNIT_ASSERT( result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_False_Given_File_Doesnt_Exist( )
{
	FileManager::Initialize( );
	bool result = FileManager::GetInstance( )->FileExists( "RandomFile" );

	CPPUNIT_ASSERT( !result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_No_Results_On_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	FileSearchResultList* results = FileManager::GetInstance( )->FileSearch( "/", "blah", true );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_Results_On_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	FileSearchResultList* results = FileManager::GetInstance( )->FileSearch( "/", "*test*", true );

	CPPUNIT_ASSERT( !results->empty( ) );

	delete results;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_No_Results_On_Non_Recursive_File_Search_Given_Invalid_File_Pattern( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	FileSearchResultList* results = FileManager::GetInstance( )->FileSearch( "/", "blah", false );

	CPPUNIT_ASSERT( results->empty( ) );

	delete results;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_Results_On_Non_Recursive_File_Search_Given_Valid_File_Pattern( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/data/gui.bad", "data/" );

	FileSearchResultList* results = FileManager::GetInstance( )->FileSearch( "/", "*test*", false );

	CPPUNIT_ASSERT( !results->empty( ) && results->size( ) == 1 );

	delete results;

	FileManager::GetInstance( )->Release( );
}
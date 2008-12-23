#include "FileManagerFixture.h"

#include "IO/FileManager.h"
#include "Logging/Logger.h"

#include "Exceptions/NullReferenceException.hpp"
#include "Exceptions/FileNotFoundException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileManagerFixture, Suites::IOSuite( ) );

void FileManagerFixture::setUp( )
{
	Logger::Initialize( );
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
	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( )->AddFileStore( "../game/bogus.bad" ), FileNotFoundException );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Add_FileStore_Given_Existing_FileStore( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/data/gui.bad" );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Throw_On_GetFile_Given_NonExisting_File( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/data/gui.bad" );

	CPPUNIT_ASSERT_THROW( FileManager::GetInstance( )->GetFile( "bogus.txt" ), FileNotFoundException );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_FileBuffer_On_GetFile_Given_Existing_File( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/data/gui.bad" );

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "test.txt" );
	CPPUNIT_ASSERT( fileBuffer != 0 );
	delete fileBuffer;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_True_Given_File_Exists( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/data/" );
	bool result = FileManager::GetInstance( )->FileExists( "gui.bad" );

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
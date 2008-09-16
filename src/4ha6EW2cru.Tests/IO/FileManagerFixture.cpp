#include "FileManagerFixture.h"

#include "IO/FileManager.h"
#include "Logging/Logger.h"

CPPUNIT_TEST_SUITE_REGISTRATION( FileManagerFixture );

void FileManagerFixture::setUp( )
{
	Logger::Initialize( );
}

void FileManagerFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Initialize_And_Release_Correctly( )
{
	bool result = FileManager::Initialize( );
	CPPUNIT_ASSERT( result );
	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_False_Adding_NonExistant_FileStore( )
{
	FileManager::Initialize( );
	bool result = FileManager::GetInstance( )->AddFileStore( "../game/bogus.bad" );

	CPPUNIT_ASSERT( !result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_True_Adding_Existing_FileStore( )
{
	FileManager::Initialize( );
	bool result = FileManager::GetInstance( )->AddFileStore( "../game/test/Test.bad" );

	CPPUNIT_ASSERT( result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_NULL_FileBuffer_If_On_NonExisting_GetFile( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test/Test.bad" );

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "bogus.txt" );
	CPPUNIT_ASSERT( fileBuffer == 0 );
	delete fileBuffer;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_FileBuffer_If_Existing_File_Found( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test/Test.bad" );

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( "Test.txt" );
	CPPUNIT_ASSERT( fileBuffer != 0 );
	delete fileBuffer;

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_True_If_File_Exists( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test/" );
	bool result = FileManager::GetInstance( )->FileExists( "Test.bad" );

	CPPUNIT_ASSERT( result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::Should_Return_False_If_File_Doesnt_Exist( )
{
	FileManager::Initialize( );
	bool result = FileManager::GetInstance( )->FileExists( "RandomFile" );

	CPPUNIT_ASSERT( !result );

	FileManager::GetInstance( )->Release( );
}

void FileManagerFixture::SHould_Throw_Given_UnInitialized_GetInstance( )
{
	FileManager::GetInstance( );
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
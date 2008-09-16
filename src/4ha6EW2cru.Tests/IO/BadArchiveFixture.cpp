#include "BadArchiveFixture.h"

#include "IO/FileManager.h"
#include "Logging/Logger.h"
#include "IO/BadArchive.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BadArchiveFixture );

void BadArchiveFixture::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );
}

void BadArchiveFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void BadArchiveFixture::Should_Return_False_From_Exists_Given_NON_Existing_File( )
{
	BadArchive* archive = new BadArchive( "../game/test/Test.bad", "BAD" );
	bool result = archive->exists( "blah" );

	CPPUNIT_ASSERT( !result );

	delete archive;
}

void BadArchiveFixture::Should_Return_True_From_Exists_Given_File_Exists( )
{
	BadArchive* archive = new BadArchive( "../game/test/Test.bad", "BAD" );
	bool result = archive->exists( "Test.txt" );

	CPPUNIT_ASSERT( result );

	delete archive;
}

void BadArchiveFixture::Sould_Return_NULL_Given_File_Not_Found( )
{
	BadArchive* archive = new BadArchive( "../game/test/Test.bad", "BAD" );
	Ogre::DataStreamPtr stream = archive->open( "blah" );

	CPPUNIT_ASSERT( stream.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Should_Return_Not_NULL_Given_File_Found( )
{
	BadArchive* archive = new BadArchive( "../game/test/Test.bad", "BAD" );
	Ogre::DataStreamPtr stream = archive->open( "Test.txt" );

	CPPUNIT_ASSERT( !stream.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Should_Find_Some_Files( )
{
	BadArchive* archive = new BadArchive( "../game/test/Test.bad", "BAD" );
	Ogre::StringVectorPtr files = archive->find( "*" );

	CPPUNIT_ASSERT( !files.isNull( ) );

	delete archive;
}
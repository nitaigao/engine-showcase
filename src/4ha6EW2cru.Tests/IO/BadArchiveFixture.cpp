#include "BadArchiveFixture.h"

#include "IO/FileSystem.h"

#include "Logging/Logger.h"
using namespace Logging;

#include "IO/BadArchive.h"
using namespace IO;

#include "Management/Management.h"

#include "Exceptions/FileNotFoundException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( BadArchiveFixture, Suites::IOSuite( ) );

void BadArchiveFixture::setUp( )
{
	Logging::Logger::Get( )->SetLogLevel( Logging::LEVEL_ALL );
	Management::Initialize( );
}

void BadArchiveFixture::tearDown( )
{
	Management::Release( );
}

void BadArchiveFixture::Should_Return_True_On_Case_Sensitive( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	
	CPPUNIT_ASSERT( archive->isCaseSensitive( )	);
	
	delete archive;
}

void BadArchiveFixture::Should_Return_DataStreamPtr_Given_File_Found( )
{
	BadArchive* archive = new BadArchive( "/", "BAD" );
	Ogre::DataStreamPtr stream = archive->open( "/data/test/test.txt" );

	CPPUNIT_ASSERT( !stream.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Sould_Return_Throw_Given_File_Not_Found( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );

	CPPUNIT_ASSERT_THROW( archive->open( "blah" ), FileNotFoundException );

	delete archive;
}

void BadArchiveFixture::Should_Return_False_From_Exists_Given_NON_Existing_File( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	bool result = archive->exists( "blah" );

	CPPUNIT_ASSERT( !result );

	delete archive;
}

void BadArchiveFixture::Should_Return_True_From_Exists_Given_File_Exists( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	bool result = archive->exists( "/data/test/test.txt" );

	CPPUNIT_ASSERT( result );

	delete archive;
}

void BadArchiveFixture::Should_Return_Empty_StringVectorPtr_On_List( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	Ogre::StringVectorPtr files = archive->list( "*" );

	CPPUNIT_ASSERT( !files.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Should_Return_Empty_FileInfoPtr_On_ListFileInfo( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	Ogre::FileInfoListPtr info = archive->listFileInfo( false, false );

	CPPUNIT_ASSERT( !info.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Should_Return_Empty_StringVectorPtr_On_Find( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	Ogre::StringVectorPtr files = archive->find( "*" );

	CPPUNIT_ASSERT( !files.isNull( ) );

	delete archive;
}

void BadArchiveFixture::Should_Return_Empty_FileInfoPtr_On_FindFileInfo( )
{
	BadArchive* archive = new BadArchive( "../../../etc/data/test/test.bad", "BAD" );
	Ogre::FileInfoListPtr info = archive->findFileInfo( "*", false, false );

	CPPUNIT_ASSERT( !info.isNull( ) );

	delete archive;
}
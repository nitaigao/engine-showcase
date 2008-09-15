#include "BadArchiveFactoryFixture.h"

#include "IO/BadArchiveFactory.h"
#include "IO/BadArchive.h"
#include "Exceptions/ArchiveNotFoundException.hpp"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BadArchiveFactoryFixture );

void BadArchiveFactoryFixture::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );
}

void BadArchiveFactoryFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void BadArchiveFactoryFixture::Should_Create_BadArchive( )
{
	BadArchiveFactory* bfactory = new BadArchiveFactory( );
	Archive* archive = bfactory->createInstance( "../game/test/Test.bad" );

	CPPUNIT_ASSERT( archive != 0 );

	bfactory->destroyInstance( archive );
	delete bfactory;
}

void BadArchiveFactoryFixture::Should_Throw_On_Non_Existing_BAD_File( )
{
	BadArchiveFactory* bfactory = new BadArchiveFactory( );
	
	CPPUNIT_ASSERT_THROW(
		bfactory->createInstance( "blah" ),
		ArchiveNotFoundException
		);

	delete bfactory;
}
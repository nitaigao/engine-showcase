#include "BadArchiveFactoryFixture.h"

#include "IO/BadArchive.h"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/NullReferenceException.hpp"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BadArchiveFactoryFixture );

void BadArchiveFactoryFixture::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );

	_bfactory = new BadArchiveFactory( );
}

void BadArchiveFactoryFixture::tearDown( )
{
	delete _bfactory;

	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void BadArchiveFactoryFixture::Should_Create_BadArchive( )
{
	Archive* archive = _bfactory->createInstance( "../game/test/Test.bad" );

	CPPUNIT_ASSERT( archive != 0 );

	_bfactory->destroyInstance( archive );
}

void BadArchiveFactoryFixture::Should_Throw_On_Non_Existing_BAD_File( )
{
	CPPUNIT_ASSERT_THROW(
		_bfactory->createInstance( "blah" ),
		FileNotFoundException
		);
}

void BadArchiveFactoryFixture::Should_Throw_On_Destroy_Given_NULL_Archive( )
{
	CPPUNIT_ASSERT_THROW( _bfactory->destroyInstance( 0 ), NullReferenceException );;
}

void BadArchiveFactoryFixture::Should_Destroy_Valid_Archive( )
{
	Archive* archive = _bfactory->createInstance( "../game/test/Test.bad" );
	_bfactory->destroyInstance( archive );
}
#include "BadArchiveFactoryFixture.h"

#include "IO/BadArchive.h"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/NullReferenceException.hpp"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( BadArchiveFactoryFixture, Suites::IOSuite( ) );

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
	Archive* archive = _bfactory->createInstance( "../game/data/test.bad" );

	CPPUNIT_ASSERT( archive != 0 );

	_bfactory->destroyInstance( archive );
}

void BadArchiveFactoryFixture::Should_Throw_On_Destroy_Given_NULL_Archive( )
{
	CPPUNIT_ASSERT_THROW( _bfactory->destroyInstance( 0 ), NullReferenceException );;
}

void BadArchiveFactoryFixture::Should_Destroy_Valid_Archive( )
{
	Archive* archive = _bfactory->createInstance( "../game/data/test.bad" );
	_bfactory->destroyInstance( archive );
}
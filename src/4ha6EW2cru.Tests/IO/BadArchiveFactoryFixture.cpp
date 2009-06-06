#include "BadArchiveFactoryFixture.h"

#include "IO/BadArchive.h"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/NullReferenceException.hpp"

#include "Logging/Logger.h"
using namespace Logging;

#include "IO/FileSystem.h"
using namespace IO;

#include "Management/Management.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( BadArchiveFactoryFixture, Suites::IOSuite( ) );

void BadArchiveFactoryFixture::setUp( )
{
	Management::Initialize( );

	_bfactory = new BadArchiveFactory( );
}

void BadArchiveFactoryFixture::tearDown( )
{
	delete _bfactory;

	Management::Release( );
}

void BadArchiveFactoryFixture::Should_Create_BadArchive( )
{
	Ogre::Archive* archive = _bfactory->createInstance( "../../../etc/data/test/test.bad" );

	CPPUNIT_ASSERT( archive != 0 );

	_bfactory->destroyInstance( archive );
}

void BadArchiveFactoryFixture::Should_Throw_On_Destroy_Given_NULL_Archive( )
{
	CPPUNIT_ASSERT_THROW( _bfactory->destroyInstance( 0 ), NullReferenceException );;
}

void BadArchiveFactoryFixture::Should_Destroy_Valid_Archive( )
{
	Ogre::Archive* archive = _bfactory->createInstance( "../../../etc/data/test/test.bad" );
	_bfactory->destroyInstance( archive );
}
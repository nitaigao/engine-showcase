#ifndef __BADARCHIVEFACTORY_FIXTURE_H
#define __BADARCHIVEFACTORY_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "IO/BadArchiveFactory.h"

class BadArchiveFactoryFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( BadArchiveFactoryFixture );
	CPPUNIT_TEST( Should_Create_BadArchive );
	CPPUNIT_TEST( Should_Throw_On_Non_Existing_BAD_File );
	CPPUNIT_TEST( Should_Throw_On_Destroy_Given_NULL_Archive );
	CPPUNIT_TEST( Should_Destroy_Valid_Archive );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Create_BadArchive( );
	void Should_Throw_On_Non_Existing_BAD_File( );
	void Should_Throw_On_Destroy_Given_NULL_Archive( );
	void Should_Destroy_Valid_Archive( );

private:

	BadArchiveFactory* _bfactory;

};

#endif
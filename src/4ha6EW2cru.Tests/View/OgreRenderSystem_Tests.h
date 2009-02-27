#ifndef __INTERACTIVEVIEWSYSTEM_TESTS_H
#define __INTERACTIVEVIEWSYSTEM_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

#include "System/IConfiguration.hpp"
#include "IO/IFileManager.hpp"

class OgreRenderSystem_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( OgreRenderSystem_Tests );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST( Should_Throw_Given_NULL_Configuration );
	CPPUNIT_TEST( Should_Throw_On_Update_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Update_Given_Initialized );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Throw_Given_NULL_Configuration( );
	void Should_Throw_On_Update_Given_Not_Intialized( );
	void Should_Update_Given_Initialized( );

private:

	IConfiguration* _configuration;
	IFileManager* _fileManager;

};

#endif
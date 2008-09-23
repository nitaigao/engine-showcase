#ifndef __HUMANVIEW_FIXTURE_H
#define __HUMANVIEW_FIXTURE_H


#include <cppunit/extensions/HelperMacros.h>

class HumanViewFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( HumanViewFixture );
	CPPUNIT_TEST( Should_Initialize_Given_Valid_Dimensions );
	CPPUNIT_TEST( Should_Initialize_FullScreen );
	CPPUNIT_TEST( Should_Initialize_Windowed );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Invalid_Dimensions );
	CPPUNIT_TEST( Should_Throw_On_Update_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Update_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Render_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Render_Given_Initialized );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Given_Valid_Dimensions( );
	void Should_Initialize_FullScreen( );
	void Should_Initialize_Windowed( );
	void Should_Throw_On_Initialize_Given_Invalid_Dimensions( );
	void Should_Throw_On_Update_Given_Not_Intialized( );
	void Should_Update_Given_Initialized( );
	void Should_Throw_On_Render_Given_Not_Intialized( );
	void Should_Render_Given_Initialized( );

};

#endif
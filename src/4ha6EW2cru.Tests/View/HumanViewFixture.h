#ifndef __HUMANVIEW_FIXTURE_H
#define __HUMANVIEW_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "System/Configuration.h"

class HumanViewFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( HumanViewFixture );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST( Should_Throw_Given_NULL_Configuration );
	CPPUNIT_TEST( Should_Throw_On_Update_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Update_Given_Initialized );
	CPPUNIT_TEST( Should_Throw_On_Render_Given_Not_Intialized );
	CPPUNIT_TEST( Should_Render_Given_Initialized );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Throw_Given_NULL_Configuration( );
	void Should_Throw_On_Update_Given_Not_Intialized( );
	void Should_Update_Given_Initialized( );
	void Should_Throw_On_Render_Given_Not_Intialized( );
	void Should_Render_Given_Initialized( );

private:

	Configuration* _configuration;

};

#endif
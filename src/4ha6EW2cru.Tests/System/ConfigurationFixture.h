#ifndef __CONFIGURATION_FIXTURE_H
#define __CONFIGURATION_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigurationFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ConfigurationFixture );
	CPPUNIT_TEST( Should_Return_IsFullScreen_True_Given_FullScreen_Set_To_True );
	CPPUNIT_TEST( Should_Return_IsFullScreen_False_Given_FullScreen_Set_To_False );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

	void Should_Return_IsFullScreen_True_Given_FullScreen_Set_To_True( );
	void Should_Return_IsFullScreen_False_Given_FullScreen_Set_To_False( );

};

#endif
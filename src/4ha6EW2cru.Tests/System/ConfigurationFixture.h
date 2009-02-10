#ifndef __CONFIGURATION_FIXTURE_H
#define __CONFIGURATION_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigurationFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ConfigurationFixture );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

};

#endif
#ifndef __SETTINGSFIXTURE_H
#define __SETTINGSFIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class SettingsFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SettingsFixture );

	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Fail_To_Initialize_Given_Wrong_Settings_File( );
	void Should_Initialize_Given_Correct_Settings_File( );

};

#endif
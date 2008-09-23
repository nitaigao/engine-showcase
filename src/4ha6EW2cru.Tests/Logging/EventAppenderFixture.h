#ifndef __EVENTAPPENDER_FIXTURE_H
#define __EVENTAPPENDER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class EventAppenderFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( EventAppenderFixture );
	CPPUNIT_TEST( Should_Throw_Given_EventManager_Not_Initialized );
	CPPUNIT_TEST( Should_Append_Message );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Throw_Given_EventManager_Not_Initialized( );
	void Should_Append_Message( );

};

#endif
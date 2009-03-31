#ifndef MOUSELISTENER_FIXTURE_H
#define MOUSELISTENER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "../Mocks/MockEventTrigger.hpp"
#include "Events/EventManager.h"

class MouseListenerFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( MouseListenerFixture );
	CPPUNIT_TEST( Should_Fire_Left_MouseReleased_Event );
	CPPUNIT_TEST( Should_Fire_Left_MousePressed_Event );
	CPPUNIT_TEST( Should_Fire_Right_MouseReleased_Event );
	CPPUNIT_TEST( Should_Fire_Right_MousePressed_Event );
	CPPUNIT_TEST( Should_Fire_MouseMoved_Event );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Fire_Left_MouseReleased_Event( );
	void Should_Fire_Left_MousePressed_Event( );
	void Should_Fire_Right_MouseReleased_Event( );
	void Should_Fire_Right_MousePressed_Event( );
	void Should_Fire_MouseMoved_Event( );

private:

	MockEventTrigger* _eventTriggerMock;
	EventManager* _eventManager;

};

#endif
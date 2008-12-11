#include "EventDataFixture.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventDataFixture, Suites::EventsSuite( ) );

void EventDataFixture::Should_Return_Correct_Keycode( )
{
	OIS::KeyCode keyCode = OIS::KC_0;
	
	KeyEventData keyEventData( keyCode, "0" );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( keyCode, keyEventData.GetKeyCode( ), 0 );
}
#include "EventDataFixture.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventDataFixture, Suites::EventsSuite( ) );

using namespace Events;

void EventDataFixture::Should_Return_Correct_Keycode( )
{
	OIS::KeyCode keyCode = OIS::KC_0;
	
	KeyEventData keyEventData( keyCode, "0" );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( keyCode, keyEventData.GetKeyCode( ), 0 );
}

void EventDataFixture::LevelChangedEventData_Should_Return_LevelName( )
{
	std::string levelName = "blah";
	LevelChangedEventData eventData( levelName );
	CPPUNIT_ASSERT( eventData.GetLevelName( ) == levelName );  
}
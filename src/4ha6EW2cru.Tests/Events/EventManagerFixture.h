#ifndef EVENTMANAGER_FIXTURE_H
#define EVENTMANAGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class EventManagerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventManagerFixture );
  CPPUNIT_TEST( Should_Queue_Event_Successfully_Given_Intiailized );
  CPPUNIT_TEST( Should_Throw_Given_NULL_Event_To_QueueEvent );
  CPPUNIT_TEST( Should_Update_Given_Intialized );
  CPPUNIT_TEST( Should_Throw_On_AddEventListener_Given_NULL_Target );
  CPPUNIT_TEST( Should_Throw_On_AddEventListener_Given_NULL_HandlerFunctor );
  CPPUNIT_TEST( Should_Add_Listener_Given_Correct_Listener_And_Initialized );
  CPPUNIT_TEST( Should_Throw_On_RemoveEventListener_Given_NULL_Target );
  CPPUNIT_TEST( Should_Throw_On_RemoveEventListener_Given_NULL_HandlerFunctor );  
  CPPUNIT_TEST( Should_Remove_Listener_Given_Correct_Listener_And_Initialized );
  CPPUNIT_TEST( Should_Still_Remove_Listener_Given_Non_Existant_Listener_And_Initialized );
  CPPUNIT_TEST( Should_Queue_Event_Given_Correct_Event );
  CPPUNIT_TEST( Should_Throw_On_Queue_Event_Given_NULL_Event );
  CPPUNIT_TEST( Should_Trigger_Event_Given_Initialized );
  CPPUNIT_TEST( Should_Throw_On_TriggerEvent_Given_Event_Is_NULL );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

	void Should_Queue_Event_Successfully_Given_Intiailized( );
	void Should_Throw_Given_NULL_Event_To_QueueEvent( );
	void Should_Update_Given_Intialized( );
	void Should_Throw_On_AddEventListener_Given_NULL_Target( );
	void Should_Throw_On_AddEventListener_Given_NULL_HandlerFunctor( );
	void Should_Add_Listener_Given_Correct_Listener_And_Initialized( );
	void Should_Throw_On_RemoveEventListener_Given_NULL_Target( );
	void Should_Throw_On_RemoveEventListener_Given_NULL_HandlerFunctor( );  
	void Should_Remove_Listener_Given_Correct_Listener_And_Initialized( );
	void Should_Still_Remove_Listener_Given_Non_Existant_Listener_And_Initialized( );
	void Should_Queue_Event_Given_Correct_Event( );
	void Should_Throw_On_Queue_Event_Given_NULL_Event( );
	void Should_Trigger_Event_Given_Initialized( );
	void Should_Throw_On_TriggerEvent_Given_Event_Is_NULL( );

};

#endif
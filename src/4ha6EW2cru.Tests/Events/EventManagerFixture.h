#ifndef __EVENTMANAGER_FIXTURE_H
#define __EVENTMANAGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class EventManagerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventManagerFixture );
  CPPUNIT_TEST( EventManager_Should_Initialize_And_Release_Correctly );
  CPPUNIT_TEST( EventManager_Should_Handle_Event );
  CPPUNIT_TEST( EventManager_Should_Perform_Update );
  CPPUNIT_TEST( Should_Add_And_Remove_Listener_Successfully );
  CPPUNIT_TEST_EXCEPTION( Should_Throw_On_UnInitialized_GetInstance, UnInitializedException );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

  void EventManager_Should_Initialize_And_Release_Correctly( );
  void EventManager_Should_Handle_Event( );
  void EventManager_Should_Perform_Update( );
  void Should_Add_And_Remove_Listener_Successfully( );
  void Should_Throw_On_UnInitialized_GetInstance( );
  void Should_Throw_On_Initialize_Given_Already_Intialized( );

};

#endif
#ifndef __MOCKEVENTSUBJECT_H
#define __MOCKEVENTSUBJECT_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "Events/IEvent.hpp"

class EventSubjectMock
{

public:

	EventSubjectMock( )
		: eventHandler_counter( 0 )
	{ };

	void EventHandler( const IEvent* const event )
	{
		eventHandler_counter++;
	};

	void FalseEventHandler( const IEvent* const event ) { };

	int eventHandler_counter;

};

#endif
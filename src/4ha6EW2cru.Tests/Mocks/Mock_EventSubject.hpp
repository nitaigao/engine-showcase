#ifndef MOCK_EVENTSUBJECT_H
#define MOCK_EVENTSUBJECT_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "Events/IEvent.hpp"

class Mock_EventSubject
{

public:

	Mock_EventSubject( )
		: eventHandler_counter( 0 )
	{ };

	void EventHandler( const Events::IEvent* const event )
	{
		eventHandler_counter++;
	};

	void FalseEventHandler( const Events::IEvent* const event ) { };

	int eventHandler_counter;

};

#endif
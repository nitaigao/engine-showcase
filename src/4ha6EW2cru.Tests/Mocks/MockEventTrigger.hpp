#ifndef __MOCKEVENTTRIGGER_H
#define __MOCKEVENTTRIGGER_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "Events/IEvent.hpp"

class MockEventTrigger : public MockObject
{

public:

	MockEventTrigger( )
		: MockObject( "MockEventTrigger", 0 )
		, handle_count( "MockEventTrigger/EventHandler", this )
	{

	}

	ExpectationCounter handle_count;

	inline void EventHandler( const Events::IEvent* event ) { handle_count.inc( ); };
};

#endif
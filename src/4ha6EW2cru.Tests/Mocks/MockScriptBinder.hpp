#ifndef MOCKSCRIPTBINDER_H
#define MOCKSCRIPTBINDER_H

#include "Events/IEventListener.hpp"

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

class MockScriptBinder
{

public:

	static int handle_count;

	static void HandleEvent( )
	{
		MockScriptBinder::handle_count++;
	}

	static void QueueEvent( )
	{
		EventManager::GetInstance( )->QueueEvent( new Event( TEST_EVENT ) );
	}

};

int MockScriptBinder::handle_count = 0;

#endif
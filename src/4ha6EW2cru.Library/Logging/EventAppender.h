#ifndef __EVENTAPPENDER_H
#define __EVENTAPPENDER_H

#include "IAppender.hpp"
#include "../Events/EventManager.h"

class EventAppender : public IAppender
{

public:

	EventAppender( EventManager* eventManager )
		: _eventManager( eventManager )
	{

	}

	void Append( const std::string message ) const;

private:

	EventManager* _eventManager;

};

#endif
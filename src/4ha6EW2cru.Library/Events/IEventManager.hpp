#ifndef __IEVENTMANAGER_H
#define __IEVENTMANAGER_H

class IEventManager
{

public:

	virtual void Update( float deltaMilliseconds ) = 0;

};

#endif
#ifndef __IOBSERVER_H
#define __IOBSERVER_H

#include "ISubject.hpp"

class IObserver
{

public:

	virtual unsigned int GetRequestedChanges( ) = 0;
	virtual void Observe( ISubject* subject, unsigned int systemChanges ) = 0;

};

#endif
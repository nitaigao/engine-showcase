#ifndef __IOBSERVER_H
#define __IOBSERVER_H

#include "ISubject.hpp"

class IObserver
{

public:

	virtual void Observe( ISubject* subject ) = 0;

};

#endif
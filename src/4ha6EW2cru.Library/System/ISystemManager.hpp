#ifndef __ISYSTEMMANAGER_H
#define __ISYSTEMMANAGER_H

#include "ISystem.hpp"

class ISystemManager
{

public:

	virtual ~ISystemManager( ) { };

	virtual void AddSystem( ISystem* system ) = 0;
	virtual ISystem* GetSystem( SystemType systemType ) = 0;

	virtual void InitializeAllSystems( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;

};

#endif
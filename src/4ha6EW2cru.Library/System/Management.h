#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H

#include "../Events/IEventManager.hpp"
#include "../Events/EventManager.h"

#include "../System/ISystemManager.hpp"

class Management
{

public:

	Management( );

	~Management( );

	static Management* GetInstance( );

	void Release( );

	static void Initialize( );

	void Update( float deltaMilliseconds );

	inline ISystemManager* GetSystemManager( ) { return _systemManager; };
	inline EventManager* GetEventManager( ) { return _eventManager; };

private:

	ISystemManager* _systemManager;
	EventManager* _eventManager;

};

#endif
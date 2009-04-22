#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H

#include "../Events/EventManager.h"

#include "../System/IServiceManager.h"
#include "../System/IPlatformManager.h"
#include "../System/ISystemManager.hpp"
#include "../IO/IFileManager.hpp"

class Management
{

public:

	virtual ~Management( );

	Management( );

	static Management* GetInstance( );

	void Release( );

	static void Initialize( );

	void Update( float deltaMilliseconds );

	inline ISystemManager* GetSystemManager( ) { return _systemManager; };
	inline EventManager* GetEventManager( ) { return _eventManager; };
	inline IPlatformManager* GetPlatformManager( ) { return _platformManager; };
	inline IFileManager* GetFileManager( ) { return _fileManager; };
	inline IServiceManager* GetServiceManager( ) { return _serviceManager; };

private:

	IServiceManager* _serviceManager;
	ISystemManager* _systemManager;
	IPlatformManager* _platformManager;
	IFileManager* _fileManager;
	EventManager* _eventManager;

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

};

#endif
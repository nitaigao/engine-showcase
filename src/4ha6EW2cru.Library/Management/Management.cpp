#include "Management.h"

#include "../Exceptions/UnInitializedException.hpp"

#include "../Service/ServiceManager.h"
#include "../System/SystemManager.h"

#include "../Platform/Win32PlatformManager.h"
using namespace Platform;

#include "../IO/FileSystem.h"
using namespace IO;

#include "../IO/ResourceCache.h"
using namespace Resources;

static Management* g_ManagementInstance = 0;

Management::Management( )
{
	_serviceManager = new ServiceManager( );
	_platformManager = new Win32PlatformManager( );
	_eventManager = new Events::EventManager( );
	_systemManager = new SystemManager( );
	_fileSystem = new FileSystem( );
	_resourceCache = new ResourceCache( );
}

Management::~Management( )
{
	delete _serviceManager;
	delete _systemManager;
	delete _eventManager;
	delete _platformManager;
	delete _fileSystem;
	delete _resourceCache;
}

void Management::Initialize( )
{
	g_ManagementInstance = new Management( );
}

Management* Management::GetInstance( )
{
	if ( 0 == g_ManagementInstance )
	{
		UnInitializedException e( "Management::GetInstance - Management has not been Initialized" );
		throw e;
	}

	return g_ManagementInstance;
}

void Management::Release( )
{
	_systemManager->Release( );
	
	/* This has been commented so we can see that we have not 
	 * released some of the EventListeners properly, go find those
	 * EventListeners and delete them! */

	//_eventManager->Release( );

	delete g_ManagementInstance;
	g_ManagementInstance = 0;
}

void Management::Update( const float& deltaMilliseconds )
{
	_systemManager->Update( deltaMilliseconds );
	_platformManager->Update( deltaMilliseconds );
	_eventManager->Update( deltaMilliseconds );
	_resourceCache->Update( deltaMilliseconds );
}
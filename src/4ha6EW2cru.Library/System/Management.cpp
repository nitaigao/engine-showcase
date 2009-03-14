#include "Management.h"

#include "../Exceptions/UnInitializedException.hpp"

#include "../System/SystemManager.h"
#include "../System/PlatformManager.h"

static Management* g_ManagementInstance = 0;

Management::Management( )
{
	_platformManager = new PlatformManager( );
	_eventManager = new EventManager( );
	_systemManager = new SystemManager( );
}

Management::~Management( )
{
	delete _systemManager;
	delete _eventManager;
	delete _platformManager;
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

void Management::Update( float deltaMilliseconds )
{
	_platformManager->Update( deltaMilliseconds );
	_eventManager->Update( deltaMilliseconds );
}
#include "Management.h"

#include "../Exceptions/UnInitializedException.hpp"

#include "../Events/EventManager.h"
#include "../System/SystemManager.h"

static Management* g_ManagementInstance = 0;

Management::Management( )
{
	_eventManager = new EventManager( );
	_systemManager = new SystemManager( );
}

Management::~Management( )
{
	delete _systemManager;
	delete _eventManager;
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
	delete g_ManagementInstance;
	g_ManagementInstance = 0;
}

void Management::Update( float deltaMilliseconds )
{
	_systemManager->Update( deltaMilliseconds );
	_eventManager->Update( deltaMilliseconds );
}
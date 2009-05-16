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
	m_serviceManager = new ServiceManager( );
	m_platformManager = new Win32PlatformManager( );
	m_eventManager = new Events::EventManager( );
	m_systemManager = new SystemManager( );
	m_fileSystem = new FileSystem( );
	m_resourceCache = new ResourceCache( );
}

Management::~Management( )
{
	delete m_serviceManager;
	delete m_systemManager;
	delete m_eventManager;
	delete m_platformManager;
	delete m_fileSystem;
	delete m_resourceCache;
}

void Management::Initialize( )
{
	g_ManagementInstance = new Management( );

	g_ManagementInstance->m_fileSystem->Initialize( );
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
	m_systemManager->Release( );
	
	/* This has been commented so we can see that we have not 
	 * released some of the EventListeners properly, go find those
	 * EventListeners and delete them! */

	//m_eventManager->Release( );

	delete g_ManagementInstance;
	g_ManagementInstance = 0;
}

void Management::Update( const float& deltaMilliseconds )
{
	m_systemManager->Update( deltaMilliseconds );
	m_platformManager->Update( deltaMilliseconds );
	m_eventManager->Update( deltaMilliseconds );
	m_resourceCache->Update( deltaMilliseconds );
}
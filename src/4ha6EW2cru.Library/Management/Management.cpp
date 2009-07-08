#include "Management.h"

#include "../Service/ServiceManager.h"
using namespace Services;

#include "../System/SystemManager.h"
#include "../System/Instrumentation.hpp"

#include "../Exceptions/UnInitializedException.hpp"

#include "../Platform/Win32PlatformManager.h"
using namespace Platform;

#include "../IO/FileSystem.h"
using namespace IO;

#include "../IO/ResourceCache.h"
using namespace Resources;

Management* g_management = 0;

Management::~Management( )
{
	delete m_systemManager;
	delete m_serviceManager;
	delete m_resourceCache;
	delete m_fileSystem;
	delete m_instrumentation;
	delete m_eventManager;
	delete m_platformManager;
}

Management::Management( )
{
	m_serviceManager = new ServiceManager( );
	m_platformManager = new Win32PlatformManager( );
	m_eventManager = new Events::EventManager( );
	m_systemManager = new SystemManager( );
	m_instrumentation = new Instrumentation( );
	m_fileSystem = new FileSystem( );
	m_resourceCache = new ResourceCache( );
}

void Management::Initialize( )
{
	g_management = new Management( );

	g_management->m_fileSystem->Initialize( );

	srand( time( 0 ) );
}

void Management::Initialize( Management* management )
{
	g_management = management;
}

bool Management::IsInitialized()
{
	return g_management != 0;
}

void Management::Release( )
{
	g_management->m_systemManager->Release( );

	delete g_management;
}

void Management::Update( const float& deltaMilliseconds )
{
	m_systemManager->Update( deltaMilliseconds );
	m_platformManager->Update( deltaMilliseconds );
	m_eventManager->Update( deltaMilliseconds );
	m_resourceCache->Update( deltaMilliseconds );
}

Management* Management::Get( )
{
	return g_management;
}
#include "Management.h"

#include "../Exceptions/UnInitializedException.hpp"

#include "../Service/ServiceManager.h"
#include "../System/SystemManager.h"
#include "../System/Instrumentation.hpp"

#include "../Platform/Win32PlatformManager.h"
using namespace Platform;

#include "../IO/FileSystem.h"
using namespace IO;

#include "../IO/ResourceCache.h"
using namespace Resources;

IServiceManager* Management::m_serviceManager = new ServiceManager( );
Platform::IPlatformManager* Management::m_platformManager = new Win32PlatformManager( );
Events::EventManager* Management::m_eventManager = new Events::EventManager( );
ISystemManager* Management::m_systemManager = new SystemManager( );
IInstrumentation* Management::m_instrumentation = new Instrumentation( );
IO::IFileSystem* Management::m_fileSystem = new FileSystem( );
Resources::IResourceCache* Management::m_resourceCache = new ResourceCache( );

void Management::Initialize( )
{
	m_fileSystem->Initialize( );

	srand( time( 0 ) );
}

void Management::Release( )
{
	m_systemManager->Release( );
	
	/* This has been commented so we can see that we have not 
	 * released some of the EventListeners properly, go find those
	 * EventListeners and delete them! */

	//m_eventManager->Release( );

	delete m_systemManager;
	delete m_serviceManager;
	delete m_resourceCache;
	delete m_fileSystem;
	delete m_instrumentation;
	delete m_eventManager;
	delete m_platformManager;
}

void Management::Update( const float& deltaMilliseconds )
{
	m_systemManager->Update( deltaMilliseconds );
	m_platformManager->Update( deltaMilliseconds );
	m_eventManager->Update( deltaMilliseconds );
	m_resourceCache->Update( deltaMilliseconds );
}
#include "OgreRenderSystem_Tests.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"
#include "Events/EventManager.h"

#include "Exceptions/ScreenDimensionsException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Mocks/Mock_Configuration.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OgreRenderSystem_Tests, Suites::ViewSuite( ) );

#include "Graphics/OgreRenderSystem.h"
#include "IO/FileManager.h"

void OgreRenderSystem_Tests::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( new ConsoleAppender( ) );

	EventManager::Initialize( );

	_fileManager = new FileManager( );
	_configuration = new Mock_Configuration( );
}

void OgreRenderSystem_Tests::tearDown( )
{
	delete _configuration;

	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void OgreRenderSystem_Tests::Should_Initialize_Correctly( )
{
	OgreRenderSystem renderSystem( _configuration, _fileManager );
	renderSystem.Initialize( );
}

void OgreRenderSystem_Tests::Should_Throw_Given_NULL_Configuration( )
{
	OgreRenderSystem renderSystem( 0, _fileManager );
	CPPUNIT_ASSERT_THROW( renderSystem.Initialize( ), NullReferenceException );
}

void OgreRenderSystem_Tests::Should_Throw_On_Update_Given_Not_Intialized( )
{
	OgreRenderSystem renderSystem ( _configuration, _fileManager );
	CPPUNIT_ASSERT_THROW( renderSystem.Update( 0 ), UnInitializedException );
}

void OgreRenderSystem_Tests::Should_Update_Given_Initialized( )
{
	OgreRenderSystem renderSystem ( _configuration, _fileManager );
	renderSystem.Initialize( );
	renderSystem.Update( 0 );
}
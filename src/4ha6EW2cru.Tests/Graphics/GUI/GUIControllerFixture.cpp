#include "GUIControllerFixture.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "IO/FileManager.h"
#include "Events/EventManager.h"

#include "Graphics/GUI/GUIController.h"

#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/NullReferenceException.hpp"

#include "Graphics/OgreRenderer.h"

#include "../../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GuiControllerFixture, Suites::GraphicsSuite( ) );

void GuiControllerFixture::setUp( )
{
	Logger::Initialize( );

	Logger::GetInstance( )->AddAppender( new ConsoleAppender( ) );

	EventManager::Initialize( );
	FileManager::Initialize( );

	FileManager::GetInstance( )->AddFileStore( "../game/test" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( 640, 480, false );
}

void GuiControllerFixture::tearDown( )
{
	delete _renderer;

	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}
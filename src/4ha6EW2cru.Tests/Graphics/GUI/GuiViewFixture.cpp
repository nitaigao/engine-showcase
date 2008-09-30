#include "GuiViewFixture.h"

#include "Graphics/gui/GuiView.h"

#include "Exceptions/FileNotFoundException.hpp"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "IO/FileManager.h"
#include "Events/EventManager.h"

#include "Graphics/OgreRenderer.h"

#include "../../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( GuiViewFixture, Suites::GraphicsSuite( ) );

void GuiViewFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
	FileManager::Initialize( );

	FileManager::GetInstance( )->AddFileStore( "../game/test" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( 640, 480, false );
}

void GuiViewFixture::tearDown( )
{
	delete _renderer;
	_renderer = 0;

	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );	
}

void GuiViewFixture::Should_Throw_On_Initialize_Given_Layout_Not_Found( )
{
	GuiView view( "blah" );
	CPPUNIT_ASSERT_THROW( view.Initialize( ), FileNotFoundException );
}

void GuiViewFixture::Should_Initialize_Correctly( )
{
	GuiView view( "test" );
	view.Initialize( );
}
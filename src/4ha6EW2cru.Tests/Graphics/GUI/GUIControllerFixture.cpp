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

/*void GuiControllerFixture::Should_Throw_On_Initialize_Given_NULL_Not_Gui( )
{
	GuiController GuiController( "existingscript", 0 );
	CPPUNIT_ASSERT_THROW( GuiController.Initialize( ), NullReferenceException );
}

void GuiControllerFixture::Should_Throw_On_Initialize_Given_Layout_Not_Found( )
{
	//GuiController GuiController( "existingscript", 0 );
	//CPPUNIT_ASSERT_THROW( GuiController.Initialize( ), FileNotFoundException );
}*/

void GuiControllerFixture::Should_Throw_On_Initialize_Given_Script_Not_Found( )
{
	GuiController GuiController( "blah" );
	CPPUNIT_ASSERT_THROW( GuiController.Initialize( ), FileNotFoundException );
}

void GuiControllerFixture::Should_Initialize_Correctly( )
{
	GuiController GuiController( "test" );
	GuiController.Initialize( );
}
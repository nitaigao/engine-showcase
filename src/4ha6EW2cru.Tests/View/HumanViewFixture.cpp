#include "HumanViewFixture.h"

#include "View/HumanView.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"
#include "Events/EventManager.h"
#include "IO/FileManager.h"

#include "Exceptions/ScreenDimensionsException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( HumanViewFixture, Suites::ViewSuite( ) );

void HumanViewFixture::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( new ConsoleAppender( ) );
	EventManager::Initialize( );

	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test" );
}

void HumanViewFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );

	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void HumanViewFixture::Should_Initialize_Given_Valid_Dimensions( )
{
	HumanView humanView;

	humanView.Initialize( 640, 480, 32, false );
}


void HumanViewFixture::Should_Initialize_FullScreen( )
{
	HumanView humanView;

	humanView.Initialize( 1920, 1200, 32, true );
}

void HumanViewFixture::Should_Initialize_Windowed( )
{
	HumanView humanView;

	humanView.Initialize( 640, 480, 32, false );
}

void HumanViewFixture::Should_Throw_On_Initialize_Given_Invalid_Dimensions( )
{
	HumanView humanView;

	CPPUNIT_ASSERT_THROW( humanView.Initialize( -1, -1, -1, false ), ScreenDimensionsException );
}

void HumanViewFixture::Should_Throw_On_Update_Given_Not_Intialized( )
{
	HumanView humanView;
	CPPUNIT_ASSERT_THROW( humanView.Update( ), UnInitializedException );
}

void HumanViewFixture::Should_Update_Given_Initialized( )
{
	HumanView humanView;
	humanView.Initialize( 640, 480, 32, false );
	humanView.Update( );
}

void HumanViewFixture::Should_Throw_On_Render_Given_Not_Intialized( )
{
	HumanView humanView;
	CPPUNIT_ASSERT_THROW( humanView.Render( ), UnInitializedException );
}

void HumanViewFixture::Should_Render_Given_Initialized( )
{
	HumanView humanView;
	humanView.Initialize( 640, 480, 32, false );
	humanView.Render( );
}
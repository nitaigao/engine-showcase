#include "HumanViewFixture.h"

#include "View/HumanView.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"
#include "Events/EventManager.h"
#include "IO/FileManager.h"

#include "Exceptions/ScreenDimensionsException.hpp"
#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( HumanViewFixture, Suites::ViewSuite( ) );

void HumanViewFixture::setUp( )
{
	_configuration = Configuration::Load( "config/game.cfg" );

	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( new ConsoleAppender( ) );
	EventManager::Initialize( );

	FileManager::Initialize( );
	FileManager::GetInstance( )->MountFileStore( "../game/test", "/" );
}

void HumanViewFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );

	EventManager::GetInstance( )->Release( );
	delete _configuration;
	Logger::GetInstance( )->Release( );
}

void HumanViewFixture::Should_Initialize_Correctly( )
{
	HumanView humanView ( _configuration );

	humanView.Initialize( );
}

void HumanViewFixture::Should_Throw_Given_NULL_Configuration( )
{
	HumanView humanView ( 0 );

	CPPUNIT_ASSERT_THROW( humanView.Initialize( ), ScreenDimensionsException );
}

void HumanViewFixture::Should_Throw_On_Update_Given_Not_Intialized( )
{
	HumanView humanView ( _configuration );
	CPPUNIT_ASSERT_THROW( humanView.Update( 0 ), UnInitializedException );
}

void HumanViewFixture::Should_Update_Given_Initialized( )
{
	HumanView humanView ( _configuration );
	humanView.Initialize( );
	humanView.Update( 0 );
}

void HumanViewFixture::Should_Throw_On_Render_Given_Not_Intialized( )
{
	HumanView humanView ( _configuration );
	CPPUNIT_ASSERT_THROW( humanView.Render( ), UnInitializedException );
}

void HumanViewFixture::Should_Render_Given_Initialized( )
{
	HumanView humanView ( _configuration );
	humanView.Initialize( );
	humanView.Render( );
}
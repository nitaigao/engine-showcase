#include "OgreRendererFixture.h"

#include "Exceptions/ScreenDimensionsException.hpp"

#include "Events/EventManager.h"
#include "IO/FileManager.h"
#include "Logging/Logger.h"
#include "System/Management.h"

#include "Exceptions/UnInitializedException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OgreRendererFixture, Suites::GraphicsSuite( ) );

void OgreRendererFixture::setUp( )
{
	Logger::Initialize( );
	Management::Initialize( );
}

void OgreRendererFixture::tearDown( )
{
	delete _renderer;

	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void OgreRendererFixture::Should_Intialize_Correctly( )
{
	_renderer->Initialize( 800, 600, 32, false );
}

void OgreRendererFixture::Should_Throw_ScreenDimensionsException_On_Initialize_Given_Negative_Dimensions( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Initialize( -1, -1, -1, -1 ), ScreenDimensionsException );
}

void OgreRendererFixture::Should_Throw_ScreenDimensionsException_On_Initialize_Given_Zero_Dimensions( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Initialize( 0, 0, 0, 0 ), ScreenDimensionsException );
}


void OgreRendererFixture::Should_Render_Given_Initialized( )
{
	_renderer->Initialize( 1, 1, 32, false );
	_renderer->Render( );
}

void OgreRendererFixture::Should_Throw_On_Render_Given_Not_Intiailized( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Render( ), UnInitializedException );
}

void OgreRendererFixture::Should_Update_Given_Initialized( )
{
	_renderer->Initialize( 1, 1, 32, false );
	_renderer->Update( 0 );
}

void OgreRendererFixture::Should_Throw_On_Update_Given_Not_Intialized( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Update( 0 ), UnInitializedException );
}

void OgreRendererFixture::Should_Return_Positive_Hwnd( )
{
	_renderer->Initialize( 1, 1, 32, false );
	CPPUNIT_ASSERT( _renderer->GetHwnd( ) > 0 );
}

void OgreRendererFixture::Should_Throw_On_GetHWND_Given_Not_Intialized( )
{
	CPPUNIT_ASSERT_THROW( _renderer->GetHwnd( ), UnInitializedException );
}
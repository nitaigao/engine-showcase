#include "OgreRendererFixture.h"

#include "Graphics/OgreRenderer.h"

#include "Exceptions/ScreenDimensionsException.hpp"

#include "Events/EventManager.h"
#include "IO/FileManager.h"
#include "Logging/Logger.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OgreRendererFixture );

void OgreRendererFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
	FileManager::Initialize( );

	_renderer = new OgreRenderer( );
}

void OgreRendererFixture::tearDown( )
{
	delete _renderer;

	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void OgreRendererFixture::Should_Intialize_Correctly( )
{
	CPPUNIT_ASSERT( _renderer->Initialize( 800, 600, false ) );
}

void OgreRendererFixture::Should_Throw_ScreenDimensionsException_On_Initialize_Given_Negative_Dimensions( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Initialize( -1, -1, -1 ), ScreenDimensionsException );
}

void OgreRendererFixture::Should_Throw_ScreenDimensionsException_On_Initialize_Given_Zero_Dimensions( )
{
	CPPUNIT_ASSERT_THROW( _renderer->Initialize( 0, 0, 0 ), ScreenDimensionsException );
}
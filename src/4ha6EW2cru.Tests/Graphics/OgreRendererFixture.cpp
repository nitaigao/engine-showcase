#include "OgreRendererFixture.h"

#include "Graphics/IRenderer.hpp"
#include "Graphics/OgreRenderer.h"

#include "Events/EventManager.h"
#include "IO/FileManager.h"
#include "Logging/Logger.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OgreRendererFixture );

void OgreRendererFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
	FileManager::Initialize( );
}

void OgreRendererFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void OgreRendererFixture::Should_Intialize_Correctly( )
{
	IRenderer* renderer = new OgreRenderer( );
	bool result = renderer->Initialize( 800, 600, false );

	CPPUNIT_ASSERT( result );

	delete renderer;
}
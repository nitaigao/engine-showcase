#include "HumanViewFixture.h"

#include "View/HumanView.h"

#include "Logging/Logger.h"
#include "Events/EventManager.h"
#include "IO/FileManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HumanViewFixture );

void HumanViewFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
}

void HumanViewFixture::tearDown( )
{
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void HumanViewFixture::Should_Intialize_Correctly( )
{
	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test" );

	HumanView humanView;
	bool result = humanView.Initialize( );

	CPPUNIT_ASSERT( result );

	FileManager::GetInstance( )->Release( );
}
#include "HumanView.h"

#include "../Events/EventListener.h"
#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../IO/FileManager.h"
#include "../Graphics/OgreRenderer.h"

#include "../Exceptions/UnInitializedException.hpp"

#include "../Utility/tinyxml/ticpp.h"

void HumanView::Initialize( int width, int height, bool fullScreen )
{
	Logger::GetInstance( )->Info( "Initializing Human View" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( width, height, fullScreen );

	_inputSystem = new InputSystem( _renderer->GetHwnd( ) );
	_inputSystem->Initialize( );
	_inputSystem->SetCaptureArea( width, height );

	EventManager::GetInstance( )->AddEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
	EventManager::GetInstance( )->AddEventListener( VIEW_CHANGE_SCREEN, this, &HumanView::OnChangeScreen );

	_isIntialized = true;
}

void HumanView::Render( )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "HumanView::Render - Attempted to render when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	_renderer->Render( );
}

void HumanView::Update( )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "HumanView::Update - Attempted to update when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}
	
	_inputSystem->Update( );
	_renderer->Update( );
}

HumanView::~HumanView( )
{
	if ( _renderer != 0 )
	{
		delete _renderer;
		_renderer = 0;
	}

	if ( _currentScreen != 0 )
	{
		delete _currentScreen;
		_currentScreen = 0;
	}

	if ( _inputSystem != 0 )
	{
		delete _inputSystem;
		_inputSystem = 0;
	}

	if ( _isIntialized )
	{
		EventManager::GetInstance( )->RemoveEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
		EventManager::GetInstance( )->RemoveEventListener( VIEW_CHANGE_SCREEN, this, &HumanView::OnChangeScreen );		
	}
}

void HumanView::OnGameInitialized( const IEvent* event )
{
	std::string bootStrap = this->LoadScreenConfig( );
	this->ChangeScreen( bootStrap );
}

void HumanView::OnChangeScreen( const IEvent* event )
{
	ChangeScreenEventData* eventData = static_cast< ChangeScreenEventData* > ( event->GetEventData( ) );
	this->ChangeScreen( eventData->GetScreenName( ) );
}

void HumanView::ChangeScreen( std::string screenName )
{
	delete _currentScreen;

	_currentScreen = new GuiScreen( screenName );
	_currentScreen->Initialize( );
}

std::string HumanView::LoadScreenConfig( )
{
	FileBuffer* configBuffer = FileManager::GetInstance( )->GetFile( "gui/screens/screens.xml" );

	std::istringstream inputStream;
	inputStream.str( configBuffer->fileBytes );

	delete configBuffer;

	TiXmlDocument tiDoc;
	inputStream >> tiDoc;

	ticpp::Document doc( &tiDoc );

	std::string bootStrap;

	ticpp::Iterator< ticpp::Node > child;
	for ( child = doc.FirstChild( )->FirstChild( ); child != child.end(); child++ )
	{
		ticpp::Element* element = child->ToElement( );
		
		std::string screenName;
		bool isBootStrap;

		element->GetAttribute( "name", &screenName );
		element->GetAttributeOrDefault( "bootstrap", &isBootStrap, false );
		bootStrap = ( isBootStrap ) ? screenName : bootStrap;
	}

	return bootStrap;
}
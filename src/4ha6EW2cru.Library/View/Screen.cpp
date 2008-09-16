#include "Screen.h"

#include "Ogre.h"
using namespace Ogre;

#include "MyGUI.h"
using namespace MyGUI;

#include "../Logging/Logger.h"
#include "../Events/Event.h"
#include "../Events/EventData.hpp"
#include "../Events/EventListener.h"
#include "../Events/EventManager.h"
#include "../IO/FileManager.h"

Screen::~Screen( )
{
	if( _script != 0 )
	{	
		delete _script;
		_script = 0;
	}
}

bool Screen::loadGUI( )
{
	bool result = false;

	if ( _gui != 0 )
	{
		std::stringstream guiPath;
		guiPath << "gui/screens/" << _screenName << "/" << _screenName << ".layout";

		if ( !FileManager::GetInstance(  )->FileExists( guiPath.str( ) ) )
		{
			std::stringstream cannotFindGUIFile;
			cannotFindGUIFile << "Screen Initialize: Gui Layout File: " << guiPath.str( ) << " doesnt exist";
			Logger::GetInstance( )->Fatal( cannotFindGUIFile.str( ) );
			return false;
		}

		_gui->destroyAllChildWidget(	);
		_gui->load( guiPath.str( ) );

		result = true;
	}

	return result;
}

bool Screen::loadScript( )
{
	bool result = false;

	if ( _script != 0 )
	{
		module( _script->GetState( ) )
		[
			class_< Screen >( "Screen" )
			,

			def( "changeScreen", &Screen::FromLua_ChangeScreen ),
			def( "showMouse", &Screen::FromLua_ShowMouse )
		];

		_script->CallFunction( "onScreenInitialized" );

		result = true;
	}

	return result;
}

bool Screen::Initialize( Gui* gui, Script* script )
{	
	_script = script;
	_gui = gui;

	std::stringstream logMessage;
	logMessage << _screenName.c_str( ) << ": Initializing";
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	if ( !loadGUI( ) )
	{
		throw std::exception( "Unable to initialize screen GUI" );
	}

	if ( !loadScript( ) )
	{
		throw std::exception( "Unable to load Screen Script" );
	}

	return true;
}

void Screen::FromLua_ShowMouse( )
{
	Gui::getInstancePtr( )->showPointer( );
}

void Screen::FromLua_ChangeScreen( std::string screenName )
{
	ChangeScreenEventData* eventData = new ChangeScreenEventData( screenName );
	EventManager::GetInstance( )->QueueEvent( new Event( CHANGE_SCREEN, eventData ) );
}
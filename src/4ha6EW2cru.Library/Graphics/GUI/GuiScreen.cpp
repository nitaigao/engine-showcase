#include "GuiScreen.h"

#include "Ogre.h"
using namespace Ogre;

#include "MyGUI.h"
using namespace MyGUI;

#include "../../Logging/Logger.h"
#include "../../Events/Event.h"
#include "../../Events/EventData.hpp"
#include "../../Events/EventListener.h"
#include "../../Events/EventManager.h"
#include "../../IO/FileManager.h"

GuiScreen::~GuiScreen( )
{
	/*if( _script != 0 )
	{	
		delete _script;
		_script = 0;
	}*/
}

bool GuiScreen::loadGUI( )
{
	bool result = false;

	if ( _gui != 0 )
	{
		std::stringstream guiPath;
		guiPath << "gui/screens/" << _name << "/" << _name << ".layout";

		if ( !FileManager::GetInstance(  )->FileExists( guiPath.str( ) ) )
		{
			std::stringstream cannotFindGUIFile;
			cannotFindGUIFile << "GuiScreen Initialize: Gui Layout File: " << guiPath.str( ) << " doesnt exist";
			Logger::GetInstance( )->Fatal( cannotFindGUIFile.str( ) );
			return false;
		}

		_gui->destroyAllChildWidget( );

		try
		{
			_gui->load( guiPath.str( ) );
		}
		catch ( MyGUIException e )
		{
			Logger::GetInstance( )->Fatal( e.what( ) );
			throw;
		}		

		result = true;
	}

	return result;
}

bool GuiScreen::loadScript( )
{
	bool result = false;

	if ( _script != 0 )
	{
		module( _script->GetState( ) )
		[
			class_< GuiComponent >( "Component" )
				.def( constructor< std::string >( ) )
				.def( "initialize", &GuiComponent::Initialize ),

			class_< GuiScreen >( "GuiScreen" ),

			def( "getScreenWidth", &GuiScreen::FromLua_GetScreenWidth ),
			def( "getScreenHeight", &GuiScreen::FromLua_GetScreenHeight ),
			def( "changeScreen", &GuiScreen::FromLua_ChangeScreen ),
			def( "showMouse", &GuiScreen::FromLua_ShowMouse )
		];

		_script->CallFunction( "onScreenInitialized" );

		result = true;
	}

	return result;
}

/*void GuiScreen::Initialize( Gui* gui, Script* script )
{	
	_script = script;
	_gui = gui;

	if ( gui == 0 )
	{
		NullReferenceException guiNullE( "GuiScreen::Initialize - Cannot Initialize with a NULL GUI" );
		Logger::GetInstance( )->Fatal( guiNullE.what( ) );
		throw guiNullE;
	}

	if ( script == 0 )
	{
		NullReferenceException scriptNullE( "GuiScreen::Initialize - Cannot Initialize with a NULL Script" );
		Logger::GetInstance( )->Fatal( scriptNullE.what( ) );
		throw scriptNullE;
	}

	loadGUI( );
	loadScript( );
}*/

void GuiScreen::FromLua_ShowMouse( )
{
	Gui::getInstancePtr( )->showPointer( );
}

void GuiScreen::FromLua_ChangeScreen( std::string screenName )
{
	ChangeScreenEventData* eventData = new ChangeScreenEventData( screenName );
	EventManager::GetInstance( )->QueueEvent( new Event( VIEW_CHANGE_SCREEN, eventData ) );
}

int GuiScreen::FromLua_GetScreenWidth( )
{
	return Gui::getInstancePtr( )->getRenderWindow( )->getWidth( );
}

int GuiScreen::FromLua_GetScreenHeight( )
{
	return Gui::getInstancePtr( )->getRenderWindow( )->getHeight( );
}
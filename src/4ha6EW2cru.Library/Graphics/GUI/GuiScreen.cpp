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
#include "../../Scripting/ScriptManager.h"
#include "../../Common/Paths.hpp"

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

void GuiScreen::Initialize( )
{
	std::stringstream scriptPath;
	scriptPath << Paths::GetScreensPath( ) << _name << "/" << _name << ".lua";

	_script = ScriptManager::GetInstance( )->CreateScript( scriptPath.str( ) );

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
	
	GuiComponent::Initialize( );
}

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
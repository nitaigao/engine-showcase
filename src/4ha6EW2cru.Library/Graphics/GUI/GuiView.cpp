#include "GUIView.h"

#include "../../Common/Paths.hpp"
#include "../../io/FileManager.h"
#include "../../Exceptions/FileNotFoundException.hpp"

#include "../../Logging/Logger.h"

void GuiView::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << Paths::GetViewsPath( ) << _name << "/" << _name << ".layout";

	if ( !FileManager::GetInstance( )->FileExists( layoutPath.str( ) ) )
	{
		FileNotFoundException e( "GuiView::Initialize - Layout file not found" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	MyGUI::Gui::getInstancePtr( )->load( layoutPath.str( ) );

	MyGUI::Gui::getInstancePtr( )->findWidgetT( "Test" )->eventKeyButtonPressed = MyGUI::newDelegate(  );
}

MyGUI::WidgetPtr GuiView::FindControl( std::string name )
{
	return MyGUI::Gui::getInstancePtr( )->findWidgetT( name );
}

void GuiView::FromLua_AddEventListener( std::string eventName, luabind::object handlerFunction )
{
	_eventHandlers[ eventName ] = handlerFunction;
}
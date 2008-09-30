#include "GUIView.h"

#include "../../Common/Paths.hpp"
#include "../../io/FileManager.h"
#include "../../Exceptions/FileNotFoundException.hpp"

#include "../../Logging/Logger.h"

GuiView::~GuiView( )
{
	for ( HandlerList::iterator i = _handlers.begin( ); i != _handlers.end( ); ++i )
	{
		delete ( *i );
	}
}

void GuiView::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << Paths::GetControlsPath( ) << _name << "/" << _name << ".layout";

	if ( !FileManager::GetInstance( )->FileExists( layoutPath.str( ) ) )
	{
		FileNotFoundException e( "GuiView::Initialize - Layout file not found" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	MyGUI::Gui::getInstancePtr( )->load( layoutPath.str( ) );
}

void GuiView::FromLua_AddEventListener( MyGUI::WidgetPtr target, GuiView* view, EventType eventType, object handlerFunction )
{
	view->AddEventListener( target, eventType, handlerFunction );
}

void GuiView::AddEventListener( MyGUI::WidgetPtr target, EventType eventType, object handlerFunction )
{
	call_function< void >( handlerFunction, "Hello" );

	object* handlerFunctionPtr = new object( handlerFunction );
	_handlers.push_back( handlerFunctionPtr );
	
	target->setUserData( static_cast< void* >( handlerFunctionPtr ) );

	switch( eventType )
	{

	case INPUT_KEY_UP:

		target->eventKeyButtonReleased = MyGUI::newDelegate( &GuiView::ToLua_OnKeyReleased );

		break;

	}
}

MyGUI::WidgetPtr GuiView::FindControl( std::string name )
{
	return MyGUI::Gui::getInstancePtr( )->findWidgetT( name );
}

void GuiView::ToLua_OnKeyReleased( MyGUI::WidgetPtr sender, MyGUI::KeyCode keyCode )
{
	object* handlerPtr = static_cast< object* >( sender->getUserData( ) );
	object handler = *handlerPtr;

	int keyCodeInt = ( int ) keyCode;
	
	call_function< void >( handler, "Hello" );
}


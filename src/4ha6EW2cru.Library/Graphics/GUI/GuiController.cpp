#include "GUIController.h"

GuiController::~GuiController( )
{
	for ( WidgetUserDataList::iterator i = _activeUserData.begin( ); i != _activeUserData.end( ); ++i )
	{
		delete ( *i )->first;
		delete ( *i );
	}
}

void GuiController::FromLua_AddEventListener( MyGUI::WidgetPtr target, GuiController* controller, EventType eventType, object handlerFunction )
{
	controller->AddEventListener( target, eventType, handlerFunction );
}

void GuiController::AddEventListener( MyGUI::WidgetPtr target, EventType eventType, object handlerFunction )
{
	object* handlerFunctionPtr = new object( handlerFunction );
	WidgetUserData* userData = new WidgetUserData( handlerFunctionPtr, this );
	target->setUserData( static_cast< void* >( userData ) );
	_activeUserData.push_back( userData );

	switch( eventType )
	{

	case INPUT_KEY_UP:

		target->eventKeyButtonReleased = MyGUI::newDelegate( &GuiController::ToLua_OnKeyReleased );

		break;

	}
}
void GuiController::ToLua_OnKeyReleased( MyGUI::WidgetPtr sender, MyGUI::KeyCode keyCode )
{
	WidgetUserData* userData = static_cast< WidgetUserData* >( sender->getUserData( ) );

	call_function< void >( *userData->first, userData->second, ( int ) keyCode );
}

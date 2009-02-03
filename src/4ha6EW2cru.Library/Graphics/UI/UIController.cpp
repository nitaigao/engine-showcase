#include "UIController.h"

#include "../../Events/EventManager.h"

UIController::UIController( object luaObject, std::string name, Gui* gui  )
	: _name( name )
	, _gui( gui )
	, _luaObject( luaObject )
{
	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, this, &UIController::EventHandler );
}

void UIController::SetWidgetScript( WidgetPtr widget, const std::string eventName, object function )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );
	
	EventHandlers* eventHandlers = NULL;
	
	if ( widgetUserData == 0 )
	{
		eventHandlers = new EventHandlers( );
		widgetUserData = new WidgetUserData( eventHandlers, this );
	}
	else
	{
		eventHandlers = widgetUserData->first;
	}
	
	object* handlerFunctionPtr = new object( function );
	eventHandlers->insert( std::make_pair( eventName, handlerFunctionPtr ) );
	
	widget->setUserData( static_cast< void* >( widgetUserData ) );
}

void UIController::EventHandler( const IEvent* event )
{
	call_member< void >( _luaObject, "onEvent" );
}
#include "UIController.h"

#include "../../Events/EventManager.h"

UIController::UIController( object luaObject, std::string name, Gui* gui  )
	: _name( name )
	, _gui( gui )
	, _luaObject( luaObject )
{
	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, this, &UIController::EventHandler );
}

UIController::~UIController( )
{
	for ( WidgetList::iterator widget = _widgetList.begin( ); widget != _widgetList.end( ); ++widget )
	{
		void* userData = ( *widget )->getUserData( );
		WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );

		for ( WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
		{
			delete ( *i ).second;
		}

		delete widgetUserData;
	}
}

void UIController::SetWidgetScript( WidgetPtr widget, const std::string eventName, object function )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );
	
	if ( widgetUserData == 0 )
	{
		widgetUserData = new WidgetUserData( );
	}
	
	object* handlerFunctionPtr = new object( function );
	widgetUserData->insert( std::make_pair( eventName, handlerFunctionPtr ) );
	
	widget->setUserData( static_cast< void* >( widgetUserData ) );
	_widgetList.push_back( widget );

	if ( eventName == "onRelease" )
	{
		widget->eventMouseButtonReleased = newDelegate( this, &UIController::OnMouseReleased );
	}
}

void UIController::EventHandler( const IEvent* event )
{
	call_member< void >( _luaObject, "onEvent" );
}

void UIController::OnMouseReleased( MyGUI::WidgetPtr widget, int left, int top, MouseButton buttonId )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );

	for ( WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
	{
		if ( ( *i ).first == "onRelease" )
		{
			object eventHandler = *( *i ).second;
			eventHandler( );
		}
	}
}
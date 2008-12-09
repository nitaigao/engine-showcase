#include "GuiModel.h"

#include "../../Events/EventManager.h"
#include "../../Events/Event.h"
#include "../../Events/EventData.hpp"

GuiModel::~GuiModel( )
{
	for( EventHandlers::iterator i = _eventHandlers.begin( ); i != _eventHandlers.end( ); ++i )
	{
		EventManager::GetInstance( )->RemoveEventListener( ( *i ).first, this, &GuiModel::EventHandler );
		_eventHandlers.erase( i );
		return;
	}
}

void GuiModel::FromLua_AddEventListener( GuiModel* eventTarget, EventType eventType, luabind::object handlerFunction )
{
	eventTarget->AddEventListener( eventType, handlerFunction );
}

void GuiModel::AddEventListener( EventType eventType, luabind::object handlerFunction )
{
	_eventHandlers[ eventType ] = handlerFunction;

	EventManager::GetInstance( )->AddEventListener( eventType, this, &GuiModel::EventHandler );
}

void GuiModel::EventHandler( const IEvent* event )
{
	luabind::object eventHandler = _eventHandlers[ event->GetEventType( ) ];

	switch( event->GetEventType( ) )
	{

	case LOG_MESSAGE_APPENDED: 

		luabind::call_function< void >( eventHandler, this, static_cast< AppenderEventData* >( event->GetEventData( ) ) );

		break;

	case INPUT_KEY_DOWN:
	case INPUT_KEY_UP:

		luabind::call_function< void >( eventHandler, this, static_cast< KeyEventData* >( event->GetEventData( ) ) );

		break;

	}
}

void GuiModel::FromLua_ExecuteCommand( GuiModel* model, std::string command )
{
	ScriptCommandEventData* eventData = new ScriptCommandEventData( command );
	EventManager::GetInstance( )->QueueEvent( new Event( SCRIPT_COMMAND_EXECUTED, eventData ) );
}
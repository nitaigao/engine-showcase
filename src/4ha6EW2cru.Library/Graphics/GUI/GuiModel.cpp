#include "GuiModel.h"

#include "../../Events/EventManager.h"
#include "../../Events/EventData.hpp"

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

	}
}
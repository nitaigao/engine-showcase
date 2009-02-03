#include "UIController.h"

#include "../../Events/EventManager.h"

UIController::UIController( object luaObject, std::string name, Gui* gui  )
	: _name( name )
	, _gui( gui )
	, _luaObject( luaObject )
{
	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, this, &UIController::EventHandler );
}

void UIController::EventHandler( const IEvent* event )
{
	call_member< void >( _luaObject, "onEvent" );
}
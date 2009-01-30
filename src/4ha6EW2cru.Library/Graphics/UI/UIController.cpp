#include "UIController.h"

#include <MyGUI.h>
using namespace MyGUI;

UIController::~UIController( )
{
	this->DestroyAllComponents( );
}

void UIController::LoadComponent( const std::string componentName, luabind::object parentController )
{
	UIComponent* component = new UIComponent( componentName, parentController, _gui );
	component->Initialize( );
	_components.push_back( component );
}

void UIController::DestroyAllComponents( )
{
	for( UIComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		delete ( *i );
	}

	_gui->destroyAllChildWidget( );
}
#include "RootUIController.h"

#include <MyGUI.h>
using namespace MyGUI;

RootUIController::~RootUIController( )
{
	this->DestroyAllComponents( );
}

void RootUIController::LoadComponent( const std::string componentName )
{
	UIComponent* component = new UIComponent( componentName, _gui );
	component->Initialize( );
	_components.push_back( component );
}

void RootUIController::DestroyAllComponents( )
{
	for( UIComponentList::iterator i = _components.begin( ); i != _components.end( ); ++i )
	{
		delete ( *i );
	}

	_gui->destroyAllChildWidget( );
}
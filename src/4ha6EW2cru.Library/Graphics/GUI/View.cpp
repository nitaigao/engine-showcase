#include "View.h"

#include "../../Common/Paths.hpp"

void View::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << Paths::GetControlsPath( ) << _name << "/" << _name << ".view.layout";

	MyGUI::Gui::getInstancePtr( )->load( layoutPath.str( ) );
}

MyGUI::Widget* View::FindControl( std::string name )
{

	return MyGUI::Gui::getInstancePtr( )->findWidgetT( name );
}


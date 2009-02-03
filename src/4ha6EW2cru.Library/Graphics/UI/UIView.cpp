#include "UIView.h"

void UIView::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << "/data/gui/components/" << _name << "/" << _name << ".layout";

	_gui->load( layoutPath.str( ) );
}

WidgetPtr UIView::FindControl( std::string name )
{
	return _gui->findWidgetT( name );
}

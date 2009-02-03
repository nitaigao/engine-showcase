#include "UIView.h"

void UIView::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << "/data/gui/components/" << _name << "/" << _name << ".layout";

	_gui->load( layoutPath.str( ) );

	EnumeratorWidgetPtr widgets = _gui->getEnumerator( );

	while( widgets.next( ) )
	{
		WidgetPtr widget = widgets.current( );
		int a= 1;
	}
}

WidgetPtr UIView::FindControl( std::string name )
{
	return _gui->findWidgetT( name );
}

#ifndef __UIVIEW_H
#define __UIVIEW_H

#include <MyGui.h>
using namespace MyGUI;

class UIView
{

public:

	UIView( std::string name, Gui* gui )
		: _gui( gui )
		, _name( name )
	{

	};

	~UIView( ) { };

	void Initialize( );

	MyGUI::WidgetPtr FindControl( std::string name );

private:

	//void SetupWidgetMessageListeners( VectorWidgetPtr )

	Gui* _gui;
	std::string _name;

	
};


#endif
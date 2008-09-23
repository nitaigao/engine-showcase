#ifndef __VIEW_H
#define __VIEW_H

#include <string>

#include <MyGUI.h>

class View
{

public:

	View( std::string name )
		: _name( name )
	{
		
	};

	void Initialize( );
	MyGUI::Widget* FindControl( std::string name );

private:

	std::string _name;

};

#endif
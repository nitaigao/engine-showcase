#ifndef __CONTROL_H
#define __CONTROL_H

#include <string>

#include "IGuiController.hpp"

class GuiController : public IGuiController
{

public:

	GuiController( const std::string name )
		: _name( name )
	{

	};

	~GuiController( ) { };

	void Initialize( );

private:

	std::string _name;

};

#endif
#ifndef __CONTROL_H
#define __CONTROL_H

#include <string>

#include "../../Scripting/Script.h"

class Control
{

public:

	Control( std::string name )
		: _name( name )
		, _controller( 0 )
	{

	};

	~Control( );

	void Initialize( );

private:

	std::string _name;
	Script* _controller;

};

#endif
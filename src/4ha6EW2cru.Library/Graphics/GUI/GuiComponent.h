#ifndef __GUICOMPONENT_H
#define __GUICOMPONENT_H

#include <string>

#include <Ogre.h>

#include "../../Scripting/Script.h"
#include "IGuiComponent.hpp"

class StringUtils
{

public:

	static std::string ToString( Ogre::UTFString input ) { return ( std::string ) input.asUTF8( ); };	
	static Ogre::UTFString ToUTFString( std::string input ) { return Ogre::UTFString( input ); };	

};

class GuiComponent : public IGuiComponent
{

public:

	GuiComponent( const std::string name )
		: _name( name )
		, _script( 0 )
	{

	};

	~GuiComponent( );

	void Initialize( );

private:

	std::string _name;
	Script* _script;

};

#endif
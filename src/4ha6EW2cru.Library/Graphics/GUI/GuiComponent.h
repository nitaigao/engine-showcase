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

enum GuiComponentTypes
{
	SCREEN,
	CONTROL
};

struct Point
{
	x = 0;
	y = 0;
};

class GuiComponent : public IGuiComponent
{

public: 

	GuiComponent( const std::string name )
		: _name( name )
		, _script( 0 )
	{
		_componentType = CONTROL;
	};

	~GuiComponent( );

	void Initialize( );
	
	/*! Returns the name of the Component*/
	inline const std::string GetName( ) const { return _name; };
	
	/*! Returns the dimensions of the Component */
	inline const Point GetDimensions( ) const { return NULL; };
	
	/*! Returns the position of the Component  */
	inline const Point GetPosition( ) const { return NULL; };
	
protected:

	GuiComponent( )
	{
	
	};

	const std::string _name;
	Script* _script;
	GuiComponentTypes _componentType;

};

#endif
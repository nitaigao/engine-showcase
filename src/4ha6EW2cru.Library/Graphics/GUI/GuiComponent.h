#ifndef __GUICOMPONENT_H
#define __GUICOMPONENT_H

#include <string>

#include <Ogre.h>

#include "../../Scripting/Script.h"
#include "IGuiComponent.hpp"


#include <MyGUI.h>

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

	/*! Casts a Widget to a Window */
	static MyGUI::WindowPtr FromLua_ToWindow( MyGUI::WidgetPtr widget )
	{
		return ( MyGUI::WindowPtr ) widget;
	};
	
protected:

	GuiComponent( )
	{
	
	};

	const std::string _name;
	Script* _script;
	GuiComponentTypes _componentType;

};

#endif
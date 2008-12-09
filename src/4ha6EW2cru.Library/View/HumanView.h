#ifndef __HUMANVIEW_H
#define __HUMANVIEW_H

#include "../Scripting/Script.h"
#include "../Input/InputSystem.h"

#include "../Graphics/GUI/GuiScreen.h"
#include "../Graphics/IRenderer.hpp"

#include "MyGUI.h"
using namespace MyGUI;

/*!
	A Human interactive View of the Game
*/
class HumanView
{

public:

	HumanView( ) 
		: _viewScript( 0 )
		, _currentScreen( 0 )
		, _inputSystem( 0 )
		, _renderer( 0 )
		, _isIntialized( false )
	{ 
	
	};

	~HumanView( );

	/*! Initializes all subsystems of the View */
	void Initialize( int width, int height, int colorDepth, bool fullScreen );

	/*! Cause all subsystems to refresh their data */
	void Update( );

	/*! Render the current Screen */
	void Render( );

	/* Changes the current Screen */
	void ChangeScreen( std::string screenName );

private:

	/*! Changes the current Screen of the View */
	static void FromLua_ChangeScreen( HumanView* sender, std::string screenName, unsigned int visibilityMask );

	/*! Handler to intercept the OnGameInitialized Event */
	void OnGameInitialized( const IEvent* event );

	/*! Handler to intercept the OnChangeScreen Event */
	void OnChangeScreen( const IEvent* event );

	/*! Loads in a list of available screens 
	* @return The BootStrap Screen 
	*/
	std::string LoadScreenConfig( );

	std::string _bootStrapScreen;

	IRenderer* _renderer;
	Script* _viewScript;
	InputSystem* _inputSystem;
	GuiScreen* _currentScreen;
	bool _isIntialized;

};

#endif
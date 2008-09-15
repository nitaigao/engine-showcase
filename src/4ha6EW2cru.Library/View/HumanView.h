#ifndef __HUMANVIEW_H
#define __HUMANVIEW_H

#include "../Scripting/Script.h"
#include "../Input/InputSystem.h"

#include "Screen.h"
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
		, _renderer( 0 )
	{ };

	~HumanView( );

	/*! Initializes all subsystems of the View  */
	bool Initialize( );

	/*! Causes all subsystems to refresh their data */
	void Update( );

	/*! Renders the current Screen */
	void Render( );

private:

	/*! Changes the current Screen of the View */
	static void FromLua_ChangeScreen( HumanView* sender, std::string screenName, unsigned int visibilityMask );

	/*! Handler to intercept the OnGameInitialized Event */
	void OnGameInitialized( const IEvent* event );

	/*! Handler to intercept the OnChangeScreen Event */
	void OnChangeScreen( const IEvent* event );

	/*! Loads in all the Resource Locations from the FileSystem */
	void LoadResources( );

	IRenderer* _renderer;
	Script* _viewScript;
	InputSystem* _inputSystem;

	Screen* _currentScreen;

};

#endif
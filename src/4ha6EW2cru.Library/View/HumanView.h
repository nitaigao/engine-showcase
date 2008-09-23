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
		, _inputSystem( 0 )
		, _renderer( 0 )
		, _isIntialized( false )
	{ 
	
	};

	~HumanView( );

	/*! Initializes all subsystems of the View */
	void Initialize( int width, int height, bool fullScreen );

	/*! Cause all subsystems to refresh their data */
	void Update( );

	/*! Render the current Screen */
	void Render( );

private:

	/*! Changes the current Screen of the View */
	static void FromLua_ChangeScreen( HumanView* sender, std::string screenName, unsigned int visibilityMask );

	/*! Handler to intercept the OnGameInitialized Event */
	void OnGameInitialized( const IEvent* event );

	/*! Handler to intercept the OnChangeScreen Event */
	void OnChangeScreen( const IEvent* event );

	IRenderer* _renderer;
	Script* _viewScript;
	InputSystem* _inputSystem;
	Screen* _currentScreen;
	bool _isIntialized;

};

#endif
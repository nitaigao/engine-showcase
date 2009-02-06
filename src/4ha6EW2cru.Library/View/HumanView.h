#ifndef __HUMANVIEW_H
#define __HUMANVIEW_H

#include "../Scripting/Script.h"
#include "../Input/InputSystem.h"

#include "../Graphics/IRenderer.hpp"

/*!
	A Human interactive View of the Game
*/
class HumanView
{

public:

	HumanView( ) 
		: _inputSystem( 0 )
		, _renderer( 0 )
		, _isIntialized( false )
	{ 
	
	};

	~HumanView( );

	/*! Initializes all subsystems of the View */
	void Initialize( int width, int height, int colorDepth, bool fullScreen );

	/*! Cause all subsystems to refresh their data */
	void Update( const float deltaMilliseconds );

	/*! Render the current Screen */
	void Render( );

private:

	IRenderer* _renderer;
	Script* _viewScript;
	InputSystem* _inputSystem;
	bool _isIntialized;

};

#endif
#ifndef __HUMANVIEW_H
#define __HUMANVIEW_H

#include "../Scripting/Script.h"
#include "../Input/InputSystem.h"

#include "../Graphics/IRenderer.hpp"
#include "../System/Configuration.h"

/*!
	A Human interactive View of the Game
*/
class HumanView
{

public:

	HumanView( Configuration* configuration )
		: _inputSystem( 0 )
		, _renderer( 0 )
		, _isIntialized( false )
		, _configuration( configuration )
	{

	};

	~HumanView( );

	/*! Initializes all subsystems of the View */
	void Initialize( );

	/*! Cause all subsystems to refresh their data */
	void Update( const float deltaMilliseconds );

	/*! Render the current Screen */
	void Render( );

private:

	/*! Handler to intercept OnVideoSettingsChanged Events */
	void OnViewSettingsChanged( const IEvent* event );

	IRenderer* _renderer;
	Script* _viewScript;
	InputSystem* _inputSystem;
	Configuration* _configuration;
	bool _isIntialized;

};

#endif

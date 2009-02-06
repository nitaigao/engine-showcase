#ifndef __OGRERENDERER_H
#define __OGRERENDERER_H

#include "IRenderer.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventData.hpp"

#include "Ogre.h"
#include "MyGUI.h"

#include "../Utility/OgreMax/OgreMaxScene.hpp"

#include "../Scripting/Script.h"

#include "UI/RootUIController.h"

#include <luabind/luabind.hpp>
using namespace luabind;

class OgreRenderer : public IRenderer
{

public:

	OgreRenderer( )
		: _root( 0 )
		, _gui( 0 )
		, _isInitialized( false )
		, _scene( 0 )
		, _badFactory( 0 )
		, _badStubCreated( false )
		, _interfaceController( 0 )
	{

	}

	virtual ~OgreRenderer( );

	void Initialize( int width, int height, int colorDepth, bool fullScreen );
	void Render( ) const;
	void Update( const float deltaMilliseconds ) const;
	size_t GetHwnd( ) const;

private:

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseMoved( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMousePressed( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseReleased( const IEvent* event );

	/*! Handler to intercept OnKeyUp Events */
	void OnKeyUp( const IEvent* event );

	/*! Handler to intercept OnKeyDown Events */
	void OnKeyDown( const IEvent* event );

	void LoadResources( );

	Ogre::Root* _root;
	MyGUI::Gui* _gui;
	bool _isInitialized;
	bool _badStubCreated;

	//Script* _rootUIController;
	RootUIController* _interfaceController;
	OgreMax::OgreMaxScene* _scene;
	Ogre::ArchiveFactory* _badFactory;

};

#endif
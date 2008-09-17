#ifndef __OGRERENDERER_H
#define __OGRERENDERER_H

#include "IRenderer.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventData.hpp"

#include "Ogre.h"
using namespace Ogre;

#include "MyGUI.h"
using namespace MyGUI;

class OgreRenderer : public IRenderer
{

public:

	OgreRenderer( );
	virtual ~OgreRenderer( );

	void Initialize( int width, int height, bool fullScreen );
	void Render( ) const;
	void Update( ) const;
	size_t GetHwnd( ) const;
	Gui* GetGui( );

private:

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseMoved( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMousePressed( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseReleased( const IEvent* event );

	void LoadResources( );

	Root* _root;
	Gui* _gui;
	bool _initialized;

	ArchiveFactory* _badFactory;

};

#endif
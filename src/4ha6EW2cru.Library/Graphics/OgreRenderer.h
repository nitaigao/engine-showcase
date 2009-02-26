#ifndef __OGRERENDERER_H
#define __OGRERENDERER_H

#include "IRenderer.hpp"

#include "Ogre.h"
#include "MyGUI.h"

#include "../Utility/OgreMax/OgreMaxScene.hpp"

#include "../Scripting/Script.h"

#include "Interface.h"

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
		, _interface( 0 )
	{

	}

	virtual ~OgreRenderer( );

	void Initialize( int width, int height, int colorDepth, bool fullScreen );
	void Render( ) const;
	void Update( const float deltaMilliseconds ) const;
	void ChangeResolution( int width, int height, bool fullscreen );
	size_t GetHwnd( ) const;
	std::vector< std::string > GetVideoModes( ) const;

private:

	void LoadResources( );

	Ogre::Root* _root;
	MyGUI::Gui* _gui;
	bool _isInitialized;
	bool _badStubCreated;

	IInterface* _interface;
	OgreMax::OgreMaxScene* _scene;
	Ogre::ArchiveFactory* _badFactory;

};

#endif

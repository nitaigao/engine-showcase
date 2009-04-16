#ifndef __OGRESYSTEMSCENE_H
#define __OGRESYSTEMSCENE_H

#include "../System/ISystem.hpp"

#include <Ogre.h>

#include "OgreRenderSystem.h"

class OgreSystemScene : public ISystemScene
{

public:

	virtual ~OgreSystemScene( ) { };

	OgreSystemScene( Ogre::SceneManager* sceneManager )
		: _sceneManager( sceneManager )
	{

	}

	inline SystemType GetType( ) { return RenderSystemType; };

	void Initialize( ) { };
	void Update( float deltaMilliseconds );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );

	Ogre::SceneManager* GetSceneManager( ) const { return _sceneManager; };

private:

	Ogre::SceneManager* _sceneManager;
	SystemComponentList _components;

	OgreSystemScene( ) { };
	OgreSystemScene( const OgreSystemScene & copy ) { };
	OgreSystemScene & operator = ( const OgreSystemScene & copy ) { return *this; };

};

#endif
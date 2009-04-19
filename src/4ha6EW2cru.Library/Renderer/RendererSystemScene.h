#ifndef __OGRESYSTEMSCENE_H
#define __OGRESYSTEMSCENE_H

#include "../System/ISystem.hpp"

#include <Ogre.h>

#include "RendererSystem.h"

class RendererSystemScene : public ISystemScene
{

public:

	virtual ~RendererSystemScene( ) { };

	RendererSystemScene( Ogre::SceneManager* sceneManager )
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

	RendererSystemScene( ) { };
	RendererSystemScene( const RendererSystemScene & copy ) { };
	RendererSystemScene & operator = ( const RendererSystemScene & copy ) { return *this; };

};

#endif
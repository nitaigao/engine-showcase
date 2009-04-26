#ifndef __OGRESYSTEMSCENE_H
#define __OGRESYSTEMSCENE_H

#include <Ogre.h>

#include "IRenderSystemScene.h"
#include "IRendererSystemComponent.hpp"

#include "RendererSystem.h"

class RendererSystemScene : public IRenderSystemScene
{

public:

	typedef std::vector< IRendererSystemComponent* > RendererSystemComponentList;

	virtual ~RendererSystemScene( ) { };

	RendererSystemScene( Ogre::SceneManager* sceneManager )
		: _sceneManager( sceneManager )
	{

	}

	void Initialize( ) { };
	void Update( float deltaMilliseconds );

	inline System::Types::Type GetType( ) { return System::Types::RENDER; };

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );

	Ogre::SceneManager* GetSceneManager( ) const { return _sceneManager; };
	IRendererSystemComponent* GetComponent( const std::string& name );

private:

	Ogre::SceneManager* _sceneManager;
	RendererSystemComponentList _components;

	RendererSystemScene( ) { };
	RendererSystemScene( const RendererSystemScene & copy ) { };
	RendererSystemScene & operator = ( const RendererSystemScene & copy ) { return *this; };

};

#endif
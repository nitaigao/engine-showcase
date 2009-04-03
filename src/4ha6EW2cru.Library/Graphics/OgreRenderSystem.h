#ifndef __OGRERENDERSYSTEM_H
#define __OGRERENDERSYSTEM_H

#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"
#include "../System/Configuration.h"

#include "../IO/IFileManager.hpp"

#include "IInterface.hpp"
#include <Ogre.h>
using namespace Ogre;

class OgreRenderSystem : public ISystem, public Ogre::WindowEventListener
{

public:

	virtual ~OgreRenderSystem( );

	OgreRenderSystem( Configuration* configuration )
		: _configuration( configuration )
		, _badStubCreated( false )
		, _isIntialized( false )
		, _root( 0 )
		, _window( 0 )
		, _interface( 0 )
		, _badFactory( 0 )
	{

	}

	virtual void Initialize( );
	virtual void Update( float deltaMilliseconds );
	virtual void Release( ) { };

	inline ISystemScene* CreateScene( );
	inline SystemType GetSystemType( ) { return RenderSystemType; };

	inline AnyValueMap GetProperties( ) { return _properties; };
	inline void SetProperty( const std::string& name, AnyValue value );

	inline Ogre::Root* GetRoot( ) { return _root; };

	virtual void windowClosed( RenderWindow* rw );

	void SetCamera( const std::string& cameraName );

private:

	void OnGraphicsSettingsUpdated( const IEvent* event );

	void Constructor( Configuration* configuration, IFileManager* fileManager );
	void LoadResources( );
	std::vector< std::string > GetVideoModes( ) const;

	AnyValueMap _properties;
	Configuration* _configuration;
	IInterface* _interface;
	bool _badStubCreated;
	bool _isIntialized;
	std::vector< std::string > _supportedVideoModes;
	std::string _skyBoxMaterial;

	Ogre::Root* _root;
	Ogre::RenderWindow* _window;
	Ogre::ArchiveFactory* _badFactory;

	OgreRenderSystem( ) { };
	OgreRenderSystem( const OgreRenderSystem & copy ) { };
	OgreRenderSystem & operator = ( const OgreRenderSystem & copy ) { return *this; };

};

#endif
#ifndef __OGRERENDERSYSTEM_H
#define __OGRERENDERSYSTEM_H

#include "../System/ISystem.hpp"
#include "../System/Configuration.h"

#include "../IO/IFileManager.hpp"

#include "IInterface.hpp"
#include <Ogre.h>
using namespace Ogre;

class OgreRenderSystem : public ISystem, public Ogre::WindowEventListener
{

public:

	OgreRenderSystem( Configuration* configuration, IFileManager* fileManager );
	OgreRenderSystem( Configuration* configuration );

	virtual ~OgreRenderSystem( );

	virtual void Initialize( );
	virtual void Update( float deltaMilliseconds );
	virtual void Release( ) { };

	inline SystemType GetSystemType( ) { return RenderSystemType; };

	inline PropertyMap GetProperties( ) { return _properties; };

	ISystemObject* CreateObject( const std::string& name, SystemType systemType )
	{
		return 0;
	}

	virtual void windowClosed( RenderWindow* rw );

private:

	OgreRenderSystem( const OgreRenderSystem & copy ) { };
	OgreRenderSystem & operator = ( const OgreRenderSystem & copy ) { return *this; };

	inline void Constructor( Configuration* configuration, IFileManager* fileManager )
	{
		_configuration = configuration;
		_badStubCreated = false;
		_isIntialized = false;
		_root = 0;
		_window = 0;
		_interface = 0;
		_badFactory = 0;
		_fileManager = fileManager;
	}

	void OnGraphicsSettingsUpdated( const IEvent* event );

	void LoadResources( );
	std::vector< std::string > GetVideoModes( ) const;

	PropertyMap _properties;
	Configuration* _configuration;
	IFileManager* _fileManager;
	IInterface* _interface;
	bool _badStubCreated;
	bool _isIntialized;

	Ogre::Root* _root;
	Ogre::RenderWindow* _window;
	Ogre::ArchiveFactory* _badFactory;

};

#endif
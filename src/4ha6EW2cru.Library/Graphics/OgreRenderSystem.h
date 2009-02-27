#ifndef __OGRERENDERSYSTEM_H
#define __OGRERENDERSYSTEM_H

#include "../System/ISystem.hpp"
#include "../System/IConfiguration.hpp"

#include "../IO/IFileManager.hpp"

#include "IInterface.hpp"
#include <Ogre.h>
using namespace Ogre;

class OgreRenderSystem : public ISystem
{

public:

	
	OgreRenderSystem( IConfiguration* configuration, IFileManager* fileManager );
	OgreRenderSystem( IConfiguration* configuration );

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

private:

	OgreRenderSystem( const OgreRenderSystem & copy ) { };
	OgreRenderSystem & operator = ( const OgreRenderSystem & copy ) { return *this; };

	inline void Constructor( IConfiguration* configuration, IFileManager* fileManager )
	{
		_configuration = configuration;
		_badStubCreated = false;
		_isIntialized = false;
		_root = 0;
		_interface = 0;
		_badFactory = 0;
		_fileManager = fileManager;
	}

	void LoadResources( );

	PropertyMap _properties;
	IConfiguration* _configuration;
	IFileManager* _fileManager;
	bool _isIntialized;

	Root* _root;
	IInterface* _interface;
	bool _badStubCreated;
	ArchiveFactory* _badFactory;

};

#endif
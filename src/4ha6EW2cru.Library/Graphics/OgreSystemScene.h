#ifndef __OGRESYSTEMSCENE_H
#define __OGRESYSTEMSCENE_H

#include "../System/ISystem.hpp"

#include <Ogre.h>
using namespace Ogre;

#include "OgreRenderSystem.h"

class OgreSystemScene : public ISystemScene
{

public:

	virtual ~OgreSystemScene( ) { };

	OgreSystemScene( OgreRenderSystem* system )
		: _system( system )
	{

	}

	inline SystemType GetType( ) { return RenderSystemType; };
	inline Root* GetRoot( ) { return _system->GetRoot( ); };
	
	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component );
	void Update( float deltaMilliseconds );
	

private:

	OgreRenderSystem* _system;

	OgreSystemScene( ) { };
	OgreSystemScene( const OgreSystemScene & copy ) { };
	OgreSystemScene & operator = ( const OgreSystemScene & copy ) { return *this; };

};

#endif
#ifndef __OGRERENDERSYSTEMCOMPONENT_H
#define __OGRERENDERSYSTEMCOMPONENT_H

#include "../Utility/OgreMax/OgreMaxModel.hpp"
#include "../System/ISystemComponent.hpp"

class OgreRenderSystemComponent : public Ogre::SceneNode, public ISystemComponent
{

public:

	virtual ~OgreRenderSystemComponent( ) { };

	OgreRenderSystemComponent( const std::string& name, Ogre::SceneManager* creator )
		: Ogre::SceneNode( creator, name ) 
		, _name( name )
	{

	}

	SystemType GetType( ) { return RenderSystemType; };
	void Initialize( SystemPropertyList properties );

private:

	std::string _name;

};

#endif
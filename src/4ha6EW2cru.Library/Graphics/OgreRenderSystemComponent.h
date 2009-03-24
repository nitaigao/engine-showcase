#ifndef __OGRERENDERSYSTEMCOMPONENT_H
#define __OGRERENDERSYSTEMCOMPONENT_H

#include "../Utility/OgreMax/OgreMaxModel.hpp"
#include "../System/ISystemComponent.hpp"

#include "OgreSystemScene.h"

class OgreRenderSystemComponent : public ISystemComponent
{

public:

	virtual ~OgreRenderSystemComponent( );

	OgreRenderSystemComponent( const std::string& name, OgreSystemScene* scene )
		: _name( name )
		, _sceneNode( 0 )
		, _scene( scene )
	{

	}

	void Initialize( SystemPropertyList properties );

	void AddObserver( IObserver* observer ) { };
	void PushChanges( unsigned int systemChanges ){ };

	inline unsigned int GetRequestedChanges( ) 
	{ 
		return 
			System::Changes::Geometry::Position | 
			System::Changes::Geometry::Orientation | 
			System::Changes::Geometry::Scale |
			System::Changes::Geometry::All |
			System::Changes::Input::Mouse_Moved;
	};

	void Observe( ISubject* subject, unsigned int systemChanges );

	unsigned int GetRequiredSystemChanges ( );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return RenderSystemType; };

private:

	std::string _name;
	OgreSystemScene* _scene;
	Ogre::SceneNode* _sceneNode;

	OgreRenderSystemComponent( ) { };
	OgreRenderSystemComponent( const OgreRenderSystemComponent & copy ) { };
	OgreRenderSystemComponent & operator = ( const OgreRenderSystemComponent & copy ) { return *this; };

};

#endif
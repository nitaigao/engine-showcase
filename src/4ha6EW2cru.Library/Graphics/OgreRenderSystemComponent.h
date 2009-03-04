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
		, _scene( scene )
	{

	}

	void Initialize( SystemPropertyList properties );
	void AddObserver( IObserver* observer ) { };
	void Observe( ISubject* subject );
	unsigned int GetRequiredSystemChanges ( );

	inline const std::string& GetName( ) { return _name; };
	inline SystemType GetType( ) { return RenderSystemType; };

private:

	std::string _name;
	OgreSystemScene* _scene;

};

#endif
#ifndef __ISCRIPTCOMPONENT_H
#define __ISCRIPTCOMPONENT_H

#include "../System/ISystemComponent.hpp"

class IScriptComponent : public ISystemComponent
{

public:

	virtual void Update( float deltaMilliseconds ) = 0;

};

#endif
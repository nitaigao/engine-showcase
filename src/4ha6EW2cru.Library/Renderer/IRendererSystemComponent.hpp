#ifndef __IRENDERERSYSTEMCOMPONENT_H
#define __IRENDERERSYSTEMCOMPONENT_H

#include "../System/ISystemComponent.hpp"

class IRendererSystemComponent : public ISystemComponent
{

public:

	virtual void PlayAnimation( const std::string& animationName, const bool& loopAnimation ) = 0;
};

#endif
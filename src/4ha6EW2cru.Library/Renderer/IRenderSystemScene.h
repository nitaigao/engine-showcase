#ifndef __IRENDERSYSTEMSCENE_H
#define __IRENDERSYSTEMSCENE_H

#include "../System/ISystemScene.hpp"
#include "IRendererSystemComponent.hpp"

class IRenderSystemScene : public ISystemScene
{

public:

	virtual ~IRenderSystemScene( ) { };

	virtual IRendererSystemComponent* GetComponent( const std::string& name ) = 0;
};

#endif
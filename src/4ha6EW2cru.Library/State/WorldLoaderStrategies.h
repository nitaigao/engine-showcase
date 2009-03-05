#ifndef __WORLDLOADERSTRATEGIES_H
#define __WORLDLOADERSTRATEGIES_H

#include <string>
#include "yaml.h"

#include "../System/ISystemScene.hpp"

class IWorldLoader_ComponentStrategy
{

public:

	virtual ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList& systemScenes ) = 0;

};

class WorldLoader_GraphicsComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList& systemScenes );

};

class WorldLoader_GeometryComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneList& systemScenes );

};

class WorldLoader_ComponentStrategy_Factory
{

public:

	static IWorldLoader_ComponentStrategy* Create( const std::string& componentType );

};

#endif
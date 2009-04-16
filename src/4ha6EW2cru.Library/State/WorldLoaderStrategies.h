#ifndef __WORLDLOADERSTRATEGIES_H
#define __WORLDLOADERSTRATEGIES_H

#include <string>
#include "yaml.h"

#include "../System/ISystemScene.hpp"


class IWorldLoader_ComponentStrategy
{

public:

	virtual ~IWorldLoader_ComponentStrategy( ) { };

	virtual ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes ) = 0;

};

class WorldLoader_ComponentStrategy_Factory
{

public:

	static IWorldLoader_ComponentStrategy* Create( const std::string& componentType );

private:

	~WorldLoader_ComponentStrategy_Factory( ) { };
	WorldLoader_ComponentStrategy_Factory( ) { };
	WorldLoader_ComponentStrategy_Factory( const WorldLoader_ComponentStrategy_Factory & copy ) { };
	WorldLoader_ComponentStrategy_Factory & operator = ( const WorldLoader_ComponentStrategy_Factory & copy ) { return *this; };

};

class WorldLoader_GraphicsComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	virtual ~WorldLoader_GraphicsComponentStrategy( ) { };
	WorldLoader_GraphicsComponentStrategy( ) { };

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes );

private:

	WorldLoader_GraphicsComponentStrategy( const WorldLoader_GraphicsComponentStrategy & copy ) { };
	WorldLoader_GraphicsComponentStrategy & operator = ( const WorldLoader_GraphicsComponentStrategy & copy ) { return *this; };

};

class WorldLoader_GeometryComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	virtual ~WorldLoader_GeometryComponentStrategy( ) { };
	WorldLoader_GeometryComponentStrategy( ) { };

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes );

private:

	WorldLoader_GeometryComponentStrategy( const WorldLoader_GeometryComponentStrategy & copy ) { };
	WorldLoader_GeometryComponentStrategy & operator = ( const WorldLoader_GeometryComponentStrategy & copy ) { return *this; };

};

class WorldLoader_PhysicsComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	virtual ~WorldLoader_PhysicsComponentStrategy( ) { };
	WorldLoader_PhysicsComponentStrategy( ) { };

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes );

private:

	WorldLoader_PhysicsComponentStrategy( const WorldLoader_PhysicsComponentStrategy & copy ) { };
	WorldLoader_PhysicsComponentStrategy & operator = ( const WorldLoader_PhysicsComponentStrategy & copy ) { return *this; };

};

class WorldLoader_InputComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	virtual ~WorldLoader_InputComponentStrategy( ) { };
	WorldLoader_InputComponentStrategy( ) { };

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes );

private:

	WorldLoader_InputComponentStrategy( const WorldLoader_InputComponentStrategy & copy ) { };
	WorldLoader_InputComponentStrategy & operator = ( const WorldLoader_InputComponentStrategy & copy ) { return *this; };

};

class WorldLoader_AIComponentStrategy : public IWorldLoader_ComponentStrategy
{

public:

	virtual ~WorldLoader_AIComponentStrategy( ) { };
	WorldLoader_AIComponentStrategy( ) { };

	ISystemComponent* CreateComponent( const std::string& entityName, const YAML::Node& componentNode, const SystemSceneMap& systemScenes );

private:

	WorldLoader_AIComponentStrategy( const WorldLoader_AIComponentStrategy & copy ) { };
	WorldLoader_AIComponentStrategy & operator = ( const WorldLoader_AIComponentStrategy & copy ) { return *this; };

};

#endif
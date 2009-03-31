#ifndef __WORLDLOADER_H
#define __WORLDLOADER_H

#include <queue>

#include "yaml.h"

#include "IWorldLoader.hpp"
#include "../io/IFileManager.hpp"

class WorldLoader : public IWorldLoader
{

public:

	virtual ~WorldLoader( );

	WorldLoader( IWorld* world )
		: _world( world )
		, _loadPosition( 0 )
		, _loadSource( new YAML::Node( ) )
	{

	}

	void Load( const std::string& levelPath );
	void Update( float deltaMilliseconds );

private:

	void LoadEntity( const YAML::Node& node );
	void LoadEntityComponents( const YAML::Node& node, IEntity* entity );

	IWorld* _world;

	YAML::Node* _loadSource;
	int _loadPosition;


	WorldLoader( ) { };
	WorldLoader( const WorldLoader & copy ) { };
	WorldLoader & operator = ( const WorldLoader & copy ) { return *this; };

};

#endif
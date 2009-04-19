#ifndef __WORLDLOADER_H
#define __WORLDLOADER_H

#include <queue>

#include "yaml.h"

#include "IWorldLoader.hpp"
#include "../io/IFileManager.hpp"

class WorldLoader : public IWorldLoader
{
	typedef std::queue< YAML::Node* > NodeList;

public:

	virtual ~WorldLoader( ) { };

	WorldLoader( IWorld* world )
		: _world( world )
		, _activeNodeIndex( 0 )
		, _loadTotal( 0 )
		, _loadProgress( 0 )
	{

	}

	void Load( const std::string& levelPath );
	void Update( float deltaMilliseconds );

private:

	void LoadNode( const YAML::Node& node );
	void LoadColor( const YAML::Node& node );
	void LoadSkyBox( const YAML::Node& node );
	void LoadEntity( const YAML::Node& node );
	void LoadLink( const YAML::Node& node );

	void LoadEntityComponents( const YAML::Node& node, IEntity* entity );

	IWorld* _world;

	NodeList _loadQueue;

	unsigned int _activeNodeIndex;
	int _loadProgress;
	int _loadTotal;

	WorldLoader( ) { };
	WorldLoader( const WorldLoader & copy ) { };
	WorldLoader & operator = ( const WorldLoader & copy ) { return *this; };

};

#endif
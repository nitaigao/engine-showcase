#ifndef __LEVELLOADER_H
#define __LEVELLOADER_H

#include "ILevelLoader.hpp"

#include "IWorld.hpp"

class LevelLoader : public ILevelLoader
{

public:

	LevelLoader( ) { };
	~LevelLoader( ) { };

	void LoadLevel( const std::string levelName, IWorld* world );


};


#endif
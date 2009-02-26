#ifndef __ILEVELLOADER_H
#define __ILEVELLOADER_H

#include <string>

#include "IWorld.hpp"

class ILevelLoader
{

public:

	virtual ~ILevelLoader( ) { };

	virtual void LoadLevel( const std::string levelName, IWorld* world ) = 0;

};

#endif
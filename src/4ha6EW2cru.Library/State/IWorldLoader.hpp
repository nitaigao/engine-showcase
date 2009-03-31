#ifndef __IWORLDLOADER_H
#define __IWORLDLOADER_H

#include "IWorld.hpp"

class IWorldLoader
{

public:

	virtual ~IWorldLoader( ) { };

	virtual void Load( const std::string& levelPath ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;

};

#endif
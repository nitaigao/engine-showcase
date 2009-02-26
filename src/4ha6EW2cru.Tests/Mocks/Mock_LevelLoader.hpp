#ifndef __MOCK_LEVEL_LOADER_H
#define __MOCK_LEVEL_LOADER_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "Logic/ILevelLoader.hpp"

class Mock_LevelLoader : public ILevelLoader, public MockObject
{

public:

	Mock_LevelLoader( )
		: MockObject( "Mock_LevelLoader", 0 )
		, loadLevel_count( "Mock_LevelLoader/LeveLoader", 0 )
	{ };
	
	ExpectationCounter loadLevel_count;

	void LoadLevel( const std::string levelName, IWorld* world )
	{
		this->loadLevel_count.inc( );
	}


private:

	Mock_LevelLoader & operator = ( const Mock_LevelLoader & copy ) { return *this; };
};


#endif
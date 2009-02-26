#include "LevelLoader.h"

#include <sstream>

#include "../io/FileManager.h"

#include <yaml.h>

void LevelLoader::LoadLevel( const std::string levelName, IWorld* world )
{
	std::stringstream levelPath;
	levelPath << "data/levels/" << levelName << "/entities.yaml";
	FileBuffer* entitiesBuffer =  FileManager::GetInstance( )->GetFile( levelPath.str( ) );

	std::stringstream bufferStream;
	bufferStream << entitiesBuffer->fileBytes;

	YAML::Parser parser( bufferStream );

	while( parser )
	{
		YAML::Node entitesDoc;
		parser.GetNextDocument( entitesDoc );

		for( YAML::Iterator it = entitesDoc.begin( ) ; it != entitesDoc.end( ); ++it ) 
		{
			// hmm didnt get to finish this

			std::string key, value;
			it.first( ) >> key;
			it.second( ) >> value;
		}
	}
}
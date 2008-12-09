#include "Configuration.h"

#include <string>
#include <sstream>
#include <iostream>

#include "../IO/FileManager.h"

Configuration* Configuration::Load( const std::string filePath )
{
	FileBuffer* configBuffer = FileManager::GetInstance( )->GetFile( filePath );

	std::stringstream configStream;
	configStream << configBuffer->fileBytes;

	delete configBuffer;

	std::string line;

	Configuration* configuration = new Configuration( );

	while( std::getline( configStream, line ) )
	{
		size_t foundSeperator = line.find( "=" );

		if ( foundSeperator != line.npos )
		{
			std::string key = line.substr( 0, foundSeperator );
			std::string value = line.substr( foundSeperator + 1, line.length( ) - foundSeperator );
			configuration->AddConfigItem( key, value );
		}
	}

	return configuration;
}

void Configuration::AddConfigItem( std::string key, std::string value )
{
	_configItems[ key ] = value;
}
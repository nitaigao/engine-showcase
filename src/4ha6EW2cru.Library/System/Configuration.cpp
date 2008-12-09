#include "Configuration.h"

#include <string>
#include <sstream>
#include <iostream>

#include "../IO/FileManager.h"
#include "../Logging/Logger.h"

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

int Configuration::FindConfigItemInt( std::string key ) 
{
	std::stringstream keyStream( this->FindConfigItemString( key ) );
	int result = 0;
	keyStream >> result;
	return result;
}

std::string Configuration::FindConfigItemString( std::string key )
{
	if ( _configItems.find( key ) != _configItems.end( ))
	{
		return _configItems[ key ];
	}	

	OutOfRangeException e( "Configuration::FindConfigItem - The specified key could not be found" );
	Logger::GetInstance( )->Fatal( e.what( ) );
	throw e;
}
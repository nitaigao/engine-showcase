#ifndef __CONFIGURATIONFILE_H
#define __CONFIGURATIONFILE_H

#include <boost/program_options.hpp>

class ConfigurationFile
{

public:

	ConfigurationFile( const FileBuffer* fileBuffer )
	{

	}

	static ConfigurationFile* Load( const std::string& filePath )
	{
		FileBuffer* buffer = FileManager::GetInstance( )->GetFile( filePath ); 
		return new ConfigurationFile( buffer );
	}

	template< class T >
	T FindConfigItem( const std::string& key )
	{
		OutOfRangeException e( "The specified configuration item could not be found" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

private:


	boost::program_options::variables_map _variables;

};

#endif
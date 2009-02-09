#ifndef __CONFIGURATIONFILE_H
#define __CONFIGURATIONFILE_H

#include <sstream>

#include <boost/program_options.hpp>

using namespace boost::program_options;

class ConfigurationFile
{

public:

	ConfigurationFile( const FileBuffer* fileBuffer )
	{
		options_description desc;
		std::stringstream fileStream;
		fileStream << fileBuffer->fileBytes;
		std::string something = std::string( fileStream.str( ) );

		int intOption;
		std::string stringOption;
		bool boolOption;

		desc.add_options( )
			( "Display.display_width", value< int >( &intOption )->default_value( 640 ), "Display Width" )
			( "Display.display_height", value< int >( &intOption )->default_value( 480 ), "Display Height" )
			( "Display.display_depth", value< int >( &intOption )->default_value( 32 ), "Display Depth" )
			( "Display.display_isfullscreen", value< bool >( &boolOption )->default_value( 1 ), "Display Fullscreen" )
			( "System.system_developer", value< bool >( &boolOption )->default_value( 0 ), "Developer Mode" )
			( "Logging.log_level", value< int >( &intOption )->default_value( 1 ), "Logging Level" )
		;
	
		store( parse_config_file( fileStream, desc ), _variables);
	}

	static ConfigurationFile* Load( const std::string& filePath )
	{
		FileBuffer* buffer = FileManager::GetInstance( )->GetFile( filePath ); 
		return new ConfigurationFile( buffer );
	}

	template< class T >
	T FindConfigItem( const std::string& key )
	{	
		try
		{
			return _variables[ key ].as< T >( );
		}
		catch( boost::bad_any_cast ac )
		{
			OutOfRangeException e( "The specified configuration item could not be found or is an invalid value for the given type" );
			Logger::GetInstance( )->Fatal( e.what( ) );
			throw e;
		}
	}

private:


	boost::program_options::variables_map _variables;

};

#endif
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <map>

#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

/*! Loads and contains game specific configuration info */
class Configuration
{

	typedef std::map< std::string, std::string > ConfigItemList;

public:

	inline static Configuration* Create( ) { return new Configuration( ); };

	/*! Loads a config file from the FileSystem */
	static Configuration* Load( const std::string filePath );

	/*! Adds a config entry */
	void AddConfigItem( std::string key, std::string value );

	/*! Finds a std::string config entry */
	std::string FindConfigItemString( std::string key );

	/*! Finds a std::string config entry */
	int FindConfigItemInt( std::string key );

protected:

	Configuration( )
	{

	};

private:

	ConfigItemList _configItems;
	
};

#endif
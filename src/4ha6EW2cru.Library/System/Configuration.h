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

	inline bool IsFullScreen( ) const { return true; };//this->FindConfigItemInt( "display_isfullscreen" ); };
	inline void SetFullScreen( bool isFullScreen ) { this->SetConfigItem( "display_isfullscreen", boolToString( isFullScreen ) ); };

	void SetConfigItem( std::string key, std::string value )
	{
		for( ConfigItemList::iterator i = _configItems.begin( ); i != _configItems.end( ); ++i )
		{
			if ( ( *i ).first == key )
			{
				( *i ).second = value;
				return;
			}
		}

		_configItems.insert( std::make_pair( key, value ) );
	};

	inline std::string boolToString( bool input )
	{
		return ( input ) ? "1" : "0";
	};


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
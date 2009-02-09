#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <map>
#include <sstream>

#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

/*! Loads and contains game specific configuration info */
class Configuration
{

	typedef std::map< std::string, std::string > ConfigItemList;

public:

	inline bool IsFullScreen( ) const { return _isFullScreen; };
	inline void SetFullScreen( bool isFullScreen ) { _isFullScreen = isFullScreen; };// this->SetConfigItem( "display_isfullscreen", BoolToString( isFullScreen ) ); };

	inline int GetDisplayWidth ( ) const { return 1280; };
	inline void SetDisplayWidth( int width ) { this->SetConfigItem( "display_width", IntToString( width ) ); };

	inline int GetDisplayHeight ( ) const { return 800; };
	inline void SetDisplayHeight( int height ) { this->SetConfigItem( "display_height", IntToString( height ) ); };

	inline int GetColorDepth( ) const { return 32; };

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

	inline std::string BoolToString( bool input )
	{
		return ( input ) ? "1" : "0";
	};

	inline std::string IntToString( int input )
	{
		std::stringstream output;
		output << input;
		return output.str( );
	}

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
		: _isFullScreen( false )
	{

	};

private:

	bool _isFullScreen;

	ConfigItemList _configItems;
	
};

#endif
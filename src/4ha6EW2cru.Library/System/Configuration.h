#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

#include "../Events/IEvent.hpp"

#include "ConfigurationFile.h"

#include <boost/any.hpp>


/*! Loads and contains game specific configuration info */
class Configuration
{
	typedef std::map< std::string, boost::any > DefaultPropertyList;

public:

	virtual ~Configuration( )
	{
		delete _configFile;
	}

	static Configuration* Load( const std::string& filePath )
	{
		ConfigurationFile* configFile = ConfigurationFile::Load( filePath );
		return new Configuration( configFile );
	};

	void SetDefault( const std::string& section, const std::string& key, const boost::any& value )
	{	
		_defaultPropertyList[ key ] = value;
	}

	template< class T >
	T Find( const std::string& section, const std::string& key )
	{
		boost::any result;

		for( DefaultPropertyList::iterator i = _defaultPropertyList.begin( ); i != _defaultPropertyList.end( ); ++i )
		{
			if ( ( *i ).first == key )
			{
				if ( ( *i ).second.type( ) == typeid( std::string ) )
				{
					result = boost::any( _configFile->FindConfigItem( section, key, boost::any_cast< std::string >( ( *i ).second ) ) );
				}

				if ( ( *i ).second.type( ) == typeid( int ) )
				{
					result = boost::any( _configFile->FindConfigItem( section, key, boost::any_cast< int >( ( *i ).second ) ) );
				}

				if ( ( *i ).second.type( ) == typeid( bool ) )
				{
					result = boost::any( _configFile->FindConfigItem( section, key, boost::any_cast< bool >( ( *i ).second ) ) );
				}
			}
		}

		return boost::any_cast< T >( result );
	}

	void Set( const std::string& section, const std::string& key, const std::string& value )
	{
		_configFile->Update( section, key, value );
		_configFile->Save( "config/game.cfg" );
	}

	void Set( const std::string& section, const std::string& key, const int& value )
	{
		_configFile->Update( section, key, value );
		_configFile->Save( "config/game.cfg" );
	}

	void Set( const std::string& section, const std::string& key, const bool& value )
	{
		_configFile->Update( section, key, value );
		_configFile->Save( "config/game.cfg" );
	}

private:

	Configuration( const Configuration & copy ) { };
	Configuration & operator = ( const Configuration & copy ) { return *this; };

	Configuration( ConfigurationFile* configFile )
		: _configFile( configFile )
	{

	};

	ConfigurationFile* _configFile;
	DefaultPropertyList _defaultPropertyList;
	
};

#endif
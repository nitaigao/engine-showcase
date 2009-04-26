#include "Configuration.h"

#include "ConfigurationFile.h"

namespace Configuration
{
	ClientConfiguration::~ClientConfiguration()
	{
		delete _configFile;
	}

	IConfiguration* ClientConfiguration::Load( const std::string& filePath )
	{
		IConfigurationFile* configFile = ConfigurationFile::Load( filePath );
		return new ClientConfiguration( configFile );
	}

	AnyValue ClientConfiguration::Find( const std::string& section, const std::string& key )
	{
		AnyValue result;

		for( DefaultPropertyList::iterator i = _defaultPropertyList.begin( ); i != _defaultPropertyList.end( ); ++i )
		{
			if ( ( *i ).first == key )
			{
				result = _configFile->FindConfigItem( section, key, ( *i ).second );
			}
		}

		return result;
	}

	void ClientConfiguration::Set( const std::string& section, const std::string& key, const AnyValue& value )
	{
		_configFile->Update( section, key, value );
		_configFile->Save( "config/game.cfg" );
	}

	void ClientConfiguration::SetDefault( const std::string& section, const std::string& key, const AnyValue& value )
	{
		_defaultPropertyList[ key ] = value;
	}
}
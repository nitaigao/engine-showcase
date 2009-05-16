#include "Configuration.h"

#include "ConfigurationFile.h"

namespace Configuration
{
	ClientConfiguration::~ClientConfiguration()
	{
		delete m_configFile;
	}

	IConfiguration* ClientConfiguration::Load( const std::string& filePath )
	{
		IConfigurationFile* configFile = ConfigurationFile::Load( filePath );
		return new ClientConfiguration( configFile );
	}

	AnyValue ClientConfiguration::Find( const std::string& section, const std::string& key )
	{
		AnyValue result;

		for( DefaultPropertyList::iterator i = m_defaultPropertyList.begin( ); i != m_defaultPropertyList.end( ); ++i )
		{
			if ( ( *i ).first == key )
			{
				result = m_configFile->FindConfigItem( section, key, ( *i ).second );
			}
		}

		return result;
	}

	void ClientConfiguration::Set( const std::string& section, const std::string& key, const AnyValue& value )
	{
		m_configFile->Update( section, key, value );
		m_configFile->Save( "config/game.cfg" );
	}

	void ClientConfiguration::SetDefault( const std::string& section, const std::string& key, const AnyValue& value )
	{
		m_defaultPropertyList[ key ] = value;
	}
}
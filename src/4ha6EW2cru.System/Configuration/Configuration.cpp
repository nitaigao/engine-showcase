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

	AnyType ClientConfiguration::Find( const std::string& section, const std::string& key )
	{
		AnyType result;

		for( DefaultPropertyList::iterator i = m_defaultPropertyList.begin( ); i != m_defaultPropertyList.end( ); ++i )
		{	
			if ( ( *i ).first == section && ( *i ).second.first == key )
			{
				result = m_configFile->FindConfigItem( section, key, ( *i ).second.second );
			}
		}

		return result;
	}

	void ClientConfiguration::Set( const std::string& section, const std::string& key, const AnyType& value )
	{
		m_configFile->Update( section, key, value );
		m_configFile->Save( );
	}

	void ClientConfiguration::SetDefault( const std::string& section, const std::string& key, const AnyType& value )
	{
		bool found = false; 

		for( DefaultPropertyList::iterator i = m_defaultPropertyList.begin( ); i != m_defaultPropertyList.end( ); ++i )
		{
			if ( ( *i ).first == section )
			{
				if ( ( *i ).second.first == key )
				{
					( *i ).second.second = value;
					found = true;
				}
			}
		}

		if ( !found )
		{
			KeyValuePair keyValue = std::make_pair( key, value );
			m_defaultPropertyList.insert( std::make_pair( section, keyValue ) );
		}
	}

	AnyType::AnyTypeMap ClientConfiguration::FindSection( const std::string& section )
	{
		AnyType::AnyTypeMap results;

		for( DefaultPropertyList::iterator i = m_defaultPropertyList.begin( ); i != m_defaultPropertyList.end( ); ++i )
		{
			if ( ( *i ).first == section )
			{
				results[ ( *i ).second.first ] = m_configFile->FindConfigItem( section, ( *i ).second.first, ( *i ).second.second );
			}
		}

		return results;
	}
}
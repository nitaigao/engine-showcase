#ifndef __CONFIGURATIONFILE_H
#define __CONFIGURATIONFILE_H

#include <sstream>

#include "../io/FileManager.h"
#include "../io/FileBuffer.hpp"

#include "../Utility/SimpleIni.h"

class ConfigurationFile
{

public:

	ConfigurationFile( const FileBuffer* fileBuffer )
	{
		_ini = new CSimpleIni( true );
		SI_Error error = _ini->Load( fileBuffer->fileBytes );
	}

	~ConfigurationFile( )
	{
		if( _ini != 0 )
		{
			delete _ini;
		}
	}

	static ConfigurationFile* Load( const std::string& filePath )
	{
		FileBuffer* buffer = FileManager::GetInstance( )->GetFile( filePath );
		ConfigurationFile* configFile = new ConfigurationFile( buffer );
		delete buffer;
		return configFile;
	}

	int FindConfigItem( const std::string& section, const std::string& key, const int& defaultValue )
	{
		return _ini->GetLongValue( section.c_str( ), key.c_str( ), defaultValue );
	}

	std::string FindConfigItem( const std::string& section, const std::string& key, const std::string& defaultValue )
	{
		return _ini->GetValue( section.c_str( ), key.c_str( ), defaultValue.c_str( ) );
	}

	bool FindConfigItem( const std::string& section, const std::string& key, const bool& defaultValue )
	{
		return _ini->GetBoolValue( section.c_str( ), key.c_str( ), defaultValue );
	}

	void Update( const std::string& section, const std::string& key, const std::string& value )
	{
		_ini->SetValue( section.c_str( ), key.c_str( ), value.c_str( ) );
	}

	void Update( const std::string& section, const std::string& key, const int& value )
	{
		_ini->SetLongValue( section.c_str( ), key.c_str( ), value );
	}

	void Update( const std::string& section, const std::string& key, const bool& value )
	{
		_ini->SetBoolValue( section.c_str( ), key.c_str( ), value );
	}

	void Save( const std::string& filePath )
	{
		std::string output;
		_ini->Save( output );

		char* outputBuffer = new char[ output.length( ) + 1 ];
		memcpy( outputBuffer, output.c_str( ), output.length( ) );
		outputBuffer[ output.length( ) ] = '\0';

		FileBuffer fileBuffer( outputBuffer, output.length( ), filePath );
		FileManager::GetInstance( )->SaveFile( fileBuffer );
	}

private:

	CSimpleIni* _ini;

};

#endif

#ifndef __ICONFIGURATION_H
#define __ICONFIGURATION_H

#include <string>

class IConfiguration
{

public:

	virtual ~IConfiguration( ) { };

	virtual int Find( const std::string& section, const std::string& key, const int& defaultValue ) = 0;
	virtual std::string Find( const std::string& section, const std::string& key, const std::string& defaultValue ) = 0;
	virtual bool Find( const std::string& section, const std::string& key, const bool& defaultValue ) = 0;
	virtual void Set( const std::string& section, const std::string& key, const std::string& value ) = 0;
	virtual void Set( const std::string& section, const std::string& key, const int& value )  = 0;
	virtual void Set( const std::string& section, const std::string& key, const bool& value ) = 0;

};

#endif
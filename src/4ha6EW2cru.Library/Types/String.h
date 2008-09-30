#ifndef __STRING_H
#define __STRING_H

#include <Ogre.h>
#include <string>

class String : public std::string
{

public:

	String( std::string input )
		: std::string( input )
	{

	}

	inline Ogre::UTFString ToUTFString( ) { return Ogre::UTFString( this->c_str( ) ); };
	inline std::string ToString( ) { return std::string( this->c_str( ) ); };

	inline std::string operator + ( std::string& input )
	{
		std::stringstream output;
		output << this << input;
		return output.str( );
	}

private:

	String( const String & copy ) { };
	String & operator = ( const String & copy ) { return *this; };

};

#endif
/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   c:\Users\NK\Development\viper\src\4ha6EW2cru.Library\Exceptions\ScriptException.hpp
*  @date   2009/04/25
*/
#ifndef __SCRIPTEXCEPTION_H
#define __SCRIPTEXCEPTION_H

/*! 
*  An Exception for when an error occurs inside a Script
*/
class ScriptException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~ScriptException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	ScriptException( )
		: std::exception( "A Script Error has occured" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	ScriptException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	ScriptException( const ScriptException & copy ) { };
	ScriptException & operator = ( const ScriptException & copy ) { return *this; };

};

#endif

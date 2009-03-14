#ifndef __SCRIPTEXCEPTION_H
#define __SCRIPTEXCEPTION_H

class ScriptException : public std::exception
{

public:

	~ScriptException( ) { };

	ScriptException( )
		: std::exception( "A Script Error has occured" )
	{

	}

	ScriptException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	ScriptException( const ScriptException & copy ) { };
	ScriptException & operator = ( const ScriptException & copy ) { return *this; };

};

#endif

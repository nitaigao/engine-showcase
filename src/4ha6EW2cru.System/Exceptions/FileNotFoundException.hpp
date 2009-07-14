/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   FileNotFoundException.hpp
*  @date   2009/04/25
*/
#ifndef FILENOTFOUNDEXCEPTION
#define FILENOTFOUNDEXCEPTION

#include <string>

/*! 
 *  An Exception for when a file cannot be located on the File System
 */
class FileNotFoundException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~FileNotFoundException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	FileNotFoundException( )
		: std::exception( "There was an error attempting to open an File" )
	{

	}

	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	FileNotFoundException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	FileNotFoundException( const FileNotFoundException & copy ) { };
	FileNotFoundException & operator = ( const FileNotFoundException & copy ) { return *this; };


};

#endif

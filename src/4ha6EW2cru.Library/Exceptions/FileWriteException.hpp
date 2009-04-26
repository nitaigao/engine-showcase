/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   c:\Users\NK\Development\viper\src\4ha6EW2cru.Library\Exceptions\FileWriteException.hpp
*  @date   2009/04/25
*/
#ifndef __FILEACCESSEXCEPTION_H
#define __FILEACCESSEXCEPTION_H

class FileWriteException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~FileWriteException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	FileWriteException( )
		: std::exception( "There was an error attempting to write to a file" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	FileWriteException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	FileWriteException( const FileWriteException & copy ) { };
	FileWriteException & operator = ( const FileWriteException & copy ) { return *this; };

};

#endif

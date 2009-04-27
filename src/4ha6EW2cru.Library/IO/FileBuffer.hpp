/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   FileBuffer.hpp
*  @date   2009/04/26
*/
#ifndef __FILEBUFFER_H
#define __FILEBUFFER_H

#include <string>

namespace IO
{
	/*! 
	 *  A container for file data loaded from the file system
	 */
	struct FileBuffer
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~FileBuffer( )
		{
			if( fileBytes != 0 )
			{
				delete fileBytes;
			}
		}

		/*! Default Constructor
		 *
		 *  @param[in] char * fileBytes
		 *  @param[in] const int & fileLength
		 *  @param[in] const std::string & filePath
		 *  @return ()
		 */
		FileBuffer( char* fileBytes, const int& fileLength, const std::string& filePath )
			: fileBytes( fileBytes )
			, fileLength( fileLength )
			, filePath( filePath )
		{

		}


		int fileLength;
		const std::string filePath;
		char* fileBytes;

	private:

		FileBuffer( const FileBuffer & copy ) { };
		FileBuffer & operator = ( const FileBuffer & copy ) { return *this; };

	};
};

#endif

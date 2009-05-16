/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IFileSystem.hpp
*  @date   2009/04/26
*/
#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "FileBuffer.hpp"
#include "FileSearchResult.hpp"

namespace IO
	{
	/*! 
	 *  An Abstract representation of the Game File System
	 */
	class IFileSystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IFileSystem( ) { };


		/*! Initializes the File System
		 *
		 *  @return (void)
		 */
		virtual void Initialize( ) = 0;


		/*! Mounts the given path into the file system
		 *
		 *  @param[in] const std::string path - can be a ZIP, BAD, PAK Archive or a Local Folder
		 *  @param[in] const std::string mountPoint - the internal path the above will be mounted at
		 *  @return (bool)
		 */
		virtual bool Mount( const std::string& path, const std::string& mountPoint ) = 0;


		/*! Returns the file data at the given path
		 *
		 *  @param[in] const std::string filePath
		 *  @param[in] bool binary
		 *  @return (FileBuffer*)
		 */
		virtual FileBuffer* GetFile( const std::string& filePath, const bool& binary ) const = 0;


		/*! Saves the given file data to the path specified in the FileBuffer
		 *
		 *  @param[in] const FileBuffer & fileBuffer
		 *  @return (void)
		 */
		virtual void SaveFile( const FileBuffer& fileBuffer ) const = 0;


		/*! Checks to see if a file exists at the given path
		 *
		 *  @param[in] const std::string filePath
		 *  @param[in] bool throwOnFail
		 *  @return (bool)
		 */
		virtual bool FileExists( const std::string& filePath, const bool& throwOnFail = false ) const = 0;


		/*! Performs a file search
		 *
		 *  @param[in] const std::string path
		 *  @param[in] const std::string searchPattern - wild card operator is *, so '/*.gif', '*.*', and '*' are all valid
		 *  @param[in] const bool recursive
		 *  @return (FileSearchResultList*)
		 */
		virtual FileSearchResult::FileSearchResultList* FileSearch( const std::string& path, const std::string& searchPattern, const bool& recursive ) const = 0;

	};
};

#endif
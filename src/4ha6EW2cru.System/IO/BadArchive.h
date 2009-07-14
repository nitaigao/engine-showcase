/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   BadArchive.h
*  @date   2009/04/26
*/
#ifndef BADARCHIVE_H
#define BADARCHIVE_H

#include "OgreArchive.h"
#include "FileSearchResult.hpp"

namespace IO
{
	/*! 
	 *  A file source to map the games file system to Ogre's Resource Manager
	 */
	class BadArchive : public Ogre::Archive
	{
		typedef std::map< std::string, FileSearchResult > TableOfContents;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~BadArchive( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string name
		 *  @param[in] const std::string type
		 *  @return ()
		 */
		BadArchive( const std::string& name, const std::string& type )
			: Archive( name, type )
		{

		}


		/*! Returns whether or not the File System is case sensitive
		 *
		 *  @param[in] void
		 *  @return (bool)
		 */
		inline bool isCaseSensitive( void ) const { return true; };


		/*! Returns the last modified time of the Archive
		 *  0 in our case as we are mapping straight to the
		 *  internal file system
		 *
		 *  @param[in] const Ogre::String & filename
		 *  @return (time_t)
		 */
		time_t getModifiedTime( const Ogre::String& filename ) { return 0; }; 


		/*! Stub that would initialize the archive
		 *
		 *  @return (void)
		 */
		inline void load( ) { };


		/*! Stub that would unload the archive
		 *
		 *  @return (void)
		 */
		inline void unload( ) { };


		/*! Opens a file from the file system
		 *
		 *  @param[in] const Ogre::String & filename
		 *  @return (Ogre::DataStreamPtr)
		 */
		Ogre::DataStreamPtr open( const Ogre::String& filename ) const;


		/*! Checks to see if the file exists on the file system
		 *
		 *  @param[in] const Ogre::String & filename
		 *  @return (bool)
		 */
		bool exists( const Ogre::String& filename );


		/*! Returns a list of files contained on the file system
		 *
		 *  @param[in] bool recursive
		 *  @param[in] bool dirs
		 *  @return (Ogre::StringVectorPtr)
		 */
		Ogre::StringVectorPtr list( bool recursive = true, bool dirs = false );


		/*! Returns a list of files with all accompanying meta data attached( path, extension etc )
		 *
		 *  @param[in] bool recursive
		 *  @param[in] bool dirs
		 *  @return (Ogre::FileInfoListPtr)
		 */
		Ogre::FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false );


		/*! Finds a file on the file system
		 *
		 *  @param[in] const Ogre::String & pattern
		 *  @param[in] bool recursive
		 *  @param[in] bool dirs
		 *  @return (Ogre::StringVectorPtr)
		 */
		Ogre::StringVectorPtr find( const Ogre::String& pattern, bool recursive = true, bool dirs = false );


		/*! Finds a file with its accompanying meta data attached ( path, extension etc )
		 *
		 *  @param[in] const Ogre::String & pattern
		 *  @param[in] bool recursive
		 *  @param[in] bool dirs
		 *  @return (Ogre::FileInfoListPtr)
		 */
		Ogre::FileInfoListPtr findFileInfo( const Ogre::String& pattern, bool recursive = true, bool dirs = false );

	private:

		TableOfContents m_tableOfContents;

		BadArchive & operator = ( const BadArchive & copy ) { return *this; };

	};
};

#endif

#ifndef __BADARCHIVE_H
#define __BADARCHIVE_H

#include "OgreArchive.h"
#include "FileSearchResult.hpp"

class BadArchive : public Ogre::Archive
{
	typedef std::map< std::string, FileSearchResult > TableOfContents;

public:

	virtual ~BadArchive( ) { };

	BadArchive( const std::string name, const std::string type )
		: Archive( name, type )
	{

	}

	inline bool isCaseSensitive( void ) const { return true; };

	time_t getModifiedTime( const Ogre::String& filename ) { return 0; }; 

	inline void load( ) { };
	inline void unload( ) { };

	Ogre::DataStreamPtr open( const Ogre::String& filename ) const;
	bool exists( const Ogre::String& filename );

	Ogre::StringVectorPtr list( bool recursive = true, bool dirs = false );
	Ogre::FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false );

	Ogre::StringVectorPtr find( const Ogre::String& pattern, bool recursive = true, bool dirs = false );
	Ogre::FileInfoListPtr findFileInfo( const Ogre::String& pattern, bool recursive = true, bool dirs = false );

private:

	TableOfContents _tableOfContents;

	BadArchive & operator = ( const BadArchive & copy ) { return *this; };

};

#endif

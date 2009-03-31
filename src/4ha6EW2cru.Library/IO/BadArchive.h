#ifndef __BADARCHIVE_H
#define __BADARCHIVE_H

#include "OgreArchive.h"

using namespace Ogre;

class BadArchive : public Ogre::Archive
{

public:

	virtual ~BadArchive( ) { };

	BadArchive( const std::string name, const std::string type )
		: Archive( name, type )
	{

	}

	inline bool isCaseSensitive( void ) const { return true; };

	time_t getModifiedTime( const String& filename ) { return 0; }; 

	inline void load( ) { };
	inline void unload( ) { };

	DataStreamPtr open( const String& filename ) const;
	bool exists( const String& filename );

	StringVectorPtr list( bool recursive = true, bool dirs = false );
	FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false );

	StringVectorPtr find( const String& pattern, bool recursive = true, bool dirs = false );
	FileInfoListPtr findFileInfo( const String& pattern, bool recursive = true, bool dirs = false );

private:

	BadArchive & operator = ( const BadArchive & copy ) { return *this; };

};

#endif

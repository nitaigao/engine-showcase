#ifndef __BADARCHIVE_H
#define __BADARCHIVE_H

#include "OgreArchive.h"

using namespace Ogre;

class BadArchive : public Ogre::Archive
{

	public:

		BadArchive( const std::string name, const std::string type );
		~BadArchive( ) { };

		inline bool isCaseSensitive( void ) const { return true; };

		inline void load( ) { };
		inline void unload( ) { };

		DataStreamPtr open( const String& filename ) const;
		bool exists( const String& filename );

		StringVectorPtr list( bool recursive = true, bool dirs = false ) { return StringVectorPtr( ); };
		FileInfoListPtr listFileInfo( bool recursive = true, bool dirs = false ) { return FileInfoListPtr( ); };

		StringVectorPtr find( const String& pattern, bool recursive = true, bool dirs = false );
		FileInfoListPtr findFileInfo( const String& pattern, bool recursive = true, bool dirs = false );

};

#endif

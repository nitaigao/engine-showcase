#ifndef __BADARCHIVEFACTORY_H
#define __BADARCHIVEFACTORY_H

#include <string>

#include "OgreArchiveFactory.h"
#include "OgreArchive.h"

class BadArchiveFactory : public Ogre::ArchiveFactory
{

public:

	virtual ~BadArchiveFactory( ) { };
	BadArchiveFactory( ) { };

	const Ogre::String& getType( ) const;

	Ogre::Archive* createInstance( const Ogre::String& name ); 

	void destroyInstance( Ogre::Archive* );  

	static const std::string BAD_ARCHTYPE;

private:

	BadArchiveFactory( const BadArchiveFactory & copy ) { };
	BadArchiveFactory & operator = ( const BadArchiveFactory & copy ) { return *this; };

};

#endif
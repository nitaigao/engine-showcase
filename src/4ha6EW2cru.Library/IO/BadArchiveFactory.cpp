#include "BadArchiveFactory.h"
#include "BadArchive.h"
#include "../Logging/Logger.h"

const std::string BadArchiveFactory::BAD_ARCHTYPE = "BAD";

const Ogre::String& BadArchiveFactory::getType( ) const
{
	return BAD_ARCHTYPE;
}

Ogre::Archive* BadArchiveFactory::createInstance( const Ogre::String& name )
{
	std::stringstream logMessage;
	logMessage << "BadArchiveFactory: Creating Instance of " << name;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	return new BadArchive( name, BAD_ARCHTYPE );
}

void BadArchiveFactory::destroyInstance( Ogre::Archive* archive )
{
	std::stringstream logMessage;
	logMessage << "BadArchiveFactory: Destroying Instance of " << archive->getName( );
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	delete archive;
}
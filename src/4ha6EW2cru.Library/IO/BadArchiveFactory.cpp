#include "BadArchiveFactory.h"
#include "BadArchive.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Management/Management.h"
#include "../Exceptions/NullReferenceException.hpp"

namespace IO
{
	static const std::string BAD_ARCHTYPE = "BAD";

	const Ogre::String& BadArchiveFactory::getType( ) const
	{
		return BAD_ARCHTYPE;
	}

	Ogre::Archive* BadArchiveFactory::createInstance( const Ogre::String& name )
	{
		std::stringstream logMessage;
		logMessage << "BadArchiveFactory::createInstance - of " << name;
		Logger::Debug( logMessage.str( ) );

		std::string archiveName = "";

		int lastIndexOfPath = name.rfind( '/' );
		if( lastIndexOfPath != name.npos )
		{
			archiveName = name.substr( lastIndexOfPath + 1, name.length( ) - lastIndexOfPath );
		}

		int extensionIndex = archiveName.find( '.' );
		if ( extensionIndex != archiveName.npos )
		{
			archiveName.replace( extensionIndex, archiveName.length( ) - extensionIndex, "" );
		}

		Management::GetInstance( )->GetFileManager( )->Mount( name, "/data" );

		return new BadArchive( archiveName, BAD_ARCHTYPE );
	}

	void BadArchiveFactory::destroyInstance( Ogre::Archive* archive )
	{
		if ( 0 == archive )
		{
			NullReferenceException e( "BadArchiveFactory::destroyInstance -- Archive is NULL" );
			Logger::Fatal( e.what( ) );
			throw e;
		}

		std::stringstream logMessage;
		logMessage << "BadArchiveFactory::destroyInstance - Instance of " << archive->getName( );
		Logger::Debug( logMessage.str( ) );

		delete archive;
	}
}
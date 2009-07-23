#include "OgreLogListener.h"

#include "Logging/Logger.h"
using namespace Logging;

namespace Renderer
{
	void OgreLogListener::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName )
	{
		std::stringstream logMessage;
		logMessage << logName << " " << message;

		switch( lml )
		{

		case Ogre::LML_TRIVIAL: 
			Debug( logMessage.str( ) );
			break;

		case Ogre::LML_NORMAL: 
			Info( logMessage.str( ) );
			break;

		case Ogre::LML_CRITICAL:
			Fatal( logMessage.str( ) );
			break;
		}

	}

}
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <map>
#include <sstream>

#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

#include "ConfigurationFile.h"

/*! Loads and contains game specific configuration info */
class Configuration
{

public:

	/* -- Graphics -- */

	inline bool IsFullScreen( ) const { return _configFile->FindConfigItem( "Graphics", "fullscreen", true ); };
	inline void SetFullScreen( bool isFullScreen ) { _configFile->Update( "Graphics", "fullscreen", isFullScreen );  };

	inline int GetDisplayWidth ( ) const { return _configFile->FindConfigItem( "Graphics", "width", 640 ); };
	inline void SetDisplayWidth( int width ) { _configFile->Update( "Graphics", "width", width );  };

	inline int GetDisplayHeight ( ) const { return _configFile->FindConfigItem( "Graphics", "height", 480 ); };
	inline void SetDisplayHeight( int height ) { _configFile->Update( "Graphics", "height", height );  };

	inline int GetColorDepth( ) const { return _configFile->FindConfigItem( "Graphics", "depth", 32 ); };

	/* -- Logging -- */

	inline int GetLoggingLevel( ) const { return _configFile->FindConfigItem( "Logging", "level", 0 ); };

	Configuration( )
	{
		_configFile = ConfigurationFile::Load( "config/game.cfg" );
	};

	virtual ~Configuration( )
	{
		delete _configFile;
	};

private:

	ConfigurationFile* _configFile;

};

#endif

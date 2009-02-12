#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"

#include "../Events/IEvent.hpp"

#include "ConfigurationFile.h"


/*! Loads and contains game specific configuration info */
class Configuration
{

	typedef std::vector< std::string > VideoModeList;

public:

	virtual ~Configuration( );

	void Initialize( );

	static Configuration* Load( const std::string& filePath );

	/* -- Graphics -- */

	inline bool IsFullScreen( ) const { return _configFile->FindConfigItem( "Graphics", "fullscreen", true ); };
	inline void SetFullScreen( bool isFullScreen ) { _configFile->Update( "Graphics", "fullscreen", isFullScreen );  };

	inline int GetDisplayWidth ( ) const { return _configFile->FindConfigItem( "Graphics", "width", 640 ); };
	inline void SetDisplayWidth( int width ) { _configFile->Update( "Graphics", "width", width );  };

	inline int GetDisplayHeight ( ) const { return _configFile->FindConfigItem( "Graphics", "height", 480 ); };
	inline void SetDisplayHeight( int height ) { _configFile->Update( "Graphics", "height", height );  };

	inline int GetColorDepth( ) const { return _configFile->FindConfigItem( "Graphics", "depth", 32 ); };

	inline const VideoModeList& GetAvailableVideoModes( ) const { return _availableVideoModes; }
	inline void SetAvailableVideoModes( VideoModeList modes ) { _availableVideoModes.assign( modes.begin( ), modes.end( ) ); };

	/* -- Logging -- */

	inline int GetLoggingLevel( ) const { return _configFile->FindConfigItem( "Logging", "level", 0 ); };

<<<<<<< HEAD:src/4ha6EW2cru.Library/System/Configuration.h
	Configuration( )
	{
		_configFile = ConfigurationFile::Load( "config/game.cfg" );
	};

	virtual ~Configuration( )
=======
private:

	Configuration( const Configuration & copy ) { };
	Configuration & operator = ( const Configuration & copy ) { return *this; };

	Configuration( ConfigurationFile* configFile )
		: _configFile( configFile )
>>>>>>> 38f8ee945f436c59dbb52a349727ae99b73fc8f5:src/4ha6EW2cru.Library/System/Configuration.h
	{

	};

	void OnConfigurationUpdated( const IEvent* event );

	ConfigurationFile* _configFile;
<<<<<<< HEAD:src/4ha6EW2cru.Library/System/Configuration.h

=======
	VideoModeList _availableVideoModes;
	
>>>>>>> 38f8ee945f436c59dbb52a349727ae99b73fc8f5:src/4ha6EW2cru.Library/System/Configuration.h
};

#endif

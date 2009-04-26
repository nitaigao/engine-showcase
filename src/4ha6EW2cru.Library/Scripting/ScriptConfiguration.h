#ifndef __SCRIPTCONFIGURATION_H
#define __SCRIPTCONFIGURATION_H

#include "IScriptConfiguration.hpp"

#include "../Management/Management.h"
#include "../Configuration/IConfiguration.hpp"
#include "../System/SystemType.hpp"

class ScriptConfiguration : public IScriptConfiguration
{

	typedef std::vector< std::string > StringList;

public:

	virtual ~ScriptConfiguration( ) { };

	ScriptConfiguration( Configuration::IConfiguration* configuration )
		: _configuration( configuration )
	{

	};

	inline bool IsFullScreen( ) const { return _configuration->Find( "Graphics", "fullscreen" ).GetValue< bool >( ); };
	inline void SetFullScreen( bool isFullScreen ) { _configuration->Set( "Graphics", "fullscreen", isFullScreen );  };

	inline int GetDisplayWidth ( ) const { return _configuration->Find( "Graphics", "width" ).GetValue< int >( ); };
	inline void SetDisplayWidth( int width ) { _configuration->Set( "Graphics", "width", width );  };

	inline int GetDisplayHeight ( ) const { return  _configuration->Find( "Graphics", "height" ).GetValue< int >( ); };
	inline void SetDisplayHeight( int height ) { _configuration->Set( "Graphics", "height", height );  };

	inline int GetColorDepth( ) const { return _configuration->Find( "Graphics", "depth" ).GetValue< int >( ); };

	inline bool IsConsole( ) const { return _configuration->Find( "Developer", "console" ).GetValue< bool >( ); };
	inline void SetConsole( bool isConsole ) { _configuration->Set( "Developer", "console", isConsole ); };

private:

	Configuration::IConfiguration* _configuration;

	ScriptConfiguration( const ScriptConfiguration & copy ) { };
	ScriptConfiguration & operator = ( const ScriptConfiguration & copy ) { return *this; };

};

#endif
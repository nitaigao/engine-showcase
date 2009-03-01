#ifndef __SCRIPTCONFIGURATION_H
#define __SCRIPTCONFIGURATION_H

#include "IScriptConfiguration.hpp"

#include "../System/Configuration.h"
#include "../System/Management.h"
#include "../System/SystemType.hpp"

class ScriptConfiguration : public IScriptConfiguration
{

	typedef std::vector< std::string > StringList;

public:

	ScriptConfiguration( Configuration* configuration )
		: _configuration( configuration )
	{

	};

	inline bool IsFullScreen( ) const { return _configuration->Find< bool >( "Graphics", "fullscreen" ); };
	inline void SetFullScreen( bool isFullScreen ) { _configuration->Set( "Graphics", "fullscreen", isFullScreen );  };

	inline int GetDisplayWidth ( ) const { return _configuration->Find< int >( "Graphics", "width" ); };
	inline void SetDisplayWidth( int width ) { _configuration->Set( "Graphics", "width", width );  };

	inline int GetDisplayHeight ( ) const { return  _configuration->Find< int >( "Graphics", "height" ); };
	inline void SetDisplayHeight( int height ) { _configuration->Set( "Graphics", "height", height );  };

	inline int GetColorDepth( ) const { return _configuration->Find< int >( "Graphics", "depth" ); };

	inline const std::vector< std::string >& GetAvailableVideoModes( ) const
	{ 
		return *Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType )->GetProperties( )[ "availableVideoModes" ].GetValue< std::vector< std::string >* >( );
	};

	inline void SetAvailableVideoModes( StringList videoModes ) { };

private:

	Configuration* _configuration;

};

#endif
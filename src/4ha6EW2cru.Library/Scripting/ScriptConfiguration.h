/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptConfiguration.h
*  @date   2009/04/27
*/
#ifndef __SCRIPTCONFIGURATION_H
#define __SCRIPTCONFIGURATION_H

#include "../Configuration/IConfiguration.hpp"
#include "IScriptConfiguration.hpp"

namespace Script
{
	/*!
	 *  A class mapping Game Configuration to LUA Script 
	 */
	class ScriptConfiguration : public IScriptConfiguration
	{

		typedef std::vector< std::string > StringList;

	public:


		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptConfiguration( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] Configuration::IConfiguration * configuration
		 *  @return ()
		 */
		ScriptConfiguration( Configuration::IConfiguration* configuration )
			: _configuration( configuration )
		{

		};


		/*! Returns whether or not the Game is in FullScreen
		*
		*  @return (bool)
		*/
		inline bool IsFullScreen( ) const { return _configuration->Find( "Graphics", "fullscreen" ).GetValue< bool >( ); };
		
		
		/*! Sets whether the game is in Full Screen
		*
		*  @param[in] bool isFullScreen
		*  @return (void)
		*/
		inline void SetFullScreen( bool isFullScreen ) { _configuration->Set( "Graphics", "fullscreen", isFullScreen );  };


		/*! Returns the Game windows Width
		*
		*  @return (int)
		*/
		inline int GetDisplayWidth ( ) const { return _configuration->Find( "Graphics", "width" ).GetValue< int >( ); };
		
		
		/*! Sets the Game window Width
		*
		*  @param[in] int width
		*  @return (void)
		*/
		inline void SetDisplayWidth( int width ) { _configuration->Set( "Graphics", "width", width );  };


		/*! Returns the Game window Height
		*
		*  @return (int)
		*/
		inline int GetDisplayHeight ( ) const { return  _configuration->Find( "Graphics", "height" ).GetValue< int >( ); };
		
		
		/*! Sets the Game window Height
		*
		*  @param[in] int height
		*  @return (void)
		*/
		inline void SetDisplayHeight( int height ) { _configuration->Set( "Graphics", "height", height );  };


		/*! Returns the Game window Color Depth
		*
		*  @return (int)
		*/
		inline int GetColorDepth( ) const { return _configuration->Find( "Graphics", "depth" ).GetValue< int >( ); };


		/*! Returns whether the Developer Console is Enabled
		*
		*  @return (bool)
		*/
		inline bool IsConsole( ) const { return _configuration->Find( "Developer", "console" ).GetValue< bool >( ); };
		
		
		/*! Sets whether the Developer Console is Enabled
		*
		*  @param[in] bool isConsole
		*  @return (void)
		*/
		inline void SetConsole( bool isConsole ) { _configuration->Set( "Developer", "console", isConsole ); };

	private:

		ScriptConfiguration( const ScriptConfiguration & copy ) { };
		ScriptConfiguration & operator = ( const ScriptConfiguration & copy ) { return *this; };

		Configuration::IConfiguration* _configuration;
	};
};

#endif
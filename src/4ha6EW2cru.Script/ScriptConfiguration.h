/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptConfiguration.h
*  @date   2009/04/27
*/
#ifndef SCRIPTCONFIGURATION_H
#define SCRIPTCONFIGURATION_H

#include "Configuration/IConfiguration.hpp"
#include "Configuration/ConfigurationTypes.hpp"

#include "IScriptConfiguration.hpp"

#include "System/SystemType.hpp"

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
			: m_configuration( configuration )
		{

		};


		/*! Returns whether or not the Game is in FullScreen
		*
		*  @return (bool)
		*/
		inline bool IsFullScreen( ) const { return m_configuration->Find( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::FullScreen ).As< bool >( ); };
		
		
		/*! Sets whether the game is in Full Screen
		*
		*  @param[in] bool isFullScreen
		*  @return (void)
		*/
		inline void SetFullScreen( bool isFullScreen ) { m_configuration->Set( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::FullScreen, isFullScreen );  };


		/*! Returns the Game windows Width
		*
		*  @return (int)
		*/
		inline int GetDisplayWidth ( ) const { return m_configuration->Find( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::Width ).As< int >( ); };
		
		
		/*! Sets the Game window Width
		*
		*  @param[in] int width
		*  @return (void)
		*/
		inline void SetDisplayWidth( int width ) { m_configuration->Set( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::Width, width );  };


		/*! Returns the Game window Height
		*
		*  @return (int)
		*/
		inline int GetDisplayHeight ( ) const { return  m_configuration->Find( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::Height ).As< int >( ); };
		
		
		/*! Sets the Game window Height
		*
		*  @param[in] int height
		*  @return (void)
		*/
		inline void SetDisplayHeight( int height ) { m_configuration->Set( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::Height, height );  };


		/*! Returns the Game window Color Depth
		*
		*  @return (int)
		*/
		inline int GetColorDepth( ) const { return m_configuration->Find( Configuration::ConfigSections::Graphics, Configuration::ConfigItems::Graphics::Depth ).As< int >( ); };


		/*! Returns whether the Developer Console is Enabled
		*
		*  @return (bool)
		*/
		inline bool IsConsole( ) const { return m_configuration->Find( Configuration::ConfigSections::Developer, Configuration::ConfigItems::Developer::Console ).As< bool >( ); };
		
		
		/*! Sets whether the Developer Console is Enabled
		*
		*  @param[in] bool isConsole
		*  @return (void)
		*/
		inline void SetConsole( bool isConsole ) { m_configuration->Set( Configuration::ConfigSections::Developer, Configuration::ConfigItems::Developer::Console, isConsole ); };


		/*! Gets whether the Mouse has an inverted Y axis
		*
		* @return ( bool )
		*/
		inline bool IsInvertY( ) const { return m_configuration->Find( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::InvertY ).As< bool >( ); };


		/*! Sets whether the Mouse has an inverted Y axis
		*
		* @param[in] bool invertY
		* @return ( void )
		*/
		inline void SetInvertY( bool invertY ) { m_configuration->Set( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::InvertY, invertY ); };


		/*! Gets whether the Mouse is smoothed
		*
		* @return ( bool )
		*/
		inline bool IsSmoothMouse( ) const { return m_configuration->Find( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::SmoothMouse ).As< bool >( ); };


		/*! Sets whether the Mouse is smoothed
		*
		* @param[in] bool smoothMouse
		* @return ( void )
		*/
		inline void SetSmoothMouse( bool smoothMouse ) { m_configuration->Set( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::SmoothMouse, smoothMouse ); };


		/*! Gets the amount of Mouse Smoothing used
		*
		* @return ( int )
		*/
		inline int GetMouseSmoothAmount( ) const { return m_configuration->Find( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::MouseSmoothAmount ).As< int >( ); };


		/*! Sets the amount of mouse smoothing used
		*
		* @param[in] const int & amount
		* @return ( void )
		*/
		inline void SetMouseSmoothAmount( const int& amount ) { m_configuration->Set( Configuration::ConfigSections::Input, Configuration::ConfigItems::Input::MouseSmoothAmount, amount ); };


		/*! Gets the SFX Volume
		*
		* @return ( int )
		*/
		inline int GetSFXVolume( ) const { return m_configuration->Find( Configuration::ConfigSections::Sound, Configuration::ConfigItems::Sound::SFXVolume ).As< int >( ); };


		/*! Sets the SFX Volume
		*
		* @param[in] const int & volume
		* @return ( void )
		*/
		inline void SetSFXVolume( const int& volume ) { m_configuration->Set( Configuration::ConfigSections::Sound, Configuration::ConfigItems::Sound::SFXVolume, volume ); };


		/*! Gets the Music Volume
		*
		* @return ( int )
		*/
		inline int GetMusicVolume( ) const { return m_configuration->Find( Configuration::ConfigSections::Sound, Configuration::ConfigItems::Sound::MusicVolume ).As< int >( ); };


		/*! Sets the Music Volume
		*
		* @param[in] const int & volume
		* @return ( void )
		*/
		inline void SetMusicVolume( const int& volume ) { m_configuration->Set( Configuration::ConfigSections::Sound, Configuration::ConfigItems::Sound::MusicVolume, volume ); };

	private:

		ScriptConfiguration( const ScriptConfiguration & copy ) { };
		ScriptConfiguration & operator = ( const ScriptConfiguration & copy ) { return *this; };

		Configuration::IConfiguration* m_configuration;
	};
};

#endif
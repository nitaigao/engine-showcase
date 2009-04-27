/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Win32PlatformManager.h
*  @date   2009/04/26
*/
#ifndef __WIN32PLATFORMMANAGER_H
#define __WIN32PLATFORMMANAGER_H

#include "IPlatformManager.h"

namespace Platform
{
	/*! 
	*  Interfaces with the Windows Platform
	*/
	class Win32PlatformManager : public IPlatformManager
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~Win32PlatformManager( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		Win32PlatformManager( )
			: _hWnd( 0 )
		{

		}


		/*! Creates an Interactive Window for the Player
		*
		*  @param[in] const std::string & title
		*  @param[in] const int & width
		*  @param[in] const int & height
		*  @param[in] const bool & fullScreen
		*  @return (void)
		*/
		void CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen );
		

		/*! Closes the Open Window
		*
		*  @return (void)
		*/
		void CloseWindow( );


		/*! Creates a Console Window
		*
		*  @return (void)
		*/
		void CreateConsoleWindow( );


		/*! Steps internal data structures
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );
		

		/*! Gets the id of the active game window
		*
		*  @return (size_t)
		*/
		size_t GetWindowId( ) const;


		/*! Returns the time in milliseconds
		 *
		 *  @return (float)
		 */
		float GetTime( ) const;


		/*! Outputs a message to the Debug Console
		*
		*  @return (void)
		*/
		void OutputDebugMessage( const std::string& message );

	private:

		size_t _hWnd;

		Win32PlatformManager( const Win32PlatformManager & copy ) { };
		Win32PlatformManager & operator = ( const Win32PlatformManager & copy ) { return *this; };

	};
};

#endif
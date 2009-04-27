/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IPlatformManager.h
*  @date   2009/04/25
*/
#ifndef __IPLATFORMMANAGER_H
#define __IPLATFORMMANAGER_H

#include <string>

namespace Platform
{
	/*! 
	 *  Interfaces with Specific Api's on the Target Platform
	 */
	class IPlatformManager
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IPlatformManager( ) { };


		/*! Creates an Interactive Window for the Player
		 *
		 *  @param[in] const std::string & title
		 *  @param[in] const int & width
		 *  @param[in] const int & height
		 *  @param[in] const bool & fullScreen
		 *  @return (void)
		 */
		virtual void CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen ) = 0;


		/*! Closes the Open Window
		 *
		 *  @return (void)
		 */
		virtual void CloseWindow( ) = 0;


		/*! Creates a Console Window
		 *
		 *  @return (void)
		 */
		virtual void CreateConsoleWindow( ) = 0;


		/*! Steps internal data structures
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		virtual void Update( const float& deltaMilliseconds ) = 0;


		/*! Gets the Id of the active game window
		 *
		 *  @return (size_t)
		 */
		virtual size_t GetWindowId( ) const = 0;

		
		/*! Returns the current time in milliseconds
		 *
		 *  @return (float)
		 */
		virtual float GetTime( ) const = 0;


		/*! Outputs a message to the Debug Console
		 *
		 *  @return (void)
		 */
		virtual void OutputDebugMessage( const std::string& message ) = 0;
	};
};

#endif
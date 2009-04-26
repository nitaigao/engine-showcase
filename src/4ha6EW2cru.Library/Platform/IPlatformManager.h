/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\IPlatformManager.h
*  @date   2009/04/25
*/
#ifndef __IPLATFORMMANAGER_H
#define __IPLATFORMMANAGER_H

#include <string>
#include <windows.h>

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
	virtual void Update( float deltaMilliseconds ) = 0;


	//TODO: This is platform specific and we should prolly move this into an exposed properties interface
	/*! Gets the HWND of the active game window
	 *
	 *  @return (size_t)
	 */
	virtual size_t GetHwnd( ) = 0;

};

#endif
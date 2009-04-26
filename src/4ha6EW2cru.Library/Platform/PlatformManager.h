/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\PlatformManager.h
*  @date   2009/04/25
*/
#ifndef __PLATFORMMANAGER_H
#define __PLATFORMMANAGER_H

#include "IPlatformManager.h"

/*! 
*  Interfaces with Specific Api's on the Target Platform
*/
class PlatformManager : public IPlatformManager
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	virtual ~PlatformManager( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	PlatformManager( )
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
	void Update( float deltaMilliseconds );
	
	//TODO: This is platform specific and we should prolly move this into an exposed properties interface
	/*! Gets the HWND of the active game window
	*
	*  @return (size_t)
	*/
	inline size_t GetHwnd( )
	{ 
		if ( _hWnd == 0 )
		{
			return ( size_t ) GetConsoleWindow( );
		}

		return _hWnd; 
	};

private:

	size_t _hWnd;

	PlatformManager( const PlatformManager & copy ) { };
	PlatformManager & operator = ( const PlatformManager & copy ) { return *this; };

};

#endif
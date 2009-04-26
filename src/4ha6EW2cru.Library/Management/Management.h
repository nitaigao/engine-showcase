/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\Management.h
*  @date   2009/04/25
*/
#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H

#include "../Events/EventManager.h"

#include "../IO/IResourceManager.hpp"
#include "../Service/IServiceManager.h"
#include "../Platform/IPlatformManager.h"
#include "../System/ISystemManager.hpp"
#include "../IO/IFileManager.hpp"

/*! 
 *  An Interface to the Game Managers
 */
class Management
{

public:

	/*! Destroys all Game Managers
	 *
	 *  @return ()
	 */
	virtual ~Management( );


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	Management( );


	/*! Returns the Singleton Instance of Management
	 *
	 *  @return (Management*)
	 */
	static Management* GetInstance( );


	/*! Releases the Singleton Instance of Management
	 *
	 *  @return (void)
	 */
	void Release( );


	/*! Creates the Singleton Instance of Management
	 *
	 *  @return (void)
	 */
	static void Initialize( );


	/*! Steps the internal data of each Manager
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	void Update( float deltaMilliseconds );


	/*! Returns the System Manager
	 *
	 *  @return (ISystemManager*)
	 */
	inline ISystemManager* GetSystemManager( ) { return _systemManager; };


	/*! Returns the Event Manager
	 *
	 *  @return (EventManager*)
	 */
	inline Events::EventManager* GetEventManager( ) { return _eventManager; };


	/*! Returns the Platform Manager
	 *
	 *  @return (IPlatformManager*)
	 */
	inline IPlatformManager* GetPlatformManager( ) { return _platformManager; };


	/*! Returns the File Manager
	 *
	 *  @return (IFileManager*)
	 */
	inline IFileManager* GetFileManager( ) { return _fileManager; };


	/*! Returns the Service Manager
	 *
	 *  @return (IServiceManager*)
	 */
	inline IServiceManager* GetServiceManager( ) { return _serviceManager; };


	/*! Returns the Resources Manager
	 *
	 *  @return (Resources::IResourceManager*)
	 */
	inline Resources::IResourceManager* GetResourceManager( ) { return _resourceManager; };

private:

	IServiceManager* _serviceManager;
	ISystemManager* _systemManager;
	IPlatformManager* _platformManager;
	IFileManager* _fileManager;
	Resources::IResourceManager* _resourceManager;
	Events::EventManager* _eventManager;

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

};

#endif
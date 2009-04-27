/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Management.h
*  @date   2009/04/25
*/
#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H

#include "../Events/EventManager.h"

#include "../IO/IResourceCache.hpp"
#include "../Service/IServiceManager.h"
#include "../Platform/IPlatformManager.h"
#include "../System/ISystemManager.hpp"
#include "../IO/IFileSystem.hpp"

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
	~Management( );


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
	void Update( const float& deltaMilliseconds );


	/*! Returns the System Manager
	 *
	 *  @return (ISystemManager*)
	 */
	inline ISystemManager* GetSystemManager( ) const { return _systemManager; };


	/*! Returns the Event Manager
	 *
	 *  @return (EventManager*)
	 */
	inline Events::EventManager* GetEventManager( ) const { return _eventManager; };


	/*! Returns the Platform Manager
	 *
	 *  @return (IPlatformManager*)
	 */
	inline Platform::IPlatformManager* GetPlatformManager( ) const { return _platformManager; };


	/*! Returns the File Manager
	 *
	 *  @return (IFileSystem*)
	 */
	inline IO::IFileSystem* GetFileManager( ) const { return _fileSystem; };


	/*! Returns the Service Manager
	 *
	 *  @return (IServiceManager*)
	 */
	inline IServiceManager* GetServiceManager( ) const { return _serviceManager; };


	/*! Returns the Resources Manager
	 *
	 *  @return (Resources::IResourceCache*)
	 */
	inline Resources::IResourceCache* GetResourceManager( ) const { return _resourceCache; };

private:

	IServiceManager* _serviceManager;
	ISystemManager* _systemManager;
	Platform::IPlatformManager* _platformManager;
	IO::IFileSystem* _fileSystem;
	Resources::IResourceCache* _resourceCache;
	Events::EventManager* _eventManager;

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

};

#endif
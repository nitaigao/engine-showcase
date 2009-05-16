/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Management.h
*  @date   2009/04/25
*/
#ifndef MANAGEMENT_H
#define MANAGEMENT_H

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
	inline ISystemManager* GetSystemManager( ) const { return m_systemManager; };


	/*! Returns the Event Manager
	 *
	 *  @return (EventManager*)
	 */
	inline Events::EventManager* GetEventManager( ) const { return m_eventManager; };


	/*! Returns the Platform Manager
	 *
	 *  @return (IPlatformManager*)
	 */
	inline Platform::IPlatformManager* GetPlatformManager( ) const { return m_platformManager; };


	/*! Returns the File Manager
	 *
	 *  @return (IFileSystem*)
	 */
	inline IO::IFileSystem* GetFileManager( ) const { return m_fileSystem; };


	/*! Returns the Service Manager
	 *
	 *  @return (IServiceManager*)
	 */
	inline IServiceManager* GetServiceManager( ) const { return m_serviceManager; };


	/*! Returns the Resources Manager
	 *
	 *  @return (Resources::IResourceCache*)
	 */
	inline Resources::IResourceCache* GetResourceManager( ) const { return m_resourceCache; };

private:

	IServiceManager* m_serviceManager;
	ISystemManager* m_systemManager;
	Platform::IPlatformManager* m_platformManager;
	IO::IFileSystem* m_fileSystem;
	Resources::IResourceCache* m_resourceCache;
	Events::EventManager* m_eventManager;

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

};

#endif
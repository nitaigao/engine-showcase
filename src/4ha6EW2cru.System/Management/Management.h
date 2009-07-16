/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Management.h
*  @date   2009/04/25
*/
#pragma once
#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "../Export.hpp"

#include "../Events/EventManager.h"
#include "../IO/IResourceCache.hpp"
#include "../Service/IServiceManager.h"
#include "../Platform/IPlatformManager.h"
#include "../System/ISystemManager.hpp"
#include "../IO/IFileSystem.hpp"
#include "../System/IInstrumentation.hpp"

/*! 
 *  An Interface to the Game Managers
 */
class Management
{

public:

	/*! Releases the Singleton Instance of Management
	 *
	 *  @return (void)
	 */
	static GAMEAPI void Release( );


	/*! Creates the Singleton Instance of Management
	 *
	 *  @return (void)
	 */
	static GAMEAPI void Initialize( );

	
	/*! Initializes Management from another instance of the Singleton, for use accross dll boundaries
	*
	* @param[in] Management * management
	* @return ( void )
	*/
	static GAMEAPI void Initialize( Management* management );


	/*! Returns whether or not the Singleton has been initialized
	*
	* @return ( bool )
	*/
	static bool IsInitialized( );


	/*! Returns the Instance of the Management Singleton
	*
	* @return ( Management* )
	*/
	static GAMEAPI Management* Get( );


	/*! Steps the internal data of each Manager
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	GAMEAPI void Update( const float& deltaMilliseconds );


	/*! Returns the System Manager
	 *
	 *  @return (ISystemManager*)
	 */
	inline ISystemManager* GetSystemManager( ) { return m_systemManager; };


	/*! Returns the Event Manager
	 *
	 *  @return (EventManager*)
	 */
	inline Events::EventManager* GetEventManager( ) { return m_eventManager; };


	/*! Returns the Platform Manager
	 *
	 *  @return (IPlatformManager*)
	 */
	inline Platform::IPlatformManager* GetPlatformManager( ) { return m_platformManager; };


	/*! Returns the File Manager
	 *
	 *  @return (IFileSystem*)
	 */
	inline IO::IFileSystem* GetFileManager( ) { return m_fileSystem; };


	/*! Returns the Service Manager
	 *
	 *  @return (IServiceManager*)
	 */
	inline Services::IServiceManager* GetServiceManager( ) { return m_serviceManager; };


	/*! Returns the Resources Manager
	 *
	 *  @return (Resources::IResourceCache*)
	 */
	inline Resources::IResourceCache* GetResourceManager( ) { return m_resourceCache; };


	/*! Returns the GetInstrumentation Provider
	*
	* @return ( IInstrumentation* )
	*/
	inline IInstrumentation* GetInstrumentation( ) { return m_instrumentation; };

protected:

	~Management( );

	Management( );

private:

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

	Services::IServiceManager* m_serviceManager;
	ISystemManager* m_systemManager;
	Platform::IPlatformManager* m_platformManager;
	IO::IFileSystem* m_fileSystem;
	Resources::IResourceCache* m_resourceCache;
	Events::EventManager* m_eventManager;
	IInstrumentation* m_instrumentation;

};

#endif
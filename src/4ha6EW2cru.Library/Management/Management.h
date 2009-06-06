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
#include "../System/IInstrumentation.hpp"

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
	~Management( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	Management( ) { };


	/*! Releases the Singleton Instance of Management
	 *
	 *  @return (void)
	 */
	static void Release( );


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
	static void Update( const float& deltaMilliseconds );


	/*! Returns the System Manager
	 *
	 *  @return (ISystemManager*)
	 */
	static inline ISystemManager* GetSystemManager( ) { return m_systemManager; };


	/*! Returns the Event Manager
	 *
	 *  @return (EventManager*)
	 */
	static inline Events::EventManager* GetEventManager( ) { return m_eventManager; };


	/*! Returns the Platform Manager
	 *
	 *  @return (IPlatformManager*)
	 */
	static inline Platform::IPlatformManager* GetPlatformManager( ) { return m_platformManager; };


	/*! Returns the File Manager
	 *
	 *  @return (IFileSystem*)
	 */
	static inline IO::IFileSystem* GetFileManager( ) { return m_fileSystem; };


	/*! Returns the Service Manager
	 *
	 *  @return (IServiceManager*)
	 */
	static inline IServiceManager* GetServiceManager( ) { return m_serviceManager; };


	/*! Returns the Resources Manager
	 *
	 *  @return (Resources::IResourceCache*)
	 */
	static inline Resources::IResourceCache* GetResourceManager( ) { return m_resourceCache; };


	/*! Returns the Instrumentation Provider
	*
	* @return ( IInstrumentation* )
	*/
	static inline IInstrumentation* GetInstrumentation( ) { return m_instrumentation; };

private:

	Management( const Management & copy ) { };
	Management & operator = ( const Management & copy ) { return *this; };

	static IServiceManager* m_serviceManager;
	static ISystemManager* m_systemManager;
	static Platform::IPlatformManager* m_platformManager;
	static IO::IFileSystem* m_fileSystem;
	static Resources::IResourceCache* m_resourceCache;
	static Events::EventManager* m_eventManager;
	static IInstrumentation* m_instrumentation;

};

#endif
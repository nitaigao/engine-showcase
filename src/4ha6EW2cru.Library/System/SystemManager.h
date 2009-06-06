/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemManager.h
*  @date   2009/04/25
*/
#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "ISystemManager.hpp"
#include "../State/World.h"

/*! 
*  Manages each System of the Game
*/
class SystemManager : public ISystemManager
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~SystemManager( ) { };


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	SystemManager( ) { };


	/*! Registers a System
	*
	*  @param[in] ISystem * system
	*  @return (void)
	*/
	void RegisterSystem( const System::Queues::Queue& systemQueue, ISystem* system );


	/*! Gets a Registered System
	*
	*  @param[in] System::Types::Type systemType
	*  @return (ISystem*)
	*/
	ISystem* GetSystem( const System::Types::Type& systemType ) const;


	/*! Initializes all Registered Systems
	*
	*  @return (void)
	*/
	void InitializeAllSystems( );


	/*! Checks to see if a system has been registered
	 *
	 *  @param[in] const System::Types::Type & systemType
	 *  @return (bool)
	 */
	bool HasSystem( const System::Types::Type& systemType ) const;


	/*! Steps each Registered System
	*
	*  @param[in] float deltaMilliseconds
	*  @return (void)
	*/
	void Update( const float& deltaMilliseconds );
	

	/*! Releases all Registered Systems
	*
	*  @return (void)
	*/
	void Release( );


	/*! Creates a World from All Registered Systems
	*
	*  @return (IWorld*)
	*/
	State::IWorld* CreateWorld( );

private:

	ISystem::SystemTypeMap _systemsByType;
	ISystem::SystemQueueMap _systemsByQueue;

	SystemManager( const SystemManager & copy ) { };
	SystemManager & operator = ( const SystemManager & copy ) { return *this; };

};

#endif
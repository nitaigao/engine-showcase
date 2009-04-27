/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SystemManager.h
*  @date   2009/04/25
*/
#ifndef __SYSTEMMANAGER_H
#define __SYSTEMMANAGER_H

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
	~SystemManager( );


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
	void RegisterSystem( ISystem* system );


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

	SystemList _systems;

	SystemManager( const SystemManager & copy ) { };
	SystemManager & operator = ( const SystemManager & copy ) { return *this; };

};

#endif
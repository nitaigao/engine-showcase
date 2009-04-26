/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\SystemManager.h
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
	virtual ~SystemManager( );


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
	ISystem* GetSystem( System::Types::Type systemType );


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
	void Update( float deltaMilliseconds );
	

	/*! Releases all Registered Systems
	*
	*  @return (void)
	*/
	void Release( );


	/*! Creates a World from All Registered Systems
	*
	*  @return (IWorld*)
	*/
	IWorld* CreateWorld( );

private:

	SystemList _systems;

	SystemManager( const SystemManager & copy ) { };
	SystemManager & operator = ( const SystemManager & copy ) { return *this; };

};

#endif
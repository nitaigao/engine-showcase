/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\ISystemManager.hpp
*  @date   2009/04/25
*/
#ifndef __ISYSTEMMANAGER_H
#define __ISYSTEMMANAGER_H

#include "ISystem.hpp"
#include "../State/IWorld.hpp"

/*! 
 *  Manages each System of the Game
 */
class ISystemManager
{

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	virtual ~ISystemManager( ) { };


	/*! Registers a System
	 *
	 *  @param[in] ISystem * system
	 *  @return (void)
	 */
	virtual void RegisterSystem( ISystem* system ) = 0;


	/*! Gets a Registered System
	 *
	 *  @param[in] System::Types::Type systemType
	 *  @return (ISystem*)
	 */
	virtual ISystem* GetSystem( System::Types::Type systemType ) = 0;


	/*! Initializes all Registered Systems
	 *
	 *  @return (void)
	 */
	virtual void InitializeAllSystems( ) = 0;


	/*! Steps each Registered System
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	virtual void Update( float deltaMilliseconds ) = 0;


	/*! Releases all Registered Systems
	 *
	 *  @return (void)
	 */
	virtual void Release( ) = 0;


	/*! Creates a World from All Registered Systems
	 *
	 *  @return (IWorld*)
	 */
	virtual IWorld* CreateWorld( ) = 0;

};

#endif
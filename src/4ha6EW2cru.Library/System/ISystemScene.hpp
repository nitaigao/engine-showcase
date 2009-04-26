/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\ISystemScene.hpp
*  @date   2009/04/25
*/
#ifndef __ISYSTEMWORLD_H
#define __ISYSTEMWORLD_H

#include <string>
#include <map>
#include <vector>

#include "ISystemComponent.hpp"

/*! 
 *  A System Specific Scene
 */
class ISystemScene
{

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	virtual ~ISystemScene( ) { };


	/*! Initializes the System Scene
	 *
	 *  @return (void)
	 */
	virtual void Initialize( ) = 0;


	/*! Steps internal data of the SystemScene
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	virtual void Update( float deltaMilliseconds ) = 0;


	/*! Gets the System::Types::Type of the SystemScene
	 *
	 *  @return (System::Types::Type)
	 */
	virtual System::Types::Type GetType( ) = 0;


	/*! Creates a SystemComponent specific to the SystemScene
	 *
	 *  @param[in] const std::string & name
	 *  @param[in] const std::string & type
	 *  @return (ISystemComponent*)
	 */
	virtual ISystemComponent* CreateComponent( const std::string& name, const std::string& type ) = 0;


	/*! Destroys a SystemComponent created by the SystemScene
	 *
	 *  @param[in] ISystemComponent * component
	 *  @return (void)
	 */
	virtual void DestroyComponent( ISystemComponent* component ) = 0;
};

typedef std::map< System::Types::Type, ISystemScene* > SystemSceneMap;
typedef std::vector< ISystemScene* > SystemSceneList;


#endif
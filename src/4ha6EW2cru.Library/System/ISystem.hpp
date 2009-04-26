/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\ISystem.hpp
*  @date   2009/04/25
*/
#ifndef __ISYSTEM_H
#define __ISYSTEM_H

#include <string>
#include <deque>
#include <map>

#include "SystemType.hpp"
#include "AnyValue.hpp"
#include "ISystemScene.hpp"

/*! 
 *  A Game System
 */
class ISystem
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	virtual ~ISystem( ) { };

	/*! Initializes the System
	*
	*  @return (void)
	*/
	virtual void Initialize( ) = 0;


	/*! Steps the System's internal data
	*
	*  @param[in] float deltaMilliseconds
	*  @return (void)
	*/
	virtual void Update( float deltaMilliseconds ) = 0;


	/*! Releases the System
	*
	*  @return (void)
	*/
	virtual void Release( ) = 0;


	/*! Returns the type of the System
	*
	*  @return (System::Types::Type)
	*/
	virtual System::Types::Type GetType( ) = 0;


	/*! Creates the System Scene
	*
	*  @return (ISystemScene*)
	*/
	virtual ISystemScene* CreateScene( ) = 0;


	/*! Gets the System's Properties
	*
	*  @return (AnyValueMap)
	*/
	virtual AnyValue::AnyValueMap GetProperties( ) = 0;


	/*! Sets a System Property
	*
	*  @param[in] const std::string & name
	*  @param[in] AnyValue value
	*  @return (void)
	*/
	virtual void SetProperty( const std::string& name, AnyValue value ) = 0;

};

typedef std::deque< ISystem* > SystemList;

#endif
/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISystem.hpp
*  @date   2009/04/25
*/
#ifndef ISYSTEM_H
#define ISYSTEM_H

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

	typedef std::map< System::Types::Type, ISystem* > SystemMap;

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
	virtual void Update( const float& deltaMilliseconds ) = 0;


	/*! Releases the System
	*
	*  @return (void)
	*/
	virtual void Release( ) = 0;


	/*! Returns the type of the System
	*
	*  @return (System::Types::Type)
	*/
	virtual System::Types::Type GetType( ) const = 0;


	/*! Creates the System Scene
	*
	*  @return (ISystemScene*)
	*/
	virtual ISystemScene* CreateScene( ) = 0;


	/*! Gets the System's Properties
	*
	*  @return (AnyValueMap)
	*/
	virtual AnyValue::AnyValueMap GetAttributes( ) const = 0;


	/*! Sets a System Property
	*
	*  @param[in] const std::string & name
	*  @param[in] AnyValue value
	*  @return (void)
	*/
	virtual void SetAttribute( const std::string& name, AnyValue value ) = 0;

};

#endif
/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\System\ISystemComponent.hpp
*  @date   2009/04/25
*/
#ifndef __ISYSTEMENTITY_H
#define __ISYSTEMENTITY_H

#include <vector>

#include "SystemType.hpp"
#include "AnyValue.hpp"

#include "IObserver.hpp"

#include "../Maths/MathQuaternion.hpp"
#include "../Maths/MathVector3.hpp"

/*! 
 *  A System Scene Component
 */
class ISystemComponent : public IObserver, public ISubject
{

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	virtual ~ISystemComponent( ) { };

	/*! Initializes the Component
	*
	*  @param[in] AnyValue::AnyValueMap properties
	*  @return (void)
	*/
	virtual void Initialize( AnyValue::AnyValueMap properties ) = 0;


	/*! Steps the internal data of the Component
	*
	*  @param[in] float deltaMilliseconds
	*  @return (void)
	*/
	virtual void Update( float deltaMilliseconds ) = 0;


	/*! Destroys the Component
	*
	*  @return (void)
	*/
	virtual void Destroy( ) = 0;


	/*! Adds an Observer to the Component
	*
	*  @param[in] IObserver * observer
	*  @return (void)
	*/
	virtual void AddObserver( IObserver* observer ) = 0;


	/*! Gets the Name of the Component
	*
	*  @return (const std::string&)
	*/
	virtual const std::string& GetName( ) = 0;


	/*! Gets the System::Types::Type of the Component
	*
	*  @return (System::Types::Type)
	*/
	virtual System::Types::Type GetType( ) = 0;


	/*! Gets the properties of the Component
	*
	*  @return (AnyValueMap)
	*/
	virtual AnyValue::AnyValueMap GetProperties( ) = 0;


	/*! Sets the Properties of the Component
	*
	*  @param[in] AnyValue::AnyValueMap systemProperties
	*  @return (void)
	*/
	virtual void SetProperties( AnyValue::AnyValueMap properties ) = 0;


	/*! Gets the Position of the Component
	*
	*  @return (MathVector3)
	*/
	virtual MathVector3 GetPosition( ) = 0;


	/*! Gets the Scale of the Component
	*
	*  @return (MathVector3)
	*/
	virtual MathVector3 GetScale( ) = 0;


	/*! Gets the Orientation of the Component
	*
	*  @return (MathQuaternion)
	*/
	virtual MathQuaternion GetOrientation( ) = 0;	

};

typedef std::vector< ISystemComponent* > SystemComponentList;


#endif
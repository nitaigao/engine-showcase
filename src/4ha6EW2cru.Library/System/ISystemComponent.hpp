/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISystemComponent.hpp
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
	virtual void Initialize( AnyValue::AnyValueMap& properties ) = 0;


	/*! Steps the internal data of the Component
	*
	*  @param[in] float deltaMilliseconds
	*  @return (void)
	*/
	virtual void Update( const float& deltaMilliseconds ) = 0;


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
	virtual const std::string& GetName( ) const = 0;


	/*! Sets the Id of the component unique to its containing World Entity
	 *
	 *  @param[in] const unsigned int & id
	 *  @return (void)
	 */
	virtual void SetId( const unsigned int& id ) = 0;


	/*! Returns a numerical Id for the component unique to its containing World Entity
	 *
	 *  @return (unsigned int)
	 */
	virtual unsigned int GetId( ) const = 0;


	/*! Gets the System::Types::Type of the Component
	*
	*  @return (System::Types::Type)
	*/
	virtual System::Types::Type GetType( ) const = 0;


	/*! Gets the properties of the Component
	*
	*  @return (AnyValueMap)
	*/
	virtual AnyValue::AnyValueMap GetProperties( ) const = 0;


	/*! Sets the Properties of the Component
	*
	*  @param[in] AnyValue::AnyValueMap systemProperties
	*  @return (void)
	*/
	virtual void SetProperties( AnyValue::AnyValueMap& properties ) = 0;


	/*! Gets the Position of the Component
	*
	*  @return (MathVector3)
	*/
	virtual Maths::MathVector3 GetPosition( ) const = 0;


	/*! Gets the Scale of the Component
	*
	*  @return (MathVector3)
	*/
	//virtual Maths::MathVector3 GetScale( ) const = 0;


	/*! Gets the Orientation of the Component
	*
	*  @return (MathQuaternion)
	*/
	virtual Maths::MathQuaternion GetOrientation( ) const = 0;	

};

typedef std::vector< ISystemComponent* > SystemComponentList;


#endif
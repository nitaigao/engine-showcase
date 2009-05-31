/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ISystemComponent.hpp
*  @date   2009/04/25
*/
#ifndef ISYSTEMENTITY_H
#define ISYSTEMENTITY_H

#include <vector>

#include "SystemType.hpp"
#include "AnyType.hpp"

#include "IObserver.hpp"
#include "ISubject.hpp"

#include "../Maths/MathQuaternion.hpp"
#include "../Maths/MathVector3.hpp"

/*! 
 *  A System Scene Component
 */
class ISystemComponent : public IObserver, public ISubject
{

public:

	typedef std::deque< ISystemComponent* > SystemComponentList;
	typedef std::map< std::string, ISystemComponent* > SystemComponentMap;

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	virtual ~ISystemComponent( ) { };

	/*! Initializes the Component
	*
	*  @param[in] AnyType::AnyValueMap properties
	*  @return (void)
	*/
	virtual void Initialize( ) = 0;


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


	/*! Gets the attributes of the Component
	*
	*  @return (AnyValueMap)
	*/
	virtual AnyType::AnyTypeMap GetAttributes( ) const = 0;

	/*! Sets an Attribute on the Component *
	 *  @param[in] const unsigned int attributeId
	 *  @param[in] const AnyType & value
	 */
	virtual void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) = 0;

};

#endif
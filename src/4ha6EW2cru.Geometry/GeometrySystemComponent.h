/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   GeometrySystemComponent.h
*  @date   2009/04/26
*/
#ifndef GEOMETRYSYSTEMCOMPONENT_H
#define GEOMETRYSYSTEMCOMPONENT_H

#include "System/IObserver.hpp"
#include "System/ISystemComponent.hpp"

#include "System/AnyType.hpp"
#include "System/SystemType.hpp"

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"

namespace Geometry
{
	/*! 
	*  A Geometry System Scene Component
	*/
	class GeometrySystemComponent : public ISystemComponent
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~GeometrySystemComponent( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @return ()
		 */
		explicit GeometrySystemComponent( const std::string& name )
			: m_name( name )
		{

		}
		

		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		inline void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { m_observers.push_back( observer ); };


		/*! Gets the properties of the Component
		*
		*  @return (AnyTypeKeyMap)
		*/
		inline AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );

	private:

		std::string m_name;

		AnyType::AnyTypeMap m_attributes;

		ObserverList m_observers;

		GeometrySystemComponent( ) { };
		GeometrySystemComponent( const GeometrySystemComponent & copy ) { };
		GeometrySystemComponent & operator = ( const GeometrySystemComponent & copy ) { return *this; };

	};
};

#endif
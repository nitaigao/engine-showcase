/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AISystemComponent.h
*  @date   2009/04/25
*/
#pragma once
#ifndef AISYSTEMCOMPONENT_H
#define AISYSTEMCOMPONENT_H

#include <string>
#include "System/SystemType.hpp"

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"

#include "IBehavior.hpp"
#include "IAISystemComponent.hpp"

namespace AI
{
	/*! 
	 *  An Artificial Intelligence System Scene Component
	 */
	class AISystemComponent : public IAISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AISystemComponent( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] IScriptComponent * scriptComponent
		 *  @return ()
		 */
		AISystemComponent( const std::string& name )
			: m_name( name )
			, m_observer( 0 )
			, m_playerDistance( 0 )
		{

		}

		/* Inherited from ISystemComponent */

		/*! Initializes the Component
		 *
		 *  @param[in] AnyType::AnyValueMap attributes
		 *  @return (void)
		 */
		virtual void Initialize( ) { };


		/*! Steps the internal data of the Component
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		virtual void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the Component
		 *
		 *  @return (void)
		 */
		virtual void Destroy( ) { };


		/*! Adds an Observer to the Component
		 *
		 *  @param[in] IObserver * observer
		 *  @return (void)
		 */
		virtual void AddObserver( IObserver* observer ) { m_observer = observer; };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		virtual AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		virtual AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters );


		/*! Gets the attributes of the Component
		 *
		 *  @return (AnyValueMap)
		 */
		inline AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };

	protected:

		AISystemComponent( ) { };
		AISystemComponent( const AISystemComponent & copy ) { };
		AISystemComponent & operator = ( const AISystemComponent & copy ) { return *this; };

		std::string m_name;

		IObserver* m_observer;

		float m_playerDistance;

		AnyType::AnyTypeMap m_attributes;

		Maths::MathVector3::MathVector3List m_activeWaypoints;

	};
};

#endif
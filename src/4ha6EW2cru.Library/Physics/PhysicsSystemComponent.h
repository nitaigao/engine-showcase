/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsSystemComponent.h
*  @date   2009/04/26
*/
#ifndef PHYSICSSYSTEMCOMPONENT_H
#define PHYSICSSYSTEMCOMPONENT_H

#include "IPhysicsSystemComponent.hpp"
#include "HavokPhysicsSystemScene.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

#include <Common/Base/hkBase.h>
#include <Common/Serialize/Util/hkLoader.h>

namespace Physics
{
	/*! 
	 *  A Standard Physics Component
	 */
	class PhysicsSystemComponent : public IPhysicsSystemComponent
	{

	public:


		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~PhysicsSystemComponent( );


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] HavokPhysicsSystemScene * scene
		 *  @param[in] int componentId
		 *  @return ()
		 */
		PhysicsSystemComponent( const std::string& name, HavokPhysicsSystemScene* scene )
			: _name ( name )
			, _scene( scene )
			, m_loadedData( 0 )
			, m_body( 0 )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		virtual void Initialize( );


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
		void Destroy( ) { };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		inline void AddObserver( IObserver* observer ) { m_observer = observer; };


		/*! Gets the properties of the Component
		*
		*  @return (AnyValueKeyMap)
		*/
		AnyValue::AnyValueKeyMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyValue & value
		*/
		inline void SetAttribute( const unsigned int& attributeId, const AnyValue& value ) { m_attributes[ attributeId ] = value; };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyValue::AnyValueMap parameters
		*  @return (AnyValue)
		*/
		AnyValue PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyValue)
		*/
		virtual AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );


		/*! Returns the RigidBody of the Component
		*
		*  @return (hkpRigidBody*)
		*/
		inline hkpRigidBody* GetRigidBody( ) const { return m_body; };

	protected:

		std::string _name;
		AnyValue::AnyValueKeyMap m_attributes;

		IObserver* m_observer;
		HavokPhysicsSystemScene* _scene;
		
		hkpRigidBody* m_body;
		hkPackfileData* m_loadedData;

		PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
		PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

	};
};

#endif
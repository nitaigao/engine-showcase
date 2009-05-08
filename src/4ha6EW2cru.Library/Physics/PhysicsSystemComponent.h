/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsSystemComponent.h
*  @date   2009/04/26
*/
#ifndef __PHYSICSSYSTEMCOMPONENT_H
#define __PHYSICSSYSTEMCOMPONENT_H

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
			, _id( 0 )
			, _scene( scene )
			, _loadedData( 0 )
			, _body( 0 )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		virtual void Initialize( AnyValue::AnyValueMap& properties );


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
		inline void AddObserver( IObserver* observer ) { _observer = observer; };


		/*! Observes a change in the Subject
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		virtual void Observe( ISubject* subject, const unsigned int& systemChanges );


		/*! Pushes any Changes to the Observers
		*
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void PushChanges( const unsigned int& systemChanges );


		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return _name; };


		/*! Sets the Id of the component unique to its containing World Entity
		*
		*  @param[in] const unsigned int & id
		*  @return (void)
		*/
		inline void SetId( const unsigned int& id ) { _id = id; };


		/*! Returns a numerical Id for the component unique to its containing World Entity
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetId( ) const { return _id; };


		/*! Gets the System::Types::Type of the Component
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::PHYSICS; };


		/*! Gets the types of Changes this component is interested in
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetRequestedChanges( ) const  
		{ 
			return 
				System::Changes::Geometry::All |
				System::Changes::Input::Move_Forward |
				System::Changes::Input::Move_Backward |
				System::Changes::Input::Strafe_Left |
				System::Changes::Input::Strafe_Right |
				System::Changes::Input::Jump;
		};


		/*! Gets the properties of the Component
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return AnyValue::AnyValueMap( ); };
		
		
		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		inline void SetProperties( AnyValue::AnyValueMap& properties ) { };


		/*! Gets the Position of the Component
		*
		*  @return (MathVector3)
		*/
		Maths::MathVector3 GetPosition( ) const { return Maths::MathVector3( _body->getPosition( ) ); };


		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return Maths::MathVector3::Zero( ); };
		
		
		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		Maths::MathQuaternion GetOrientation( ) const { return Maths:: MathQuaternion( _body->getRotation( ) ); };


		/*! Returns the RigidBody of the Component
		*
		*  @return (hkpRigidBody*)
		*/
		inline hkpRigidBody* GetRigidBody( ) const { return _body; };

	protected:

		std::string _name;
		unsigned int _id;

		IObserver* _observer;
		HavokPhysicsSystemScene* _scene;
		
		hkpRigidBody* _body;
		hkPackfileData* _loadedData;

		PhysicsSystemComponent( const PhysicsSystemComponent & copy ) { };
		PhysicsSystemComponent & operator = ( const PhysicsSystemComponent & copy ) { return *this; };

	};
};

#endif
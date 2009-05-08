/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputSystemComponent.h
*  @date   2009/04/26
*/
#ifndef __INPUTSYSTEMCOMPONENT_H
#define __INPUTSYSTEMCOMPONENT_H

#include <string>
#include <ois/OIS.h>

#include "../System/SystemType.hpp"
#include "../System/ISystemComponent.hpp"

namespace Input
{
	/*! 
	 *  An Input System Scene Component
	 */
	class InputSystemComponent : public ISystemComponent
	{
	
	public:
	
		/*! Default Destructor
		*
		*  @return ()
		*/
		~InputSystemComponent( ) { };
	
	
		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @return ()
		*/
		InputSystemComponent( const std::string& name )
			: _name( name )
			, _id( 0 )
			, _observer( 0 )
		{
	
		}
	
	
		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( AnyValue::AnyValueMap& properties ) { };
	
	
		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds ) { };
	
	
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
		void Observe( ISubject* subject, const unsigned int& systemChanges ) { };
	
	
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
		inline System::Types::Type GetType( ) const { return System::Types::INPUT; };
	
	
		/*! Gets the types of Changes this component is interested in
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetRequestedChanges( ) const  { return System::Changes::None; };
	
	
		/*! Gets the properties of the Component
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return _properties; };
	
	
		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		inline void SetProperties( AnyValue::AnyValueMap& properties ) { _properties = properties; };
	
	
		/*! Gets the Position of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetPosition( ) const { return Maths::MathVector3::Zero( ); };
	
	
		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return Maths::MathVector3::Zero( ); };
	
	
		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		inline Maths::MathQuaternion GetOrientation( ) const { return Maths::MathQuaternion::Identity( ); };
	
	
		/*! Callback from the Scene to notify the component a key has been pressed
		 *
		 *  @param[in] const OIS::KeyCode & keyCode
		 *  @param[in] const std::string & keyText
		 *  @return (void)
		 */
		virtual void KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText );
	
	
		/*! Callback from the Scene to notify the component a key has been released
		 *
		 *  @param[in] const OIS::KeyCode & keyCode
		 *  @param[in] const std::string & keyText
		 *  @return (void)
		 */
		virtual void KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText );
	
	private:
	
		std::string _name;
		unsigned int _id;

		IObserver* _observer;
	
		AnyValue::AnyValueMap _properties;
	
		InputSystemComponent( ) { };
		InputSystemComponent( const InputSystemComponent & copy ) { };
		InputSystemComponent & operator = ( const InputSystemComponent & copy ) { return *this; };
	
	};
};

#endif
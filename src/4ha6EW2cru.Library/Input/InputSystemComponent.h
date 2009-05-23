/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputSystemComponent.h
*  @date   2009/04/26
*/
#ifndef INPUTSYSTEMCOMPONENT_H
#define INPUTSYSTEMCOMPONENT_H

#include <string>
#include <ois/OIS.h>

#include "IInputSystemComponent.hpp"

#include "../System/SystemType.hpp"

namespace Input
{
	/*! 
	 *  An Input System Scene Component
	 */
	class InputSystemComponent : public IInputSystemComponent
	{
	
	public:
	
		/*! Default Destructor
		*
		*  @return ()
		*/
		~InputSystemComponent( ) { };
	
	
		/*! Default Constructor
		*
		*  @return ()
		*/
		InputSystemComponent( ) { };
	
	
		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( ) { };
	
	
		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );
	
	
		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( ) { };


		/*! Gets the properties of the Component
		*
		*  @return (AnyTypeKeyMap)
		*/
		AnyType::AnyTypeKeyMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyType & value
		*/
		inline void SetAttribute( const unsigned int& attributeId, const AnyType& value ) { m_attributes[ attributeId ] = value; };


		/*! Adds an Observer to the Component
		*
		*  @param[in] IObserver * observer
		*  @return (void)
		*/
		void AddObserver( IObserver* observer ) { m_observers.push_back( observer ); };


		/*! Gets the System::Types::Type of the Component
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::INPUT; };


		/*! Posts a message to observers
		*
		*  @param[in] const std::string & message
		*  @param[in] AnyType::AnyValueMap parameters
		*  @return (AnyType)
		*/
		AnyType PushMessage( const System::Message& message, AnyType::AnyTypeKeyMap parameters );


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyType)
		*/
		AnyType Message( const System::Message& message, AnyType::AnyTypeKeyMap parameters );


		/*! Called by the Scene when the user presses a mouse button
		*
		* @param[in] const OIS::MouseEvent & arg
		* @param[in] OIS::MouseButtonID id
		* @return ( void )
		*/
		void MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


		/*! Called by the Scene when the user releases a mouse button
		*
		* @param[in] const OIS::MouseEvent & arg
		* @param[in] OIS::MouseButtonID id
		* @return ( void )
		*/
		void MouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	private:
	
		InputSystemComponent( const InputSystemComponent & copy ) { };
		InputSystemComponent & operator = ( const InputSystemComponent & copy ) { return *this; };

		ObserverList m_observers;
		AnyType::AnyTypeKeyMap m_attributes;
	
	};
};

#endif
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
		*  @param[in] const std::string & name
		*  @return ()
		*/
		InputSystemComponent( const std::string& name, OIS::Mouse* mouse, OIS::Keyboard* keyboard )
			: m_name( name )
			, m_mouse( mouse )
			, m_keyboard( keyboard )
		{
		}
	
	
		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
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
		*  @return (AnyValueKeyMap)
		*/
		AnyValue::AnyValueKeyMap GetAttributes( ) const { return m_attributes; };


		/*! Sets an Attribute on the Component *
		*
		*  @param[in] const unsigned int attributeId
		*  @param[in] const AnyValue & value
		*/
		inline void SetAttribute( const unsigned int& attributeId, const AnyValue& value ) { m_attributes[ attributeId ] = value; };


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


		void MousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		void MouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

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
		AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );
	
	private:
	
		InputSystemComponent( ) { };
		InputSystemComponent( const InputSystemComponent & copy ) { };
		InputSystemComponent & operator = ( const InputSystemComponent & copy ) { return *this; };

		OIS::Keyboard* m_keyboard;
		OIS::Mouse* m_mouse;

		std::string m_name;

		ObserverList m_observers;
		AnyValue::AnyValueKeyMap m_attributes;
	
	};
};

#endif
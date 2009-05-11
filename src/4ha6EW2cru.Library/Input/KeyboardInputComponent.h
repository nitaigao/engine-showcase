/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   KeyboardInputComponent.h
*  @date   2009/05/09
*/
#ifndef __KEYBOARDINPUTCOMPONENT_H
#define __KEYBOARDINPUTCOMPONENT_H

#include <ois/OIS.h>

#include "IKeyboardInputComponent.hpp"

namespace Input
{
	/*!
	 *  A Keyboard Input Component 
	 */
	class KeyboardInputComponent : public IKeyboardInputComponent
	{

	public:

		~KeyboardInputComponent( ) { };

		KeyboardInputComponent( const std::string& name, OIS::Keyboard* keyboard )
			: _name( name )
			, _id( 0 )
			, _keyboard( keyboard )
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
		void Update( const float& deltaMilliseconds );


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
		void AddObserver( IObserver* observer ) { _observers.push_back( observer ); };


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
		inline unsigned int GetRequestedChanges( ) const 
		{
			return System::Changes::Geometry::All;
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
		inline Maths::MathVector3 GetPosition( ) const { return _position; };


		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return _scale; };


		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		inline Maths::MathQuaternion GetOrientation( ) const { return _orientation; };



	private:

		KeyboardInputComponent( const KeyboardInputComponent & copy ) { };
		KeyboardInputComponent & operator = ( const KeyboardInputComponent & copy ) { return *this; };

		std::string _name;
		unsigned int _id;

		OIS::Keyboard* _keyboard;

		ObserverList _observers;

		Maths::MathVector3 _position;
		Maths::MathVector3 _scale;
		Maths::MathQuaternion _orientation;

	};
};

#endif
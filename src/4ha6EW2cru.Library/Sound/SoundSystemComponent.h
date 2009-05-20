/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundSystemComponent.h
*  @date   2009/04/27
*/
#ifndef SOUNDSYSTEMCOMPONENT_H
#define SOUNDSYSTEMCOMPONENT_H

#include "ISoundSystemComponent.hpp"
#include "ISoundScene.hpp"

namespace Sound
{
	/*!
	 *  A Sound System Component 
	 */
	class SoundSystemComponent : public ISoundSystemComponent
	{

		typedef std::map< std::string, FMOD::Event* > SoundEventList;

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~SoundSystemComponent( ) { };


		/*! Default Constructor
		*
		*  @param[in] const std::string & name
		*  @return ()
		*/
		SoundSystemComponent( const std::string& name, ISoundScene* soundSystemScene )
			: m_name( name )
			, m_soundSystemScene( soundSystemScene )
		{

		}

		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		inline void Initialize( ) { };


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
		inline void AddObserver( IObserver* observer ) { };


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
		inline AnyValue PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters ) { return AnyValue( ); };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyValue)
		*/
		AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );

		void TriggerEvent( const std::string& eventPath );
		void KeyoutEvent( const std::string& eventPath );

	private:

		SoundSystemComponent( const SoundSystemComponent & copy ) { };
		SoundSystemComponent & operator = ( const SoundSystemComponent & copy ) { return *this; };

		static FMOD_RESULT F_CALLBACK OnSoundEventStarted(	FMOD_EVENT* event, FMOD_EVENT_CALLBACKTYPE  type, void* param1, void* param2, void* userdata );

		ISoundScene* m_soundSystemScene;

		std::string m_name;
		AnyValue::AnyValueKeyMap m_attributes;

		SoundEventList m_activeSoundEvents;

	};
};

#endif
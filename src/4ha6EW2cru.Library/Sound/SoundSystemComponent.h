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
			, m_id( 0 )
			, m_soundSystemScene( soundSystemScene )
		{

		}

		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( AnyValue::AnyValueMap& properties );


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
		inline void AddObserver( IObserver* observer ) { m_observers.push_back( observer ); };


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
		void PushChanges( const unsigned int& systemChanges ) { };


		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline const std::string& GetName( ) const { return m_name; };


		/*! Sets the Id of the component unique to its containing World Entity
		*
		*  @param[in] const unsigned int & id
		*  @return (void)
		*/
		inline void SetId( const unsigned int& id ) { m_id = id; };


		/*! Returns a numerical Id for the component unique to its containing World Entity
		*
		*  @return (unsigned int)
		*/
		inline unsigned int GetId( ) const { return m_id; };


		/*! Gets the System::Types::Type of the Component
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::SOUND; };


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
		inline AnyValue::AnyValueMap GetAttributes( ) const { return AnyValue::AnyValueMap( ); };


		/*! Sets the Properties of the Component
		*
		*  @param[in] AnyValue::AnyValueMap systemProperties
		*  @return (void)
		*/
		inline void SetAttributes( AnyValue::AnyValueMap& properties ) { };


		/*! Gets the Position of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetPosition( ) const { return m_position; };


		/*! Gets the Scale of the Component
		*
		*  @return (MathVector3)
		*/
		inline Maths::MathVector3 GetScale( ) const { return m_scale; };


		/*! Gets the Orientation of the Component
		*
		*  @return (MathQuaternion)
		*/
		inline Maths::MathQuaternion GetOrientation( ) const { return m_orientation; };


		/*! Messages the Component to influence its internal state
		*
		*  @param[in] const std::string & message
		*  @return (AnyValue)
		*/
		AnyValue Message( const std::string& message, AnyValue::AnyValueMap parameters );

		void TriggerEvent( const std::string& eventPath );
		void KeyoutEvent( const std::string& eventPath );

	private:

		SoundSystemComponent( const SoundSystemComponent & copy ) { };
		SoundSystemComponent & operator = ( const SoundSystemComponent & copy ) { return *this; };

		static FMOD_RESULT F_CALLBACK OnSoundEventStarted(	FMOD_EVENT* event, FMOD_EVENT_CALLBACKTYPE  type, void* param1, void* param2, void* userdata );

		ISoundScene* m_soundSystemScene;

		std::string m_name;
		unsigned int m_id;

		ObserverList m_observers;

		Maths::MathVector3 m_position;
		Maths::MathVector3 m_scale;
		Maths::MathQuaternion m_orientation;

		SoundEventList m_activeSoundEvents;

	};
};

#endif
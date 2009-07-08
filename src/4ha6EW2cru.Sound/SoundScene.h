/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundScene.h
*  @date   2009/04/27
*/
#ifndef SOUNDSCENE_H
#define SOUNDSCENE_H

#include "ISoundScene.hpp"
#include "ISoundSystem.hpp"

#include "Events/IEvent.hpp"
#include "Events/EventType.hpp"

#include <fmod.hpp>
#include <fmod_event.hpp>

namespace Sound
{
	/*!
	 *  A Sound Scene 
	 */
	class SoundScene : public ISoundScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundScene( ) { };


		/*! Default Constructor
		*
		*  @param[in] ISystem * system
		*  @return ()
		*/
		explicit SoundScene( ISoundSystem* soundSystem )
			: m_soundSystem( soundSystem )
		{

		}


		/*! Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( ) { };


		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::SOUND; };


		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );


		/*! Destroys a SystemComponent created by the SystemScene
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void DestroyComponent( ISystemComponent* component );

		inline ISoundSystem* GetSoundSystem( ) { return m_soundSystem; };

	private:

		ISoundSystem* m_soundSystem;
	};
};

#endif
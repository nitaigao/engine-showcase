/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundSystem.h
*  @date   2009/04/27
*/
#ifndef __SOUNDSYSTEM_H
#define __SOUNDSYSTEM_H

#include <fmod.hpp>
#include <fmod_event.hpp>

#include "../Service/IService.hpp"

#include "ISoundSystem.hpp"

namespace Sound
{
	/*!
	 *  The Sound System 
	 */
	class SoundSystem : public ISoundSystem, public IService
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundSystem( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		SoundSystem( )
			: _fmodSystem( 0 )
			, _eventSystem( 0 )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( );


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::SOUND; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyValueMap)
		*/
		inline AnyValue::AnyValueMap GetProperties( ) const { return AnyValue::AnyValueMap( ); };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyValue value
		*  @return (void)
		*/
		inline void SetProperty( const std::string& name, AnyValue value ) { };		


		/*! Sound Service Interface
		 *
		 *  @param[in] const std::string & actionName
		 *  @param[in] AnyValue::AnyValueMap & parameters
		 *  @return (AnyValue::AnyValueMap)
		 */
		AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters );

	private:

		SoundSystem( const SoundSystem & copy ) { };
		SoundSystem & operator = ( const SoundSystem & copy ) { return *this; };

		FMOD::System* _fmodSystem;
		FMOD::EventSystem* _eventSystem;

		static FMOD_RESULT F_CALLBACK FileOpen( const char* name, int unicode, unsigned int* filesize, void** handle, void** userdata );
		static FMOD_RESULT F_CALLBACK FileClose( void* handle, void*  userdata );
		static FMOD_RESULT F_CALLBACK FileRead( void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata );
		static FMOD_RESULT F_CALLBACK FileSeek( void* handle, unsigned int pos, void* userdata );
	};
};

#endif
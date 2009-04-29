#include "SoundSystem.h"

#include "SoundScene.h"

#include "../Management/Management.h"

#include "../IO/IResource.hpp"
using namespace Resources;

#include "../IO/FileBuffer.hpp"
using namespace IO;

namespace Sound
{
	ISystemScene* SoundSystem::CreateScene()
	{
		return new SoundScene( );
	}

	void SoundSystem::Initialize()
	{
		FMOD_RESULT result;
		_fmodSystem = 0;

		result = System_Create( &_fmodSystem );

		if ( result != FMOD_OK )
		{
			// failed to create FMOD System
		}

		unsigned int version = 0;

		result = _fmodSystem->getVersion( &version );

		if ( version < FMOD_VERSION )
		{
			// wrong FMOD version
		}

		int driverCount = 0;
		result = _fmodSystem->getNumDrivers( &driverCount );


		if ( driverCount < 1 )
		{
			result = _fmodSystem->setOutput( FMOD_OUTPUTTYPE_NOSOUND );
		}
		else
		{
			FMOD_CAPS driverCaps;
			FMOD_SPEAKERMODE speakerMode;

			result = _fmodSystem->getDriverCaps( 0, &driverCaps, 0, 0, &speakerMode );

			if ( result != FMOD_OK )
			{
				// could not get driver caps
			}

			if ( driverCaps & FMOD_CAPS_HARDWARE_EMULATED )
			{
				char name[ 256 ];
				result = _fmodSystem->getDriverInfo( 0, name, 256, 0 );

				if ( result != FMOD_OK )
				{
					// could not get driver info
				}

				if ( strstr( name, "SigmaTel" ) )
				{
					result = _fmodSystem->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR );

					if ( result != FMOD_OK )
					{
						// count not set Sigmatel software format
					}
				}
			}
		}

		result = _fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );

		if ( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
		{
			result = _fmodSystem->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );

			if ( result != FMOD_OK )
			{
				// could not set speaker mode
			}

			result = _fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );

			if ( result != FMOD_OK )
			{
				// could not init FMOD with standard stereo setup
			}
		}

		result = _fmodSystem->setFileSystem( &SoundSystem::FileOpen, &SoundSystem::FileClose, &SoundSystem::FileRead, &SoundSystem::FileSeek, 0 );

		if ( result != FMOD_OK )
		{
			// error binding IO functions
		}

		result = EventSystem_Create( &_eventSystem );

		if ( result != FMOD_OK )
		{
			// error creating event system
		}

		result = _eventSystem->getSystemObject( &_fmodSystem );

		if ( result != FMOD_OK )
		{
			// error getting the FMOD system object
		}

		result = _eventSystem->init( 256, FMOD_INIT_NORMAL, 0);

		if ( result != FMOD_OK )
		{
			// error initializing the FMOD event system
		}

		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
	}

	void SoundSystem::Release()
	{
		_eventSystem->release( );
		_fmodSystem->release( );
	}

	void SoundSystem::Update( const float& deltaMilliseconds )
	{
		_eventSystem->update( );
		_fmodSystem->update( );
	}

	AnyValue::AnyValueMap SoundSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;
		FMOD_RESULT result;

		if( actionName == "load" )
		{
			std::string filePath = parameters[ "filePath" ].GetValue< std::string >( );
			result = _eventSystem->load( filePath.c_str( ), 0, 0 );
		}
		
		if ( actionName == "playMusic" )
		{
			FMOD::Event* event = 0;
			std::string eventPath = parameters[ "eventPath" ].GetValue< std::string >( );
			result = _eventSystem->getEvent( eventPath.c_str( ), FMOD_EVENT_DEFAULT, &event );
			
			if ( result == FMOD_OK )
			{
				result = event->start( );
			}
		}

		results[ "result" ] = ( result == FMOD_OK );

		return results;
	}

	FMOD_RESULT F_CALLBACK SoundSystem::FileOpen( const char* name, int unicode, unsigned int* filesize, void** handle, void** userdata )
	{
		IResource* resource = Management::GetInstance( )->GetResourceManager( )->GetResource( name );
		resource->AddReference( );

		*handle = resource;
		*filesize = resource->GetFileBuffer( )->fileLength;

		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK SoundSystem::FileClose( void* handle, void* userdata )
	{
		IResource* resource = reinterpret_cast< IResource* >( handle );
		resource->RemoveReference( );

		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK SoundSystem::FileRead( void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata )
	{
		IResource* resource = reinterpret_cast< IResource* >( handle );
		FileBuffer* fileBuffer = resource->GetFileBuffer( );

		memcpy( buffer, fileBuffer->fileBytes + fileBuffer->filePosition, sizebytes );

		fileBuffer->filePosition += sizebytes;

		return FMOD_OK;
	}

	FMOD_RESULT F_CALLBACK SoundSystem::FileSeek( void* handle, unsigned int pos, void* userdata )
	{
		IResource* resource = reinterpret_cast< IResource* >( handle );
		FileBuffer* fileBuffer = resource->GetFileBuffer( );

		fileBuffer->filePosition = pos;
		return FMOD_OK;
	}
}
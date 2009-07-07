#ifndef SOUNDSYSTEM_SPECS_H
#define SOUNDSYSTEM_SPECS_H

#include "../Suites.h"
#include "Specification.hpp"

#include "Management/Management.h"

#include "Sound/SoundSystem.h"
using namespace Sound;

class SoundSystem_Specs_BaseContext : public Testing::Specificaton< Sound::SoundSystem >
{

protected:

	ISystemScene* _scene;

public:

	void SetupTest( )
	{
		Management::Initialize( );
		Management::Get( )->GetFileManager( )->Mount( "../../../etc", "/" ); 

		_scene = 0;
		_subject = new SoundSystem( );
	}

	void TearDownTest( )
	{
		if ( _scene != 0 )
		{
			delete _scene;
		}

		Management::Release( );
	}
};



class when_the_sound_system_is_not_initialized : public SoundSystem_Specs_BaseContext
{

public:

	void initializing_should_not_throw_any_errors( ) { }

	CPPUNIT_TEST_SUITE( when_the_sound_system_is_not_initialized );
	CPPUNIT_TEST( initializing_should_not_throw_any_errors );
	CPPUNIT_TEST_SUITE_END( );
};
CPPUNIT_TEST_SUITE_REGISTRATION( when_the_sound_system_is_not_initialized );


class when_the_sound_system_is_initialized : public SoundSystem_Specs_BaseContext
{

public:

	void EstablishBaseContext( )
	{
		_subject->Initialize( 0 );
		_subject->Update( 0 );
		_subject->Release( );
	}

	void release_should_not_crash_the_system( ) { };
	void update_should_not_crash_the_system( ) { };

	CPPUNIT_TEST_SUITE( when_the_sound_system_is_initialized );
	CPPUNIT_TEST( release_should_not_crash_the_system );
	CPPUNIT_TEST( update_should_not_crash_the_system );
	CPPUNIT_TEST_SUITE_END( );
};
CPPUNIT_TEST_SUITE_REGISTRATION( when_the_sound_system_is_initialized );

class when_the_service_interface_is_called_with_a_command_of_playMusic : public SoundSystem_Specs_BaseContext
{

	AnyType::AnyTypeMap _results;

public:

	void EstablishBaseContext( )
	{
		_subject->Initialize( 0 );

		AnyType::AnyTypeMap parameters;
		parameters[ "filePath" ] = "/data/sound/music/ux.fev";
		_subject->Execute( "load", parameters );

		parameters[ "eventPath" ] = "ux/ui/title";
		_results = _subject->Execute( "playMusic", parameters );

		_subject->Release( );
	}

	void it_should_play_some_music_and_not_crash( )
	{
		
	}

	CPPUNIT_TEST_SUITE( when_the_service_interface_is_called_with_a_command_of_playMusic );
	CPPUNIT_TEST( it_should_play_some_music_and_not_crash );
	CPPUNIT_TEST_SUITE_END( );
};
CPPUNIT_TEST_SUITE_REGISTRATION( when_the_service_interface_is_called_with_a_command_of_playMusic );

class when_the_service_interface_is_called_with_a_command_of_load : public SoundSystem_Specs_BaseContext
{

	AnyType::AnyTypeMap _results;

public:

	void EstablishBaseContext( )
	{
		_subject->Initialize( 0 );

		AnyType::AnyTypeMap parameters;
		parameters[ "filePath" ] = "/data/sound/music/ux.fev";

		_results = _subject->Execute( "load", parameters );

		_subject->Release( );
	}

	void it_should_load_the_given_file_and_not_crash( )
	{

	}

	CPPUNIT_TEST_SUITE( when_the_service_interface_is_called_with_a_command_of_load );
	CPPUNIT_TEST( it_should_load_the_given_file_and_not_crash );
	CPPUNIT_TEST_SUITE_END( );
};
CPPUNIT_TEST_SUITE_REGISTRATION( when_the_service_interface_is_called_with_a_command_of_load );

class when_creating_a_scene_and_the_system_is_intialized : public SoundSystem_Specs_BaseContext
{

public:

	void EstablishBaseContext( )
	{
		_scene = _subject->CreateScene( );
	}
	
	void the_scene_should_not_be_null( )
	{
		CPPUNIT_ASSERT( _scene != 0 );
	}

	CPPUNIT_TEST_SUITE( when_creating_a_scene_and_the_system_is_intialized );
	CPPUNIT_TEST( the_scene_should_not_be_null );
	CPPUNIT_TEST_SUITE_END( );
};
CPPUNIT_TEST_SUITE_REGISTRATION( when_creating_a_scene_and_the_system_is_intialized );

#endif
#ifndef SOUNDSCENE_TESTS_H
#define SOUNDSCENE_TESTS_H

#include "Specification.hpp"

#include "Sound/SoundScene.h"
using namespace Sound;

class SoundScene_Specs_BaseContext : public Testing::Specificaton< SoundScene >
{

public:

	ISystemComponent* _component;

	void SetupTest( )
	{
		_subject = new SoundScene( 0 );
		_component = 0;
	}

	void TearDownTest( )
	{
		if ( _component != 0 )
		{
			delete _component;
		}
	}
};

class when_the_scene_is_initialized : public SoundScene_Specs_BaseContext
{

	void EstablishBaseContext( )
	{
		_component = _subject->CreateComponent( "test", "default" );
	}

	void should_a_component_on_create_component( )
	{
		CPPUNIT_ASSERT( _component != 0  );
	}

	CPPUNIT_TEST_SUITE( when_the_scene_is_initialized );
	CPPUNIT_TEST( should_a_component_on_create_component );
	CPPUNIT_TEST_SUITE_END( );
};

#include "../Suites.h"
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( when_the_scene_is_initialized, Suites::SoundSuite( ) );

#endif

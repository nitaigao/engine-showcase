#include "StringFixture.h"

#include "Types/String.h"

#include <Ogre.h>

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( StringFixture, Suites::TypesSuite( ) );

void StringFixture::Should_Accept_String_As_Constructor( )
{
	String input( "Hello" );
}

void StringFixture::Should_Accept_UTFString_As_Constructor( )
{
	Ogre::UTFString utfInput( "hello" );
	String input( utfInput );
}

void StringFixture::Should_Convert_To_UTFString_On_ToUTFSTring( )
{
	std::string testString = "hello";

	String input( testString );
	Ogre::UTFString result = input.ToUTFString( );
	CPPUNIT_ASSERT( result.asUTF8( ) == testString );
}

void StringFixture::Should_Convert_On_ToString_As_STD_String( )
{
	std::string testString = "hello";
	String input( testString );

	CPPUNIT_ASSERT( testString == input.ToString( ) );
}

void StringFixture::Should_Access_As_STD_String( )
{
	std::string testString = "hello";
	String input( testString );

	CPPUNIT_ASSERT( testString == input );
};
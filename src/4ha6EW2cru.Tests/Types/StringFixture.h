#ifndef __STRING_FIXTURE_H
#define __STRING_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Scripting/Script.h"
#include "Exceptions/ScriptException.hpp"

class StringFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( StringFixture );

	CPPUNIT_TEST( Should_Accept_String_As_Constructor );
	CPPUNIT_TEST( Should_Accept_UTFString_As_Constructor );
	CPPUNIT_TEST( Should_Convert_To_UTFString_On_ToUTFSTring );
	CPPUNIT_TEST( Should_Convert_On_ToString_As_STD_String );
	CPPUNIT_TEST( Should_Access_As_STD_String );

	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Accept_String_As_Constructor( );
	void Should_Accept_UTFString_As_Constructor( );
	void Should_Convert_To_UTFString_On_ToUTFSTring( );
	void Should_Convert_On_ToString_As_STD_String( );
	void Should_Access_As_STD_String( );
};

#endif


#ifndef __SCRIPT_FIXTURE_H
#define __SCRIPT_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ScriptFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( ScriptFixture );
  CPPUNIT_TEST( Should_Not_Initialize_NonExistant_Script );
  CPPUNIT_TEST( Should_Load_Script_Successfully );
  CPPUNIT_TEST( Should_Recieve_Event );
  CPPUNIT_TEST( Should_Call_Function );
  CPPUNIT_TEST_EXCEPTION( Should_Throw_On_Script_Error, std::exception );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Not_Initialize_NonExistant_Script( );
	void Should_Load_Script_Successfully( );
	void Should_Recieve_Event( );
	void Should_Call_Function( );
	void Should_Throw_On_Script_Error( );

};

#endif
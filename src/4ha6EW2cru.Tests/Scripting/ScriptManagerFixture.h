#ifndef __SCRIPTMANAGER_FIXTURE_H
#define __SCRIPTMANAGER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ScriptManagerFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ScriptManagerFixture );

	CPPUNIT_TEST( Should_Initialize_And_Release_Correctly );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Intialized );
	CPPUNIT_TEST( Should_Throw_On_Uninitialized_Release );
	CPPUNIT_TEST( Should_Throw_On_UnInitialized_GetInstance );
	CPPUNIT_TEST( Should_GetInstance_Correctly_Given_Intiailized );
	CPPUNIT_TEST( Should_Create_A_New_Script );
	CPPUNIT_TEST( Should_Destroy_Script );
	CPPUNIT_TEST( Should_Throw_On_DestroyScript_Given_NULL_Script );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

	void Should_Initialize_And_Release_Correctly( );
	void Should_Throw_On_Initialize_Given_Already_Intialized( );
	void Should_Throw_On_Uninitialized_Release( );
	void Should_Throw_On_UnInitialized_GetInstance( );
	void Should_GetInstance_Correctly_Given_Intiailized( );
	void Should_Create_A_New_Script( );
	void Should_Destroy_Script( );
	void Should_Throw_On_DestroyScript_Given_NULL_Script( );

};

#endif
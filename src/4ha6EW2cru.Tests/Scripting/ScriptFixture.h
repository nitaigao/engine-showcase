#ifndef __SCRIPT_FIXTURE_H
#define __SCRIPT_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Scripting/Script.h"
#include "Exceptions/ScriptException.hpp"

class ScriptFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ScriptFixture );

	CPPUNIT_TEST( Should_Initialize_Given_Valid_Script );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Invalid_Script );
	CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Initialized );

	CPPUNIT_TEST( Should_Throw_On_CreateFromFileBuffer_Given_NULL_FileBuffer );
	CPPUNIT_TEST( Should_Throw_On_CreateFromFileBuffer_Given_Invalid_FileBuffer );
	CPPUNIT_TEST( Should_CreateFromFileBuffer_Given_Valid_FileBuffer );

	CPPUNIT_TEST( Should_Throw_On_CallFunction_Given_Not_Initialized );
	CPPUNIT_TEST( Should_Throw_On_CallFunction_Given_Empty_FunctionName );
	CPPUNIT_TEST( Should_Throw_On_CallFunction_Given_Non_Existing_FunctionName );
	CPPUNIT_TEST( Should_Throw_On_CallFunction_Given_Lua_Error );
	CPPUNIT_TEST( Should_CallFunction_Given_Valid_FunctionName );

	CPPUNIT_TEST( Should_Thow_On_GetState_Given_Not_Initialized );
	CPPUNIT_TEST( Should_GetState_Given_Intialized );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Given_Valid_Script( );
	void Should_Throw_On_Initialize_Given_Invalid_Script( );
	void Should_Throw_On_Initialize_Given_Already_Initialized( );

	void Should_Throw_On_CreateFromFileBuffer_Given_NULL_FileBuffer( );
	void Should_Throw_On_CreateFromFileBuffer_Given_Invalid_FileBuffer( );
	void Should_CreateFromFileBuffer_Given_Valid_FileBuffer( );

	void Should_Throw_On_CallFunction_Given_Not_Initialized( );
	void Should_Throw_On_CallFunction_Given_Empty_FunctionName( );
	void Should_Throw_On_CallFunction_Given_Non_Existing_FunctionName( );
	void Should_Throw_On_CallFunction_Given_Lua_Error( );
	void Should_CallFunction_Given_Valid_FunctionName( );

	void Should_Thow_On_GetState_Given_Not_Initialized( );
	void Should_GetState_Given_Intialized( );

private:

	Script* _script;

};

#endif
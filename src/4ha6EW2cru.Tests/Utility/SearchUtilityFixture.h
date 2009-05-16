#ifndef SEARCHUTILITYFIXTURE_H
#define SEARCHUTILITYFIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class SearchUtilityFixture : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( SearchUtilityFixture );
	CPPUNIT_TEST( Should_Set_WildCard_Character_Given_Valid_Character );
	CPPUNIT_TEST( Should_Realise_A_StartsWith_Search_Given_Valid_Search_Pattern );
	CPPUNIT_TEST( Should_Realise_An_EndsWith_Search_Given_Valid_Search_Pattern );
	CPPUNIT_TEST( Should_Realise_A_Contains_Search_Given_Valid_Search_Pattern );
	CPPUNIT_TEST( Should_Realise_Search_String_Given_Valid_Search_Pattern );
	CPPUNIT_TEST( Should_Return_False_On_FindMatch_Given_StartsWithSearch_And_Invalid_Match_Input );
	CPPUNIT_TEST( Should_Return_False_On_FindMatch_Given_EndsWithSearch_And_Invalid_Match_Input );
	CPPUNIT_TEST( Should_Return_False_On_FindMatch_Given_ContainsSearch_And_Invalid_Match_Input );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Set_WildCard_Character_Given_Valid_Character( );
	void Should_Realise_A_StartsWith_Search_Given_Valid_Search_Pattern( );
	void Should_Realise_An_EndsWith_Search_Given_Valid_Search_Pattern( );
	void Should_Realise_A_Contains_Search_Given_Valid_Search_Pattern( );
	void Should_Return_False_On_FindMatch_Given_StartsWithSearch_And_Invalid_Match_Input( );
	void Should_Realise_Search_String_Given_Valid_Search_Pattern( );
	void Should_Return_False_On_FindMatch_Given_EndsWithSearch_And_Invalid_Match_Input( );
	void Should_Return_False_On_FindMatch_Given_ContainsSearch_And_Invalid_Match_Input( );

};

#endif
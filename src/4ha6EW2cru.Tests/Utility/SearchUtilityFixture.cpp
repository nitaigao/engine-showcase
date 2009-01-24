#include "SearchUtilityFixture.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SearchUtilityFixture, Suites::UtilitySuite( ) );

#include "Utility/SearchUtility.hpp"

void SearchUtilityFixture::Should_Set_WildCard_Character_Given_Valid_Character( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( wildCard );

	CPPUNIT_ASSERT( searchUtility.GetWildCard( ) == wildCard );
}

void SearchUtilityFixture::Should_Realise_A_StartsWith_Search_Given_Valid_Search_Pattern( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "hello*" );

	CPPUNIT_ASSERT( searchUtility.IsStartsWithSearch( ) );
}

void SearchUtilityFixture::Should_Realise_An_EndsWith_Search_Given_Valid_Search_Pattern( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "*hello" );

	CPPUNIT_ASSERT( searchUtility.IsEndsWithSearch( ) );
}

void SearchUtilityFixture::Should_Realise_A_Contains_Search_Given_Valid_Search_Pattern( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "*hello*" );

	CPPUNIT_ASSERT( searchUtility.IsContainsSearch( ) );
}

void SearchUtilityFixture::Should_Realise_Search_String_Given_Valid_Search_Pattern( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "*hello*" );

	CPPUNIT_ASSERT( searchUtility.GetSearchString( ) == "hello" );
}

void SearchUtilityFixture::Should_Return_False_On_FindMatch_Given_StartsWithSearch_And_Invalid_Match_Input( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "hello*" );

	CPPUNIT_ASSERT( !searchUtility.FindMatch( "hel" ) );
}

void SearchUtilityFixture::Should_Return_False_On_FindMatch_Given_EndsWithSearch_And_Invalid_Match_Input( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "*hello" );

	CPPUNIT_ASSERT( !searchUtility.FindMatch( "hel" ) );
}

void SearchUtilityFixture::Should_Return_False_On_FindMatch_Given_ContainsSearch_And_Invalid_Match_Input( )
{
	std::string wildCard = "*";

	SearchUtility searchUtility;
	searchUtility.SetWildCard( "*" );
	searchUtility.SetSearchPattern( "*hello*" );

	CPPUNIT_ASSERT( !searchUtility.FindMatch( "hel" ) );
}
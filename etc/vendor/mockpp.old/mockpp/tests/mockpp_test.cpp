/***************************************************************************
        mockpp_test.cpp  -  unit tests for global mockpp functions
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: mockpp_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/compat/Asserter.h>


class mockpp_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( mockpp_test );

#ifdef MOCKPP_UNICODE
  CPPUNIT_TEST(test_unicode_latin1_conv);
#endif

CPPUNIT_TEST_SUITE_END();

#endif

 public:

#ifdef MOCKPP_UNICODE
  MOCKPP_TESTMETHOD_DECL void test_unicode_latin1_conv();
#endif

};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( mockpp_test );
#ifdef MOCKPP_UNICODE
MOCKPP_CXXTEST(mockpp_test, test_unicode_latin1_conv);
#endif

#elif defined(MOCKPP_USE_BOOSTTEST)

#ifdef MOCKPP_UNICODE
MOCKPP_BOOST_TEST(mockpp_test::test_unicode_latin1_conv);
#endif

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (mockpp_test);

#endif // frameworks



#ifdef MOCKPP_UNICODE

void mockpp_test::test_unicode_latin1_conv()
{
  MOCKPP_STL::string lat1 = "123456�{}�";
  MOCKPP_STL::basic_string<wchar_t> uni = MOCKPP_NS::getUnicode(lat1);

  MOCKPP_ASSERT_TRUE(uni.length() == lat1.length());

  for (unsigned i = 0; i < lat1.length(); ++i)
    MOCKPP_ASSERT_TRUE((wchar_t)lat1[i] == uni[i]);

  MOCKPP_ASSERT_TRUE(lat1 == MOCKPP_NS::getLatin1(uni));

  //

  int unused = 1;
  MOCKPP_UNUSED(unused);
}

#endif // MOCKPP_UNICODE




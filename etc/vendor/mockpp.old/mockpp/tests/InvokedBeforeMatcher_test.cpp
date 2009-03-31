/***************************************************************************
  InvokedBeforeMatcher_test.cpp  -  unit tests for InvokedBeforeMatcher class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: InvokedBeforeMatcher_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/InvokedBeforeMatcher.h>


class InvokedBeforeMatcher_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( InvokedBeforeMatcher_test );

  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( InvokedBeforeMatcher_test );
MOCKPP_CXXTEST(InvokedBeforeMatcher_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(InvokedBeforeMatcher_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (InvokedBeforeMatcher_test);

#endif // frameworks



void InvokedBeforeMatcher_test::test_invoke()
{
  MOCKPP_NS::InvokedRecorder irec;
  MOCKPP_NS::InvokedBeforeMatcher<int> iam(&irec, MOCKPP_PCHAR("beforematch"));

  MOCKPP_ASSERT_TRUE(iam.matches(1) == true);
  MOCKPP_ASSERT_TRUE(iam.hasDescription() == true);

  MOCKPP_NS::String s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(iam.describeTo(s) == MOCKPP_PCHAR("hallobefore beforematch (not invoked)"));
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("hallobefore beforematch (not invoked)"));

  iam.incInvoked(1);
  irec.incInvoked();

  MOCKPP_ASSERT_TRUE(irec.hasBeenInvoked() == true);
  MOCKPP_ASSERT_TRUE(iam.matches(1) == false);

  s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(iam.describeTo(s) == (MOCKPP_PCHAR("hallobefore beforematch (invoked)")));
  MOCKPP_ASSERT_TRUE(s == (MOCKPP_PCHAR("hallobefore beforematch (invoked)")));

  MOCKPP_TRY
  {
    iam.incInvoked(1);
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("called out of order"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif
}




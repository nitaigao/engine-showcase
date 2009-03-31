/***************************************************************************
    InvokeAtLeastMatcher_test.cpp  -  unit tests for InvokeAtLeastMatcher class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: InvokeAtLeastMatcher_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/InvokeAtLeastMatcher.h>


class InvokeAtLeastMatcher_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( InvokeAtLeastMatcher_test );

  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( InvokeAtLeastMatcher_test );
MOCKPP_CXXTEST(InvokeAtLeastMatcher_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(InvokeAtLeastMatcher_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (InvokeAtLeastMatcher_test);

#endif // frameworks



void InvokeAtLeastMatcher_test::test_invoke()
{
  MOCKPP_NS::InvokeAtLeastMatcher ir(2);

  MOCKPP_ASSERT_TRUE(ir.matches() == true);
  MOCKPP_ASSERT_TRUE(ir.hasBeenInvoked() == false);

  MOCKPP_TRY
  {
    ir.verify();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("not invoked often enough"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif

  ir.incInvoked();
  MOCKPP_ASSERT_TRUE(ir.hasBeenInvoked() == true);

  MOCKPP_TRY
  {
    ir.verify();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("not invoked often enough"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif

  MOCKPP_ASSERT_TRUE(ir.matches() == true);
  MOCKPP_ASSERT_TRUE(ir.hasBeenInvoked() == true);

  ir.incInvoked();
  MOCKPP_ASSERT_TRUE(ir.matches() == true);
  MOCKPP_ASSERT_TRUE(ir.getInvocationCount() == 2);
  ir.verify();

  MOCKPP_NS::String s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(ir.describeTo(s).find(MOCKPP_PCHAR("at least")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("at least")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("hallo")) != MOCKPP_NS::String::npos);

  ir.incInvoked();
  MOCKPP_ASSERT_TRUE(ir.matches() == true);

  s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(ir.describeTo(s).find(MOCKPP_PCHAR("at least")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("at least")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("hallo")) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE(s.find(MOCKPP_PCHAR("has been")) == MOCKPP_NS::String::npos);

}




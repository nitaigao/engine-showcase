/***************************************************************************
    InvokedRecorder_test.cpp  -  unit tests for InvokedRecorder class
                             -------------------
    begin                : Sat Aug 27 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: InvokedRecorder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/matcher/InvokedRecorder.h>


class InvokedRecorder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( InvokedRecorder_test );

  CPPUNIT_TEST(test_invoke);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( InvokedRecorder_test );
MOCKPP_CXXTEST(InvokedRecorder_test, test_invoke);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(InvokedRecorder_test::test_invoke);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (InvokedRecorder_test);

#endif // frameworks



void InvokedRecorder_test::test_invoke()
{
  MOCKPP_NS::InvokedRecorder ir;

  MOCKPP_TRY
  {
    ir.verifyHasBeenInvoked();
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("was not invoked"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif

  MOCKPP_ASSERT_TRUE(ir.hasBeenInvoked() == false);
  MOCKPP_ASSERT_TRUE(ir.getInvocationCount() == 0);
  MOCKPP_ASSERT_TRUE(ir.matches() == true);

  ir.verify();
  ir.incInvoked();
  ir.verify();

  MOCKPP_ASSERT_TRUE(ir.hasBeenInvoked() == true);
  MOCKPP_ASSERT_TRUE(ir.getInvocationCount() == 1);
  MOCKPP_ASSERT_TRUE(ir.matches() == true);

  MOCKPP_NS::String s = MOCKPP_PCHAR("hallo");
  MOCKPP_ASSERT_TRUE(ir.describeTo(s) == MOCKPP_PCHAR("hallo") );
  MOCKPP_ASSERT_TRUE(s == MOCKPP_PCHAR("hallo") );

  ir.verifyHasBeenInvoked();
  ir.verifyHasBeenInvokedExactly(1);

  MOCKPP_TRY
  {
    ir.verifyHasBeenInvokedExactly(2);
    MOCKPP_FAIL(MOCKPP_PCHAR("Did not throw the exception"));
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &x)
  {
    MOCKPP_ASSERT_TRUE(x.getMessage().find(MOCKPP_PCHAR("number of times:"))
                       != MOCKPP_NS::String::npos);
  }

  catch(...)
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("Caught unexpected exception"));
  }
#endif
}




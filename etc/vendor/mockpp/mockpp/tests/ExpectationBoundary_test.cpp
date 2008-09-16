/***************************************************************************
   ExpectationBoundary_test.cpp  -  unit tests for ExpectationBoundary class
                             -------------------
    begin                : Sat Dez 28 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ExpectationBoundary_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#undef  MOCKPP_BOUNDARY_DELTA  // after inclusion of mockpp.h!
#define MOCKPP_BOUNDARY_DELTA 1

#include "SelectFramework.h"

#include <mockpp/ExpectationBoundary.h>
#include <mockpp/compat/Asserter.h>


class ExpectationBoundary_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ExpectationBoundary_test );

  CPPUNIT_TEST(test_hasExpectations);
  CPPUNIT_TEST(test_name);
  CPPUNIT_TEST(test_ifNoExpected);
  CPPUNIT_TEST(test_innerRanges);
  CPPUNIT_TEST(test_delta);
  CPPUNIT_TEST(test_clear);

  CPPUNIT_TEST(test_expectNothing);
  CPPUNIT_TEST(test_expectNothing_revoked);
  CPPUNIT_TEST(test_expectActualImmediate);
  CPPUNIT_TEST(test_expectActualVerify);

  CPPUNIT_TEST(fail_clearActual);
  CPPUNIT_TEST(fail_expectActualImmediate);
  CPPUNIT_TEST(fail_expectActualVerify);
  CPPUNIT_TEST(fail_expectNothing);
  CPPUNIT_TEST(fail_bounds);

  CPPUNIT_TEST(fail_ifNoActual);
  CPPUNIT_TEST(fail_negativeDelta);
  CPPUNIT_TEST(fail_positiveDelta);
  CPPUNIT_TEST(fail_delta1);
  CPPUNIT_TEST(fail_delta2);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void test_expectNothing();
  MOCKPP_TESTMETHOD_DECL void test_expectNothing_revoked();
  MOCKPP_TESTMETHOD_DECL void fail_expectNothing();

  MOCKPP_TESTMETHOD_DECL void test_expectActualImmediate();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualImmediate();

  MOCKPP_TESTMETHOD_DECL void test_expectActualVerify();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualVerify();

  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void fail_clearActual();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();
  MOCKPP_TESTMETHOD_DECL void test_innerRanges();
  MOCKPP_TESTMETHOD_DECL void test_delta();

  MOCKPP_TESTMETHOD_DECL void fail_delta1();
  MOCKPP_TESTMETHOD_DECL void fail_delta2();

  MOCKPP_TESTMETHOD_DECL void fail_bounds();
  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual();
  MOCKPP_TESTMETHOD_DECL void fail_negativeDelta();
  MOCKPP_TESTMETHOD_DECL void fail_positiveDelta();
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ExpectationBoundary_test );

MOCKPP_CXXTEST(ExpectationBoundary_test, test_hasExpectations);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_name);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_ifNoExpected);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_innerRanges);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_delta);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_clear);

MOCKPP_CXXTEST(ExpectationBoundary_test, test_expectNothing);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationBoundary_test, test_expectActualVerify);

MOCKPP_CXXTEST(ExpectationBoundary_test, fail_clearActual);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_expectActualVerify);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_expectNothing);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_bounds);

MOCKPP_CXXTEST(ExpectationBoundary_test, fail_ifNoActual);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_negativeDelta);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_positiveDelta);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_delta1);
MOCKPP_CXXTEST(ExpectationBoundary_test, fail_delta2);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_hasExpectations);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_name);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_ifNoExpected);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_innerRanges);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_delta);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_clear);

MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_expectNothing);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::test_expectActualVerify);

MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_clearActual);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_expectNothing);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_bounds);

MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_ifNoActual);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_negativeDelta);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_positiveDelta);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_delta1);
MOCKPP_BOOST_TEST(ExpectationBoundary_test::fail_delta2);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ExpectationBoundary_test);

#endif // frameworks



void ExpectationBoundary_test::test_name()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("testRange"), 0);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == (MOCKPP_PCHAR("testRange")));
}


void ExpectationBoundary_test::test_hasExpectations()
{
  {
    MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("testRange"), 0);
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.setExpected(2, 4);
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MOCKPP_NS::ExpectationBoundary<int> ec2 (MOCKPP_PCHAR("testRange"), 0);
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void ExpectationBoundary_test::test_clear()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setExpected(3, 5);
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
}


void ExpectationBoundary_test::fail_clearActual()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("testRange"), 0);
  ec.setExpected(3, 5);
  ec.setActual(4);
  ec.clearActual();
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::fail_ifNoActual()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingRange"), 0);
  ec.setExpected(1, 2);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify();  // fails because no value
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::test_ifNoExpected()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingRange"), 0);
  ec.setActual(3); // no fail yet
}


static void doNothing(MOCKPP_NS::ExpectationBoundary<int> &ec)
{
  ec.setExpected(3, 5);
  ec.setExpectNothing();
}


void ExpectationBoundary_test::test_expectNothing()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingRange"), 0);
  doNothing(ec);
  ec.verify();  // no fail
}


void ExpectationBoundary_test::test_expectNothing_revoked()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingRange"), 0);
  ec.setExpectNothing();
  ec.setExpected(3, 5);
  ec.setActual(4);
  ec.verify();  // no fail
}


void ExpectationBoundary_test::fail_expectNothing()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("nothingRange"), 0);
  doNothing(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(3);  // fails even if equal
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


static void doActualImmediate(MOCKPP_NS::ExpectationBoundary<int> &ec)
{
  ec.setExpected(1233, 1235);
  ec.setActual(1234);
}


void ExpectationBoundary_test::test_expectActualImmediate()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("actualRange"), 0);
  doActualImmediate(ec);
  ec.verify(); // no fail
}


void ExpectationBoundary_test::fail_expectActualImmediate()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("actualRange"), 0);
  doActualImmediate(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(4321); // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


static void doActualVerify(MOCKPP_NS::ExpectationBoundary<int> &ec)
{
  ec.setExpected(1234, 1235);
  ec.setFailOnVerify();
  ec.setActual(4321); // should not fail
}


void ExpectationBoundary_test::test_expectActualVerify()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("verifyRange"), 0);
  doActualVerify(ec); // no fail
}


void ExpectationBoundary_test::fail_expectActualVerify()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("verifyRange"), 0);
  doActualVerify(ec); // no fail
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify(); // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::test_innerRanges()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("innerRange"), 0);

  ec.setExpected(100-5, 100+5);
  ec.setActual(105);
  ec.setActual(95);
}


void ExpectationBoundary_test::test_delta()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("delta"), 0);

  ec.setExpectedDelta(100, 5);
  ec.setActual(105);
  ec.setActual(95);
}


void ExpectationBoundary_test::fail_delta1()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("delta1"), 0);

  ec.setExpectedDelta(100, -5);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(106);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::fail_delta2()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("delta2"), 0);

  ec.setExpectedDelta(100, -5);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(94);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::fail_positiveDelta()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("posDelta"), 0);

  ec.setExpected(100-5, 100+5);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(94);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::fail_negativeDelta()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("negDelta"), 0);

  ec.setExpected(100-5, 100+5);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setActual(106);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationBoundary_test::fail_bounds()
{
  MOCKPP_NS::ExpectationBoundary<int> ec (MOCKPP_PCHAR("fail_bounds"), 0);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.setExpected(101, 99);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}




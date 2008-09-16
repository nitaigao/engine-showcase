/***************************************************************************
   ExpectationValue_test.cpp  -  unit tests for ExpectationValue class
                             -------------------
    begin                : Sat Dez 28 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ExpectationValue_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/ExpectationValue.h>
#include <mockpp/compat/Asserter.h>


class ExpectationValue_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ExpectationValue_test );

  CPPUNIT_TEST(test_hasExpectations);
  CPPUNIT_TEST(test_name);
  CPPUNIT_TEST(test_clear);

  CPPUNIT_TEST(test_expectNothing);
  CPPUNIT_TEST(test_expectNothing_revoked);
  CPPUNIT_TEST(test_expectActualImmediate);
  CPPUNIT_TEST(test_expectActualVerify);
  CPPUNIT_TEST(test_ifNoExpected);

  CPPUNIT_TEST(fail_clearActual);
  CPPUNIT_TEST(fail_expectActualImmediate);
  CPPUNIT_TEST(fail_expectActualVerify);
  CPPUNIT_TEST(fail_expectNothing);

  CPPUNIT_TEST(fail_ifNoActual);

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

  MOCKPP_TESTMETHOD_DECL void fail_clearActual();

  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();

  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual();
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ExpectationValue_test );

MOCKPP_CXXTEST(ExpectationValue_test, test_hasExpectations);
MOCKPP_CXXTEST(ExpectationValue_test, test_name);
MOCKPP_CXXTEST(ExpectationValue_test, test_clear);

MOCKPP_CXXTEST(ExpectationValue_test, test_expectNothing);
MOCKPP_CXXTEST(ExpectationValue_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(ExpectationValue_test, test_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationValue_test, test_expectActualVerify);
MOCKPP_CXXTEST(ExpectationValue_test, test_ifNoExpected);

MOCKPP_CXXTEST(ExpectationValue_test, fail_clearActual);
MOCKPP_CXXTEST(ExpectationValue_test, fail_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationValue_test, fail_expectActualVerify);
MOCKPP_CXXTEST(ExpectationValue_test, fail_expectNothing);

MOCKPP_CXXTEST(ExpectationValue_test, fail_ifNoActual);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ExpectationValue_test::test_hasExpectations);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_name);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_clear);

MOCKPP_BOOST_TEST(ExpectationValue_test::test_expectNothing);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationValue_test::test_ifNoExpected);

MOCKPP_BOOST_TEST(ExpectationValue_test::fail_clearActual);
MOCKPP_BOOST_TEST(ExpectationValue_test::fail_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationValue_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationValue_test::fail_expectNothing);

MOCKPP_BOOST_TEST(ExpectationValue_test::fail_ifNoActual);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ExpectationValue_test);

#endif // frameworks



void ExpectationValue_test::test_name()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("testValue"), 0);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == (MOCKPP_PCHAR("testValue")));
}


void ExpectationValue_test::test_hasExpectations()
{
  {
    MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("testValue"), 0);
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.setExpected(2);
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MOCKPP_NS::ExpectationValue<int> ec2 (MOCKPP_PCHAR("testValue"), 0);
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void ExpectationValue_test::test_clear()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setExpected(3);
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
  ec.verify();  // no fail because no expectations
}


void ExpectationValue_test::fail_clearActual()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("testValue"), 0);
  ec.setExpected(3);
  ec.setActual(3);
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


void ExpectationValue_test::test_ifNoExpected()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setActual(3); // fails because no expectation value
}


void ExpectationValue_test::fail_ifNoActual()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setExpected(3);
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


static void doNothing(MOCKPP_NS::ExpectationValue<int> &ec)
{
  ec.setExpected(3);
  ec.setExpectNothing();
}


void ExpectationValue_test::test_expectNothing()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  doNothing(ec);
  ec.verify();  // no fail
}


void ExpectationValue_test::test_expectNothing_revoked()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setExpectNothing();
  ec.setExpected(3);
  ec.setActual(3);
  ec.verify();  // no fail
}


void ExpectationValue_test::fail_expectNothing()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
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


static void doActualImmediate(MOCKPP_NS::ExpectationValue<int> &ec)
{
  ec.setExpected(1234);
  ec.setActual(1234);
}


void ExpectationValue_test::test_expectActualImmediate()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("actualValue"), 0);
  doActualImmediate(ec);
  ec.verify(); // no fail
}


void ExpectationValue_test::fail_expectActualImmediate()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("actualValue"), 0);
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


static void doActualVerify(MOCKPP_NS::ExpectationValue<int> &ec)
{
  ec.setExpected(1234);
  ec.setFailOnVerify();
  ec.setActual(4321); // should not fail
}


void ExpectationValue_test::test_expectActualVerify()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("verifyValue"), 0);
  doActualVerify(ec); // no fail
}


void ExpectationValue_test::fail_expectActualVerify()
{
  MOCKPP_NS::ExpectationValue<int> ec (MOCKPP_PCHAR("verifyValue"), 0);
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




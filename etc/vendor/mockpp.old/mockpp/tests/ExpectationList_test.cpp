/***************************************************************************
   ExpectationList_test.cpp  -  unit tests for ExpectationList class
                             -------------------
    begin                : Sat Dez 28 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ExpectationList_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#define MOCKPP_ENABLE_DEFAULT_FORMATTER

#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include "SelectFramework.h"

#include <mockpp/ExpectationList.h>
#include <mockpp/compat/Asserter.h>


class ExpectationList_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ExpectationList_test );

  CPPUNIT_TEST(test_chain);
  CPPUNIT_TEST(test_hasExpectations);
  CPPUNIT_TEST(test_name);
  CPPUNIT_TEST(test_many);
  CPPUNIT_TEST(test_balance);
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
  CPPUNIT_TEST(fail_many);

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

  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void fail_many();
  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void fail_clearActual();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();

  MOCKPP_TESTMETHOD_DECL void test_chain();
  MOCKPP_TESTMETHOD_DECL void test_balance();
  MOCKPP_TESTMETHOD_DECL void test_many();
  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual();
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ExpectationList_test );

MOCKPP_CXXTEST(ExpectationList_test, test_chain);
MOCKPP_CXXTEST(ExpectationList_test, test_hasExpectations);
MOCKPP_CXXTEST(ExpectationList_test, test_name);
MOCKPP_CXXTEST(ExpectationList_test, test_many);
MOCKPP_CXXTEST(ExpectationList_test, test_balance);
MOCKPP_CXXTEST(ExpectationList_test, test_clear);

MOCKPP_CXXTEST(ExpectationList_test, test_expectNothing);
MOCKPP_CXXTEST(ExpectationList_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(ExpectationList_test, test_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationList_test, test_expectActualVerify);
MOCKPP_CXXTEST(ExpectationList_test, test_ifNoExpected);

MOCKPP_CXXTEST(ExpectationList_test, fail_clearActual);
MOCKPP_CXXTEST(ExpectationList_test, fail_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationList_test, fail_expectActualVerify);
MOCKPP_CXXTEST(ExpectationList_test, fail_expectNothing);
MOCKPP_CXXTEST(ExpectationList_test, fail_many);

MOCKPP_CXXTEST(ExpectationList_test, fail_ifNoActual);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ExpectationList_test::test_chain);
MOCKPP_BOOST_TEST(ExpectationList_test::test_hasExpectations);
MOCKPP_BOOST_TEST(ExpectationList_test::test_name);
MOCKPP_BOOST_TEST(ExpectationList_test::test_many);
MOCKPP_BOOST_TEST(ExpectationList_test::test_balance);
MOCKPP_BOOST_TEST(ExpectationList_test::test_clear);

MOCKPP_BOOST_TEST(ExpectationList_test::test_expectNothing);
MOCKPP_BOOST_TEST(ExpectationList_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(ExpectationList_test::test_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationList_test::test_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationList_test::test_ifNoExpected);

MOCKPP_BOOST_TEST(ExpectationList_test::fail_clearActual);
MOCKPP_BOOST_TEST(ExpectationList_test::fail_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationList_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationList_test::fail_expectNothing);
MOCKPP_BOOST_TEST(ExpectationList_test::fail_many);

MOCKPP_BOOST_TEST(ExpectationList_test::fail_ifNoActual);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ExpectationList_test);

#endif // frameworks



void ExpectationList_test::test_chain()
{
  int exp_prep[10] = { 0, 1 };
  MOCKPP_NS::ExpectationList<int> ab (MOCKPP_PCHAR("testExpectationList"), 0);

  ab.addExpected(200)
    .addExpected(exp_prep+0, exp_prep+2)
    .addExpected(700);

  ab.addActual(200);
  ab.addActual(0);
  ab.addActual(1);
  ab.addActual(700);
  ab.verify();
}


void ExpectationList_test::test_name()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("testList"), 0);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == (MOCKPP_PCHAR("testList")));
}


void ExpectationList_test::test_hasExpectations()
{
  {
    MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("testList"), 0);
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.addExpected(2);
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MOCKPP_NS::ExpectationList<int> ec2 (MOCKPP_PCHAR("testList"), 0);
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void ExpectationList_test::test_clear()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.addExpected(1234);
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
  ec.verify();  // no fail because no expectations
}


void ExpectationList_test::fail_clearActual()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("testList"), 0);
  ec.addExpected(3);
  ec.addActual(3);
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


void ExpectationList_test::test_ifNoExpected()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingList"), 0);
  ec.addActual(3); // fails because no expectation value
}


void ExpectationList_test::fail_ifNoActual()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingList"), 0);
  ec.addExpected(3);
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


static void doNothing(MOCKPP_NS::ExpectationList<int> &ec)
{
  ec.addExpected(3);
  ec.setExpectNothing();
}


void ExpectationList_test::test_expectNothing()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingList"), 0);
  doNothing(ec);
  ec.verify();  // no fail
}


void ExpectationList_test::test_expectNothing_revoked()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingList"), 0);
  ec.setExpectNothing();
  ec.addExpected(3);
  ec.addActual(3);
  ec.verify();  // no fail
}


void ExpectationList_test::fail_expectNothing()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("nothingList"), 0);
  doNothing(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.addActual(3);  // fails even if equal
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


static void doActualImmediate(MOCKPP_NS::ExpectationList<int> &ec)
{
  ec.addExpected(1234);
  ec.addExpected(5678);
  ec.addExpected(8765);
  ec.addExpected(4321);

  ec.addActual(1234);
  ec.addActual(5678);
  ec.addActual(8765);
  ec.addActual(4321);
}


void ExpectationList_test::test_expectActualImmediate()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("actualList"), 0);
  doActualImmediate(ec);
  ec.verify(); // no fail
}


void ExpectationList_test::test_many()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("actualList"), 0);
  int prep[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  ec.addExpected(prep+0, prep+10);
  ec.addActual(prep+0, prep+10);
}


void ExpectationList_test::fail_many()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("actualList"), 0);

  int prep[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  ec.addExpected(prep+0, prep+10);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.addActual(prep+2, prep+10);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationList_test::fail_expectActualImmediate()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("actualList"), 0);
  doActualImmediate(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.addActual(4321); // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


static void doActualVerify(MOCKPP_NS::ExpectationList<int> &ec)
{
  ec.addExpected(1234);
  ec.setFailOnVerify();
  ec.addActual(4321); // should not fail
}


void ExpectationList_test::test_expectActualVerify()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("verifyList"), 0);
  doActualVerify(ec); // no fail
}


void ExpectationList_test::fail_expectActualVerify()
{
  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("verifyList"), 0);
  doActualVerify(ec); // no fail
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.verify();        // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationList_test::test_balance()
{
  MOCKPP_NS::ExpectationList<MOCKPP_STL::vector<int> >
    serialize_tracker(MOCKPP_PCHAR("serialize_tracker"), 0);

  MOCKPP_NS::ExpectationList<int> ec (MOCKPP_PCHAR("verifyList"), 0);

  ec.addExpected(1);
  ec.addExpected(2);
  ec.addExpected(3);
  ec.addExpected(4);
  ec.addExpected(5);

  ec.balanceActual();  // copy last element
  ec.balanceActual();  // copy last element
  ec.balanceActual();  // copy last element
  ec.balanceActual();  // copy last element
  ec.balanceActual();  // copy last element

  ec.verify();
}




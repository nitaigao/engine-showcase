/***************************************************************************
   ExpectationCounter_test.cpp  -  unit tests for ExpectationCounter class
                             -------------------
    begin                : Sat Dez 28 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ExpectationCounter_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
 * along with this program; if not,
 write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/

#include <mockpp/mockpp.h> // always first

#include "SelectFramework.h"

#include <mockpp/ExpectationCounter.h>
#include <mockpp/compat/Asserter.h>


class ExpectationCounter_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ExpectationCounter_test );

 CPPUNIT_TEST(test_hasExpectations);
 CPPUNIT_TEST(test_name);
 CPPUNIT_TEST(test_clearActual);
 CPPUNIT_TEST(test_clear);

 CPPUNIT_TEST(test_expectNothing);
 CPPUNIT_TEST(test_expectNothing_revoked);
 CPPUNIT_TEST(test_expectActualImmediate);
 CPPUNIT_TEST(test_expectActualVerify);
 CPPUNIT_TEST(test_ifNoExpected);

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

  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void test_clearActual();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();

  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual();
  MOCKPP_TESTMETHOD_DECL void doNothing(MOCKPP_NS::ExpectationCounter &ec);
  MOCKPP_TESTMETHOD_DECL void doActualImmediate(MOCKPP_NS::ExpectationCounter &ec);
  MOCKPP_TESTMETHOD_DECL void doActualVerify(MOCKPP_NS::ExpectationCounter &ec);
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ExpectationCounter_test );

MOCKPP_CXXTEST(ExpectationCounter_test, test_hasExpectations);
MOCKPP_CXXTEST(ExpectationCounter_test, test_name);
MOCKPP_CXXTEST(ExpectationCounter_test, test_clearActual);
MOCKPP_CXXTEST(ExpectationCounter_test, test_clear);

MOCKPP_CXXTEST(ExpectationCounter_test, test_expectNothing);
MOCKPP_CXXTEST(ExpectationCounter_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(ExpectationCounter_test, test_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationCounter_test, test_expectActualVerify);
MOCKPP_CXXTEST(ExpectationCounter_test, test_ifNoExpected);

MOCKPP_CXXTEST(ExpectationCounter_test, fail_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationCounter_test, fail_expectActualVerify);
MOCKPP_CXXTEST(ExpectationCounter_test, fail_expectNothing);

MOCKPP_CXXTEST(ExpectationCounter_test, fail_ifNoActual);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ExpectationCounter_test::test_hasExpectations);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_name);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_clearActual);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_clear);

MOCKPP_BOOST_TEST(ExpectationCounter_test::test_expectNothing);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationCounter_test::test_ifNoExpected);

MOCKPP_BOOST_TEST(ExpectationCounter_test::fail_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationCounter_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationCounter_test::fail_expectNothing);

MOCKPP_BOOST_TEST(ExpectationCounter_test::fail_ifNoActual);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ExpectationCounter_test);

#endif // frameworks



void ExpectationCounter_test::test_name()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("testcounter"), 0);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == (MOCKPP_PCHAR("testcounter")));
}


void ExpectationCounter_test::test_hasExpectations()
{
  {
    MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("testcounter"), 0);
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.setExpected(2);
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MOCKPP_NS::ExpectationCounter ec2 (MOCKPP_PCHAR("testcounter"), 0);
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void ExpectationCounter_test::test_clear()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingValue"), 0);
  ec.setExpected(3);
  ec.inc();
  ec.reset();
  ec.verify();  // no fail because no expectations
}


void ExpectationCounter_test::test_clearActual()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("testcounter"), 0);
  ec.setExpected(3);
  ec.inc();
  ec.inc();
  ec.inc();   // no fail anyway
  ec.clearActual();
  ec.inc();
  ec.inc();
  ec.inc();   // no fail even now
  ec.verify();
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
}


void ExpectationCounter_test::test_ifNoExpected()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingcounter"), 0);
  ec.inc(); // no fail yet
}


void ExpectationCounter_test::fail_ifNoActual()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingcounter"), 0);
  ec.setExpected(2);
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


void ExpectationCounter_test::doNothing(MOCKPP_NS::ExpectationCounter &ec)
{
  ec.setExpected(3);
  ec.setExpectNothing();
}


void ExpectationCounter_test::test_expectNothing()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingcounter"), 0);
  doNothing(ec);
  ec.verify();  // no fail
}


void ExpectationCounter_test::test_expectNothing_revoked()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingcounter"), 0);
  ec.setExpectNothing();
  ec.setExpected(3);
  ec.inc();
  ec.inc();
  ec.inc();
  ec.verify();  // no fail
}


void ExpectationCounter_test::fail_expectNothing()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("nothingcounter"), 0);
  doNothing(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.inc();  // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationCounter_test::doActualImmediate(MOCKPP_NS::ExpectationCounter &ec)
{
  ec.setExpected(2);
  ec.inc();
  ec.inc();
}


void ExpectationCounter_test::test_expectActualImmediate()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("actualcounter"), 0);
  doActualImmediate(ec);
  ec.verify(); // no fail
}


void ExpectationCounter_test::fail_expectActualImmediate()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("actualcounter"), 0);
  doActualImmediate(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.inc();   // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationCounter_test::doActualVerify(MOCKPP_NS::ExpectationCounter &ec)
{
  ec.setExpected(2);
  ec.setFailOnVerify();
  ec.inc();
  ec.inc();
  ec.inc();
  ec.inc();
}


void ExpectationCounter_test::test_expectActualVerify()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("verifycounter"), 0);
  doActualVerify(ec); // no fail
}


void ExpectationCounter_test::fail_expectActualVerify()
{
  MOCKPP_NS::ExpectationCounter ec (MOCKPP_PCHAR("verifycounter"), 0);
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




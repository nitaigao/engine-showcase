/***************************************************************************
   ExpectationMap_test.cpp  -  unit tests for ExpectationMap class
                             -------------------
    begin                : Sat Dez 28 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ExpectationMap_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include MOCKPP_VECTOR_H

#include "SelectFramework.h"

#include <mockpp/ExpectationMap.h>
#include <mockpp/compat/Asserter.h>


class ExpectationMap_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ExpectationMap_test );

  CPPUNIT_TEST(test_chain);
  CPPUNIT_TEST(test_hasExpectations);
  CPPUNIT_TEST(test_name);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_many);
  CPPUNIT_TEST(test_get);
  CPPUNIT_TEST(test_missing);
  CPPUNIT_TEST(test_pair_get);
  CPPUNIT_TEST(test_pair_getmany);

  CPPUNIT_TEST(test_expectNothing);
  CPPUNIT_TEST(test_expectNothing_revoked);
  CPPUNIT_TEST(test_expectActualImmediate);
  CPPUNIT_TEST(test_expectActualVerify);
  CPPUNIT_TEST(test_ifNoExpected);

  CPPUNIT_TEST(fail_expectActualImmediate);
  CPPUNIT_TEST(fail_expectActualVerify);
  CPPUNIT_TEST(fail_expectNothing);
  CPPUNIT_TEST(fail_many);

  CPPUNIT_TEST(fail_ifNoActual);
  CPPUNIT_TEST(fail_clearActual);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void test_expectNothing_revoked();
  MOCKPP_TESTMETHOD_DECL void test_expectNothing();
  MOCKPP_TESTMETHOD_DECL void fail_expectNothing();

  MOCKPP_TESTMETHOD_DECL void test_expectActualImmediate();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualImmediate();

  MOCKPP_TESTMETHOD_DECL void test_expectActualVerify();
  MOCKPP_TESTMETHOD_DECL void fail_expectActualVerify();

  MOCKPP_TESTMETHOD_DECL void test_chain();
  MOCKPP_TESTMETHOD_DECL void fail_many();
  MOCKPP_TESTMETHOD_DECL void fail_clearActual();
  MOCKPP_TESTMETHOD_DECL void test_name();
  MOCKPP_TESTMETHOD_DECL void test_missing();
  MOCKPP_TESTMETHOD_DECL void test_get();
  MOCKPP_TESTMETHOD_DECL void test_pair_get();
  MOCKPP_TESTMETHOD_DECL void test_pair_getmany();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_hasExpectations();
  MOCKPP_TESTMETHOD_DECL void test_ifNoExpected();

  MOCKPP_TESTMETHOD_DECL void test_many();
  MOCKPP_TESTMETHOD_DECL void fail_ifNoActual();
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ExpectationMap_test );

MOCKPP_CXXTEST(ExpectationMap_test, test_chain);
MOCKPP_CXXTEST(ExpectationMap_test, test_hasExpectations);
MOCKPP_CXXTEST(ExpectationMap_test, test_name);
MOCKPP_CXXTEST(ExpectationMap_test, test_clear);
MOCKPP_CXXTEST(ExpectationMap_test, test_many);
MOCKPP_CXXTEST(ExpectationMap_test, test_get);
MOCKPP_CXXTEST(ExpectationMap_test, test_missing);
MOCKPP_CXXTEST(ExpectationMap_test, test_pair_get);
MOCKPP_CXXTEST(ExpectationMap_test, test_pair_getmany);

MOCKPP_CXXTEST(ExpectationMap_test, test_expectNothing);
MOCKPP_CXXTEST(ExpectationMap_test, test_expectNothing_revoked);
MOCKPP_CXXTEST(ExpectationMap_test, test_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationMap_test, test_expectActualVerify);
MOCKPP_CXXTEST(ExpectationMap_test, test_ifNoExpected);

MOCKPP_CXXTEST(ExpectationMap_test, fail_expectActualImmediate);
MOCKPP_CXXTEST(ExpectationMap_test, fail_expectActualVerify);
MOCKPP_CXXTEST(ExpectationMap_test, fail_expectNothing);
MOCKPP_CXXTEST(ExpectationMap_test, fail_many);

MOCKPP_CXXTEST(ExpectationMap_test, fail_ifNoActual);
MOCKPP_CXXTEST(ExpectationMap_test, fail_clearActual);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ExpectationMap_test::test_chain);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_hasExpectations);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_name);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_clear);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_many);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_get);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_missing);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_pair_get);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_pair_getmany);

MOCKPP_BOOST_TEST(ExpectationMap_test::test_expectNothing);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_expectNothing_revoked);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationMap_test::test_ifNoExpected);

MOCKPP_BOOST_TEST(ExpectationMap_test::fail_expectActualImmediate);
MOCKPP_BOOST_TEST(ExpectationMap_test::fail_expectActualVerify);
MOCKPP_BOOST_TEST(ExpectationMap_test::fail_expectNothing);
MOCKPP_BOOST_TEST(ExpectationMap_test::fail_many);

MOCKPP_BOOST_TEST(ExpectationMap_test::fail_ifNoActual);
MOCKPP_BOOST_TEST(ExpectationMap_test::fail_clearActual);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ExpectationMap_test);

#endif // frameworks



void ExpectationMap_test::test_chain()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  MOCKPP_STL::vector<MOCKPP_STL::pair<int, MOCKPP_STL::string> > vp;
  vp.push_back(MOCKPP_STL::make_pair(3, (const char*)"three")); // gcc2.95 needs this "const char*" thing
  vp.push_back(MOCKPP_STL::make_pair(4, (const char*)"four"));

  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string> ab =
    MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>
      (MOCKPP_PCHAR("testExpectationMap"), 0)
        .addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(6,"six"))
        .addExpected(vp.begin(), vp.end())
        .addExpectedMissing(100)
        .addExpected(10, "ten")
        .addExpected(11, "elf");

  ab.addActual(6);
  ab.addActual(3);
  ab.addActual(4);
  ab.addActual(100);
  ab.addActual(10);
  ab.addActual(11);
  ab.verify();
}


void ExpectationMap_test::test_name()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("testMap"), 0);
  MOCKPP_ASSERT_TRUE(ec.getVerifiableName() == MOCKPP_PCHAR("testMap"));
}


void ExpectationMap_test::test_hasExpectations()
{
  {
    MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("testMap"), 0);
    MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
    ec.addExpected(2, "two");
    MOCKPP_ASSERT_TRUE(true == ec.hasExpectations());
  }

  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec2 (MOCKPP_PCHAR("testMap"), 0);
  MOCKPP_ASSERT_TRUE(false == ec2.hasExpectations());
  ec2.setExpectNothing();
  MOCKPP_ASSERT_TRUE(true == ec2.hasExpectations());
}


void ExpectationMap_test::test_clear()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("testMap"), 0);
  ec.addExpected(3, "three");
  ec.reset();
  MOCKPP_ASSERT_TRUE(false == ec.hasExpectations());
}


void ExpectationMap_test::fail_clearActual()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("testMap"), 0);
  ec.addExpected(3, "three");
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


void ExpectationMap_test::test_ifNoExpected()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("nothingMap"), 0);
  ec.addActual(3); // fails because no expectation value
}


void ExpectationMap_test::fail_ifNoActual()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("nothingMap"), 0);
  ec.addExpected(3, "three");
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


static void doNothing(MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  &ec)
{
  ec.addExpected(3, "three");
  ec.setExpectNothing();
}


void ExpectationMap_test::test_expectNothing()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("nothingMap"), 0);
  doNothing(ec);
  ec.verify();  // no fail
}


void ExpectationMap_test::test_expectNothing_revoked()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("nothingMap"), 0);
  ec.setExpectNothing();
  ec.addExpected(3, "three");
  ec.addActual(3);
  ec.verify();  // no fail
}


void ExpectationMap_test::fail_expectNothing()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("nothingMap"), 0);
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


void ExpectationMap_test::test_get()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  ec.addExpected(3, "three");
  ec.addExpected(4, "four");
  MOCKPP_ASSERT_TRUE("three" == ec.get(3));
}


void ExpectationMap_test::test_pair_get()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  ec.addExpected(MOCKPP_STL::make_pair(3, (const char*)"three")); // gcc2.95 needs this "const char*" thing
  ec.addExpected(MOCKPP_STL::make_pair(4, (const char*)"four"));
  MOCKPP_ASSERT_TRUE("three" == ec.get(3));
  MOCKPP_ASSERT_TRUE("four" == ec.get(4));
}


void ExpectationMap_test::test_pair_getmany()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  MOCKPP_STL::vector<MOCKPP_STL::pair<int, MOCKPP_STL::string> > vp;
  vp.push_back(MOCKPP_STL::make_pair(3, (const char*)"three")); // gcc2.95 needs this "const char*" thing
  vp.push_back(MOCKPP_STL::make_pair(4, (const char*)"four"));
  vp.push_back(MOCKPP_STL::make_pair(5, (const char*)"five"));
  vp.push_back(MOCKPP_STL::make_pair(6, (const char*)"six"));
  vp.push_back(MOCKPP_STL::make_pair(7, (const char*)"seven"));
  ec.addExpected(vp.begin(), vp.end());

  MOCKPP_ASSERT_TRUE("three" == ec.get(3));
  MOCKPP_ASSERT_TRUE("four" == ec.get(4));
}


void ExpectationMap_test::test_many()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);

  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(6,"zero"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(5, "one"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(4, "two"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(3, "three"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(2, "four"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(1, "five"));

  ec.addActual(1);
  ec.addActual(2);
  ec.addActual(3);
  ec.addActual(4);
  ec.addActual(5);
  ec.addActual(6);
}


void ExpectationMap_test::fail_many()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);

  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(6,"zero"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(5, "one"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(4, "two"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(3, "three"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(2, "four"));
  ec.addExpected(MOCKPP_STL::pair<int, MOCKPP_STL::string>(1, "five"));

  bool thrown = false;
  MOCKPP_TRY
  {
    ec.addActual(10);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


static void doActualImmediate(MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  &ec)
{
  ec.addExpected(1234, "1234");
  ec.addExpected(5678, "5678");
  ec.addExpected(8765, "8765");
  ec.addExpected(4321, "1234");

  ec.addActual(1234);
  ec.addActual(5678);
  ec.addActual(8765);
  ec.addActual(4321);
}


void ExpectationMap_test::test_expectActualImmediate()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  doActualImmediate(ec);
  ec.verify(); // no fail
}


void ExpectationMap_test::fail_expectActualImmediate()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  doActualImmediate(ec);
  bool thrown = false;
  MOCKPP_TRY
  {
    ec.addActual(1111); // fails
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void ExpectationMap_test::test_missing()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("actualMap"), 0);
  ec.addExpectedMissing(1234);
  ec.addExpectedMissing(5678);
  ec.addExpectedMissing(8765);
  ec.addExpectedMissing(4321);

  ec.addActual(1234);
  ec.addActual(5678);
  ec.addActual(8765);
  ec.addActual(4321);
  ec.verify(); // no fail
}


static void doActualVerify(MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  &ec)
{
  ec.addExpected(1234, "1234");
  ec.setFailOnVerify();
  ec.addActual(4321); // should not fail
}


void ExpectationMap_test::test_expectActualVerify()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("verifyMap"), 0);
  doActualVerify(ec); // no fail
}


void ExpectationMap_test::fail_expectActualVerify()
{
  MOCKPP_NS::ExpectationMap<int, MOCKPP_STL::string>  ec (MOCKPP_PCHAR("verifyMap"), 0);
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




/***************************************************************************
             MockObject_test.cpp  -  unit tests for MockObject class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: MockObject_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <mockpp/MockObject.h>


class MockObject_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( MockObject_test );

  CPPUNIT_TEST(test_left_deep_verify);
  CPPUNIT_TEST(test_right_deep_verify);
  CPPUNIT_TEST(test_deep_verify);

  CPPUNIT_TEST(fail_left_deep_verify);
  CPPUNIT_TEST(fail_right_deep_verify);
  CPPUNIT_TEST(fail_deep_verify_1);
  CPPUNIT_TEST(fail_deep_verify_2);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_left_deep_verify();
  MOCKPP_TESTMETHOD_DECL void test_right_deep_verify();

  MOCKPP_TESTMETHOD_DECL void fail_left_deep_verify();
  MOCKPP_TESTMETHOD_DECL void fail_right_deep_verify();

  MOCKPP_TESTMETHOD_DECL void test_deep_verify();

  MOCKPP_TESTMETHOD_DECL void fail_deep_verify_1();
  MOCKPP_TESTMETHOD_DECL void fail_deep_verify_2();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( MockObject_test );

MOCKPP_CXXTEST(MockObject_test, test_left_deep_verify);
MOCKPP_CXXTEST(MockObject_test, test_right_deep_verify);
MOCKPP_CXXTEST(MockObject_test, test_deep_verify);

MOCKPP_CXXTEST(MockObject_test, fail_left_deep_verify);
MOCKPP_CXXTEST(MockObject_test, fail_right_deep_verify);
MOCKPP_CXXTEST(MockObject_test, fail_deep_verify_1);
MOCKPP_CXXTEST(MockObject_test, fail_deep_verify_2);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(MockObject_test::test_left_deep_verify);
MOCKPP_BOOST_TEST(MockObject_test::test_right_deep_verify);
MOCKPP_BOOST_TEST(MockObject_test::test_deep_verify);

MOCKPP_BOOST_TEST(MockObject_test::fail_left_deep_verify);
MOCKPP_BOOST_TEST(MockObject_test::fail_right_deep_verify);
MOCKPP_BOOST_TEST(MockObject_test::fail_deep_verify_1);
MOCKPP_BOOST_TEST(MockObject_test::fail_deep_verify_2);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (MockObject_test);

#endif // frameworks



class MyMockObject : public MOCKPP_NS::MockObject
{
  public:
    typedef MOCKPP_NS::ExpectationValue<int> evi_t;

    MyMockObject (MOCKPP_NS::VerifiableList *parent = 0)
     : MOCKPP_NS::MockObject(MOCKPP_PCHAR("mo"), parent),
       lev1a(MOCKPP_PCHAR("lev1a"), this),
       lev1b(MOCKPP_PCHAR("lev1b"), this)
    {
    }


    void execTest()
    {
      lev1a.setFailOnVerify();
      lev1a.setExpected(12345);
      lev1a.setActual(12345);
    }


    void execFail()
    {
      lev1a.setFailOnVerify();
      lev1a.setExpected(12345);
      lev1a.setActual(54321);
    }


    // make public for test purposes
    unsigned numVerifiables() const
    {
# ifdef _MSC_VER
      return VerifiableList::numVerifiables();
#else
      return MOCKPP_NS::VerifiableList::numVerifiables();
#endif
    }

  private:
    evi_t lev1a;
    evi_t lev1b;
};


void MockObject_test::test_left_deep_verify()
{
  MyMockObject mo_rt;
  MyMockObject mo_left(&mo_rt);
  MyMockObject mo_right(&mo_rt);

  MOCKPP_ASSERT_TRUE(4 == mo_rt.numVerifiables()); // two internal, two external children

  mo_left.execTest();
  mo_rt.verify();
}


void MockObject_test::test_right_deep_verify()
{
  MyMockObject mo_rt;
  MyMockObject mo_left(&mo_rt);
  MyMockObject mo_right(&mo_rt);

  MOCKPP_ASSERT_TRUE(4 == mo_rt.numVerifiables()); // two internal, two external children

  mo_right.execTest();
  mo_rt.verify();
}


void MockObject_test::fail_left_deep_verify()
{
  MyMockObject mo_rt;
  MyMockObject mo_left(&mo_rt);
  MyMockObject mo_right(&mo_rt);

  mo_left.execFail();
  bool thrown = false;
  MOCKPP_TRY
  {
    mo_rt.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void MockObject_test::fail_right_deep_verify()
{
  MyMockObject mo_rt;
  MyMockObject mo_left(&mo_rt);
  MyMockObject mo_right(&mo_rt);

  mo_right.execFail();
  bool thrown = false;
  MOCKPP_TRY
  {
    mo_rt.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void MockObject_test::test_deep_verify()
{
  typedef MOCKPP_NS::ExpectationValue<int> evi_t;

  MOCKPP_NS::MockObject root(MOCKPP_PCHAR("root"), 0);

  MOCKPP_NS::MockObject lev1a(MOCKPP_PCHAR("lev1a"), &root);
  MOCKPP_NS::MockObject lev1b(MOCKPP_PCHAR("lev1b"), &root);

  evi_t lev21a(MOCKPP_PCHAR("lev21a"), &lev1a);
  evi_t lev21b(MOCKPP_PCHAR("lev21b"), &lev1a);

  evi_t lev22a(MOCKPP_PCHAR("lev22a"), &lev1b);
  evi_t lev22b(MOCKPP_PCHAR("lev22b"), &lev1b);

  lev22a.setFailOnVerify();
  lev22a.setExpected(12345);
  lev22a.setActual(12345);

  lev21a.setFailOnVerify();
  lev21a.setExpected(12345);
  lev21a.setActual(12345);

  root.verify(); // follow chain without fail
}


void MockObject_test::fail_deep_verify_1()
{
  typedef MOCKPP_NS::ExpectationValue<int> evi_t;

  MOCKPP_NS::MockObject root(MOCKPP_PCHAR("root"), 0);

  MOCKPP_NS::MockObject lev1a(MOCKPP_PCHAR("lev1a"), &root);
  MOCKPP_NS::MockObject lev1b(MOCKPP_PCHAR("lev1b"), &root);

  evi_t lev21a(MOCKPP_PCHAR("lev21a"), &lev1a);
  evi_t lev21b(MOCKPP_PCHAR("lev21b"), &lev1a);

  evi_t lev22a(MOCKPP_PCHAR("lev22a"), &lev1b);
  evi_t lev22b(MOCKPP_PCHAR("lev22b"), &lev1b);

  lev21a.setFailOnVerify();
  lev21a.setExpected(12345);
  lev21a.setActual(54321);

  bool thrown = false;
  MOCKPP_TRY
  {
    root.verify();  // follow chain and fail
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void MockObject_test::fail_deep_verify_2()
{
  typedef MOCKPP_NS::ExpectationValue<int> evi_t;

  MOCKPP_NS::MockObject root(MOCKPP_PCHAR("root"), 0);

  MOCKPP_NS::MockObject lev1a(MOCKPP_PCHAR("lev1a"), &root);
  MOCKPP_NS::MockObject lev1b(MOCKPP_PCHAR("lev1b"), &root);

  evi_t lev21a(MOCKPP_PCHAR("lev21a"), &lev1a);
  evi_t lev21b(MOCKPP_PCHAR("lev21b"), &lev1a);

  evi_t lev22a(MOCKPP_PCHAR("lev22a"), &lev1b);
  evi_t lev22b(MOCKPP_PCHAR("lev22b"), &lev1b);

  lev22a.setFailOnVerify();
  lev22a.setExpected(12345);
  lev22a.setActual(54321);

  bool thrown = false;
  MOCKPP_TRY
  {
    root.verify();  // follow chain and fail
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}




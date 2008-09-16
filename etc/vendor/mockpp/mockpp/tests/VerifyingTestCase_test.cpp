/***************************************************************************
  VerifyingTestCase_test.cpp  -  unit tests for VerifyingTestCase class
                            -------------------
   begin                : Wed Sep 4 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

  $Id: VerifyingTestCase_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#if defined(MOCKPP_USE_CPPUNIT)

#include "SelectFramework.h"

#include <stdexcept>

#ifdef _MSC_VER // at least MSCV6 incomplete
# include <mockpp/util/AutoPointer.h>
#else
# include <memory>
#endif

#include <mockpp/framework/VerifyingTestCase.h>

#include <mockpp/ExpectationValue.h>

#include <mockpp/compat/Asserter.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>

MOCKPP_ANON_NS_START


class VerifyingTestCase_test : public MOCKPP_NS::VerifyingTestCase
{
  public:

    VerifyingTestCase_test();

CPPUNIT_TEST_SUITE( VerifyingTestCase_test );

      MOCKPP_TEST(test_register );
      MOCKPP_TEST(test_run );
      MOCKPP_TEST_EXCEPTION(test_throw, MOCKPP_NS::AssertionFailedError);

      MOCKPP_VERIFYING_TEST( test_ok );
      MOCKPP_VERIFYING_TEST_EXCEPTION( test_nok, MOCKPP_NS::AssertionFailedError);

CPPUNIT_TEST_SUITE_END();

  public:

    unsigned m_setup;
    unsigned m_teardown;

    virtual void setUp();
    virtual void tearDown();
   MOCKPP_TESTMETHOD_DECL void check_setupTeardown();

   MOCKPP_TESTMETHOD_DECL void test_register();
   MOCKPP_TESTMETHOD_DECL void test_run();
   MOCKPP_TESTMETHOD_DECL void test_throw();
   MOCKPP_TESTMETHOD_DECL void test_ok();
   MOCKPP_TESTMETHOD_DECL void test_nok();
};


#if defined (MOCKPP_USE_CXXTEST)

/*
MOCKPP_CXXTEST_SUITE_REGISTRATION( VerifyingTestCase_test );

MOCKPP_TEST(test_register );
MOCKPP_TEST(test_run );
MOCKPP_TEST_EXCEPTION(test_throw);

MOCKPP_VERIFYING_TEST( test_ok );
MOCKPP_VERIFYING_TEST_EXCEPTION( test_nok);
*/

#elif defined(MOCKPP_USE_BOOSTTEST)

/*
MOCKPP_TEST(test_register );
MOCKPP_TEST(test_run );
MOCKPP_TEST_EXCEPTION(test_throw);

MOCKPP_VERIFYING_TEST( test_ok );
MOCKPP_VERIFYING_TEST_EXCEPTION( test_nok);
*/

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( VerifyingTestCase_test );

#endif // frameworks



VerifyingTestCase_test::VerifyingTestCase_test()
{
  m_setup = 0;
  m_teardown = 0;
}

void VerifyingTestCase_test::check_setupTeardown()
{
  MOCKPP_ASSERT_TRUE(m_setup > 0);
  if (m_setup > 1)
    MOCKPP_ASSERT_TRUE(m_teardown > 0);
}


void VerifyingTestCase_test::tearDown()
{
//  m_teardown++;
}


void VerifyingTestCase_test::setUp()
{
  m_setup++;
}


void VerifyingTestCase_test::test_register()
{
  check_setupTeardown();

  MOCKPP_NS::VerifyingTestCase mtc("dummy");

  MOCKPP_TRY
  {
    mtc.verify();
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("no sub-objects to verify")) != MOCKPP_NS::String::npos);
  }
#endif
}


void VerifyingTestCase_test::test_run()
{
  check_setupTeardown();

  MOCKPP_NS::VerifyingTestCase mtc("dummy");
  MOCKPP_NS::ExpectationValue<int> ev(MOCKPP_PCHAR("ev"), &mtc) ;
  ev.setExpected(123);
  ev.setActual(123);
  mtc.verify();
}


void VerifyingTestCase_test::test_ok()
{
  check_setupTeardown();

  MOCKPP_NS::ExpectationValue<int> *ev = new MOCKPP_NS::ExpectationValue<int>(MOCKPP_PCHAR("ev"));
  registerVerifiable(ev);
  ev->setExpected(123);
  ev->setActual(123);

  MOCKPP_NS::ExpectationValue<int> evs(MOCKPP_PCHAR("ev"), this) ;
  evs.setExpected(123);
  evs.setActual(123);

  ev->verify();
}


void VerifyingTestCase_test::test_throw()
{
  check_setupTeardown();

  MOCKPP_ASSERT_TRUE(false);
}


void VerifyingTestCase_test::test_nok()
{
  check_setupTeardown();

  MOCKPP_NS::ExpectationValue<int> *ev = new MOCKPP_NS::ExpectationValue<int>(MOCKPP_PCHAR("ev"));
  registerVerifiable(ev);
  ev->setExpected(123);
  // missing actual
}

MOCKPP_NS_END

#endif // HACE_CPPUNIT




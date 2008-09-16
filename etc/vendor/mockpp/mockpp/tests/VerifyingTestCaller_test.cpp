/***************************************************************************
  VerifyingTestCaller_test.cpp  -  unit tests for VerifyingTestCaller class
                            -------------------
   begin                : Sun Jan 23 2005
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

  $Id: VerifyingTestCaller_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <stdexcept>

#if defined(_MSC_VER) && (_MSC_VER < 1300) // at least MSCV6 incomplete
# include <mockpp/util/AutoPointer.h>
#else
# include <memory>
#endif

#include <mockpp/compat/Asserter.h>

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

#include <mockpp/framework/VerifyingTestCaller.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>

#endif


MOCKPP_ANON_NS_START


class VerifyingTestCaller_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VerifyingTestCaller_test );

  CPPUNIT_TEST( test_run_ref );
  CPPUNIT_TEST( test_run_ptr );
  CPPUNIT_TEST( test_throw );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_run_ref();
   MOCKPP_TESTMETHOD_DECL void test_run_ptr();
   MOCKPP_TESTMETHOD_DECL void test_throw();
};


#if defined (MOCKPP_USE_CXXTEST)

/*
MOCKPP_CXXTEST_SUITE_REGISTRATION( VerifyingTestCaller_test );

MOCKPP_CXXTEST(VerifyingTestCaller_test,  test_run_ref );
MOCKPP_CXXTEST(VerifyingTestCaller_test,  test_run_ptr );
MOCKPP_CXXTEST(VerifyingTestCaller_test,  test_throw );
*/

#elif defined(MOCKPP_USE_BOOSTTEST)

/*
MOCKPP_BOOST_TEST(VerifyingTestCaller_test:: test_run_ref );
MOCKPP_BOOST_TEST(VerifyingTestCaller_test:: test_run_ptr );
MOCKPP_BOOST_TEST(VerifyingTestCaller_test:: test_throw );
*/

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( VerifyingTestCaller_test );


class VerifyingTestCaseDummy : public MOCKPP_NS::VerifyingTestCase
{
  public:

    mutable unsigned cnt;
    mutable unsigned clr;
    mutable unsigned ver;

    VerifyingTestCaseDummy( )
      : MOCKPP_NS::VerifyingTestCase("dummy")
      , cnt( 0 )
      , clr( 0 )
      , ver( 0 )
    {}

    virtual void verify()
    {
      ver = ++cnt;
    }

    virtual void clearVerifiables()
    {
      clr = ++cnt;
    }

   MOCKPP_TESTMETHOD_DECL void method()
    {
    }

   MOCKPP_TESTMETHOD_DECL void thrower()
    {
      MOCKPP_THROW(1);
    }
};


void VerifyingTestCaller_test::test_run_ref()
{
  VerifyingTestCaseDummy vd;
  MOCKPP_NS::VerifyingTestCaller<VerifyingTestCaseDummy, true>
    caller(MOCKPP_STD_NS::string("caller"), &VerifyingTestCaseDummy::method, vd);
  caller.runTest();
  MOCKPP_ASSERT_EQUALS((unsigned)2, vd.ver);
  MOCKPP_ASSERT_EQUALS((unsigned)3, vd.clr);
}


void VerifyingTestCaller_test::test_run_ptr()
{
  VerifyingTestCaseDummy *vd = new VerifyingTestCaseDummy;
  MOCKPP_NS::VerifyingTestCaller<VerifyingTestCaseDummy, true>
    caller(MOCKPP_STD_NS::string("caller"), &VerifyingTestCaseDummy::method, vd);
  caller.runTest();
  MOCKPP_ASSERT_EQUALS((unsigned)2, vd->ver);
  MOCKPP_ASSERT_EQUALS((unsigned)3, vd->clr);
}


void VerifyingTestCaller_test::test_throw()
{
  VerifyingTestCaseDummy vd;
  MOCKPP_NS::VerifyingTestCaller<VerifyingTestCaseDummy, true>
    caller("caller", &VerifyingTestCaseDummy::thrower, vd);
  try
  {
    caller.runTest();
  }
  catch(...)
  {
  }
  MOCKPP_ASSERT_EQUALS((unsigned)0, vd.ver);
  MOCKPP_ASSERT_EQUALS((unsigned)2, vd.clr);
}


#endif // frameworks


MOCKPP_NS_END




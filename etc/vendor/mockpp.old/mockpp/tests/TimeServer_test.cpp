/***************************************************************************
        TimeServer_test.cpp  -  unit tests for *TimeServer classes
                             -------------------
    begin                : Fri 06 05 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: TimeServer_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/production/TimeServer.h>
#include <mockpp/MockTimeServer.h>



MOCKPP_ANON_NS_START

class TimeServer_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( TimeServer_test );

  CPPUNIT_TEST(test_mock);
  CPPUNIT_TEST(test_real);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

 MOCKPP_TESTMETHOD_DECL void test_real();
 MOCKPP_TESTMETHOD_DECL void test_mock();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( TimeServer_test );
MOCKPP_CXXTEST(TimeServer_test, test_mock);
MOCKPP_CXXTEST(TimeServer_test, test_real);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(TimeServer_test::test_mock);
MOCKPP_BOOST_TEST(TimeServer_test::test_real);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (TimeServer_test);

#endif // frameworks



void TimeServer_test::test_mock()
{
  MOCKPP_NS::MockTimeServer rt;
  rt.addUnixTime(0x1234);
  rt.addUnixTime(0x5678);
  MOCKPP_NS::TimeServer *ts = &rt;

  MOCKPP_STD_NS::time_t t1 = ts->getUnixTime();
  MOCKPP_ASSERT_EQUALS((MOCKPP_STD_NS::time_t)0x1234, t1);

  t1 = ts->getUnixTime();
  MOCKPP_ASSERT_EQUALS((MOCKPP_STD_NS::time_t)0x5678, t1);
}


void TimeServer_test::test_real()
{
  MOCKPP_NS::RealTimeServer rt;
  MOCKPP_NS::TimeServer *ts = &rt;
  MOCKPP_STD_NS::time_t t1 = ts->getUnixTime();
  MOCKPP_STD_NS::time_t t2 = MOCKPP_STD_NS::time(0);

  MOCKPP_ASSERT_TRUE(t1       <= t2);  // up to +2s should always succeed
  MOCKPP_ASSERT_TRUE((t1 + 2) >= t2);
}


MOCKPP_NS_END




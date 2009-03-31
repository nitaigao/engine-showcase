/***************************************************************************
   ResponseVector_Test.cpp  -  unit tests for ResponseVector classes
                             -------------------
    begin                : Sun Oct 30 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ResponseVector_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it Or/or modify
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


#include <mockpp/visiting/VisitableMockObject.h>



class ResponseVector_Test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ResponseVector_Test );

  CPPUNIT_TEST(test_value);
  CPPUNIT_TEST(test_thrower);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_value();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ResponseVector_Test );
MOCKPP_CXXTEST(ResponseVector_Test, test_value);
MOCKPP_CXXTEST(ResponseVector_Test, test_thrower);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ResponseVector_Test::test_value);
MOCKPP_BOOST_TEST(ResponseVector_Test::test_thrower);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ResponseVector_Test);

#endif // frameworks



void ResponseVector_Test::test_value()
{
  MOCKPP_NS::ResponseVector3<long, int, int, int> rtv(MOCKPP_PCHAR("rtv3"), 0);
  rtv.add(MOCKPP_NS::make_throwable(int(123)), 10, 20, 30, 2);
  rtv.add(MOCKPP_NS::make_throwable(int(321)), 30, 20, 10, MOCKPP_UNLIMITED);

  rtv.add(long(456), 11, 21, 31, 2);
  rtv.add(long(654), 31, 21, 11, MOCKPP_UNLIMITED);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(t->throw_me(),
                        MOCKPP_PCHAR("321"),
                         int,
                         int(321));
#endif

  long r;
  MOCKPP_ASSERT_FALSE(rtv.find(r, 32, 22, 12));
  MOCKPP_ASSERT_TRUE(rtv.find(r, 31, 21, 11));
  MOCKPP_ASSERT_TRUE(r == 654);

  MOCKPP_ASSERT_TRUE(rtv.find(r, 11, 21, 31));
  MOCKPP_ASSERT_TRUE(r == 456);

  MOCKPP_ASSERT_TRUE(rtv.find(r, 31, 21, 11));
  MOCKPP_ASSERT_TRUE(r == 654);

  MOCKPP_ASSERT_TRUE(rtv.find(r, 11, 21, 31));
  MOCKPP_ASSERT_TRUE(r == 456);

  MOCKPP_ASSERT_TRUE(rtv.find(r, 31, 21, 11));
  MOCKPP_ASSERT_TRUE(r == 654);

  MOCKPP_ASSERT_FALSE(rtv.find(r, 11, 21, 31));

  MOCKPP_ASSERT_FALSE(rtv.find(t, 10, 10, 10));

  MOCKPP_ASSERT_TRUE(rtv.find(t, 10, 20, 30));
  MOCKPP_ASSERT_TRUE(rtv.find(t, 10, 20, 30));
  MOCKPP_ASSERT_FALSE(rtv.find(t, 10, 20, 30));

  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
}


void ResponseVector_Test::test_thrower()
{
  MOCKPP_NS::ResponseThrowableVector3<int, int, int> rtv(MOCKPP_PCHAR("rtv3"), 0);

  rtv.add(MOCKPP_NS::make_throwable(int(123)), 10, 20, 30, 1);
  rtv.add(MOCKPP_NS::make_throwable(int(321)), 30, 20, 10, MOCKPP_UNLIMITED);
  rtv.add(0, 10, 10, 10, MOCKPP_UNLIMITED);

  MOCKPP_NS::Throwable *t;

  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(t->throw_me(),
                        MOCKPP_PCHAR("321"),
                         int,
                         int(321));
#endif

  MOCKPP_ASSERT_TRUE(rtv.find(t, 10, 20, 30));
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(t->throw_me(),
                        MOCKPP_PCHAR("123"),
                         int,
                         int(123));
#endif

  MOCKPP_ASSERT_FALSE(rtv.find(t, 10, 10, 10));

  MOCKPP_ASSERT_FALSE(rtv.find(t, 10, 20, 30));

  MOCKPP_ASSERT_TRUE(rtv.find(t, 30, 20, 10));
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(t->throw_me(),
                        MOCKPP_PCHAR("321"),
                         int,
                         int(321));
#endif
}





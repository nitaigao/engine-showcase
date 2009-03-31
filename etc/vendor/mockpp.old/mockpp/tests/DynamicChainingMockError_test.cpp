/***************************************************************************
DynamicChainingMockError_test.cpp  -  unit tests for DynamicChainingMockError class
                             -------------------
    begin                : Wed Sep 4 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: DynamicChainingMockError_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/DynamicChainingMockError.h>
#include <mockpp/chaining/AbstractDynamicChainingMock.h>
#include <mockpp/chaining/FIFOInvocationDispatcher.h>
#include <mockpp/chaining/LIFOInvocationDispatcher.h>

#include <mockpp/constraint/ConstraintSet.h>



MOCKPP_ANON_NS_START


class DynamicChainingMockError_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( DynamicChainingMockError_test );

  CPPUNIT_TEST(test_describe);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_describe();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( DynamicChainingMockError_test );
MOCKPP_CXXTEST(DynamicChainingMockError_test, test_describe);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(DynamicChainingMockError_test::test_describe);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (DynamicChainingMockError_test);

#endif // frameworks


template <typename R, typename I>
class AID : public MOCKPP_NS::AbstractInvocationDispatcher<R, I>
{
  public:

    R dispatch_result;

    virtual R dispatch( const I &invocation )
    {
      return this->defaultStub->invoke(invocation);
    }
};


void DynamicChainingMockError_test::test_describe()
{
  typedef MOCKPP_NS::Invocation0 SetterInvocation;

  MOCKPP_NS::AbstractDynamicChainingMock<int, SetterInvocation>
     adc (new MOCKPP_NS::LIFOInvocationDispatcher<int, SetterInvocation>, MOCKPP_PCHAR("adcm-name"), 0);

  SetterInvocation invoc(MOCKPP_PCHAR("InvocationDummy::setProp"));

  AID<int, SetterInvocation> dispatcher;

  MOCKPP_NS::DynamicChainingMockError<int, SetterInvocation>
    dcme (&adc, invoc, &dispatcher, MOCKPP_PCHAR("error-description") );

  MOCKPP_NS::String ref (MOCKPP_PCHAR("adcm-name: error-description\n")
                      MOCKPP_PCHAR("Invoked: InvocationDummy::setProp()\n")
                      MOCKPP_PCHAR("Allowed: ")
                      MOCKPP_PCHAR("no expectations set"));
  MOCKPP_ASSERT_TRUE(dcme.getMessage() == ref);
  MOCKPP_NS::String s;
  MOCKPP_ASSERT_TRUE(dcme.writeTo(s) == ref);
  MOCKPP_ASSERT_TRUE(s == ref);
}


MOCKPP_NS_END




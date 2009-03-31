/***************************************************************************
    StubBuilder_test.cpp  -  unit tests for StubBuilder class
                             -------------------
    begin                : Sat Sep 18 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: StubBuilder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include "SelectFramework.h"

#include <mockpp/Throwable.h>

#include <mockpp/builder/StubBuilder.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/StubHolder.h>
#include <mockpp/stub/ThrowStub.h>


#include "classes_ABCDE.h"



class StubBuilder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( StubBuilder_test );

  CPPUNIT_TEST(test_instance);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_instance();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( StubBuilder_test );
MOCKPP_CXXTEST(StubBuilder_test, test_instance);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(StubBuilder_test::test_instance);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (StubBuilder_test);

#endif // frameworks



typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;
typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> VoidInvocation;

MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(VoidInvocation)


template <typename R,  // Returntype
          typename I>  // Invocation
class MyStubBuilder : public MOCKPP_NS::StubBuilder<R, I>
{
  public:

    virtual ~MyStubBuilder()
    {
    }

    virtual void id( const MOCKPP_NS::String &/*id*/ )
    {
    }

    virtual void setStub( const MOCKPP_NS::StubHolder<R, I> &stubAction )
    {
      stub = stubAction;
    }

    typename MOCKPP_NS::Stub<R, I>::AP  stub;
};


void StubBuilder_test::test_instance()
{
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp5"), ca, cb, cc, cd, ce);

  MyStubBuilder<int, GetterInvocation> sbi;
  sbi.will(MOCKPP_NS::returnValue<int>(123));
  MOCKPP_ASSERT_TRUE(sbi.stub->invoke(ginv) == 123);

  MyStubBuilder<void, VoidInvocation> sbv;
  sbv.isVoid();
  MOCKPP_ASSERT_TRUE(sbv.stub->toString() == MOCKPP_PCHAR("is void") );

  MyStubBuilder<void, VoidInvocation> sbt;
  MOCKPP_NS::ThrowableItem it;
  it.take( 123 );
  sbt.will(new MOCKPP_NS::ThrowStub<void>(it));
  MOCKPP_ASSERT_TRUE( sbt.stub->toString().find( MOCKPP_PCHAR( "throws <" ) ) != MOCKPP_NS::String::npos );
}




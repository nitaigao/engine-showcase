/***************************************************************************
  InvocationMocker_test.cpp  -  unit tests for InvocationMocker class
                            -------------------
  begin                : Wed Sep 4 2004
  copyright            : (C) 2002-2008 by Ewald Arnold
  email                : mockpp at ewald-arnold dot de

  $Id: InvocationMocker_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/InvocationMocker.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/mockpp_pti.h>
#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/ThrowStub.h>

#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsGreaterThan.h>


#include "classes_ABCDE.h"


typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> SetterInvocation;
typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;


MOCKPP_PTI_IMPLEMENT(SetterInvocation)


MOCKPP_ANON_NS_START


class InvocationMocker_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( InvocationMocker_test );

  CPPUNIT_TEST( test_default_describer );
  CPPUNIT_TEST( test_describer );
  CPPUNIT_TEST( test_instance );
  CPPUNIT_TEST( test_instance_void );
  CPPUNIT_TEST( test_matches );
  CPPUNIT_TEST( test_verify );
  CPPUNIT_TEST( test_void_invoke );
  CPPUNIT_TEST( test_invoke );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_default_describer();
   MOCKPP_TESTMETHOD_DECL void test_describer();
   MOCKPP_TESTMETHOD_DECL void test_instance();
   MOCKPP_TESTMETHOD_DECL void test_instance_void();
   MOCKPP_TESTMETHOD_DECL void test_matches();
   MOCKPP_TESTMETHOD_DECL void test_verify();
   MOCKPP_TESTMETHOD_DECL void test_void_invoke();
   MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( InvocationMocker_test );

MOCKPP_CXXTEST(InvocationMocker_test,  test_default_describer );
MOCKPP_CXXTEST(InvocationMocker_test,  test_describer );
MOCKPP_CXXTEST(InvocationMocker_test,  test_instance );
MOCKPP_CXXTEST(InvocationMocker_test,  test_instance_void );
MOCKPP_CXXTEST(InvocationMocker_test,  test_matches );
MOCKPP_CXXTEST(InvocationMocker_test,  test_verify );
MOCKPP_CXXTEST(InvocationMocker_test,  test_void_invoke );
MOCKPP_CXXTEST(InvocationMocker_test,  test_invoke );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(InvocationMocker_test:: test_default_describer );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_describer );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_instance );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_instance_void );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_matches );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_verify );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_void_invoke );
MOCKPP_BOOST_TEST(InvocationMocker_test:: test_invoke );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( InvocationMocker_test );

#endif // frameworks



template <typename I>
class MyInvocationMatcher : public MOCKPP_NS::InvocationMatcher<I>
{
  public:

    mutable bool matched;
    mutable bool match_result;
    mutable bool verified;
    mutable bool isinvoked;

    virtual bool matches( const I &/*invocation*/ )
    {
      matched = true;
      return match_result;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      buffer += MOCKPP_PCHAR("MyInvocationMatcher");
      return buffer;
    }

    virtual void verify()
    {
      verified = true;
    }

    virtual void incInvoked( const I &/*invocation*/ )
    {
      isinvoked = true;
    }

    virtual bool hasDescription()
    {
      return true;
    }
};


template <typename R,  // Returntype
          typename I>  // Invocation
class MyStub : public MOCKPP_NS::Stub<R, I>
{
  public:

    bool stubbed;

    virtual R invoke( const I &/*invocation*/ )
    {
       stubbed = true;
       return R();
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &/*buffer*/ ) const
    {
      return MOCKPP_NS::String();
    }
};


template <typename I>  // Invocation
class MyVoidStub : public MOCKPP_NS::Stub<void, I>
{
  public:

    bool stubbed;

    virtual void invoke( const I &/*invocation*/ )
    {
       stubbed = true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      return buffer;
    }
};


void InvocationMocker_test::test_instance()
{
  MOCKPP_NS::InvocationMocker<int, GetterInvocation> im;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation>::DefaultDescriber ddes;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation>::Describer des;
}



void InvocationMocker_test::test_instance_void()
{
  MOCKPP_NS::InvocationMocker<void, SetterInvocation> im;
  MOCKPP_NS::InvocationMocker<void, SetterInvocation>::DefaultDescriber ddes;
  MOCKPP_NS::InvocationMocker<void, SetterInvocation>::Describer des;
}


void InvocationMocker_test::test_matches()
{
  MOCKPP_NS::InvocationMocker<int, GetterInvocation> im;
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MyInvocationMatcher<GetterInvocation> *ivm1 = new MyInvocationMatcher<GetterInvocation>;
  MyInvocationMatcher<GetterInvocation> *ivm2 = new MyInvocationMatcher<GetterInvocation>;

  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );

  ivm1->matched = false;
  ivm2->matched = false;
  ivm1->match_result = false;
  ivm2->match_result = true;
  MOCKPP_ASSERT_TRUE(im.matches(ginv) == false);
  MOCKPP_ASSERT_TRUE(ivm1->matched == true);
  MOCKPP_ASSERT_TRUE(ivm2->matched == false);

  ivm1->matched = true;
  ivm2->matched = false;
  ivm1->match_result = true;
  ivm2->match_result = false;
  MOCKPP_ASSERT_TRUE(im.matches(ginv) == false);
  MOCKPP_ASSERT_TRUE(ivm1->matched == true);
  MOCKPP_ASSERT_TRUE(ivm2->matched == true);

  ivm1->matched = false;
  ivm2->matched = false;
  ivm1->match_result = true;
  ivm2->match_result = true;
  MOCKPP_ASSERT_TRUE(im.matches(ginv) == true);
  MOCKPP_ASSERT_TRUE(ivm1->matched == true);
  MOCKPP_ASSERT_TRUE(ivm2->matched == true);
}


void InvocationMocker_test::test_verify()
{
  typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation> im;
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MyInvocationMatcher<GetterInvocation> *ivm1 = new MyInvocationMatcher<GetterInvocation>;
  MyInvocationMatcher<GetterInvocation> *ivm2 = new MyInvocationMatcher<GetterInvocation>;

  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );

  ivm1->verified = false;
  ivm2->verified = false;
  im.verify();
  MOCKPP_ASSERT_TRUE(ivm1->verified == true);
  MOCKPP_ASSERT_TRUE(ivm2->verified == true);
}


void InvocationMocker_test::test_void_invoke()
{
  typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;
  MOCKPP_NS::InvocationMocker<void, GetterInvocation> im;
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MyInvocationMatcher<GetterInvocation> *ivm1 = new MyInvocationMatcher<GetterInvocation>;
  MyInvocationMatcher<GetterInvocation> *ivm2 = new MyInvocationMatcher<GetterInvocation>;

  MyVoidStub<GetterInvocation> *stub = new MyVoidStub<GetterInvocation>;

  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );
  im.setStub(stub);

  ivm1->isinvoked = false;
  ivm2->isinvoked = false;
  stub->stubbed = false;
  im.invoke(ginv);
  MOCKPP_ASSERT_TRUE(ivm1->isinvoked == true);
  MOCKPP_ASSERT_TRUE(ivm2->isinvoked == true);
  MOCKPP_ASSERT_TRUE(stub->stubbed == true);
}


void InvocationMocker_test::test_invoke()
{
  typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation> im;
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MyInvocationMatcher<GetterInvocation> *ivm1 = new MyInvocationMatcher<GetterInvocation>;
  MyInvocationMatcher<GetterInvocation> *ivm2 = new MyInvocationMatcher<GetterInvocation>;

  MyStub<int, GetterInvocation> *stub = new MyStub<int, GetterInvocation>;

  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );
  im.setStub(stub);

  ivm1->isinvoked = false;
  ivm2->isinvoked = false;
  stub->stubbed = false;
  im.invoke(ginv);
  MOCKPP_ASSERT_TRUE(ivm1->isinvoked == true);
  MOCKPP_ASSERT_TRUE(ivm2->isinvoked == true);
  MOCKPP_ASSERT_TRUE(stub->stubbed == true);
}


void InvocationMocker_test::test_default_describer()
{
  typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;

  MOCKPP_NS::InvocationMocker<int, GetterInvocation>::DefaultDescriber *descr =
      new MOCKPP_NS::InvocationMocker<int, GetterInvocation>::DefaultDescriber;
  MOCKPP_ASSERT_TRUE(descr->hasDescription());

  MOCKPP_NS::InvocationMocker<int, GetterInvocation> im(descr);
  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MyInvocationMatcher<GetterInvocation> *ivm1 = new MyInvocationMatcher<GetterInvocation>;
  MyInvocationMatcher<GetterInvocation> *ivm2 = new MyInvocationMatcher<GetterInvocation>;

  im.setStub(MOCKPP_NS::returnValue<int>(13));
  im.setName(MOCKPP_PCHAR("magic-name"));
  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );
  MOCKPP_ASSERT_TRUE(im.hasDescription());
  MOCKPP_ASSERT_TRUE(im.toString() == MOCKPP_PCHAR("MyInvocationMatcher, MyInvocationMatcher, returns <13> [magic-name]") );

  im.setName(MOCKPP_PCHAR(""));
  MOCKPP_ASSERT_TRUE(im.toString() == MOCKPP_PCHAR("MyInvocationMatcher, MyInvocationMatcher, returns <13>") );
}


void InvocationMocker_test::test_describer()
{
  typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> GetterInvocation;
  MOCKPP_NS::InvocationMocker<int, GetterInvocation>::Describer *descr = new MOCKPP_NS::InvocationMocker<int, GetterInvocation>::Describer;
  MOCKPP_ASSERT_TRUE(descr->hasDescription());
  MOCKPP_NS::InvocationMocker<int, GetterInvocation>im(descr);

  CA ca;
  CB cb;
  CC cc;
  CD cd;
  CE ce;
  GetterInvocation ginv(MOCKPP_PCHAR("InvocationDummy::getProp"), ca, cb, cc, cd, ce);

  MOCKPP_NS::ConstraintSet5<CA, CB, CC, CD, CE> cs1 (new MOCKPP_NS::IsEqual<CA>(CA()),
                                                  new MOCKPP_NS::IsEqual<CB>(CB()),
                                                  new MOCKPP_NS::IsEqual<CC>(CC()),
                                                  new MOCKPP_NS::IsEqual<CD>(CD()),
                                                  new MOCKPP_NS::IsEqual<CE>(CE()));

  MOCKPP_NS::ConstraintSet5<CA, CB, CC, CD, CE> cs2 (new MOCKPP_NS::IsGreaterThan<CA>(CA()),
                                                  new MOCKPP_NS::IsGreaterThan<CB>(CB()),
                                                  new MOCKPP_NS::IsGreaterThan<CC>(CC()),
                                                  new MOCKPP_NS::IsGreaterThan<CD>(CD()),
                                                  new MOCKPP_NS::IsGreaterThan<CE>(CE()));

  MOCKPP_NS::InvocationMatcher<GetterInvocation> *ivm1 = new MOCKPP_NS::ArgumentsMatcher<GetterInvocation>(cs1);
  MOCKPP_NS::InvocationMatcher<GetterInvocation> *ivm2 = new MOCKPP_NS::ArgumentsMatcher<GetterInvocation>(cs2);

  im.setStub(MOCKPP_NS::returnValue<int>(42));
  im.setName(MOCKPP_PCHAR("magic-name"));
  im.addMatcher( ivm1 );
  im.addMatcher( ivm2 );
  MOCKPP_ASSERT_TRUE(im.hasDescription());

  MOCKPP_ASSERT_TRUE(im.toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&), (greaterThan CA&, greaterThan CB&, greaterThan CC&, greaterThan CD&, greaterThan CE&), returns <42> [magic-name]"));

  im.setName(MOCKPP_PCHAR(""));
  MOCKPP_ASSERT_TRUE(im.toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&), (greaterThan CA&, greaterThan CB&, greaterThan CC&, greaterThan CD&, greaterThan CE&), returns <42>"));
}


MOCKPP_NS_END




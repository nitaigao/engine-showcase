/***************************************************************************
InvocationMockerBuilder_test.cpp  -  unit tests for InvocationMockerBuilder class
                            -------------------
  begin                : Wed Sep 1 2004
  copyright            : (C) 2002-2008 by Ewald Arnold
  email                : mockpp at ewald-arnold dot de

  $Id: InvocationMockerBuilder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/builder/InvocationMockerBuilder.h>
#include <mockpp/builder/ArgumentsMatchBuilder.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/matcher/InvokeCountMatcher.h>

#include <mockpp/chaining/mockpp_pti.h>

#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/StubHolder.h>

#include <mockpp/constraint/IsEqual.h>


#include "classes_ABCDE.h"
#include "MyBuilderNamespace.h"


typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Getter5Invocation;
typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Setter5Invocation;
typedef MOCKPP_NS::Invocation0 Getter0Invocation;
typedef MOCKPP_NS::Invocation0 Setter0Invocation;


MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(Setter0Invocation)
MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(Setter5Invocation)


MOCKPP_ANON_NS_START


class InvocationMockerBuilder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( InvocationMockerBuilder_test );

  CPPUNIT_TEST( test_instance0 );
  CPPUNIT_TEST( test_instance5 );
  CPPUNIT_TEST( test_instancev0 );
  CPPUNIT_TEST( test_instancev5 );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_instance0();
   MOCKPP_TESTMETHOD_DECL void test_instance5();
   MOCKPP_TESTMETHOD_DECL void test_instancev0();
   MOCKPP_TESTMETHOD_DECL void test_instancev5();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( InvocationMockerBuilder_test );
MOCKPP_CXXTEST(InvocationMockerBuilder_test,  test_instance0 );
MOCKPP_CXXTEST(InvocationMockerBuilder_test,  test_instance5 );
MOCKPP_CXXTEST(InvocationMockerBuilder_test,  test_instancev0 );
MOCKPP_CXXTEST(InvocationMockerBuilder_test,  test_instancev5 );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(InvocationMockerBuilder_test:: test_instance0 );
MOCKPP_BOOST_TEST(InvocationMockerBuilder_test:: test_instance5 );
MOCKPP_BOOST_TEST(InvocationMockerBuilder_test:: test_instancev0 );
MOCKPP_BOOST_TEST(InvocationMockerBuilder_test:: test_instancev5 );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( InvocationMockerBuilder_test );

#endif // frameworks



template <typename AMB>  // ArgumentsMatchBuilder
class InvocationMockerBuilderDummy : public MOCKPP_NS::InvocationMockerBuilder<AMB>
{
  public:

    typedef typename AMB::ReturnType R;
    typedef typename AMB::InvocationType I;
    typedef typename AMB::ConstraintSetType MOCKPP_CS;

    InvocationMockerBuilderDummy( MOCKPP_NS::StubMatchersCollection<R, I> *in_mocker,
                                  MOCKPP_NS::BuilderNamespace *in_builderNamespace )
      : MOCKPP_NS::InvocationMockerBuilder<AMB>( in_mocker, in_builderNamespace )
    {}

#if defined( __BORLANDC__) || (defined(_MSC_VER) && (_MSC_VER < 1300))
    MOCKPP_NS::MatchBuilder<R, I> &with_cs( const MOCKPP_CS &constraintset )
    {
      return internalWith_cs(constraintset);
    }
#else
    using MOCKPP_NS::InvocationMockerBuilder<AMB>::with_cs;
#endif
};


template <typename R,    // Returntype
          typename I>    // Invocation
class MyStubMatchersCollection : public MOCKPP_NS::StubMatchersCollection<R, I>
{
  public:

    virtual ~MyStubMatchersCollection()
    {}

    virtual void setName( const MOCKPP_NS::String &in_name )
    {
      name = in_name;
    }

    virtual void addMatcher( const MOCKPP_NS::MatcherHolder<I> &matcher )
    {
      last_matcher = matcher;
    }

    virtual void setStub( const MOCKPP_NS::StubHolder<R, I> &stub )
    {
      last_stub = stub;
    }

    MOCKPP_NS::String name;
    typename MOCKPP_NS::InvocationMatcher<I>::AP last_matcher;
    typename MOCKPP_NS::Stub<R, I>::AP           last_stub;
};


void InvocationMockerBuilder_test::test_instance5()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder5<int, Getter5Invocation> ArgMatchBuilder;

  MyStubMatchersCollection<int, Getter5Invocation> mocker;
  MyBuilderNamespace table;

  InvocationMockerBuilderDummy<ArgMatchBuilder> imb( &mocker, &table );

  imb.match( new MOCKPP_NS::InvokeCountMatcher( 2 ) ).id( MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 2 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "matchcounter" ) )->getBuilder() == &imb );

  imb.expect( new MOCKPP_NS::InvokeCountMatcher( 4 ) ).id( MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 4 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "expectcounter" ) )->getBuilder() == &imb );

  MOCKPP_NS::ConstraintSet5<CA, CB, CC, CD, CE>
    cs5( new MOCKPP_NS::IsEqual<CA>( CA() ),
         new MOCKPP_NS::IsEqual<CB>( CB() ),
         new MOCKPP_NS::IsEqual<CC>( CC() ),
         new MOCKPP_NS::IsEqual<CD>( CD() ),
         new MOCKPP_NS::IsEqual<CE>( CE() ) );
  imb.with_cs( cs5 ).id( MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&)" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "withcscounter" ) )->getBuilder() == &imb );

  imb.with( new MOCKPP_NS::IsEqual<CA>( CA() ),
            new MOCKPP_NS::IsEqual<CB>( CB() ),
            new MOCKPP_NS::IsEqual<CC>( CC() ),
            new MOCKPP_NS::IsEqual<CD>( CD() ),
            new MOCKPP_NS::IsEqual<CE>( CE() ) ).id( MOCKPP_PCHAR( "with5counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "with5counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&)" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "with5counter" ) )->getBuilder() == &imb );

  imb.setStub( MOCKPP_NS::returnValue<int>( 2 ) );
  MOCKPP_ASSERT_TRUE( mocker.last_stub->toString() == MOCKPP_PCHAR( "returns <2>" ) );
}


void InvocationMockerBuilder_test::test_instance0()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder0<int, Getter0Invocation> ArgMatchBuilder;

  MyStubMatchersCollection<int, Getter0Invocation> mocker;
  MyBuilderNamespace table;

  InvocationMockerBuilderDummy<ArgMatchBuilder> imb( &mocker, &table );

  imb.match( new MOCKPP_NS::InvokeCountMatcher( 2 ) ).id( MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 2 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "matchcounter" ) )->getBuilder() == &imb );
  imb.expect( new MOCKPP_NS::InvokeCountMatcher( 4 ) ).id( MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 4 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "expectcounter" ) )->getBuilder() == &imb );

  MOCKPP_NS::ConstraintSet0 cs;
  imb.with_cs( cs ).id( MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "()" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "withcscounter" ) )->getBuilder() == &imb );

  imb.with().id( MOCKPP_PCHAR( "with0counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "with0counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "(no arguments)" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "with0counter" ) )->getBuilder() == &imb );

  imb.setStub( MOCKPP_NS::returnValue<int>( 5 ) );
  MOCKPP_ASSERT_TRUE( mocker.last_stub->toString() == MOCKPP_PCHAR( "returns <5>" ) );
}


void InvocationMockerBuilder_test::test_instancev0()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder0<void, Setter0Invocation> ArgMatchBuilder;

  MyStubMatchersCollection<void, Setter0Invocation> mocker;
  MyBuilderNamespace table;

  InvocationMockerBuilderDummy<ArgMatchBuilder> imb( &mocker, &table );

  imb.match( new MOCKPP_NS::InvokeCountMatcher( 2 ) ).id( MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 2 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "matchcounter" ) )->getBuilder() == &imb );

  imb.expect( new MOCKPP_NS::InvokeCountMatcher( 4 ) ).id( MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 4 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "expectcounter" ) )->getBuilder() == &imb );

  MOCKPP_NS::ConstraintSet0 cs;
  imb.with_cs( cs ).id( MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "()" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "withcscounter" ) )->getBuilder() == &imb );

  imb.with_cs(cs).id( MOCKPP_PCHAR( "with0counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "with0counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "()" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "with0counter" ) )->getBuilder() == &imb );

  imb.setStub( new MOCKPP_NS::VoidStub);
  MOCKPP_ASSERT_TRUE( mocker.last_stub->toString() == MOCKPP_PCHAR( "is void" ) );
}


void InvocationMockerBuilder_test::test_instancev5()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder5<void, Setter5Invocation> ArgMatchBuilder;

  MyStubMatchersCollection<void, Setter5Invocation> mocker;
  MyBuilderNamespace table;

  InvocationMockerBuilderDummy<ArgMatchBuilder> imb( &mocker, &table );

  imb.match( new MOCKPP_NS::InvokeCountMatcher( 2 ) ).id( MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 2 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "matchcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "matchcounter" ) )->getBuilder() == &imb );

  imb.expect( new MOCKPP_NS::InvokeCountMatcher( 4 ) ).id( MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "expected 4 times, invoked 0 times" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "expectcounter" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "expectcounter" ) )->getBuilder() == &imb );

  MOCKPP_NS::ConstraintSet5<CA, CB, CC, CD, CE> cs5( new MOCKPP_NS::IsEqual<CA>( CA() ),
                                                  new MOCKPP_NS::IsEqual<CB>( CB() ),
                                                  new MOCKPP_NS::IsEqual<CC>( CC() ),
                                                  new MOCKPP_NS::IsEqual<CD>( CD() ),
                                                  new MOCKPP_NS::IsEqual<CE>( CE() ) );
  imb.with_cs( cs5 ).id( MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "withcscounter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&)" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "withcscounter" ) )->getBuilder() == &imb );

  imb.with( new MOCKPP_NS::IsEqual<CA>( CA() ),
            new MOCKPP_NS::IsEqual<CB>( CB() ),
            new MOCKPP_NS::IsEqual<CC>( CC() ),
            new MOCKPP_NS::IsEqual<CD>( CD() ),
            new MOCKPP_NS::IsEqual<CE>( CE() ) ).id( MOCKPP_PCHAR( "with5counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.name == MOCKPP_PCHAR( "with5counter" ) );
  MOCKPP_ASSERT_TRUE( mocker.last_matcher->toString() == MOCKPP_PCHAR( "(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&)" ) );
  MOCKPP_ASSERT_TRUE( table.lookupID( MOCKPP_PCHAR( "with5counter" ) )->getBuilder() == &imb );

  imb.setStub( new MOCKPP_NS::VoidStub);
  MOCKPP_ASSERT_TRUE( mocker.last_stub->toString() == MOCKPP_PCHAR( "is void" ) );
}


MOCKPP_NS_END




/***************************************************************************
 ChainingMockBuilder_test.cpp  -  unit tests for ChainingMockBuilder class
                             -------------------
    begin                : Wed Sep 4 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: ChainingMockBuilder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include MOCKPP_VECTOR_H

#include <mockpp/chaining/ChainingMockBuilder.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/builder/ArgumentsMatchBuilder.h>

#include <mockpp/matcher/InvokeAtLeastOnceMatcher.h>

#include <mockpp/stub/ReturnStub.h>

#include <mockpp/compat/Asserter.h>


#include "classes_ABCDE.h"
#include "MyBuilderNamespace.h"
#include "MyMatchBuilder.h"


class ChainingMockBuilder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainingMockBuilder_test );

  CPPUNIT_TEST( test_forward );
  CPPUNIT_TEST( test_invoke );
  CPPUNIT_TEST( test_register );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_forward();
   MOCKPP_TESTMETHOD_DECL void test_register();
   MOCKPP_TESTMETHOD_DECL void test_invoke();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainingMockBuilder_test );
MOCKPP_CXXTEST(ChainingMockBuilder_test,  test_forward );
MOCKPP_CXXTEST(ChainingMockBuilder_test,  test_invoke );
MOCKPP_CXXTEST(ChainingMockBuilder_test,  test_register );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainingMockBuilder_test:: test_forward );
MOCKPP_BOOST_TEST(ChainingMockBuilder_test:: test_invoke );
MOCKPP_BOOST_TEST(ChainingMockBuilder_test:: test_register );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( ChainingMockBuilder_test );

#endif // frameworks



typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Getter5Invocation;


template <typename R,    // Returntype
          typename I>    // Invocation
class InvokableDummy : public MOCKPP_NS::Invokable<R, I>
{
  public:

    mutable bool            verified;
    mutable R               invoke_return;
    mutable bool            match_return;
    mutable MOCKPP_NS::String  describe_return;

    virtual bool matches( const I & /*invocation*/ )
    {
      return match_return;
    }

    virtual R invoke( const I & /*invocation*/ )
    {
      return invoke_return;
    }

    virtual void reset()
    {}

    virtual bool hasDescription()
    {
      return true;
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String & /*buffer*/ ) const
    {
      return describe_return;
    }

    virtual void verify()
    {
      verified = true;
    }
};


template <typename R,    // Returntype
          typename I>    // Invocation
class ChainingMockDummy : public MOCKPP_NS::DynamicChainingMock<R, I>
{
  public:

    ChainingMockDummy()
        : MOCKPP_NS::DynamicChainingMock<R, I> ( MOCKPP_PCHAR( "ChainingMockBuilder" ) )
    {}

    mutable bool verified;
    mutable bool resetted;
    mutable typename MOCKPP_NS::Stub<R, I>::AP stub;
    mutable MOCKPP_STL::vector<MOCKPP_NS::Invokable<R, I>*> invokables;
    mutable MOCKPP_NS::String string_return;

    virtual ~ChainingMockDummy()
    {
      for ( unsigned i = 0; i < invokables.size(); ++i )
        delete invokables[ i ];
    }

    virtual MOCKPP_NS::String toString() const
    {
      if ( string_return.length() != 0 )
        return string_return;

      MOCKPP_NS::String s;
      for ( unsigned i = 0; i < invokables.size(); ++i )
      {
        if ( i != 0 )
          s += MOCKPP_PCHAR( "\n" ) ;
        s += invokables[ i ] ->toString();
      }
      return s;
    }

    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &in_stub )
    {
      stub = in_stub;
    }

    virtual void addInvokable( typename MOCKPP_NS::Invokable<R, I>::AP invokable )
    {
      invokables.push_back( invokable.release() );
    }

    virtual void reset()
    {
      resetted = true;
    }

    virtual void verify()
    {
      verified = true;
    }
};


void ChainingMockBuilder_test::test_forward()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder5<int, Getter5Invocation> ArgMatchBuilder;

  MyBuilderNamespace bns;
  ChainingMockDummy<int, Getter5Invocation> cmd;
  MOCKPP_NS::ChainingMockBuilder<ArgMatchBuilder> cm ( &cmd, &bns, MOCKPP_PCHAR( "ChainingMockBuilder??" ));

  cmd.verified = false;
  cm.verify();
  MOCKPP_ASSERT_TRUE( cmd.verified == true );

  cmd.string_return = MOCKPP_PCHAR( "describe??" );
  MOCKPP_ASSERT_TRUE( cm.toString() == MOCKPP_PCHAR( "describe??" ) );

  cm.setDefaultStub( MOCKPP_NS::returnValue<int>( 123 ) );
  MOCKPP_ASSERT_TRUE( cmd.stub->toString() == MOCKPP_PCHAR( "returns <123>" ) );

  cmd.resetted = false;
  cm.reset();
  MOCKPP_ASSERT_TRUE( cmd.resetted == true );
}


void ChainingMockBuilder_test::test_invoke()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder5<int, Getter5Invocation> ArgMatchBuilder;

  MyBuilderNamespace bns;

  ChainingMockDummy<int, Getter5Invocation> cmd;
  MOCKPP_NS::ChainingMockBuilder<ArgMatchBuilder> cm ( &cmd, &bns, MOCKPP_PCHAR( "ChainingMockBuilder??" ));

  InvokableDummy<int, Getter5Invocation> *id = new InvokableDummy<int, Getter5Invocation>;

  cm.stubs().will( MOCKPP_NS::returnValue<int>( 321 ) ).id( MOCKPP_PCHAR( "id1a" ) );

  cm.stubs().will( new MOCKPP_NS::ThrowStub<int>( MOCKPP_NS::make_throwable( 1 ) ) ).id( MOCKPP_PCHAR( "id1b" ) );

  MOCKPP_NS::ConstraintSet5<CA, CB, CC, CD, CE> cs ( new MOCKPP_NS::IsEqual<CA>( CA() ),
                                                  new MOCKPP_NS::IsEqual<CB>( CB() ),
                                                  new MOCKPP_NS::IsEqual<CC>( CC() ),
                                                  new MOCKPP_NS::IsEqual<CD>( CD() ),
                                                  new MOCKPP_NS::IsEqual<CE>( CE() ) );

  cm.expects( new MOCKPP_NS::ArgumentsMatcher<Getter5Invocation>( cs ))
    .will( new MOCKPP_NS::ThrowStub<int>( MOCKPP_NS::make_throwable( 1 ) ) );

  cm.expects( MOCKPP_NS::once() )
    .will( new MOCKPP_NS::ThrowStub<int>( MOCKPP_NS::make_throwable( 1 ) ) )
    .id( MOCKPP_PCHAR( "id3" ) );

  cm.addInvokable( id );
  MOCKPP_NS::String s = cmd.toString();
  MOCKPP_ASSERT_TRUE( s.find(MOCKPP_PCHAR( "returns <321> [id1a]\n" )) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE( s.find(MOCKPP_PCHAR( "[id1b]\n" )) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE( s.find(MOCKPP_PCHAR( "(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&" )) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE( s.find(MOCKPP_PCHAR( "expected once" )) != MOCKPP_NS::String::npos);

  MOCKPP_ASSERT_TRUE( cm.lookupID( MOCKPP_PCHAR( "id1a" ) ) != 0 );
}


void ChainingMockBuilder_test::test_register()
{
  typedef MOCKPP_NS::ArgumentsMatchBuilder5<int, Getter5Invocation> ArgMatchBuilder;

  MyBuilderNamespace bns;

  ChainingMockDummy<int, Getter5Invocation> cmd;
  MOCKPP_NS::ChainingMockBuilder<ArgMatchBuilder> cm ( &cmd, &bns, MOCKPP_PCHAR( "ChainingMockBuilder??" ));

  MyMatchBuilder<void, int> mbd1;
  MyMatchBuilder<void, int> mbd2;
  cm.registerUniqueID(MOCKPP_PCHAR("first-name"), new MOCKPP_NS::MatchBuilderAdapter<void, int>(&mbd1));
  cm.registerUniqueID(MOCKPP_PCHAR("second-name"), new MOCKPP_NS::MatchBuilderAdapter<void, int>(&mbd2));

  cm.lookupID(MOCKPP_PCHAR("first-name"));
  cm.lookupID(MOCKPP_PCHAR("second-name"));

  MOCKPP_TRY
  {
     cm.registerUniqueID(MOCKPP_PCHAR("second-name"), 0);
     MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( const MOCKPP_NS::AssertionFailedError &ae )
  {
    MOCKPP_ASSERT_TRUE(ae.getMessage().find(MOCKPP_PCHAR( "duplicate invocation named" )) != MOCKPP_NS::String::npos);
  }
#endif

  cm.reset();
  MOCKPP_TRY
  {
     cm.lookupID(MOCKPP_PCHAR("second-name"));
     MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }

#ifndef MOCKPP_NO_EXCEPTIONS
  catch ( const MOCKPP_NS::AssertionFailedError &ae )
  {
    MOCKPP_ASSERT_TRUE(ae.getMessage().find(MOCKPP_PCHAR( "no expected invocation named" )) != MOCKPP_NS::String::npos);
  }
#endif
}




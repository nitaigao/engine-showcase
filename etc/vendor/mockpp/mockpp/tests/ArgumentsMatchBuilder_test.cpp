/***************************************************************************
 ArgumentsMatchBuilder_test.cpp  -  unit tests for ArgumentsMatchBuilder class
                             -------------------
    begin                : Thu Aug 31 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ArgumentsMatchBuilder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/builder/ArgumentsMatchBuilder.h>

#include <mockpp/matcher/InvocationMatcher.h>
#include <mockpp/matcher/ArgumentsMatcher.h>
#include <mockpp/matcher/MatcherHolder.h>

#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/StubHolder.h>

#include <mockpp/constraint/IsEqual.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>


#include "classes_ABCDE.h"


MOCKPP_ANON_NS_START


class ArgumentsMatchBuilder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ArgumentsMatchBuilder_test );

  CPPUNIT_TEST(test_instance);

CPPUNIT_TEST_SUITE_END();

#endif

  public:

  MOCKPP_TESTMETHOD_DECL void test_instance();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ArgumentsMatchBuilder_test );
MOCKPP_CXXTEST(ArgumentsMatchBuilder_test, test_instance);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ArgumentsMatchBuilder_test::test_instance);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ArgumentsMatchBuilder_test);

#endif // frameworks



typedef MOCKPP_NS::Invocation0 Getter0Invocation;
typedef MOCKPP_NS::Invocation1<CA> Getter1Invocation;
typedef MOCKPP_NS::Invocation2<CA, CB> Getter2Invocation;
typedef MOCKPP_NS::Invocation3<CA, CB, CC> Getter3Invocation;
typedef MOCKPP_NS::Invocation4<CA, CB, CC, CD> Getter4Invocation;
typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Getter5Invocation;

typedef MOCKPP_NS::Invocation0 Void0Invocation;
typedef MOCKPP_NS::Invocation1<CA> Void1Invocation;
typedef MOCKPP_NS::Invocation2<CA, CB> Void2Invocation;
typedef MOCKPP_NS::Invocation3<CA, CB, CC> Void3Invocation;
typedef MOCKPP_NS::Invocation4<CA, CB, CC, CD> Void4Invocation;
typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Void5Invocation;


template <typename AMB>
class Container0 : public AMB
{
  public:

    typedef typename AMB::ReturnType R;
    typedef typename AMB::InvocationType I;

    virtual ~Container0()
    {}

    virtual MOCKPP_NS::MatchBuilder<R, I>& addMatcher(const MOCKPP_NS::MatcherHolder<I> &matcher)
    {
       last_matcher = matcher;
       return *this;
    }

    virtual MOCKPP_NS::MatchBuilder<R, I> &match( const MOCKPP_NS::MatcherHolder<I> &/*customMatcher*/ )
    {
       return *this;
    }

    virtual void setStub(const MOCKPP_NS::StubHolder<R, I> &/*stubAction*/ )
    {
    }

    virtual MOCKPP_NS::BuilderNamespace *getBuilderNamespace()
    {
      return 0;
    }

    virtual MOCKPP_NS::StubMatchersCollection<R, I> * getMocker()
    {
      return 0;
    }

    virtual void id( const MOCKPP_NS::String &/*id*/ )
    {
    }

    typename MOCKPP_NS::InvocationMatcher<I>::AP   last_matcher;
};


template <typename AMB>
class ContainerX : public AMB
{
  public:

    typedef typename AMB::ReturnType R;
    typedef typename AMB::InvocationType I;
//    typedef typename AMB::ConstraintSetType MOCKPP_CS;

    virtual ~ContainerX()
    {}

    virtual MOCKPP_NS::MatchBuilder<R, I>& with_cs( const typename AMB::ConstraintSetType &constraintset )
    {
       return addMatcher( new MOCKPP_NS::ArgumentsMatcher<I>( constraintset ) );
    }

    virtual MOCKPP_NS::MatchBuilder<R, I>& addMatcher(const MOCKPP_NS::MatcherHolder<I> &matcher)
    {
       last_matcher = matcher;
       return *this;
    }

    virtual MOCKPP_NS::MatchBuilder<R, I> &match( const MOCKPP_NS::MatcherHolder<I> &/*customMatcher*/ )
    {
       return *this;
    }

    virtual void id( const MOCKPP_NS::String &/*id*/ )
    {
    }

    virtual MOCKPP_NS::BuilderNamespace *getBuilderNamespace()
    {
      return 0;
    }

    virtual MOCKPP_NS::StubMatchersCollection<R, I> * getMocker()
    {
      return 0;
    }

    virtual void setStub( const MOCKPP_NS::StubHolder<R, I> &/*stubAction*/ )
    {
    }

    typename MOCKPP_NS::InvocationMatcher<I>::AP   last_matcher;
};


void ArgumentsMatchBuilder_test::test_instance()
{
 {
   Container0<MOCKPP_NS::ArgumentsMatchBuilder0<int, Getter0Invocation> > c0;
   c0.withNoArguments();
 }
//----------------------------------------

#if 0

{
   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<void, Void0Invocation> > cxv;
   cxv.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter0Invocation> > cx0;
   cx0.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter1Invocation> > cx1;
   cx1.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter2Invocation> > cx2;
   cx2.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter3Invocation> > cx3;
   cx3.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter4Invocation> > cx4;
   cx4.withAnyArguments();

   Container0<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter5Invocation> > cx5;
   cx5.withAnyArguments();
 }

#endif

 //----------------------------------------
 {
   CA ca;
   CB cb;
   CC cc;
   CD cd;
   CE ce;

#if 0

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilderX<int, Getter0Invocation> > cx;
   cx.withAnyArguments();
   MOCKPP_ASSERT_TRUE( cx.last_matcher->toString() == MOCKPP_PCHAR("(any arguments)")  );

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder0<int, Getter0Invocation> > c0;
   c0.withNoArguments();
   MOCKPP_ASSERT_TRUE( c0.last_matcher->toString() == MOCKPP_PCHAR("(no arguments)")  );

#endif

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder1<int, Getter1Invocation> > c1;
   c1.with(new MOCKPP_NS::IsEqual<CA>(ca));

   MOCKPP_ASSERT_TRUE( c1.last_matcher->toString() == MOCKPP_PCHAR("(equalTo CA&)")  );

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder2<int, Getter2Invocation> > c2;
   c2.with(new MOCKPP_NS::IsEqual<CA>(ca),
           new MOCKPP_NS::IsEqual<CB>(cb));
   MOCKPP_ASSERT_TRUE( c2.last_matcher->toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&)")  );

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder3<int, Getter3Invocation> > c3;
   c3.with(new MOCKPP_NS::IsEqual<CA>(ca),
           new MOCKPP_NS::IsEqual<CB>(cb),
           new MOCKPP_NS::IsEqual<CC>(cc));
   MOCKPP_ASSERT_TRUE( c3.last_matcher->toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&, equalTo CC&)")  );

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder4<int, Getter4Invocation> > c4;
   c4.with(new MOCKPP_NS::IsEqual<CA>(ca),
           new MOCKPP_NS::IsEqual<CB>(cb),
           new MOCKPP_NS::IsEqual<CC>(cc),
           new MOCKPP_NS::IsEqual<CD>(cd));
   MOCKPP_ASSERT_TRUE( c4.last_matcher->toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&)")  );

   ContainerX<MOCKPP_NS::ArgumentsMatchBuilder5<int, Getter5Invocation> > c5;
   c5.with(new MOCKPP_NS::IsEqual<CA>(CA()),
           new MOCKPP_NS::IsEqual<CB>(CB()),
           new MOCKPP_NS::IsEqual<CC>(CC()),
           new MOCKPP_NS::IsEqual<CD>(CD()),
           new MOCKPP_NS::IsEqual<CE>(CE()));
   MOCKPP_ASSERT_TRUE( c5.last_matcher->toString() == MOCKPP_PCHAR("(equalTo CA&, equalTo CB&, equalTo CC&, equalTo CD&, equalTo CE&)")  );

  c1.with(MOCKPP_NS::any());
  c2.with(MOCKPP_NS::any(), MOCKPP_NS::any());
  c3.with(MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any());
  c4.with(MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any());
  c5.with(MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any(), MOCKPP_NS::any());

 }
}


MOCKPP_NS_END




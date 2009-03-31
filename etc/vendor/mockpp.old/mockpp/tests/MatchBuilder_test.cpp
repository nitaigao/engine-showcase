/***************************************************************************
      MatchBuilder_test.cpp  -  unit tests for MatchBuilder class
                            -------------------
   begin                : Sat Sep 20 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   $Id: MatchBuilder_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/builder/MatchBuilder.h>
#include <mockpp/builder/BuilderNamespace.h>

#include <mockpp/compat/Asserter.h>

#include <mockpp/stub/StubHolder.h>

#include "classes_ABCDE.h"
#include "MyBuilderNamespace.h"
#include "MyMatchBuilder.h"



MOCKPP_ANON_NS_START


class MatchBuilder_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( MatchBuilder_test );

  CPPUNIT_TEST( test_order );
  CPPUNIT_TEST( test_order_defered );

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_order();
   MOCKPP_TESTMETHOD_DECL void test_order_defered();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( MatchBuilder_test );
MOCKPP_CXXTEST(MatchBuilder_test,  test_order );
MOCKPP_CXXTEST(MatchBuilder_test,  test_order_defered );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(MatchBuilder_test:: test_order );
MOCKPP_BOOST_TEST(MatchBuilder_test:: test_order_defered );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( MatchBuilder_test );

#endif // frameworks



typedef MOCKPP_NS::Invocation5<CA, CB, CC, CD, CE> Getter5Invocation;
typedef MOCKPP_NS::Invocation4<CA, CB, CC, CD> Getter4Invocation;


void MatchBuilder_test::test_order()
{
  MyMatchBuilder<void, Getter5Invocation> mbx;
  MyMatchBuilder<void, Getter5Invocation> mb1;

  mbx.bn.registerUniqueID( MOCKPP_PCHAR( "after x" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mbx) );
  mbx.bn.registerUniqueID( MOCKPP_PCHAR( "before x" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mbx) );

  mb1.bn.registerUniqueID( MOCKPP_PCHAR( "after 1" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mb1) );
  mb1.bn.registerUniqueID( MOCKPP_PCHAR( "before 1" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mb1) );

  mb1.last_match.reset( 0 );
  mb1.after( MOCKPP_PCHAR( "after 1" ) );
  MOCKPP_ASSERT_TRUE( mb1.mocker.last_matcher->toString().find( MOCKPP_PCHAR( "after after 1" ) ) != MOCKPP_NS::String::npos );
  MOCKPP_ASSERT_TRUE( mb1.last_match.get() != 0 );

  mb1.last_match.reset( 0 );
  mb1.before( MOCKPP_PCHAR( "before 1" ) );
  MOCKPP_ASSERT_TRUE( mb1.mocker.last_matcher->toString().find( MOCKPP_PCHAR( "before before 1" ) ) != MOCKPP_NS::String::npos );
  MOCKPP_ASSERT_TRUE( mb1.last_match.get() != 0 );

  mbx.last_match.reset( 0 );
  mb1.after( mbx.bn, MOCKPP_PCHAR( "after x" ) );
  MOCKPP_ASSERT_TRUE( mb1.mocker.last_matcher->toString().find( MOCKPP_PCHAR( "after after x on" ) ) != MOCKPP_NS::String::npos );
  MOCKPP_ASSERT_TRUE( mbx.last_match.get() != 0 );

  mbx.last_match.reset( 0 );
  mb1.before( mbx.bn, MOCKPP_PCHAR( "before x" ) );
  MOCKPP_ASSERT_TRUE( mb1.mocker.last_matcher->toString().find( MOCKPP_PCHAR( "before before x on" ) ) != MOCKPP_NS::String::npos );
  MOCKPP_ASSERT_TRUE( mbx.last_match.get() != 0 );
}


void MatchBuilder_test::test_order_defered()
{
  MyMatchBuilder<void, Getter5Invocation> mb;
  MyMatchBuilder<void, Getter5Invocation> mba;
  MyMatchBuilder<void, Getter5Invocation> mbb;

  mb.after( MOCKPP_PCHAR( "after" ) );
  mb.before( MOCKPP_PCHAR( "before" ) );
  MOCKPP_ASSERT_TRUE(mb.bn.lookupID(MOCKPP_PCHAR( "after" )) == 0);
  MOCKPP_ASSERT_TRUE(mb.bn.lookupID(MOCKPP_PCHAR( "before" )) == 0);

  mb.bn.registerUniqueID( MOCKPP_PCHAR( "after" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mba ));
  mb.bn.registerUniqueID( MOCKPP_PCHAR( "before" ), new MOCKPP_NS::MatchBuilderAdapter<void, Getter5Invocation>(&mbb) );
  MOCKPP_ASSERT_TRUE(mb.bn.lookupID(MOCKPP_PCHAR( "after" ))->getBuilder() == &mba);
  MOCKPP_ASSERT_TRUE(mb.bn.lookupID(MOCKPP_PCHAR( "before" ))->getBuilder() == &mbb);
}


MOCKPP_NS_END




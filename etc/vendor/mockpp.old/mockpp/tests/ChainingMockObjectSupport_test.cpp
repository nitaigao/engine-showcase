/***************************************************************************

ChainingMockObjectSupport_test.cpp  -  unit tests for ChainingMockObjectSupport
                            -------------------
   begin                : Sat Aug 21 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

 $Id: ChainingMockObjectSupport_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

//#include <iostream>

#include "SelectFramework.h"

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/ChainableMockObject.h>

#include <mockpp/constraint/IsAnything.h>
#include <mockpp/constraint/TriggeredOutbound.h>
#include <mockpp/constraint/TypelessConstraint.h>

#include <mockpp/stub/CustomStub.h>



MOCKPP_ANON_NS_START


class ChainingMockObjectSupport_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
  public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainingMockObjectSupport_test );

  CPPUNIT_TEST( test_closeto_func );
  CPPUNIT_TEST( test_any_func );
  CPPUNIT_TEST( test_nothing_func );
  CPPUNIT_TEST( test_equal_func );
  CPPUNIT_TEST( test_not_equal_func );
  CPPUNIT_TEST( test_greater_func );
  CPPUNIT_TEST( test_greater_equal_func );
  CPPUNIT_TEST( test_less_func );
  CPPUNIT_TEST( test_same_func );
  CPPUNIT_TEST( test_less_equal_func );
#ifndef MOCKPP_NO_RTTI
  CPPUNIT_TEST( test_instance );
#endif
  CPPUNIT_TEST( test_invoke_not );
  CPPUNIT_TEST( test_invoke_or );
  CPPUNIT_TEST( test_invoke_and );
  CPPUNIT_TEST( test_actor );
  CPPUNIT_TEST( test_counts );
  CPPUNIT_TEST( test_string_contains );
  CPPUNIT_TEST( test_string_ends );
  CPPUNIT_TEST( test_string_starts );
  CPPUNIT_TEST( test_consecutive_typeless );
  CPPUNIT_TEST( test_consecutive_custom );
  CPPUNIT_TEST( test_trigger);

CPPUNIT_TEST_SUITE_END();

#endif

  public:

   MOCKPP_TESTMETHOD_DECL void test_string_contains();
   MOCKPP_TESTMETHOD_DECL void test_string_ends();
   MOCKPP_TESTMETHOD_DECL void test_string_starts();
   MOCKPP_TESTMETHOD_DECL void test_actor();
   MOCKPP_TESTMETHOD_DECL void test_counts();
   MOCKPP_TESTMETHOD_DECL void test_consecutive_typeless();
   MOCKPP_TESTMETHOD_DECL void test_consecutive_custom();

   MOCKPP_TESTMETHOD_DECL void test_invoke_and();
   MOCKPP_TESTMETHOD_DECL void test_invoke_or();
   MOCKPP_TESTMETHOD_DECL void test_invoke_not();
#ifndef MOCKPP_NO_RTTI
   MOCKPP_TESTMETHOD_DECL void test_instance();
#endif
   MOCKPP_TESTMETHOD_DECL void test_same_func();
   MOCKPP_TESTMETHOD_DECL void test_less_equal_func();
   MOCKPP_TESTMETHOD_DECL void test_less_func();
   MOCKPP_TESTMETHOD_DECL void test_greater_equal_func();
   MOCKPP_TESTMETHOD_DECL void test_greater_func();
   MOCKPP_TESTMETHOD_DECL void test_equal_func();
   MOCKPP_TESTMETHOD_DECL void test_not_equal_func();
   MOCKPP_TESTMETHOD_DECL void test_nothing_func();
   MOCKPP_TESTMETHOD_DECL void test_any_func();
   MOCKPP_TESTMETHOD_DECL void test_closeto_func();
   MOCKPP_TESTMETHOD_DECL void test_trigger();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainingMockObjectSupport_test );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_closeto_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_any_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_nothing_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_equal_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_not_equal_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_greater_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_greater_equal_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_less_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_same_func );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_less_equal_func );
#ifndef MOCKPP_NO_RTTI
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_instance );
#endif
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_invoke_not );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_invoke_or );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_invoke_and );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_actor );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_counts );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_string_contains );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_string_ends );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_string_starts );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_consecutive_typeless );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_consecutive_custom );
MOCKPP_CXXTEST(ChainingMockObjectSupport_test,  test_trigger);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_closeto_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_any_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_nothing_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_equal_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_not_equal_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_greater_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_greater_equal_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_less_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_same_func );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_less_equal_func );
#ifndef MOCKPP_NO_RTTI
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_instance );
#endif
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_invoke_not );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_invoke_or );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_invoke_and );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_actor );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_counts );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_string_contains );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_string_ends );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_string_starts );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_consecutive_typeless );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_consecutive_custom );
MOCKPP_BOOST_TEST(ChainingMockObjectSupport_test:: test_trigger);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION ( ChainingMockObjectSupport_test );

#endif // frameworks



class A
{
   public:

     virtual ~A() {}

     int a;
};


class B : public A
{
  public:

    virtual ~B() {}

    int a;
};


class C : public B
{
  public:

  virtual ~C() { }

  int c;
};


class D
{
  public:

    virtual ~D() {}

    const char *pc;
};


#ifndef MOCKPP_NO_RTTI

void ChainingMockObjectSupport_test::test_instance()
{
  A a;
  B b;
  C c;
  D d;

  MOCKPP_NS::Constraint<A*>::AP instance0 = MOCKPP_NS::isA<A, B>( b );
  MOCKPP_UNUSED(instance0);
  MOCKPP_NS::Constraint<A*>::AP instance1 = MOCKPP_NS::isA<A>( b );
  MOCKPP_UNUSED(instance1);
  MOCKPP_NS::Constraint<A*>::AP instance2 = MOCKPP_NS::isA<A, B>();
  MOCKPP_UNUSED(instance2);

  MOCKPP_NS::Constraint<A*>::AP instance = MOCKPP_NS::isA<A>( B() );
  MOCKPP_ASSERT_TRUE( instance->eval((B*) &a ) == false );
  MOCKPP_ASSERT_TRUE( instance->eval( &b ) == true );
  MOCKPP_ASSERT_TRUE( instance->eval( &c ) == true );
  MOCKPP_ASSERT_TRUE( instance->eval((B*) &d ) == false );

  B &rb = *((B*) 0);
  MOCKPP_ASSERT_TRUE( instance->eval( &rb ) == false );
}

#endif // MOCKPP_NO_RTTI

void ChainingMockObjectSupport_test::test_string_contains()
{
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs1 (MOCKPP_NS::stringContains(MOCKPP_NS::String(MOCKPP_PCHAR("hallo pcs"))));
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("hallo pcs") ) == true );
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("xxxxx") ) == false );

  MOCKPP_NS::String str = MOCKPP_PCHAR ("hallo str");
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs2 (MOCKPP_NS::stringContains(str));
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("hallo str") ) == true );
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("xxxxx") ) == false );
}


void ChainingMockObjectSupport_test::test_string_ends()
{
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs1 (MOCKPP_NS::endsWith(MOCKPP_NS::String(MOCKPP_PCHAR("pcs"))));
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("hallo pcs") ) == true );
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("xxxxx") ) == false );

  MOCKPP_NS::String str = MOCKPP_PCHAR ("str");
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs2 (MOCKPP_NS::endsWith(str));
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("hallo str") ) == true );
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("xxxxx") ) == false );
}


void ChainingMockObjectSupport_test::test_string_starts()
{
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs1 (MOCKPP_NS::startsWith(MOCKPP_NS::String(MOCKPP_PCHAR("pcs"))));
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("pcs hallo") ) == true );
  MOCKPP_ASSERT_TRUE( cs1->eval( MOCKPP_PCHAR("xxxxx") ) == false );

  MOCKPP_NS::String str = MOCKPP_PCHAR ("str");
  MOCKPP_NS::Constraint<MOCKPP_NS::String>::AP cs2 (MOCKPP_NS::startsWith(str));
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("str hallo") ) == true );
  MOCKPP_ASSERT_TRUE( cs2->eval( MOCKPP_PCHAR("xxxxx") ) == false );
}


void ChainingMockObjectSupport_test::test_greater_equal_func()
{
  MOCKPP_NS::Constraint<int>::AP greaterequal = MOCKPP_NS::ge ( 10 );
  MOCKPP_ASSERT_TRUE( greaterequal->eval( 11 ) == true );
  MOCKPP_ASSERT_TRUE( greaterequal->eval( 10 ) == true );
  MOCKPP_ASSERT_TRUE( greaterequal->eval( 9 ) == false );
}


void ChainingMockObjectSupport_test::test_greater_func()
{
  MOCKPP_NS::Constraint<int>::AP greater = MOCKPP_NS::gt ( 10 );
  MOCKPP_ASSERT_TRUE( greater->eval( 11 ) == true );
  MOCKPP_ASSERT_TRUE( greater->eval( 10 ) == false );
  MOCKPP_ASSERT_TRUE( greater->eval( 9 ) == false );
}


void ChainingMockObjectSupport_test::test_less_equal_func()
{
  MOCKPP_NS::Constraint<int>::AP lessequal = MOCKPP_NS::le ( 10 );
  MOCKPP_ASSERT_TRUE( lessequal->eval( 11 ) == false );
  MOCKPP_ASSERT_TRUE( lessequal->eval( 10 ) == true );
  MOCKPP_ASSERT_TRUE( lessequal->eval( 9 ) == true );
}


void ChainingMockObjectSupport_test::test_less_func()
{
  MOCKPP_NS::Constraint<int>::AP less = MOCKPP_NS::lt ( 10 );
  MOCKPP_ASSERT_TRUE( less->eval( 11 ) == false );
  MOCKPP_ASSERT_TRUE( less->eval( 10 ) == false );
  MOCKPP_ASSERT_TRUE( less->eval( 9 ) == true );
}


void ChainingMockObjectSupport_test::test_not_equal_func()
{
  MOCKPP_NS::Constraint<MOCKPP_STL::string>::AP myequal = MOCKPP_NS::ne( MOCKPP_STL::string( "string" ) );
  MOCKPP_ASSERT_TRUE( myequal->eval( "string" ) == false );
  MOCKPP_ASSERT_TRUE( myequal->eval( "not-string" ) == true );
}


void ChainingMockObjectSupport_test::test_equal_func()
{
  MOCKPP_NS::Constraint<MOCKPP_STL::string>::AP myequal_s = MOCKPP_NS::eq( MOCKPP_STL::string( "string" ) );
  MOCKPP_ASSERT_TRUE( myequal_s->eval( "string" ) == true );
  MOCKPP_ASSERT_TRUE( myequal_s->eval( "not-string" ) == false );

  MOCKPP_NS::Constraint<int>::AP myequal_i = MOCKPP_NS::eq( 1 );
  MOCKPP_ASSERT_TRUE( myequal_i->eval( 1 ) == true );
  MOCKPP_ASSERT_TRUE( myequal_i->eval( 2 ) == false );

  MOCKPP_NS::Constraint<unsigned int>::AP myequal_ui = MOCKPP_NS::eq( (unsigned int)1 );
  MOCKPP_ASSERT_TRUE( myequal_ui->eval( 1 ) == true );
  MOCKPP_ASSERT_TRUE( myequal_ui->eval( 2 ) == false );
}


void ChainingMockObjectSupport_test::test_same_func()
{
  MOCKPP_STL::string str( "string" );
  MOCKPP_NS::Constraint<MOCKPP_STL::string>::AP same = MOCKPP_NS::same( str );
  MOCKPP_ASSERT_TRUE( same->eval( str ) == true );
  MOCKPP_ASSERT_TRUE( same->eval( "string" ) == false );
}


void ChainingMockObjectSupport_test::test_closeto_func()
{
  MOCKPP_NS::Constraint<float>::AP myclose = MOCKPP_NS::eq( 2.0f, 0.5f );
  MOCKPP_ASSERT_TRUE( myclose->eval( 2.4f ) == true );
  MOCKPP_ASSERT_TRUE( myclose->eval( 2 ) == true );
  MOCKPP_ASSERT_TRUE( myclose->eval( 1.6f ) == true );
  MOCKPP_ASSERT_TRUE( myclose->eval( 1 ) == false );
  MOCKPP_ASSERT_TRUE( myclose->eval( 3 ) == false );
}


void ChainingMockObjectSupport_test::test_any_func()
{
  MOCKPP_NS::TypelessConstraint::AP myany = MOCKPP_NS::any();
  MOCKPP_ASSERT_TRUE( myany->typelessEval( ) == true );
}


void ChainingMockObjectSupport_test::test_nothing_func()
{
  MOCKPP_NS::TypelessConstraint::AP mynothing = MOCKPP_NS::nothing();
  MOCKPP_ASSERT_TRUE( mynothing->typelessEval() == false );
}


void ChainingMockObjectSupport_test::test_invoke_not()
{
  MOCKPP_NS::IsAnything myany;
  MOCKPP_ASSERT_TRUE( myany.typelessEval() == true );

  MOCKPP_NS::TypelessConstraint::AP acb = new MOCKPP_NS::IsAnything;
  MOCKPP_NS::Constraint<bool>::AP mynot = MOCKPP_NS::logic_not<bool>( acb );

  MOCKPP_ASSERT_TRUE( mynot->eval( true ) == false );
  MOCKPP_ASSERT_TRUE( mynot->eval( false ) == false );
}


void ChainingMockObjectSupport_test::test_invoke_and()
{
  MOCKPP_NS::Constraint<bool>::AP v_true = new MOCKPP_NS::IsEqual<bool>( true );
  MOCKPP_NS::Constraint<bool>::AP v_false = new MOCKPP_NS::IsEqual<bool>( false );

  MOCKPP_NS::Constraint<bool>::AP and1 = MOCKPP_NS::logic_and<bool>( v_true, v_false );
  MOCKPP_UNUSED(and1);

  MOCKPP_NS::Constraint<bool>::AP and2
  = MOCKPP_NS::logic_and<bool>( new MOCKPP_NS::IsEqual<bool>( false ),
                             new MOCKPP_NS::IsEqual<bool>( true ) );

  MOCKPP_ASSERT_TRUE( and2->eval( true ) == false );
  MOCKPP_ASSERT_TRUE( and2->eval( false ) == false );

  and2 = MOCKPP_NS::logic_and<bool>( new MOCKPP_NS::IsEqual<bool>( true ),
                                  new MOCKPP_NS::IsEqual<bool>( true ) );

  MOCKPP_ASSERT_TRUE( and2->eval( true ) == true );
  MOCKPP_ASSERT_TRUE( and2->eval( false ) == false );
}


void ChainingMockObjectSupport_test::test_invoke_or()
{
  MOCKPP_NS::Constraint<int>::AP v_1 = new MOCKPP_NS::IsEqual<int>( 1 );
  MOCKPP_NS::Constraint<int>::AP v_5 = new MOCKPP_NS::IsEqual<int>( 5 );

  MOCKPP_NS::Constraint<int>::AP
  or0 = MOCKPP_NS::logic_or<int>( new MOCKPP_NS::IsEqual<int>( 1 ),
                               new MOCKPP_NS::IsEqual<int>( 5 ) );
  MOCKPP_UNUSED(or0);

  MOCKPP_NS::Constraint<int>::AP or1 = MOCKPP_NS::logic_or<int>( v_1, v_5 );
  or1 = MOCKPP_NS::logic_or<int>( new MOCKPP_NS::IsEqual<int>( 1 ),
                               new MOCKPP_NS::IsEqual<int>( 5 ) );

  MOCKPP_ASSERT_TRUE( or1->eval( 1 ) == true );
  MOCKPP_ASSERT_TRUE( or1->eval( 2 ) == false );
  MOCKPP_ASSERT_TRUE( or1->eval( 4 ) == false );
  MOCKPP_ASSERT_TRUE( or1->eval( 5 ) == true );
}


void ChainingMockObjectSupport_test::test_counts()
{
  MOCKPP_NS::TypelessMatcher::AP matcher;

  matcher = MOCKPP_NS::never( MOCKPP_PCHAR( "never?" ) );
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "never expected (never?)" ) );

  matcher = MOCKPP_NS::never();
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "never expected" ) );

  matcher = MOCKPP_NS::exactly( 3 );
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected 3 times, invoked 0 times" ) );

  matcher = MOCKPP_NS::atLeast( 3 );
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected at least 3 times, invoked 0 times" ) );

  matcher = MOCKPP_NS::atLeastOnce();
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected at least once" ) );

  matcher = MOCKPP_NS::unlimited();
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected unlimited invocations" ) );

  matcher = MOCKPP_NS::atMost( 3 );
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected at most 3 times, invoked 0 times" ) );

  matcher = MOCKPP_NS::once();
  MOCKPP_ASSERT_TRUE( matcher->toString() == MOCKPP_PCHAR( "expected once" ) );
}


void ChainingMockObjectSupport_test::test_actor()
{
  MOCKPP_NS::TypelessStub<int>::AP stub (new MOCKPP_NS::ReturnStub<int>( 1 ));

  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <1>" ) );

  stub = MOCKPP_NS::throwException<int>( 1 );
  MOCKPP_ASSERT_TRUE( stub->toString().find( MOCKPP_PCHAR( "throws <" ) ) != MOCKPP_NS::String::npos );
}


class MyStub : public MOCKPP_NS::CustomStub<int, int>
{
  public:

    MyStub(unsigned pos)
     : MOCKPP_NS::CustomStub<int, int>(MOCKPP_PCHAR("custom"))
     , position(pos)
    {
    }

    virtual MOCKPP_NS::String describeTo( MOCKPP_NS::String &buffer ) const
    {
      MOCKPP_NS::String fmt = MOCKPP_PCHAR("%1");
      fmt << position;
      internalDescribeTo(buffer);
      buffer += fmt;
      return buffer;
    }

    virtual int invoke( const int &/*invocation*/ )
    {
      return 0;
    }

  private:

    long position;
};


void ChainingMockObjectSupport_test::test_consecutive_custom()
{
  MOCKPP_NS::Stub<int, int>::AP stub;

#ifdef __BORLANDC__
  stub = MOCKPP_NS::onConsecutiveCalls<int, int>( new MyStub(1u),
                                                  new MyStub(2u) );
#else
  stub = MOCKPP_NS::onConsecutiveCalls<int, int>( MOCKPP_NS::Stub<int, int>::AP( new MyStub(1u)),
                                                  MOCKPP_NS::Stub<int, int>::AP( new MyStub(2u)) );
#endif

  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "custom1, and then custom2" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MyStub( (long)1 ),
                                        new MyStub( (long)2 ));
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "custom1, and then custom2" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MyStub( (long)1 ),
                                        new MyStub( (long)2 ),
                                        new MyStub( (long)3 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "custom1, and then custom2, and then custom3" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MyStub( (long)1 ),
                                        new MyStub( (long)2 ),
                                        new MyStub( (long)3 ),
                                        new MyStub( (long)4 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "custom1, and then custom2, and then custom3, and then custom4" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MyStub( (long)1 ),
                                        new MyStub( (long)2 ),
                                        new MyStub( (long)3 ),
                                        new MyStub( (long)4 ),
                                        new MyStub( (long)5 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "custom1, and then custom2, and then custom3, and then custom4, and then custom5" ) );
}


void ChainingMockObjectSupport_test::test_consecutive_typeless()
{
  MOCKPP_NS::TypelessStub<long>::AP stub;

  stub = MOCKPP_NS::onConsecutiveCalls<long>( MOCKPP_NS::returnValue<long>( 2l ),
                                              MOCKPP_NS::returnValue<long>( 3l ));
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <2>, and then returns <3>" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MOCKPP_NS::ReturnStub<long>( 1 ),
                                        new MOCKPP_NS::ReturnStub<long>( 2 ));
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <1>, and then returns <2>" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MOCKPP_NS::ReturnStub<long>( 1 ),
                                        new MOCKPP_NS::ReturnStub<long>( 2 ),
                                        new MOCKPP_NS::ReturnStub<long>( 3 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <1>, and then returns <2>, and then returns <3>" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MOCKPP_NS::ReturnStub<long>( 1 ),
                                        new MOCKPP_NS::ReturnStub<long>( 2 ),
                                        new MOCKPP_NS::ReturnStub<long>( 3 ),
                                        new MOCKPP_NS::ReturnStub<long>( 4 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <1>, and then returns <2>, and then returns <3>, and then returns <4>" ) );

  stub = MOCKPP_NS::onConsecutiveCalls( new MOCKPP_NS::ReturnStub<long>( 1 ),
                                        new MOCKPP_NS::ReturnStub<long>( 2 ),
                                        new MOCKPP_NS::ReturnStub<long>( 3 ),
                                        new MOCKPP_NS::ReturnStub<long>( 4 ),
                                        new MOCKPP_NS::ReturnStub<long>( 5 ) );
  MOCKPP_ASSERT_TRUE( stub->toString() == MOCKPP_PCHAR( "returns <1>, and then returns <2>, and then returns <3>, and then returns <4>, and then returns <5>" ) );

  long intvec[] = { 12, 78 };
  stub = MOCKPP_NS::onConsecutiveCalls<long>( MOCKPP_NS::returnValue<long>( &intvec[0], &intvec[2]));
  MOCKPP_ASSERT_TRUE( stub->toString().find( MOCKPP_PCHAR( "12" )) != MOCKPP_NS::String::npos);
  MOCKPP_ASSERT_TRUE( stub->toString().find( MOCKPP_PCHAR( "78" )) != MOCKPP_NS::String::npos);

  stub = MOCKPP_NS::randomValue<long>( 1234, 5678 );
//  std::wcout << stub->toString().c_str() << std::endl;
  MOCKPP_ASSERT_TRUE( stub->toString().find(MOCKPP_PCHAR("random {1234 .. 5678}")) != MOCKPP_NS::String::npos);

  stub = MOCKPP_NS::randomValue<long>( 5678 );
  MOCKPP_ASSERT_TRUE( stub->toString().find(MOCKPP_PCHAR("random {0 .. 5678}")) != MOCKPP_NS::String::npos);
}


class Test
{
  public:

   virtual ~Test()
   {}

   virtual unsigned uident(unsigned userno, MOCKPP_STL::string &user, MOCKPP_STL::string &adr) = 0;

   virtual void vident(unsigned userno, MOCKPP_STL::string &user, MOCKPP_STL::string &adr) = 0;

};


class TestMock : public Test
               , public MOCKPP_NS::ChainableMockObject
{
  public:

    TestMock()
    : MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("name"), 0)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(uident, _)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(vident, _)
    {}

    MOCKPP_CHAINABLE_EXT3(TestMock, unsigned, uident, unsigned, MOCKPP_STL::string &, MOCKPP_STL::string &,
                                    unsigned, _,      unsigned, MOCKPP_STL::string  , MOCKPP_STL::string );

    MOCKPP_VOID_CHAINABLE_EXT3(TestMock, vident, unsigned, MOCKPP_STL::string &, MOCKPP_STL::string &,
                                         _,      unsigned, MOCKPP_STL::string,   MOCKPP_STL::string );

};


void ChainingMockObjectSupport_test::test_trigger()
{
  TestMock test;
  MOCKPP_CHAINER_FOR_EXT(TestMock, uident, _) uidenter(&test);
  MOCKPP_CHAINER_FOR_EXT(TestMock, vident, _) videnter(&test);

/////////////////

  MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string> *tc_1a = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("number-one");
  MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string> *tc_1b = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("address-one");
  uidenter.expects(MOCKPP_NS::once())
          .with(MOCKPP_NS::eq(1u),
                tc_1a,
                tc_1b)
          .will(MOCKPP_NS::returnValueAndTrigger(11u, tc_1a, tc_1b));

  MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string> *tc_2a = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("number-two");
  MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string> *tc_2b = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("address-two");
  uidenter.expects(MOCKPP_NS::once())
          .with(MOCKPP_NS::eq(2u),
                tc_2a,
                tc_2b)
          .will(MOCKPP_NS::returnValueAndTrigger(22u, tc_2a, tc_2b));

  MOCKPP_STL::string s;
  MOCKPP_STL::string adr;
  unsigned ret;

  ret = test.uident(1, s, adr);
  MOCKPP_ASSERT_EQUALS(11u, ret);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("number-one"), s);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("address-one"), adr);

  ret = test.uident(2, s, adr);
  MOCKPP_ASSERT_EQUALS(22u, ret);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("number-two"), s);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("address-two"), adr);

/////////////////

  tc_1a = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("number-one");
  tc_1b = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("address-one");
  videnter.expects(MOCKPP_NS::once())
          .with(MOCKPP_NS::eq(1u),
                tc_1a,
                tc_1b)
          .will(MOCKPP_NS::trigger(tc_1a, tc_1b));

  tc_2a = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("number-two");
  tc_2b = new MOCKPP_NS::TriggeredOutbound<MOCKPP_STL::string>("address-two");
  videnter.expects(MOCKPP_NS::once())
          .with(MOCKPP_NS::eq(2u),
                tc_2a,
                tc_2b)
          .will(MOCKPP_NS::trigger(tc_2a, tc_2b));

  test.vident(1, s, adr);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("number-one"), s);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("address-one"), adr);

  test.vident(2, s, adr);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("number-two"), s);
  MOCKPP_ASSERT_EQUALS(MOCKPP_STL::string("address-two"), adr);
}



MOCKPP_NS_END




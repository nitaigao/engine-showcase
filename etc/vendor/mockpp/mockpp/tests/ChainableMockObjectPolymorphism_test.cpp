/***************************************************************************
                   ChainableMockObjectPolymorphism_test.cpp
                                 -
      unit tests for MockVisitor class and polymorphic parameter methods
                             -------------------
    begin                : Tue Sep 20 2005
    copyright            : (C) 2005-2007 contributed by Mathieu Champlon
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockObjectPolymorphism_test.cpp 1274 2007-01-07 15:52:48Z ewald-arnold $

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

// #include <iostream>

#include "SelectFramework.h"

#include <mockpp/chaining/ChainableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>



class MockVisitorPolymorphism_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( MockVisitorPolymorphism_test );

  CPPUNIT_TEST(test_visitor_retrieves_parameter);
  CPPUNIT_TEST(test_visitor_retrieves_mock_parameter);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_visitor_retrieves_mock_parameter();
  MOCKPP_TESTMETHOD_DECL void test_visitor_retrieves_parameter();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( MockVisitorPolymorphism_test );
MOCKPP_CXXTEST(MockVisitorPolymorphism_test, test_visitor_retrieves_parameter);
MOCKPP_CXXTEST(MockVisitorPolymorphism_test, test_visitor_retrieves_mock_parameter);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(MockVisitorPolymorphism_test::test_visitor_retrieves_parameter);
MOCKPP_BOOST_TEST(MockVisitorPolymorphism_test::test_visitor_retrieves_mock_parameter);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (MockVisitorPolymorphism_test);

#endif // frameworks


class Parameter
{
public:

  virtual ~Parameter()
  {}

  virtual void doSomething() = 0;
};


class Visitor
{
public:

  virtual ~Visitor()
  {}

  virtual void visit1( const Parameter & ) = 0;
  virtual void visit2( const Parameter &, int ) = 0;
  virtual void visit3( const Parameter &, const Parameter &, const Parameter & ) = 0;
};


class UnderTest
{
public:

  UnderTest( const Parameter &in_parameter1,
             const Parameter &in_parameter2,
             const Parameter &in_parameter3 )
    : parameter1( in_parameter1 )
    , parameter2( in_parameter2 )
    , parameter3( in_parameter3 )
  {}

  UnderTest( const Parameter &in_parameter1)
    : parameter1( in_parameter1 )
    , parameter2( in_parameter1 )
    , parameter3( in_parameter1 )
  {}

  void accept1( Visitor &visitor )
  {
//     MOCKPP_STD_NS::cout <<  "accept1\n";
    visitor.visit1( parameter1 );
  }

  void accept3( Visitor &visitor )
  {
//     MOCKPP_STD_NS::cout <<  "accept3\n";
    visitor.visit3( parameter1, parameter2, parameter3 );
  }

private:

  const Parameter& parameter1;
  const Parameter& parameter2;
  const Parameter& parameter3;
};


class MockParameter : public MOCKPP_NS::ChainableMockObject,
                      public Parameter
{
public:

  MockParameter(const MOCKPP_NS::String &name)
    : MOCKPP_NS::ChainableMockObject(name, 0)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(doSomething)
  {}

  MOCKPP_VOID_CHAINABLE0(MockParameter, doSomething);

  unsigned a;
};


#ifdef MOCKPP_USE_INVOCATION_EQUALS

template <>
bool MOCKPP_NS::invocationComparison<MockParameter>(const MockParameter &left,
                                                 const MockParameter &right)
{
  MOCKPP_STD_NS::cout <<  "invocationComparison<MockParameter>\n";
  return left.a == right.a;
}


template <>
bool MOCKPP_NS::invocationComparison<Parameter>(const Parameter &left,
                                             const Parameter &right)
{
  MOCKPP_STD_NS::cout <<  "invocationComparison<Parameter>\n";
  return MOCKPP_NS::isSameComparison(left, right);
}

#endif // MOCKPP_USE_INVOCATION_EQUALS


class MockVisitor1 : public MOCKPP_NS::ChainableMockObject,
                     public Visitor
{
public:

  MockVisitor1(const MOCKPP_NS::String &name)
    : MOCKPP_NS::ChainableMockObject(name, 0)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(visit1)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(visit2)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(visit3)
  {}

  MOCKPP_VOID_CHAINABLE_EXT1(MockVisitor1, visit1, const Parameter &,
                                                 , Parameter);

  MOCKPP_VOID_CHAINABLE_EXT2(MockVisitor1, visit2, const Parameter &, int,
                                                 , Parameter        , int);

  MOCKPP_VOID_CHAINABLE_EXT3(MockVisitor1, visit3, const Parameter &, const Parameter &, const Parameter &,
                                                 , Parameter,         Parameter,         Parameter);
};


void MockVisitorPolymorphism_test::test_visitor_retrieves_parameter()
{
  MockParameter mp1(MOCKPP_PCHAR("mock-parameter1"));
  MockParameter mp2(MOCKPP_PCHAR("mock-parameter2"));

//  mp1 == mp2;  <== does not work! missing operator==

  UnderTest ut1(mp1);
  MockVisitor1 mv1(MOCKPP_PCHAR("mock-visitor1"));
  MOCKPP_CHAINER_FOR(MockVisitor1, visit1) chain1(&mv1);
  chain1.expects(MOCKPP_NS::once())
        .with(MOCKPP_NS::same<Parameter>(mp1));
  ut1.accept1(mv1);

  UnderTest ut3(mp1, mp2, mp1);
  MockVisitor1 mv3(MOCKPP_PCHAR("mock-visitor3"));
  MOCKPP_CHAINER_FOR(MockVisitor1, visit3) chain3(&mv3);
  chain3.expects(MOCKPP_NS::once())
        .with(MOCKPP_NS::same<Parameter>(mp1),
              MOCKPP_NS::logic_not<Parameter>(MOCKPP_NS::same<Parameter>(mp1)),
              MOCKPP_NS::same<Parameter>(mp1));
  chain3.expects(MOCKPP_NS::once())
        .with(MOCKPP_NS::same<Parameter>(mp1),
              MOCKPP_NS::same<Parameter>(mp2),
              MOCKPP_NS::same<Parameter>(mp1));
  ut3.accept3(mv3);
  ut3.accept3(mv3);

  mv1.verify();
  mv3.verify();
}


class MockVisitor2 : public MOCKPP_NS::ChainableMockObject,
                     public Visitor
{
public:

  MockVisitor2(const MOCKPP_NS::String &name)
    : MOCKPP_NS::ChainableMockObject(name, 0)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(visit1)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(visit2)
    , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(visit3)
  {}

  MOCKPP_VOID_CHAINABLE_EXT1(MockVisitor2, visit1, const Parameter &,
                                                 , Parameter);

  MOCKPP_VOID_CHAINABLE_EXT2(MockVisitor2, visit2, const Parameter &, int,
                                                 , Parameter        , int);

  MOCKPP_VOID_CHAINABLE_EXT3(MockVisitor2, visit3, const Parameter &, const Parameter &, const Parameter &,
                                                 , Parameter,         Parameter,         Parameter);
};


void MockVisitorPolymorphism_test::test_visitor_retrieves_mock_parameter()
{
  MockParameter mp(MOCKPP_PCHAR("mock-parameter"));
  UnderTest ut(mp);

  MockVisitor2 mv(MOCKPP_PCHAR("mock-visitor"));
  MOCKPP_CHAINER_FOR(MockVisitor2, visit1) chain(&mv);
  chain.expects(MOCKPP_NS::once())
       .with(MOCKPP_NS::same<Parameter>(mp));

  ut.accept1(mv);

  mv.verify();
}




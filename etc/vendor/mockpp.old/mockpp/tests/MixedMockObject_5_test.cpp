/***************************************************************************
                     MixedMockObject_5_test.cpp
                                 -
       unit tests for MixedMockObject class and 5 parameter methods
                             -------------------
    begin                : Sat Mar 05 2005
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: MixedMockObject_5_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/ExpectationValue.h>
#include <mockpp/MixedMockObject.h>
#include <mockpp/util/AssertMo.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/mockpp_pti.h>

#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsNot.h>

#include "classes_ABCDE.h"



//MOCKPP_TYPELESSSTUBADAPTER_PTI_IMPL(xx)


class MixedMockObject_5_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( MixedMockObject_5_test );

  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_chainer);
  CPPUNIT_TEST(test_return);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_chainer();
  MOCKPP_TESTMETHOD_DECL void test_return();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( MixedMockObject_5_test );
MOCKPP_CXXTEST(MixedMockObject_5_test, test_controller);
MOCKPP_CXXTEST(MixedMockObject_5_test, test_chainer);
MOCKPP_CXXTEST(MixedMockObject_5_test, test_return);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(MixedMockObject_5_test::test_controller);
MOCKPP_BOOST_TEST(MixedMockObject_5_test::test_chainer);
MOCKPP_BOOST_TEST(MixedMockObject_5_test::test_return);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (MixedMockObject_5_test);

#endif // frameworks


class MyMixedMockObject_5_Interface
{
  public:

     virtual ~MyMixedMockObject_5_Interface()
     {}

     virtual int chain_mixed5 (const CA&, const CB&, const CC&, const CD&, const CE&) = 0;
     virtual int chain_mixed4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int chain_mixed3 (const CA&, const CB&, const CC&) = 0;
     virtual int chain_mixed2 (const CA&, const CB&) = 0;
     virtual int chain_mixed1 (const CA&) = 0;
     virtual int chain_mixed0 () = 0;

     virtual int visit_mixed5 (const CA&, const CB&, const CC&, const CD&, const CE&) = 0;
     virtual int visit_mixed4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int visit_mixed3 (const CA&, const CB&, const CC&) = 0;
     virtual int visit_mixed2 (const CA&, const CB&) = 0;
     virtual int visit_mixed1 (const CA&) = 0;
     virtual int visit_mixed0 () = 0;
};


class MyMixedMockObject_5 : public MOCKPP_NS::MixedMockObject,
                            public MyMixedMockObject_5_Interface
{
  public:

    MyMixedMockObject_5(const MOCKPP_NS::String &name)
      : MOCKPP_NS::MixedMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(chain_mixed5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE4(chain_mixed4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(chain_mixed3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(chain_mixed2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(chain_mixed1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chain_mixed0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(visit_mixed5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(visit_mixed4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(visit_mixed3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(visit_mixed2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(visit_mixed1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(visit_mixed0)
    {}

    MOCKPP_CHAINABLE5(MyMixedMockObject_5, int, chain_mixed5, CA, CB, CC, CD, CE);
    MOCKPP_CHAINABLE4(MyMixedMockObject_5, int, chain_mixed4, CA, CB, CC, CD);
    MOCKPP_CHAINABLE3(MyMixedMockObject_5, int, chain_mixed3, CA, CB, CC);
    MOCKPP_CHAINABLE2(MyMixedMockObject_5, int, chain_mixed2, CA, CB);
    MOCKPP_CHAINABLE1(MyMixedMockObject_5, int, chain_mixed1, CA);
    MOCKPP_CHAINABLE0(MyMixedMockObject_5, int, chain_mixed0);

    MOCKPP_VISITABLE5(MyMixedMockObject_5, int, visit_mixed5, CA, CB, CC, CD, CE);
    MOCKPP_VISITABLE4(MyMixedMockObject_5, int, visit_mixed4, CA, CB, CC, CD);
    MOCKPP_VISITABLE3(MyMixedMockObject_5, int, visit_mixed3, CA, CB, CC);
    MOCKPP_VISITABLE2(MyMixedMockObject_5, int, visit_mixed2, CA, CB);
    MOCKPP_VISITABLE1(MyMixedMockObject_5, int, visit_mixed1, CA);
    MOCKPP_VISITABLE0(MyMixedMockObject_5, int, visit_mixed0);
};


void MixedMockObject_5_test::test_controller()
{
  MyMixedMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyMixedMockObject_5, visit_mixed5) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));
  ctr.addReturnValue(1);
  ctr.addReturnValue(11);
  ctr.setDefaultReturnValue(123);
  ctr.addResponseValue(0, 1, 2, 3, 4, 5);
  ctr.addResponseValue(1, 0, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 0, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(0)), 1, 2, 3, 4, 5);
}


void MixedMockObject_5_test::test_chainer()
{
  MyMixedMockObject_5 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed0) c_chainer0(&mcm);
  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed1) c_chainer1(&mcm);
  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed2) c_chainer2(&mcm);
  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed3) c_chainer3(&mcm);
  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed4) c_chainer4(&mcm);
  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed5) c_chainer5(&mcm);

  c_chainer1.expects(MOCKPP_NS::once())
            .with(MOCKPP_NS::any());

  c_chainer2.expects(MOCKPP_NS::once())
            .with(MOCKPP_NS::any(),
                  MOCKPP_NS::any());
}


void MixedMockObject_5_test::test_return()
{
  MyMixedMockObject_5 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyMixedMockObject_5, chain_mixed5) c_chainer5(&mcm);
  MOCKPP_CONTROLLER_FOR(MyMixedMockObject_5, visit_mixed5) ctr (&mcm);

  c_chainer5.expects(MOCKPP_NS::once())
            .with(MOCKPP_NS::eq(CA(1)),
                  MOCKPP_NS::eq(CB(2)),
                  MOCKPP_NS::eq(CC(3)),
                  MOCKPP_NS::eq(CD(4)),
                  MOCKPP_NS::eq(CE(5)))
             .will(MOCKPP_NS::returnValue(12345));

  c_chainer5.expects(MOCKPP_NS::once())
            .with(MOCKPP_NS::eq(CA(5)),
                  MOCKPP_NS::eq(CB(4)),
                  MOCKPP_NS::eq(CC(3)),
                  MOCKPP_NS::eq(CD(2)),
                  MOCKPP_NS::eq(CE(1)))
             .will(MOCKPP_NS::returnValue(54321));

  ctr.setDefaultReturnValue(123);
  ctr.addReturnValue(1,2);
  ctr.addReturnValue(11, 2);

  mcm.visit_mixed5(1, 2, 3, 4, 5);
  mcm.visit_mixed5(1, 2, 3, 4, 5);
  mcm.visit_mixed5(2, 2, 3, 4, 5);
  mcm.visit_mixed5(2, 2, 3, 4, 5);
  mcm.visit_mixed5(3, 2, 3, 4, 5);
  mcm.visit_mixed5(4, 2, 3, 4, 5);

  mcm.activate();

  MOCKPP_ASSERT_TRUE(1 == mcm.visit_mixed5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(1 == mcm.visit_mixed5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(11 == mcm.visit_mixed5(2, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(11 == mcm.visit_mixed5(2, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(123 == mcm.visit_mixed5(3, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(123 == mcm.visit_mixed5(4, 2, 3, 4, 5));

  MOCKPP_ASSERT_TRUE(12345 == mcm.chain_mixed5(CA(1), CB(2), CC(3), CD(4), CE(5)));
  MOCKPP_ASSERT_TRUE(54321 == mcm.chain_mixed5(CA(5), CB(4), CC(3), CD(2), CE(1)));

  mcm.verify();
}




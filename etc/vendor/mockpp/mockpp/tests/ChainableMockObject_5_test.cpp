/***************************************************************************
                     ChainableMockObject_5_test.cpp
                                 -
  unit tests for ChainableMockObject class and 5 parameter methods
                             -------------------
    begin                : Thu Sep 28 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: ChainableMockObject_5_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/chaining/ChainableMockObject.h>

#include "classes_ABCDE.h"


#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/constraint/OutBound.h>


class ChainableMockObject_5_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( ChainableMockObject_5_test );

  CPPUNIT_TEST(test_outbound);
  CPPUNIT_TEST(test_chainer);
  CPPUNIT_TEST(test_chainer_ext);
  CPPUNIT_TEST( test_parameter_5 );

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_outbound();
  MOCKPP_TESTMETHOD_DECL void test_parameter_5();
  MOCKPP_TESTMETHOD_DECL void test_chainer_ext();
  MOCKPP_TESTMETHOD_DECL void test_chainer();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( ChainableMockObject_5_test );
MOCKPP_CXXTEST(ChainableMockObject_5_test, test_outbound);
MOCKPP_CXXTEST(ChainableMockObject_5_test, test_chainer);
MOCKPP_CXXTEST(ChainableMockObject_5_test, test_chainer_ext);
MOCKPP_CXXTEST(ChainableMockObject_5_test,  test_parameter_5 );

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(ChainableMockObject_5_test::test_outbound);
MOCKPP_BOOST_TEST(ChainableMockObject_5_test::test_chainer);
MOCKPP_BOOST_TEST(ChainableMockObject_5_test::test_chainer_ext);
MOCKPP_BOOST_TEST(ChainableMockObject_5_test:: test_parameter_5 );

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (ChainableMockObject_5_test);

#endif // frameworks


class MyChainableMockObject_5_Interface
{
  public:

    virtual ~MyChainableMockObject_5_Interface()
    {}

     virtual int chainable5 (const CA&, const CB&, const CC&, const CD&, const CE&) = 0;
     virtual int chainable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int chainable3 (const CA&, const CB&, const CC&) = 0;
     virtual int chainable2 (const CA&, const CB&) = 0;
     virtual int chainable1 (const CA&) = 0;
     virtual int chainable0 () = 0;

     virtual int const_chainable5 (const CA&, const CB&, const CC&, const CD&, const CE&) const = 0;
     virtual int const_chainable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual int const_chainable3 (const CA&, const CB&, const CC&) const = 0;
     virtual int const_chainable2 (const CA&, const CB&) const = 0;
     virtual int const_chainable1 (const CA&) const = 0;
     virtual int const_chainable0 () const = 0;
};


class MyChainableMockObject_5 : public MOCKPP_NS::ChainableMockObject,
                                public MyChainableMockObject_5_Interface
{
  public:

    MyChainableMockObject_5(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(chainable5)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE4(chainable4)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(chainable3)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(chainable2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(chainable1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(chainable0)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(const_chainable5)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE4(const_chainable4)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE3(const_chainable3)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE2(const_chainable2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE1(const_chainable1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE0(const_chainable0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE5(chainable_i)
    {}

    virtual ~MyChainableMockObject_5()
    {}

    MOCKPP_CHAINABLE5(MyChainableMockObject_5, int, chainable5, CA, CB, CC, CD, CE);
    MOCKPP_CHAINABLE4(MyChainableMockObject_5, int, chainable4, CA, CB, CC, CD);
    MOCKPP_CHAINABLE3(MyChainableMockObject_5, int, chainable3, CA, CB, CC);
    MOCKPP_CHAINABLE2(MyChainableMockObject_5, int, chainable2, CA, CB);
    MOCKPP_CHAINABLE1(MyChainableMockObject_5, int, chainable1, CA);
    MOCKPP_CHAINABLE0(MyChainableMockObject_5, int, chainable0);

    MOCKPP_CONST_CHAINABLE5(MyChainableMockObject_5, int, const_chainable5, CA, CB, CC, CD, CE);
    MOCKPP_CONST_CHAINABLE4(MyChainableMockObject_5, int, const_chainable4, CA, CB, CC, CD);
    MOCKPP_CONST_CHAINABLE3(MyChainableMockObject_5, int, const_chainable3, CA, CB, CC);
    MOCKPP_CONST_CHAINABLE2(MyChainableMockObject_5, int, const_chainable2, CA, CB);
    MOCKPP_CONST_CHAINABLE1(MyChainableMockObject_5, int, const_chainable1, CA);
    MOCKPP_CONST_CHAINABLE0(MyChainableMockObject_5, int, const_chainable0);

    MOCKPP_CHAINABLE5(MyChainableMockObject_5, int, chainable_i, unsigned, unsigned, unsigned, unsigned, unsigned);
};


class MyChainableMockObject_5_ex_Interface
{
  public:

    virtual ~MyChainableMockObject_5_ex_Interface()
    {}

     virtual int chainable5 (const CA&, const CB&, const CC&, const CD&, CE) = 0;
     virtual int chainable5 (const CA&, const CB&, const CC&, const CD&, CF) = 0;
     virtual int chainable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int chainable3 (const CA&, const CB&, const CC&) = 0;
     virtual int chainable2 (const CA&, const CB&) = 0;
     virtual int chainable1 (const CA&) = 0;
     virtual int chainable0 () = 0;

     virtual int const_chainable5 (const CA&, const CB&, const CC&, const CD&, CE) const = 0;
     virtual int const_chainable5 (const CA&, const CB&, const CC&, const CD&, CF) const = 0;
     virtual int const_chainable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual int const_chainable3 (const CA&, const CB&, const CC&) const = 0;
     virtual int const_chainable2 (const CA&, const CB&) const = 0;
     virtual int const_chainable1 (const CA&) const = 0;
     virtual int const_chainable0 () const = 0;

     virtual int const_chainable5_p (const CA*, const CB*, const CC*, const CD*, const CE*) const = 0;
     virtual int const_chainable4_p (const CA*, const CB*, const CC*, const CD*) const = 0;
     virtual int const_chainable3_p (const CA*, const CB*, const CC*) const = 0;
     virtual int const_chainable2_p (const CA*, const CB*) const = 0;
     virtual int const_chainable1_p (const CA*) const = 0;
     virtual int const_chainable0_p () const = 0;
};


class MyChainableMockObject_5ex : public MOCKPP_NS::ChainableMockObject,
                                  public MyChainableMockObject_5_ex_Interface
{
  public:

    MyChainableMockObject_5ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::ChainableMockObject(name, 0)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(chainable5, over1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(chainable5, over2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT4(chainable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(chainable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT2(chainable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(chainable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT0(chainable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(const_chainable5, over1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(const_chainable5, over2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT4(const_chainable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(const_chainable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT2(const_chainable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(const_chainable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT0(const_chainable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(const_chainable5_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT4(const_chainable4_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT3(const_chainable3_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT2(const_chainable2_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT1(const_chainable1_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT0(const_chainable0_p, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_CHAINABLE_EXT5(outbound, Ext)
    {}

    virtual ~MyChainableMockObject_5ex()
    {}

    MOCKPP_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, chainable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                     int, over1,      CA,        CB,        CC,        CD,        CE);

    MOCKPP_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, chainable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                     int, over2,      CA,        CB,        CC,        CD,        CF);

    MOCKPP_CHAINABLE_EXT4(MyChainableMockObject_5ex, int, chainable4, const CA&, const CB&, const CC&, const CD&,
                                                     int, Ext,        CA,        CB,        CC,        CD);

    MOCKPP_CHAINABLE_EXT3(MyChainableMockObject_5ex, int, chainable3, const CA&, const CB&, const CC&,
                                                     int, Ext,        CA,        CB,        CC);

    MOCKPP_CHAINABLE_EXT2(MyChainableMockObject_5ex, int, chainable2, const CA&, const CB&,
                                                     int, Ext,        CA,        CB);

    MOCKPP_CHAINABLE_EXT1(MyChainableMockObject_5ex, int, chainable1, const CA&,
                                                     int, Ext,        CA);

    MOCKPP_CHAINABLE_EXT0(MyChainableMockObject_5ex, int, chainable0,
                                                     int, Ext);


    MOCKPP_CONST_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, const_chainable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                           int, over1,            CA,        CB,        CC,        CD,        CE);

    MOCKPP_CONST_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, const_chainable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                           int, over2,            CA,        CB,        CC,        CD,        CF);

    MOCKPP_CONST_CHAINABLE_EXT4(MyChainableMockObject_5ex, int, const_chainable4, const CA&, const CB&, const CC&, const CD&,
                                                           int, Ext,              CA,        CB,        CC,        CD);

    MOCKPP_CONST_CHAINABLE_EXT3(MyChainableMockObject_5ex, int, const_chainable3, const CA&, const CB&, const CC&,
                                                           int, Ext,              CA,        CB,        CC);

    MOCKPP_CONST_CHAINABLE_EXT2(MyChainableMockObject_5ex, int, const_chainable2, const CA&, const CB&,
                                                           int, Ext,              CA,        CB);

    MOCKPP_CONST_CHAINABLE_EXT1(MyChainableMockObject_5ex, int, const_chainable1, const CA&,
                                                           int, Ext,              CA);

    MOCKPP_CONST_CHAINABLE_EXT0(MyChainableMockObject_5ex, int, const_chainable0,
                                                           int, Ext);


    MOCKPP_CONST_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, const_chainable5_p, const CA*, const CB*, const CC*, const CD*, const CE*,
                                                           int, Ext,                CA*,       CB*,       CC*,       CD*,       CE*);

    MOCKPP_CONST_CHAINABLE_EXT4(MyChainableMockObject_5ex, int, const_chainable4_p, const CA*, const CB*, const CC*, const CD*,
                                                           int, Ext,                CA*,       CB*,       CC*,       CD*);

    MOCKPP_CONST_CHAINABLE_EXT3(MyChainableMockObject_5ex, int, const_chainable3_p, const CA*, const CB*, const CC*,
                                                           int, Ext,                CA*,       CB*,       CC*);

    MOCKPP_CONST_CHAINABLE_EXT2(MyChainableMockObject_5ex, int, const_chainable2_p, const CA*, const CB*,
                                                           int, Ext,                CA*,       CB*);

    MOCKPP_CONST_CHAINABLE_EXT1(MyChainableMockObject_5ex, int, const_chainable1_p, const CA*,
                                                           int, Ext,                CA*);

    MOCKPP_CONST_CHAINABLE_EXT0(MyChainableMockObject_5ex, int, const_chainable0_p,
                                                           int, Ext);

    MOCKPP_CHAINABLE_EXT5(MyChainableMockObject_5ex, int, outbound, unsigned &, unsigned &, unsigned &, unsigned &, unsigned &,
                                                     int, Ext,      unsigned,   unsigned,   unsigned,   unsigned,   unsigned );
};


void ChainableMockObject_5_test::test_parameter_5()
{
  MyChainableMockObject_5 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable_i) chain(&mcm);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq( (unsigned int) 12),
             MOCKPP_NS::eq( (unsigned int) 23),
             MOCKPP_NS::eq( (unsigned int) 34),
             MOCKPP_NS::eq( (unsigned int) 45),
             MOCKPP_NS::eq( (unsigned int) 56))
       .will(MOCKPP_NS::returnValue<int>(12345));

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::eq( (unsigned int) 65),
             MOCKPP_NS::eq( (unsigned int) 54),
             MOCKPP_NS::eq( (unsigned int) 43),
             MOCKPP_NS::eq( (unsigned int) 32),
             MOCKPP_NS::eq( (unsigned int) 21))
       .will(MOCKPP_NS::returnValue<int>(54321));

  MOCKPP_ASSERT_TRUE(mcm.chainable_i(12, 23, 34, 45, 56) == 12345);
  MOCKPP_ASSERT_TRUE(mcm.chainable_i(65, 54, 43, 32, 21) == 54321);

  MOCKPP_TRY
  {
    mcm.chainable_i(1, 2, 3, 4, 5);
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("should have thrown"), false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &ex)
  {
    MOCKPP_ASSERT_TRUE(ex.getMessage().find(MOCKPP_PCHAR("unexpected invocation")) != MOCKPP_NS::String::npos);
  }
#endif

  mcm.verify();
}


void ChainableMockObject_5_test::test_chainer()
{
  MyChainableMockObject_5 mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable0) c_chainer0(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable1) c_chainer1(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable1) c_chainer2(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable1) c_chainer3(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable1) c_chainer4(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, const_chainable1) c_chainer5(&mcm);

  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable0) chainer0(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable1) chainer1(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable1) chainer2(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable1) chainer3(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable1) chainer4(&mcm);
  MOCKPP_CHAINER_FOR(MyChainableMockObject_5, chainable1) chainer5(&mcm);
}


void ChainableMockObject_5_test::test_chainer_ext()
{
  MyChainableMockObject_5ex mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable0, Ext) c_chainer0(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable1, Ext) c_chainer1(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable1, Ext) c_chainer2(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable1, Ext) c_chainer3(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable1, Ext) c_chainer4(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, const_chainable1, Ext) c_chainer5(&mcm);

  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable0, Ext) chainer0(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable1, Ext) chainer1(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable1, Ext) chainer2(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable1, Ext) chainer3(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable1, Ext) chainer4(&mcm);
  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, chainable1, Ext) chainer5(&mcm);
}


void ChainableMockObject_5_test::test_outbound()
{
  MyChainableMockObject_5ex mcm(MOCKPP_PCHAR("chainable-object"));

  MOCKPP_CHAINER_FOR_EXT(MyChainableMockObject_5ex, outbound, Ext) chain(&mcm);

  chain.stubs(MOCKPP_NS::once())
       .with(MOCKPP_NS::outBound( (unsigned int) 1123),
             MOCKPP_NS::outBound( (unsigned int) 2123),
             MOCKPP_NS::outBound( (unsigned int) 3123),
             MOCKPP_NS::outBound( (unsigned int) 4123),
             MOCKPP_NS::outBound( (unsigned int) 5123));

  unsigned val1 = 0;
  unsigned val2 = 0;
  unsigned val3 = 0;
  unsigned val4 = 0;
  unsigned val5 = 0;
  mcm.outbound(val1,val2, val3, val4, val5);
  MOCKPP_ASSERT_EQUALS((unsigned)1123, val1);
  MOCKPP_ASSERT_EQUALS((unsigned)2123, val2);
  MOCKPP_ASSERT_EQUALS((unsigned)3123, val3);
  MOCKPP_ASSERT_EQUALS((unsigned)4123, val4);
  MOCKPP_ASSERT_EQUALS((unsigned)5123, val5);
}




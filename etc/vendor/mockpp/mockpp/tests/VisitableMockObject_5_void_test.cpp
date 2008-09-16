/***************************************************************************
                     VisitableMockObject_5_void_test.cpp
                                 -
  unit tests for VisitableMockObject class and 5 parameter void methods
                             -------------------
    begin                : Die Feb 18 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_5_void_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <mockpp/visiting/VisitableMockObject.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsNot.h>
#include <mockpp/constraint/OutBound.h>

#include "classes_ABCDE.h"


class VisitableMockObject_5_void_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_5_void_test );

  CPPUNIT_TEST(test_outbound);
  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_parameter);
  CPPUNIT_TEST(test_parameter_ex);
  CPPUNIT_TEST(test_thrower);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_default_throw);
  CPPUNIT_TEST(test_constraint);
  CPPUNIT_TEST(test_controller_constraint);
  CPPUNIT_TEST(test_controller_constraint_ext);

  CPPUNIT_TEST(fail_add_throw_after_active);
  CPPUNIT_TEST(fail_add_throw_val_after_active);
  CPPUNIT_TEST(fail_set_throw_after_active);
  CPPUNIT_TEST(fail_add_method_after_active);
  CPPUNIT_TEST(fail_parameter);
  CPPUNIT_TEST(fail_unused_throwable);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_outbound();
  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_default_throw();
  MOCKPP_TESTMETHOD_DECL void test_parameter();
  MOCKPP_TESTMETHOD_DECL void test_parameter_ex();
  MOCKPP_TESTMETHOD_DECL void test_constraint();
  MOCKPP_TESTMETHOD_DECL void test_controller_constraint();
  MOCKPP_TESTMETHOD_DECL void test_controller_constraint_ext();

  MOCKPP_TESTMETHOD_DECL void fail_unused_throwable();
  MOCKPP_TESTMETHOD_DECL void fail_add_method_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_val_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_parameter();
};

#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_5_void_test );

MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_outbound);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_controller);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_parameter);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_parameter_ex);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_thrower);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_clear);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_default_throw);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_constraint);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_controller_constraint);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, test_controller_constraint_ext);

MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_add_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_add_throw_val_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_set_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_add_method_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_parameter);
MOCKPP_CXXTEST(VisitableMockObject_5_void_test, fail_unused_throwable);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_outbound);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_controller);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_parameter_ex);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_thrower);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_clear);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_default_throw);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_constraint);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_controller_constraint);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::test_controller_constraint_ext);

MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_add_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_add_throw_val_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_set_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_add_method_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_5_void_test::fail_unused_throwable);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_5_void_test);

#endif // frameworks



class MyVisitableMockObject_5_void_Interface
{
  public:

     virtual ~MyVisitableMockObject_5_void_Interface()
     {}

     virtual void visitable5 (const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual void visitable4 (const unsigned&, const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual void visitable3 (const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual void visitable2 (const unsigned&, const unsigned&) = 0;
     virtual void visitable1 (const unsigned&) = 0;
     virtual void visitable0 () = 0;

     virtual void const_visitable5 (const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual void const_visitable4 (const unsigned&, const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual void const_visitable3 (const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual void const_visitable2 (const unsigned&, const unsigned&) const = 0;
     virtual void const_visitable1 (const unsigned&) const = 0;
     virtual void const_visitable0 () const = 0;

     virtual void abc_visitable5 (const CA&, const CB&, const CC&, const CD&, const CE&) = 0;
     virtual void abc_visitable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual void abc_visitable3 (const CA&, const CB&, const CC&) = 0;
     virtual void abc_visitable2 (const CA&, const CB&) = 0;
     virtual void abc_visitable1 (const CA&) = 0;
     virtual void abc_visitable0 () = 0;

     virtual void abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, const CE&) const = 0;
     virtual void abc_const_visitable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual void abc_const_visitable3 (const CA&, const CB&, const CC&) const = 0;
     virtual void abc_const_visitable2 (const CA&, const CB&) const = 0;
     virtual void abc_const_visitable1 (const CA&) const = 0;
     virtual void abc_const_visitable0 () const = 0;
};


class MyVisitableMockObject_5_void_test : public MOCKPP_NS::VisitableMockObject,
                                          public MyVisitableMockObject_5_void_Interface
{
  public:

    MyVisitableMockObject_5_void_test(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(const_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(const_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(const_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(const_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(const_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(const_visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(abc_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(abc_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(abc_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(abc_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(abc_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(abc_visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE5(abc_const_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE4(abc_const_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE3(abc_const_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE2(abc_const_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE1(abc_const_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE0(abc_const_visitable0)
    {}

    MOCKPP_VOID_VISITABLE5(MyVisitableMockObject_5_void_test, visitable5, unsigned, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VOID_VISITABLE4(MyVisitableMockObject_5_void_test, visitable4, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VOID_VISITABLE3(MyVisitableMockObject_5_void_test, visitable3, unsigned, unsigned, unsigned);
    MOCKPP_VOID_VISITABLE2(MyVisitableMockObject_5_void_test, visitable2, unsigned, unsigned);
    MOCKPP_VOID_VISITABLE1(MyVisitableMockObject_5_void_test, visitable1, unsigned);
    MOCKPP_VOID_VISITABLE0(MyVisitableMockObject_5_void_test, visitable0);

    MOCKPP_VOID_CONST_VISITABLE5(MyVisitableMockObject_5_void_test, const_visitable5, unsigned, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VOID_CONST_VISITABLE4(MyVisitableMockObject_5_void_test, const_visitable4, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VOID_CONST_VISITABLE3(MyVisitableMockObject_5_void_test, const_visitable3, unsigned, unsigned, unsigned);
    MOCKPP_VOID_CONST_VISITABLE2(MyVisitableMockObject_5_void_test, const_visitable2, unsigned, unsigned);
    MOCKPP_VOID_CONST_VISITABLE1(MyVisitableMockObject_5_void_test, const_visitable1, unsigned);
    MOCKPP_VOID_CONST_VISITABLE0(MyVisitableMockObject_5_void_test, const_visitable0);

    MOCKPP_VOID_VISITABLE5(MyVisitableMockObject_5_void_test, abc_visitable5, CA, CB, CC, CD, CE);
    MOCKPP_VOID_VISITABLE4(MyVisitableMockObject_5_void_test, abc_visitable4, CA, CB, CC, CD);
    MOCKPP_VOID_VISITABLE3(MyVisitableMockObject_5_void_test, abc_visitable3, CA, CB, CC);
    MOCKPP_VOID_VISITABLE2(MyVisitableMockObject_5_void_test, abc_visitable2, CA, CB);
    MOCKPP_VOID_VISITABLE1(MyVisitableMockObject_5_void_test, abc_visitable1, CA);
    MOCKPP_VOID_VISITABLE0(MyVisitableMockObject_5_void_test, abc_visitable0);

    MOCKPP_VOID_CONST_VISITABLE5(MyVisitableMockObject_5_void_test, abc_const_visitable5, CA, CB, CC, CD, CE);
    MOCKPP_VOID_CONST_VISITABLE4(MyVisitableMockObject_5_void_test, abc_const_visitable4, CA, CB, CC, CD);
    MOCKPP_VOID_CONST_VISITABLE3(MyVisitableMockObject_5_void_test, abc_const_visitable3, CA, CB, CC);
    MOCKPP_VOID_CONST_VISITABLE2(MyVisitableMockObject_5_void_test, abc_const_visitable2, CA, CB);
    MOCKPP_VOID_CONST_VISITABLE1(MyVisitableMockObject_5_void_test, abc_const_visitable1, CA);
    MOCKPP_VOID_CONST_VISITABLE0(MyVisitableMockObject_5_void_test, abc_const_visitable0);

    bool isClear()
    {
      return    visitable5DefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable5Throwables.size() == 0

             && visitable5Parameter1.size() == 0
             && visitable5Parameter2.size() == 0
             && visitable5Parameter3.size() == 0
             && visitable5Parameter4.size() == 0
             && visitable5Parameter5.size() == 0;
    }
};

class MyVisitableMockObject_5_void_ex_Interface
{
  public:

     virtual ~MyVisitableMockObject_5_void_ex_Interface()
     {}

     virtual void visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long) = 0;
     virtual void visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual void visitable4 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual void visitable3 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual void visitable2 (const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual void visitable1 (const MOCKPP_STL::string &) = 0;
     virtual void visitable0 () = 0;

     virtual void const_visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long) const = 0;
     virtual void const_visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual void const_visitable4 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual void const_visitable3 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual void const_visitable2 (const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual void const_visitable1 (const MOCKPP_STL::string &) const = 0;
     virtual void const_visitable0 () const = 0;

     virtual void abc_visitable5 (const CA&, const CB&, const CC&, const CD&, CE) = 0;
     virtual void abc_visitable5 (const CA&, const CB&, const CC&, const CD&, CF) = 0;
     virtual void abc_visitable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual void abc_visitable3 (const CA&, const CB&, const CC&) = 0;
     virtual void abc_visitable2 (const CA&, const CB&) = 0;
     virtual void abc_visitable1 (const CA&) = 0;
     virtual void abc_visitable0 () = 0;

     virtual void cba_visitable5 (const CE&, const CD&, const CC&, const CB&, CA) = 0;
     virtual void cba_visitable5 (const CF&, const CD&, const CC&, const CB&, CA) = 0;
     virtual void cba_visitable4 (const CE&, const CD&, const CC&, const CB&) = 0;
     virtual void cba_visitable3 (const CE&, const CD&, const CC&) = 0;
     virtual void cba_visitable2 (const CE&, const CD&) = 0;
     virtual void cba_visitable1 (const CE&) = 0;
     virtual void cba_visitable0 () = 0;

     virtual void abc_visitable5_p (const CA*, const CB*, const CC*, const CD*, const CE*) = 0;
     virtual void abc_visitable4_p (const CA*, const CB*, const CC*, const CD*) = 0;
     virtual void abc_visitable3_p (const CA*, const CB*, const CC*) = 0;
     virtual void abc_visitable2_p (const CA*, const CB*) = 0;
     virtual void abc_visitable1_p (const CA*) = 0;
     virtual void abc_visitable0_p () = 0;

     virtual void abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, CE) const = 0;
     virtual void abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, CF) const = 0;
     virtual void abc_const_visitable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual void abc_const_visitable3 (const CA&, const CB&, const CC&) const = 0;
     virtual void abc_const_visitable2 (const CA&, const CB&) const = 0;
     virtual void abc_const_visitable1 (const CA&) const = 0;
     virtual void abc_const_visitable0 () const = 0;
};



class MyVisitableMockObject_5_void_ex : public MOCKPP_NS::VisitableMockObject,
                                        public MyVisitableMockObject_5_void_ex_Interface
{
  public:

    MyVisitableMockObject_5_void_ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(const_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(const_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(const_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(const_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(const_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(const_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(const_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(abc_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(abc_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(abc_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(abc_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(abc_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(abc_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(abc_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(cba_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(cba_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(cba_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(cba_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(cba_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(cba_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(cba_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(abc_visitable5_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(abc_visitable4_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(abc_visitable3_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(abc_visitable2_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(abc_visitable1_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(abc_visitable0_p, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(abc_const_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(abc_const_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT4(abc_const_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT3(abc_const_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT2(abc_const_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT1(abc_const_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT0(abc_const_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VOID_VISITABLE_EXT5(outbound, Ext)
    {}

    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                Ext1,       MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long,
                                                                Ext2,       MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         long);

    MOCKPP_VOID_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, visitable4, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, visitable3, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, visitable2, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, visitable1, const MOCKPP_STL::string &,
                                                                Ext,        MOCKPP_STL::string);

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, visitable0,
                                                                Ext);


    MOCKPP_VOID_CONST_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, const_visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                      Ext1,             MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_CONST_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, const_visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long,
                                                                      Ext2,             MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         long);

    MOCKPP_VOID_CONST_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, const_visitable4, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                      Ext,              MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_CONST_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, const_visitable3, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                      Ext,              MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_CONST_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, const_visitable2, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                                      Ext,              MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VOID_CONST_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, const_visitable1, const MOCKPP_STL::string &,
                                                                      Ext,              MOCKPP_STL::string);

    MOCKPP_VOID_CONST_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, const_visitable0,
                                                                      Ext);


    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, abc_visitable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                                Ext1,           CA,        CB,        CC,        CD,        CE);

    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, abc_visitable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                                Ext2,           CA,        CB,        CC,        CD,        CF);

    MOCKPP_VOID_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, abc_visitable4, const CA&, const CB&, const CC&, const CD&,
                                                                Ext,            CA,        CB,        CC,        CD);

    MOCKPP_VOID_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, abc_visitable3, const CA&, const CB&, const CC&,
                                                                Ext,            CA,        CB,        CC);

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, abc_visitable2, const CA&, const CB&,
                                                                Ext,            CA,        CB);

    MOCKPP_VOID_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, abc_visitable1, const CA&,
                                                                Ext,        CA);

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, abc_visitable0,
                                                                Ext);


    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, cba_visitable5, const CE&, const CD&, const CC&, const CB&, CA,
                                                                Ext1,           CE,        CD,        CC,        CB,        CA);

    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, cba_visitable5, const CF&, const CD&, const CC&, const CB&, CA,
                                                                Ext2,           CF,        CD,        CC,        CB,        CA);

    MOCKPP_VOID_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, cba_visitable4, const CE&, const CD&, const CC&, const CB&,
                                                                Ext,            CE,        CD,        CC,        CB);

    MOCKPP_VOID_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, cba_visitable3, const CE&, const CD&, const CC&,
                                                                Ext,            CE,        CD,        CC);

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, cba_visitable2, const CE&, const CD&,
                                                                Ext,            CE,        CD);

    MOCKPP_VOID_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, cba_visitable1, const CE&,
                                                                Ext,        CE);

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, cba_visitable0,
                                                                Ext);


    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, abc_visitable5_p, const CA*, const CB*, const CC*, const CD*, const CE*,
                                                                Ext,              CA*,       CB*,       CC*,       CD*,             CE*);

    MOCKPP_VOID_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, abc_visitable4_p, const CA*, const CB*, const CC*, const CD*,
                                                                Ext,              CA*,       CB*,       CC*,       CD*);

    MOCKPP_VOID_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, abc_visitable3_p, const CA*, const CB*, const CC*,
                                                                Ext,              CA*,       CB*,       CC*);

    MOCKPP_VOID_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, abc_visitable2_p, const CA*, const CB*,
                                                                Ext,              CA*,       CB*);

    MOCKPP_VOID_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, abc_visitable1_p, const CA*,
                                                                Ext,              CA*);

    MOCKPP_VOID_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, abc_visitable0_p,
                                                                Ext);


    MOCKPP_VOID_CONST_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, abc_const_visitable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                                      Ext1,                 CA,        CB,        CC,        CD,        CE);

    MOCKPP_VOID_CONST_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, abc_const_visitable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                                      Ext2,                 CA,        CB,        CC,        CD,        CF);

    MOCKPP_VOID_CONST_VISITABLE_EXT4(MyVisitableMockObject_5_void_ex, abc_const_visitable4, const CA&, const CB&, const CC&, const CD&,
                                                                      Ext,                  CA,        CB,        CC,        CD);

    MOCKPP_VOID_CONST_VISITABLE_EXT3(MyVisitableMockObject_5_void_ex, abc_const_visitable3, const CA&, const CB&, const CC&,
                                                                      Ext,                  CA,        CB,        CC);

    MOCKPP_VOID_CONST_VISITABLE_EXT2(MyVisitableMockObject_5_void_ex, abc_const_visitable2, const CA&, const CB&,
                                                                      Ext,                  CA,        CB);

    MOCKPP_VOID_CONST_VISITABLE_EXT1(MyVisitableMockObject_5_void_ex, abc_const_visitable1, const CA&,
                                                                      Ext,                  CA);

    MOCKPP_VOID_CONST_VISITABLE_EXT0(MyVisitableMockObject_5_void_ex, abc_const_visitable0,
                                                                      Ext);

    MOCKPP_VOID_VISITABLE_EXT5(MyVisitableMockObject_5_void_ex, outbound, unsigned &, unsigned &, unsigned &, unsigned &, unsigned &,
                                                                Ext,      unsigned,   unsigned,   unsigned,   unsigned,   unsigned );

    bool isClear()
    {
      return    visitable5Ext1DefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable5Ext1Throwables.size() == 0

             && visitable5Ext1ResponseValues.size() == 0
             && visitable5Ext1Parameter1.size() == 0
             && visitable5Ext1Parameter2.size() == 0
             && visitable5Ext1Parameter3.size() == 0
             && visitable5Ext1Parameter4.size() == 0
             && visitable5Ext1Parameter5.size() == 0

             && visitable5Ext2DefaultThrowable.get() == 0
             && visitable5Ext2Throwables.size() == 0

             && visitable5Ext2ResponseValues.size() == 0
             && visitable5Ext2Parameter1.size() == 0
             && visitable5Ext2Parameter2.size() == 0
             && visitable5Ext2Parameter3.size() == 0
             && visitable5Ext2Parameter4.size() == 0
             && visitable5Ext2Parameter5.size() == 0

             && const_visitable5Ext2DefaultThrowable.get() == 0
             && const_visitable5Ext2Throwables.size() == 0

             && const_visitable5Ext2ResponseValues.size() == 0
             && const_visitable5Ext2Parameter1.size() == 0
             && const_visitable5Ext2Parameter2.size() == 0
             && const_visitable5Ext2Parameter3.size() == 0
             && const_visitable5Ext2Parameter4.size() == 0
             && const_visitable5Ext2Parameter5.size() == 0

             && const_visitable5Ext1DefaultThrowable.get() == 0
             && const_visitable5Ext1Throwables.size() == 0

             && const_visitable5Ext1ResponseValues.size() == 0
             && const_visitable5Ext1Parameter1.size() == 0
             && const_visitable5Ext1Parameter2.size() == 0
             && const_visitable5Ext1Parameter3.size() == 0
             && const_visitable5Ext1Parameter4.size() == 0
             && const_visitable5Ext1Parameter5.size() == 0;
    }
};


void VisitableMockObject_5_void_test::test_controller()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 1, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(0)), 0, 2, 3, 4, 5);
}


void VisitableMockObject_5_void_test::test_thrower()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 1")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string 2"), 2);

  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(22, 2, 3, 4, 5);
  mvo.visitable5(222, 2, 3, 4, 5);
  mvo.visitable5(2222, 2, 3, 4, 5);
  mvo.visitable5(22222, 2, 3, 4, 5);
  mvo.visitable5(222222, 2, 3, 4, 5);

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable5(2, 2, 3, 4, 5), MOCKPP_PCSTRING("1"),
                         MOCKPP_STL::string, "exceptional string 1");

  MOCKPP_ASSERT_THROWING(mvo.visitable5(22, 2, 3, 4, 5), MOCKPP_PCSTRING("2"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable5(222, 2, 3, 4, 5), MOCKPP_PCSTRING("3"),
                         MOCKPP_STL::string, "exceptional string 2");
#endif
  mvo.visitable5(2222, 2, 3, 4, 5);
  mvo.visitable5(22222, 2, 3, 4, 5);
  mvo.visitable5(222222, 2, 3, 4, 5);
  mvo.verify();
}


void VisitableMockObject_5_void_test::fail_add_method_after_active()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.activate();
  mvo.visitable5(1, 2, 3, 4, 5);
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable5(1, 2, 3, 4, 5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::fail_add_throw_after_active()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::fail_add_throw_val_after_active()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::fail_set_throw_after_active()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string")));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::fail_parameter()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(11, 2, 3, 4, 5);
  mvo.visitable5(111, 2, 3, 4, 5);
  mvo.activate();
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(11, 2, 3, 4, 5);
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable5(222, 2, 3, 4, 5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::test_parameter_ex()
{
  MyVisitableMockObject_5_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable5, Ext1) ctr5 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable4, Ext) ctr4 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable3, Ext) ctr3 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable2, Ext) ctr2 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable1, Ext) ctr1 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, visitable0, Ext) ctr0 (&mvo);

  mvo.visitable5("1", "2", "3", "4", "5");
  mvo.visitable5("11", "2", "3", "4", "5");
  mvo.visitable5("111", "2", "3", "4", "5");
  mvo.visitable4("1", "2", "3", "4");
  mvo.visitable3("2", "3", "4");
  mvo.visitable2("3", "4");
  mvo.visitable1("4");
  mvo.visitable0();

  mvo.activate();

  mvo.visitable5("1", "2", "3", "4", "5");
  mvo.visitable5("11", "2", "3", "4", "5");
  mvo.visitable5("111", "2", "3", "4", "5");
  mvo.visitable4("1", "2", "3", "4");
  mvo.visitable3("2", "3", "4");
  mvo.visitable2("3", "4");
  mvo.visitable1("4");
  mvo.visitable0();

  mvo.verify();
}


void VisitableMockObject_5_void_test::test_parameter()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable4) ctr4 (&mvo);
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable3) ctr3 (&mvo);
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable2) ctr2 (&mvo);
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable1) ctr1 (&mvo);
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable0) ctr0 (&mvo);


  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(11, 2, 3, 4, 5);
  mvo.visitable5(111, 2, 3, 4, 5);
  mvo.visitable4(1,2,3,4);
  mvo.visitable3(2,3,4);
  mvo.visitable2(3,4);
  mvo.visitable1(4);
  mvo.visitable0();

  mvo.activate();

  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(11, 2, 3, 4, 5);
  mvo.visitable5(111, 2, 3, 4, 5);
  mvo.visitable4(1,2,3,4);
  mvo.visitable3(2,3,4);
  mvo.visitable2(3,4);
  mvo.visitable1(4);
  mvo.visitable0();

  mvo.verify();
}


void VisitableMockObject_5_void_test::test_default_throw()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 2")), 922, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 1")), 911, 2, 3, 4, 5);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw default")));

  mvo.visitable5(922, 2, 3, 4, 5);
  mvo.visitable5(911, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable5(922, 2, 3, 4, 5), MOCKPP_PCHAR("response 1"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 2"));

  MOCKPP_ASSERT_THROWING(mvo.visitable5(911, 2, 3, 4, 5), MOCKPP_PCHAR("response 2"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 1"));

  MOCKPP_ASSERT_THROWING(mvo.visitable5(1, 2, 3, 4, 5), MOCKPP_PCHAR("default response"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw default"));

  MOCKPP_ASSERT_THROWING(mvo.visitable5(2, 2, 3, 4, 5), MOCKPP_PCHAR("default response"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw default"));
#endif
  mvo.verify();
}


void VisitableMockObject_5_void_test::test_clear()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);

  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 0, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);
  mvo.activate();

  mvo.reset();
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);
}


void VisitableMockObject_5_void_test::fail_unused_throwable()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw default")));
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.verify();
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_void_test::test_constraint()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);

  mvo.visitable5(new MOCKPP_NS::IsEqual<unsigned>(12),
                 new MOCKPP_NS::IsEqual<unsigned>(34),
                 new MOCKPP_NS::IsEqual<unsigned>(56),
                 new MOCKPP_NS::IsEqual<unsigned>(78),
                 new MOCKPP_NS::IsEqual<unsigned>(90));

  mvo.activate();
  mvo.visitable5(12, 34, 56, 78, 90);
  mvo.verify();
}


void VisitableMockObject_5_void_test::test_controller_constraint()
{
  MyVisitableMockObject_5_void_test mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5_void_test, visitable5) ctr (&mvo);

  mvo.visitable5(MOCKPP_NS::eq<unsigned>(10, 5),
                 MOCKPP_NS::eq<unsigned>(20, 5),
                 MOCKPP_NS::eq<unsigned>(30, 5),
                 MOCKPP_NS::eq<unsigned>(40, 5),
                 MOCKPP_NS::eq<unsigned>(50, 5));

  mvo.visitable5(MOCKPP_NS::eq<unsigned>(10, 5),
                 MOCKPP_NS::eq<unsigned>(20, 5),
                 MOCKPP_NS::eq<unsigned>(30, 5),
                 MOCKPP_NS::eq<unsigned>(40, 5),
                 MOCKPP_NS::eq<unsigned>(50, 5));

  mvo.visitable5(1, 1, 1, 1, 1);

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable((int)2),
                           MOCKPP_NS::eq<unsigned>(8),
                           MOCKPP_NS::eq<unsigned>(18),
                           MOCKPP_NS::eq<unsigned>(28),
                           MOCKPP_NS::eq<unsigned>(38),
                           MOCKPP_NS::eq<unsigned>(48));

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable((int)1),
                           MOCKPP_NS::eq<unsigned>(12),
                           MOCKPP_NS::eq<unsigned>(22),
                           MOCKPP_NS::eq<unsigned>(32),
                           MOCKPP_NS::eq<unsigned>(42),
                           MOCKPP_NS::eq<unsigned>(52));
  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable5(8, 18, 28, 38, 48),
                         MOCKPP_PCHAR("mvo.visitable5(8, 18, 28, 38, 48)"),
                         int,
                         2);

  MOCKPP_ASSERT_THROWING(mvo.visitable5(12, 22, 32, 42, 52),
                         MOCKPP_PCHAR("mvo.visitable5(12, 22, 32, 42, 52)"),
                         int,
                         1);
#endif
  mvo.visitable5(1, 1, 1, 1, 1);
  mvo.verify();
}


void VisitableMockObject_5_void_test::test_controller_constraint_ext()
{
  MyVisitableMockObject_5_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, abc_visitable5, Ext1) abc1_ctr5 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, abc_visitable5, Ext2) abc2_ctr5 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, cba_visitable5, Ext1) cba_ctr5 (&mvo);

  mvo.abc_visitable5(MOCKPP_NS::eq<CA>(10),
                     MOCKPP_NS::eq<CB>(20),
                     MOCKPP_NS::eq<CC>(30),
                     MOCKPP_NS::eq<CD>(40),
                     MOCKPP_NS::eq<CE>(50));

  mvo.abc_visitable5(MOCKPP_NS::eq<CA>(10),
                     MOCKPP_NS::eq<CB>(20),
                     MOCKPP_NS::eq<CC>(30),
                     MOCKPP_NS::eq<CD>(40),
                     MOCKPP_NS::eq<CF>(50));

  mvo.cba_visitable5(MOCKPP_NS::eq<CE>(10),
                     MOCKPP_NS::eq<CD>(20),
                     MOCKPP_NS::eq<CC>(30),
                     MOCKPP_NS::eq<CB>(40),
                     MOCKPP_NS::eq<CA>(50));

  mvo.abc_visitable5(MOCKPP_NS::eq<CA>(11),
                     MOCKPP_NS::eq<CB>(21),
                     MOCKPP_NS::eq<CC>(31),
                     MOCKPP_NS::eq<CD>(41),
                     MOCKPP_NS::eq<CE>(51));

  mvo.abc_visitable5(MOCKPP_NS::eq<CA>(11),
                     MOCKPP_NS::eq<CB>(21),
                     MOCKPP_NS::eq<CC>(31),
                     MOCKPP_NS::eq<CD>(41),
                     MOCKPP_NS::eq<CF>(51));

  mvo.cba_visitable5(MOCKPP_NS::eq<CE>(11),
                     MOCKPP_NS::eq<CD>(21),
                     MOCKPP_NS::eq<CC>(31),
                     MOCKPP_NS::eq<CB>(41),
                     MOCKPP_NS::eq<CA>(51));

  abc1_ctr5.addResponseThrowable(MOCKPP_NS::make_throwable((int)2),
                                 MOCKPP_NS::eq<CA>(10),
                                 MOCKPP_NS::eq<CB>(20),
                                 MOCKPP_NS::eq<CC>(30),
                                 MOCKPP_NS::eq<CD>(40),
                                 MOCKPP_NS::eq<CE>(50));

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CE(50)),
                         MOCKPP_PCHAR("mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CE(50))"),
                         int,
                         2);
#endif

  mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CF(50));

  mvo.cba_visitable5(CE(10), CD(20), CC(30), CB(40), CA(50));

  mvo.abc_visitable5(CA(11), CB(21), CC(31), CD(41), CE(51));
  mvo.abc_visitable5(CA(11), CB(21), CC(31), CD(41), CF(51));
  mvo.cba_visitable5(CE(11), CD(21), CC(31), CB(41), CA(51));

  mvo.verify();
}


void VisitableMockObject_5_void_test::test_outbound()
{
  MyVisitableMockObject_5_void_ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5_void_ex, outbound, Ext) ctr1 (&mvo);

  mvo.outbound(MOCKPP_NS::outBound<unsigned>(1123),
               MOCKPP_NS::outBound<unsigned>(2123),
               MOCKPP_NS::outBound<unsigned>(3123),
               MOCKPP_NS::outBound<unsigned>(4123),
               MOCKPP_NS::outBound<unsigned>(5123));
  mvo.activate();

  unsigned val1 = 0;
  unsigned val2 = 0;
  unsigned val3 = 0;
  unsigned val4 = 0;
  unsigned val5 = 0;
  mvo.outbound(val1,val2, val3, val4,val5);
  MOCKPP_ASSERT_EQUALS((unsigned)1123, val1);
  MOCKPP_ASSERT_EQUALS((unsigned)2123, val2);
  MOCKPP_ASSERT_EQUALS((unsigned)3123, val3);
  MOCKPP_ASSERT_EQUALS((unsigned)4123, val4);
  MOCKPP_ASSERT_EQUALS((unsigned)5123, val5);
}




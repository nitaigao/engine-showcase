/***************************************************************************
                     VisitableMockObject_5_test.cpp
                                 -
  unit tests for VisitableMockObject class and 5 parameter methods
                             -------------------
    begin                : Mit Feb 19 2003
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: VisitableMockObject_5_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <mockpp/util/AssertMo.h>

#include <mockpp/chaining/ChainingMockObjectSupport.h>

#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsNot.h>

#include "classes_ABCDE.h"



class VisitableMockObject_5_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( VisitableMockObject_5_test );

  CPPUNIT_TEST(test_controller);
  CPPUNIT_TEST(test_thrower);
  CPPUNIT_TEST(test_parameter);
  CPPUNIT_TEST(test_parameter_ex);
  CPPUNIT_TEST(test_response);
  CPPUNIT_TEST(test_return);
  CPPUNIT_TEST(test_clear);
  CPPUNIT_TEST(test_default_throw);
  CPPUNIT_TEST(test_inline_pre_1_2);
  CPPUNIT_TEST(test_inline_post_1_2);
  CPPUNIT_TEST(test_constraint);
  CPPUNIT_TEST(test_controller_constraint);
  CPPUNIT_TEST(test_controller_constraint_ext);

  CPPUNIT_TEST(fail_add_throw_after_active);
  CPPUNIT_TEST(fail_add_throw_val_after_active);
  CPPUNIT_TEST(fail_set_throw_after_active);
  CPPUNIT_TEST(fail_add_method_after_active);
  CPPUNIT_TEST(fail_add_return_after_active);
  CPPUNIT_TEST(fail_set_return_after_active);
  CPPUNIT_TEST(fail_parameter);
  CPPUNIT_TEST(fail_return);
  CPPUNIT_TEST(fail_unused_throwable);
  CPPUNIT_TEST(fail_unused_value);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_controller();
  MOCKPP_TESTMETHOD_DECL void test_thrower();
  MOCKPP_TESTMETHOD_DECL void test_parameter();
  MOCKPP_TESTMETHOD_DECL void test_parameter_ex();
  MOCKPP_TESTMETHOD_DECL void test_response();
  MOCKPP_TESTMETHOD_DECL void test_return();
  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_default_throw();
  MOCKPP_TESTMETHOD_DECL void test_inline_pre_1_2();
  MOCKPP_TESTMETHOD_DECL void test_inline_post_1_2();
  MOCKPP_TESTMETHOD_DECL void test_constraint();
  MOCKPP_TESTMETHOD_DECL void test_controller_constraint();
  MOCKPP_TESTMETHOD_DECL void test_controller_constraint_ext();

  MOCKPP_TESTMETHOD_DECL void fail_unused_value();
  MOCKPP_TESTMETHOD_DECL void fail_unused_throwable();
  MOCKPP_TESTMETHOD_DECL void fail_parameter();
  MOCKPP_TESTMETHOD_DECL void fail_return();
  MOCKPP_TESTMETHOD_DECL void fail_add_method_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_val_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_throw_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_add_return_after_active();
  MOCKPP_TESTMETHOD_DECL void fail_set_return_after_active();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( VisitableMockObject_5_test );

MOCKPP_CXXTEST(VisitableMockObject_5_test, test_controller);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_thrower);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_parameter);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_parameter_ex);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_response);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_return);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_clear);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_default_throw);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_inline_pre_1_2);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_inline_post_1_2);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_constraint);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_controller_constraint);
MOCKPP_CXXTEST(VisitableMockObject_5_test, test_controller_constraint_ext);

MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_add_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_add_throw_val_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_set_throw_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_add_method_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_add_return_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_set_return_after_active);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_parameter);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_return);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_unused_throwable);
MOCKPP_CXXTEST(VisitableMockObject_5_test, fail_unused_value);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_controller);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_thrower);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_parameter_ex);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_response);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_return);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_clear);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_default_throw);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_inline_pre_1_2);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_inline_post_1_2);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_constraint);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_controller_constraint);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::test_controller_constraint_ext);

MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_add_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_add_throw_val_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_set_throw_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_add_method_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_add_return_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_set_return_after_active);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_parameter);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_return);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_unused_throwable);
MOCKPP_BOOST_TEST(VisitableMockObject_5_test::fail_unused_value);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (VisitableMockObject_5_test);

#endif // frameworks



class MyVisitableMockObject_5_Interface
{
  public:

     virtual ~MyVisitableMockObject_5_Interface()
     {}

     virtual int visitable5 (const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual int visitable4 (const unsigned&, const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual int visitable3 (const unsigned&, const unsigned&, const unsigned&) = 0;
     virtual int visitable2 (const unsigned&, const unsigned&) = 0;
     virtual int visitable1 (const unsigned&) = 0;
     virtual int visitable0 () = 0;

     virtual int const_visitable5 (const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual int const_visitable4 (const unsigned&, const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual int const_visitable3 (const unsigned&, const unsigned&, const unsigned&) const = 0;
     virtual int const_visitable2 (const unsigned&, const unsigned&) const = 0;
     virtual int const_visitable1 (const unsigned&) const = 0;
     virtual int const_visitable0 () const = 0;

     virtual int abc_visitable5 (const CA&, const CB&, const CC&, const CD&, const CE&) = 0;
     virtual int abc_visitable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int abc_visitable3 (const CA&, const CB&, const CC&) = 0;
     virtual int abc_visitable2 (const CA&, const CB&) = 0;
     virtual int abc_visitable1 (const CA&) = 0;
     virtual int abc_visitable0 () = 0;

//      virtual int abc_visitable5_p (const CA*, const CB*, const CC*, const CD*, const CE*) = 0;
//      virtual int abc_visitable4_p (const CA*, const CB*, const CC*, const CD*) = 0;
//      virtual int abc_visitable3_p (const CA*, const CB*, const CC*) = 0;
//      virtual int abc_visitable2_p (const CA*, const CB*) = 0;
//      virtual int abc_visitable1_p (const CA*) = 0;
//      virtual int abc_visitable0_p () = 0;

     virtual int abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, const CE&) const = 0;
     virtual int abc_const_visitable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual int abc_const_visitable3 (const CA&, const CB&, const CC&) const = 0;
     virtual int abc_const_visitable2 (const CA&, const CB&) const = 0;
     virtual int abc_const_visitable1 (const CA&) const = 0;
     virtual int abc_const_visitable0 () const = 0;
};


class MyVisitableMockObject_5 : public MOCKPP_NS::VisitableMockObject,
                                public MyVisitableMockObject_5_Interface
{
  public:

    MyVisitableMockObject_5(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(const_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(const_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(const_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(const_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(const_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(const_visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(abc_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(abc_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(abc_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(abc_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(abc_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(abc_visitable0),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE5(abc_const_visitable5),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE4(abc_const_visitable4),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE3(abc_const_visitable3),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE2(abc_const_visitable2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE1(abc_const_visitable1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE0(abc_const_visitable0)
    {}

    MOCKPP_VISITABLE5(MyVisitableMockObject_5, int, visitable5, unsigned, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VISITABLE4(MyVisitableMockObject_5, int, visitable4, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_VISITABLE3(MyVisitableMockObject_5, int, visitable3, unsigned, unsigned, unsigned);
    MOCKPP_VISITABLE2(MyVisitableMockObject_5, int, visitable2, unsigned, unsigned);
    MOCKPP_VISITABLE1(MyVisitableMockObject_5, int, visitable1, unsigned);
    MOCKPP_VISITABLE0(MyVisitableMockObject_5, int, visitable0);

    MOCKPP_CONST_VISITABLE5(MyVisitableMockObject_5, int, const_visitable5, unsigned, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_CONST_VISITABLE4(MyVisitableMockObject_5, int, const_visitable4, unsigned, unsigned, unsigned, unsigned);
    MOCKPP_CONST_VISITABLE3(MyVisitableMockObject_5, int, const_visitable3, unsigned, unsigned, unsigned);
    MOCKPP_CONST_VISITABLE2(MyVisitableMockObject_5, int, const_visitable2, unsigned, unsigned);
    MOCKPP_CONST_VISITABLE1(MyVisitableMockObject_5, int, const_visitable1, unsigned);
    MOCKPP_CONST_VISITABLE0(MyVisitableMockObject_5, int, const_visitable0);

    MOCKPP_VISITABLE5(MyVisitableMockObject_5, int, abc_visitable5, CA, CB, CC, CD, CE);
    MOCKPP_VISITABLE4(MyVisitableMockObject_5, int, abc_visitable4, CA, CB, CC, CD);
    MOCKPP_VISITABLE3(MyVisitableMockObject_5, int, abc_visitable3, CA, CB, CC);
    MOCKPP_VISITABLE2(MyVisitableMockObject_5, int, abc_visitable2, CA, CB);
    MOCKPP_VISITABLE1(MyVisitableMockObject_5, int, abc_visitable1, CA);
    MOCKPP_VISITABLE0(MyVisitableMockObject_5, int, abc_visitable0);

    MOCKPP_CONST_VISITABLE5(MyVisitableMockObject_5, int, abc_const_visitable5, CA, CB, CC, CD, CE);
    MOCKPP_CONST_VISITABLE4(MyVisitableMockObject_5, int, abc_const_visitable4, CA, CB, CC, CD);
    MOCKPP_CONST_VISITABLE3(MyVisitableMockObject_5, int, abc_const_visitable3, CA, CB, CC);
    MOCKPP_CONST_VISITABLE2(MyVisitableMockObject_5, int, abc_const_visitable2, CA, CB);
    MOCKPP_CONST_VISITABLE1(MyVisitableMockObject_5, int, abc_const_visitable1, CA);
    MOCKPP_CONST_VISITABLE0(MyVisitableMockObject_5, int, abc_const_visitable0);

    bool isClear()
    {
      return    visitable5DefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable5Throwables.size() == 0

             && visitable5ResponseValues.size() == 0
             && visitable5Parameter1.size() == 0
             && visitable5Parameter2.size() == 0
             && visitable5Parameter3.size() == 0
             && visitable5Parameter4.size() == 0
             && visitable5Parameter5.size() == 0

             && visitable5haveDefaultReturnValue == false
             && visitable5DefaultReturnValueUsed == false
             && visitable5ReturnValues.hasMoreObjects() == false;
        }
};


class MyVisitableMockObject_5_ex_Interface
{
  public:

     virtual ~MyVisitableMockObject_5_ex_Interface()
     {}

     virtual const MOCKPP_STL::string visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long) = 0;
     virtual const MOCKPP_STL::string visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual const MOCKPP_STL::string visitable4 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual const MOCKPP_STL::string visitable3 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual const MOCKPP_STL::string visitable2 (const MOCKPP_STL::string &, const MOCKPP_STL::string &) = 0;
     virtual const MOCKPP_STL::string visitable1 (const MOCKPP_STL::string &) = 0;
     virtual const MOCKPP_STL::string visitable0 () = 0;

     virtual const MOCKPP_STL::string const_visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long) const = 0;
     virtual const MOCKPP_STL::string const_visitable5 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual const MOCKPP_STL::string const_visitable4 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual const MOCKPP_STL::string const_visitable3 (const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual const MOCKPP_STL::string const_visitable2 (const MOCKPP_STL::string &, const MOCKPP_STL::string &) const = 0;
     virtual const MOCKPP_STL::string const_visitable1 (const MOCKPP_STL::string &) const = 0;
     virtual const MOCKPP_STL::string const_visitable0 () const = 0;

     virtual int abc_visitable5 (const CA&, const CB&, const CC&, const CD&, CE) = 0;
     virtual int abc_visitable5 (const CA&, const CB&, const CC&, const CD&, CF) = 0;
     virtual int abc_visitable4 (const CA&, const CB&, const CC&, const CD&) = 0;
     virtual int abc_visitable3 (const CA&, const CB&, const CC&) = 0;
     virtual int abc_visitable2 (const CA&, const CB&) = 0;
     virtual int abc_visitable1 (const CA&) = 0;
     virtual int abc_visitable0 () = 0;

     virtual int cba_visitable5 (const CE&, const CD&, const CC&, const CB&, CA) = 0;
     virtual int cba_visitable5 (const CF&, const CD&, const CC&, const CB&, CA) = 0;
     virtual int cba_visitable4 (const CE&, const CD&, const CC&, const CB&) = 0;
     virtual int cba_visitable3 (const CE&, const CD&, const CC&) = 0;
     virtual int cba_visitable2 (const CE&, const CD&) = 0;
     virtual int cba_visitable1 (const CE&) = 0;
     virtual int cba_visitable0 () = 0;

     virtual int abc_visitable5_p (const CA*, const CB*, const CC*, const CD*, const CE*) = 0;
     virtual int abc_visitable4_p (const CA*, const CB*, const CC*, const CD*) = 0;
     virtual int abc_visitable3_p (const CA*, const CB*, const CC*) = 0;
     virtual int abc_visitable2_p (const CA*, const CB*) = 0;
     virtual int abc_visitable1_p (const CA*) = 0;
     virtual int abc_visitable0_p () = 0;

     virtual int abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, CE) const = 0;
     virtual int abc_const_visitable5 (const CA&, const CB&, const CC&, const CD&, CF) const = 0;
     virtual int abc_const_visitable4 (const CA&, const CB&, const CC&, const CD&) const = 0;
     virtual int abc_const_visitable3 (const CA&, const CB&, const CC&) const = 0;
     virtual int abc_const_visitable2 (const CA&, const CB&) const = 0;
     virtual int abc_const_visitable1 (const CA&) const = 0;
     virtual int abc_const_visitable0 () const = 0;
};


class MyVisitableMockObject_5ex : public MOCKPP_NS::VisitableMockObject,
                                  public MyVisitableMockObject_5_ex_Interface
{
  public:

    MyVisitableMockObject_5ex(const MOCKPP_NS::String &name)
      : MOCKPP_NS::VisitableMockObject(name, 0),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(visitable5, over1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(visitable5, over2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(visitable4, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(visitable3, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(visitable2, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(visitable1, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(visitable0, Ext),

        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(const_visitable5, over1),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(const_visitable5, over2),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(const_visitable4, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(const_visitable3, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(const_visitable2, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(const_visitable1, Ext),
        MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(const_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(abc_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(abc_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(abc_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(abc_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(abc_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(abc_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(abc_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(cba_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(cba_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(cba_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(cba_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(cba_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(cba_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(cba_visitable0, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(abc_visitable5_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(abc_visitable4_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(abc_visitable3_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(abc_visitable2_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(abc_visitable1_p, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(abc_visitable0_p, Ext)

      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(abc_const_visitable5, Ext1)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT5(abc_const_visitable5, Ext2)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT4(abc_const_visitable4, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT3(abc_const_visitable3, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT2(abc_const_visitable2, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT1(abc_const_visitable1, Ext)
      , MOCKPP_CONSTRUCT_MEMBERS_FOR_VISITABLE_EXT0(abc_const_visitable0, Ext)
    {}

    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long,
                                                     MOCKPP_STL::string,       over1,      MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         long);

    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string,       over2,      MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT4(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable4, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string,       Ext,        MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT3(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable3, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string,       Ext,        MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT2(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable2, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string,       Ext,        MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT1(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable1, const MOCKPP_STL::string &,
                                                     MOCKPP_STL::string,       Ext,        MOCKPP_STL::string);

    MOCKPP_VISITABLE_EXT0(MyVisitableMockObject_5ex, const MOCKPP_STL::string, visitable0,
                                                     MOCKPP_STL::string,       Ext);



    MOCKPP_CONST_VISITABLE_EXT5(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, long,
                                                           MOCKPP_STL::string,       over1,            MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         long);

    MOCKPP_CONST_VISITABLE_EXT5(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable5, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                           MOCKPP_STL::string,       over2,            MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_CONST_VISITABLE_EXT4(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable4, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                           MOCKPP_STL::string,       Ext,              MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_CONST_VISITABLE_EXT3(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable3, const MOCKPP_STL::string &, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                           MOCKPP_STL::string,       Ext,              MOCKPP_STL::string,         MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_CONST_VISITABLE_EXT2(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable2, const MOCKPP_STL::string &, const MOCKPP_STL::string &,
                                                           MOCKPP_STL::string,       Ext,              MOCKPP_STL::string,         MOCKPP_STL::string);

    MOCKPP_CONST_VISITABLE_EXT1(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable1, const MOCKPP_STL::string &,
                                                           MOCKPP_STL::string,       Ext,              MOCKPP_STL::string);

    MOCKPP_CONST_VISITABLE_EXT0(MyVisitableMockObject_5ex, const MOCKPP_STL::string, const_visitable0,
                                                           MOCKPP_STL::string,       Ext);

    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, abc_visitable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                     int, Ext1,           CA,        CB,        CC,        CD,        CE);

    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, abc_visitable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                     int, Ext2,           CA,        CB,        CC,        CD,        CF);

    MOCKPP_VISITABLE_EXT4(MyVisitableMockObject_5ex, int, abc_visitable4, const CA&, const CB&, const CC&, const CD&,
                                                     int, Ext,            CA,        CB,        CC,        CD);

    MOCKPP_VISITABLE_EXT3(MyVisitableMockObject_5ex, int, abc_visitable3, const CA&, const CB&, const CC&,
                                                     int, Ext,            CA,        CB,        CC);

    MOCKPP_VISITABLE_EXT2(MyVisitableMockObject_5ex, int, abc_visitable2, const CA&, const CB&,
                                                     int, Ext,            CA,        CB);

    MOCKPP_VISITABLE_EXT1(MyVisitableMockObject_5ex, int, abc_visitable1, const CA&,
                                                     int, Ext,        CA);

    MOCKPP_VISITABLE_EXT0(MyVisitableMockObject_5ex, int, abc_visitable0,
                                                     int, Ext);


    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, cba_visitable5, const CE&, const CD&, const CC&, const CB&, CA,
                                                     int, Ext1,           CE,        CD,        CC,        CB,        CA);

    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, cba_visitable5, const CF&, const CD&, const CC&, const CB&, CA,
                                                     int, Ext2,           CF,        CD,        CC,        CB,        CA);

    MOCKPP_VISITABLE_EXT4(MyVisitableMockObject_5ex, int, cba_visitable4, const CE&, const CD&, const CC&, const CB&,
                                                     int, Ext,            CE,        CD,        CC,        CB);

    MOCKPP_VISITABLE_EXT3(MyVisitableMockObject_5ex, int, cba_visitable3, const CE&, const CD&, const CC&,
                                                     int, Ext,            CE,        CD,        CC);

    MOCKPP_VISITABLE_EXT2(MyVisitableMockObject_5ex, int, cba_visitable2, const CE&, const CD&,
                                                     int, Ext,            CE,        CD);

    MOCKPP_VISITABLE_EXT1(MyVisitableMockObject_5ex, int, cba_visitable1, const CE&,
                                                     int, Ext,        CE);

    MOCKPP_VISITABLE_EXT0(MyVisitableMockObject_5ex, int, cba_visitable0,
                                                     int, Ext);


    MOCKPP_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, abc_visitable5_p, const CA*, const CB*, const CC*, const CD*, const CE*,
                                                     int, Ext,              CA*,       CB*,       CC*,       CD*,       CE*);

    MOCKPP_VISITABLE_EXT4(MyVisitableMockObject_5ex, int, abc_visitable4_p, const CA*, const CB*, const CC*, const CD*,
                                                     int, Ext,              CA*,       CB*,       CC*,       CD*);

    MOCKPP_VISITABLE_EXT3(MyVisitableMockObject_5ex, int, abc_visitable3_p, const CA*, const CB*, const CC*,
                                                     int, Ext,              CA*,       CB*,       CC*);

    MOCKPP_VISITABLE_EXT2(MyVisitableMockObject_5ex, int, abc_visitable2_p, const CA*, const CB*,
                                                     int, Ext,              CA*,       CB*);

    MOCKPP_VISITABLE_EXT1(MyVisitableMockObject_5ex, int, abc_visitable1_p, const CA*,
                                                     int, Ext,              CA*);

    MOCKPP_VISITABLE_EXT0(MyVisitableMockObject_5ex, int, abc_visitable0_p,
                                                     int, Ext);


    MOCKPP_CONST_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, abc_const_visitable5, const CA&, const CB&, const CC&, const CD&, CE,
                                                           int, Ext1,                 CA,        CB,        CC,        CD,        CE);

    MOCKPP_CONST_VISITABLE_EXT5(MyVisitableMockObject_5ex, int, abc_const_visitable5, const CA&, const CB&, const CC&, const CD&, CF,
                                                           int, Ext2,                 CA,        CB,        CC,        CD,        CF);

    MOCKPP_CONST_VISITABLE_EXT4(MyVisitableMockObject_5ex, int, abc_const_visitable4, const CA&, const CB&, const CC&, const CD&,
                                                           int, Ext,                  CA,        CB,        CC,        CD);

    MOCKPP_CONST_VISITABLE_EXT3(MyVisitableMockObject_5ex, int, abc_const_visitable3, const CA&, const CB&, const CC&,
                                                           int, Ext,                  CA,        CB,        CC);

    MOCKPP_CONST_VISITABLE_EXT2(MyVisitableMockObject_5ex, int, abc_const_visitable2, const CA&, const CB&,
                                                           int, Ext,                  CA,        CB);

    MOCKPP_CONST_VISITABLE_EXT1(MyVisitableMockObject_5ex, int, abc_const_visitable1, const CA&,
                                                           int, Ext,                  CA);
    MOCKPP_CONST_VISITABLE_EXT0(MyVisitableMockObject_5ex, int, abc_const_visitable0,
                                                           int, Ext);
    bool isClear()
    {
      return    visitable5over2DefaultThrowable.get() == 0  // check preprocessed doc-template.h for the actual members
             && visitable5over2Throwables.size() == 0

             && visitable5over2ResponseValues.size() == 0
             && visitable5over2Parameter1.size() == 0
             && visitable5over2Parameter2.size() == 0
             && visitable5over2Parameter3.size() == 0
             && visitable5over2Parameter4.size() == 0
             && visitable5over2Parameter5.size() == 0

             && visitable5over2haveDefaultReturnValue == false
             && visitable5over2DefaultReturnValueUsed == false
             && visitable5over2ReturnValues.hasMoreObjects() == false

             && visitable5over1DefaultThrowable.get() == 0
             && visitable5over1Throwables.size() == 0

             && visitable5over1ResponseValues.size() == 0
             && visitable5over1Parameter1.size() == 0
             && visitable5over1Parameter2.size() == 0
             && visitable5over1Parameter3.size() == 0
             && visitable5over1Parameter4.size() == 0
             && visitable5over1Parameter5.size() == 0

             && visitable5over1haveDefaultReturnValue == false
             && visitable5over1DefaultReturnValueUsed == false
             && visitable5over1ReturnValues.hasMoreObjects() == false;
    }
};


void VisitableMockObject_5_test::test_controller()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
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


void VisitableMockObject_5_test::test_thrower()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 1")));
  ctr.addThrowable(MOCKPP_STL::string("exceptional string 2"), 2);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("exceptional string 3")));

  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable5(1, 2, 3, 4, 5), MOCKPP_PCSTRING("1"),
                         MOCKPP_STL::string, "exceptional string 1");

  MOCKPP_ASSERT_THROWING(mvo.visitable5(1, 2, 3, 4, 5), MOCKPP_PCSTRING("2"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable5(1, 2, 3, 4, 5), MOCKPP_PCSTRING("3"),
                         MOCKPP_STL::string, "exceptional string 2");

  MOCKPP_ASSERT_THROWING(mvo.visitable5(1, 2, 3, 4, 5), MOCKPP_PCSTRING("4"),
                         MOCKPP_STL::string, "exceptional string 3");
#endif
}


void VisitableMockObject_5_test::fail_add_method_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.activate();
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


void VisitableMockObject_5_test::fail_add_throw_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
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


void VisitableMockObject_5_test::fail_add_throw_val_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
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


void VisitableMockObject_5_test::fail_set_throw_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
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


void VisitableMockObject_5_test::fail_parameter()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.setDefaultReturnValue(1);
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


void VisitableMockObject_5_test::fail_add_return_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.addReturnValue(1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_test::fail_set_return_after_active()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  mvo.activate();
  bool thrown = false;
  MOCKPP_TRY
  {
    ctr.setDefaultReturnValue(1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_test::fail_return()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);

  ctr.addReturnValue(1);
  ctr.addReturnValue(11);

  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);

  mvo.activate();

  MOCKPP_ASSERT_TRUE(1 == mvo.visitable5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(11 == mvo.visitable5(2, 2, 3, 4, 5));
  bool thrown = false;
  MOCKPP_TRY
  {
    mvo.visitable5(3, 2, 3, 4, 5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void VisitableMockObject_5_test::test_return()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);

  ctr.setDefaultReturnValue(123);
  ctr.addReturnValue(1,2);
  ctr.addReturnValue(11, 2);

  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);
  mvo.visitable5(4, 2, 3, 4, 5);

  mvo.activate();

  MOCKPP_ASSERT_TRUE(1 == mvo.visitable5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(1 == mvo.visitable5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(11 == mvo.visitable5(2, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(11 == mvo.visitable5(2, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(123 == mvo.visitable5(3, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(123 == mvo.visitable5(4, 2, 3, 4, 5));
  mvo.verify();
}


void VisitableMockObject_5_test::test_parameter()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.setDefaultReturnValue(1);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable4) ctr4 (&mvo);
  ctr4.setDefaultReturnValue(4);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable3) ctr3 (&mvo);
  ctr3.setDefaultReturnValue(3);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable2) ctr2 (&mvo);
  ctr2.setDefaultReturnValue(2);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable1) ctr1 (&mvo);
  ctr1.setDefaultReturnValue(1);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable0) ctr0 (&mvo);
  ctr0.setDefaultReturnValue(0);

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


void VisitableMockObject_5_test::test_parameter_ex()
{
  MyVisitableMockObject_5ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable5, over2) ctr5 (&mvo);
  ctr5.setDefaultReturnValue("5");

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable4, Ext) ctr4 (&mvo);
  ctr4.setDefaultReturnValue("4");

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable3, Ext) ctr3 (&mvo);
  ctr3.setDefaultReturnValue("3");

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable2, Ext) ctr2 (&mvo);
  ctr2.setDefaultReturnValue("2");

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable1, Ext) ctr1 (&mvo);
  ctr1.setDefaultReturnValue("1");

  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, visitable0, Ext) ctr0 (&mvo);
  ctr0.setDefaultReturnValue("0");

  mvo.visitable5("1", "2", "3", "4", "5");
  mvo.visitable5("11", "2", "3", "4", "5");
  mvo.visitable5("111", "2", "3", "4", "5");
  mvo.visitable4("1", "2", "3", "4");
  mvo.visitable3("2", "3", "4");
  mvo.visitable2("3", "4");
  mvo.visitable1("4");
  mvo.visitable0();

  mvo.activate();

  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("5") == mvo.visitable5("1", "2", "3", "4", "5"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("5") == mvo.visitable5("11", "2", "3", "4", "5"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("5") == mvo.visitable5("111", "2", "3", "4", "5"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("4") == mvo.visitable4("1", "2", "3", "4"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("3") == mvo.visitable3("2", "3", "4"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("2") == mvo.visitable2("3", "4"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("1") == mvo.visitable1("4"));
  MOCKPP_ASSERT_TRUE(MOCKPP_STL::string("0") == mvo.visitable0());

  mvo.verify();
}


void VisitableMockObject_5_test::test_default_throw()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 2")), 922, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 1")), 911, 2, 3, 4, 5);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw default")));

  mvo.visitable5(922, 2, 3, 4, 5);
  mvo.visitable5(911, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);

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
}


void VisitableMockObject_5_test::test_response()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);

  ctr.setDefaultReturnValue(111);
  ctr.addResponseValue(123, 456, 2, 3, 4, 5);
  ctr.addResponseValue(321, 654, 2, 3, 4, 5);

  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 2")), 922, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw me 1")), 911, 2, 3, 4, 5);

  mvo.visitable5(456, 2, 3, 4, 5);
  mvo.visitable5(654, 2, 3, 4, 5);
  mvo.visitable5(1, 2, 3, 4, 5);
  mvo.visitable5(922, 2, 3, 4, 5);
  mvo.visitable5(911, 2, 3, 4, 5);
  mvo.visitable5(2, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);

  mvo.activate();

  MOCKPP_ASSERT_TRUE(123 == mvo.visitable5(456, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(321 == mvo.visitable5(654, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(111 == mvo.visitable5(1, 2, 3, 4, 5));

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.visitable5(922, 2, 3, 4, 5), MOCKPP_PCHAR("response 1"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 2"));

  MOCKPP_ASSERT_THROWING(mvo.visitable5(911, 2, 3, 4, 5), MOCKPP_PCHAR("response 2"),
                         MOCKPP_STL::string, MOCKPP_STL::string("throw me 1"));
#endif
  MOCKPP_ASSERT_TRUE(111 == mvo.visitable5(2, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(111 == mvo.visitable5(3, 2, 3, 4, 5));

  mvo.verify();
}



void VisitableMockObject_5_test::test_clear()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);

  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);

  ctr.addThrowable(MOCKPP_STL::string("exceptional string"), 3);
  ctr.setDefaultThrowable(MOCKPP_NS::make_throwable(int(123)));

  ctr.addReturnValue(1);
  ctr.setDefaultReturnValue(123);

  ctr.addResponseValue(1, 0, 2, 3, 4, 5);
  ctr.addResponseThrowable(MOCKPP_NS::make_throwable(int(1)), 0, 2, 3, 4, 5);
  mvo.visitable5(3, 2, 3, 4, 5);
  mvo.activate();

  mvo.reset();
  MOCKPP_ASSERT_TRUE(mvo.isClear() == true);
}


void VisitableMockObject_5_test::fail_unused_throwable()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
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


void VisitableMockObject_5_test::fail_unused_value()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.setDefaultReturnValue(111);
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


void VisitableMockObject_5_test::test_inline_pre_1_2()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.addReturnValue(111);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 1")));
  ctr.addReturnValue(222);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 2")));

  mvo.unsetThrowablesInline(); // old pre-1.2 mode
  mvo.activate();

  MOCKPP_TRY
  {
    mvo.visitable5(1, 2, 3, 4, 5);
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 1"));
  }
#endif

  MOCKPP_TRY
  {
    mvo.visitable5(1, 2, 3, 4, 5);
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 2"));
  }
#endif

  MOCKPP_ASSERT_TRUE(111 == mvo.visitable5(1, 2, 3, 4, 5));
  MOCKPP_ASSERT_TRUE(222 == mvo.visitable5(1, 2, 3, 4, 5));

  mvo.verify();
}


void VisitableMockObject_5_test::test_inline_post_1_2()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.addReturnValue(111);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 1")));
  ctr.addReturnValue(222);
  ctr.addThrowable(MOCKPP_NS::make_throwable(MOCKPP_STL::string("throw 2")));
  mvo.activate();

  MOCKPP_ASSERT_TRUE(111 == mvo.visitable5(1, 2, 3, 4, 5));

  MOCKPP_TRY
  {
    mvo.visitable5(1, 2, 3, 4, 5);
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 1"));
  }
#endif

  MOCKPP_ASSERT_TRUE(222 == mvo.visitable5(1, 2, 3, 4, 5));

  MOCKPP_TRY
  {
    mvo.visitable5(1, 2, 3, 4, 5);
    MOCKPP_FAIL(MOCKPP_PCHAR("should have thrown here"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_STL::string &ex)
  {
    MOCKPP_ASSERT_EQUALS(ex, MOCKPP_STL::string("throw 2"));
  }
#endif

  mvo.verify();
}


void VisitableMockObject_5_test::test_constraint()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.setDefaultReturnValue(123);

  mvo.visitable5(new MOCKPP_NS::IsEqual<unsigned>(12),
                 new MOCKPP_NS::IsEqual<unsigned>(34),
                 new MOCKPP_NS::IsEqual<unsigned>(56),
                 new MOCKPP_NS::IsEqual<unsigned>(78),
                 new MOCKPP_NS::IsEqual<unsigned>(90));

  mvo.activate();
  mvo.visitable5(12, 34, 56, 78, 90);
  mvo.verify();
}


void VisitableMockObject_5_test::test_controller_constraint()
{
  MyVisitableMockObject_5 mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR(MyVisitableMockObject_5, visitable5) ctr (&mvo);
  ctr.setDefaultReturnValue(123);

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

  ctr.addResponseValue(2, MOCKPP_NS::eq<unsigned>(9),
                          MOCKPP_NS::eq<unsigned>(19),
                          MOCKPP_NS::eq<unsigned>(29),
                          MOCKPP_NS::eq<unsigned>(39),
                          MOCKPP_NS::eq<unsigned>(49));

  ctr.addResponseValue(1, MOCKPP_NS::eq<unsigned>(11),
                          MOCKPP_NS::eq<unsigned>(21),
                          MOCKPP_NS::eq<unsigned>(31),
                          MOCKPP_NS::eq<unsigned>(41),
                          MOCKPP_NS::eq<unsigned>(51));

  mvo.activate();
  MOCKPP_ASSERT_TRUE(1 == mvo.visitable5(11, 21, 31, 41, 51));
  MOCKPP_ASSERT_TRUE(2 == mvo.visitable5(9, 19, 29, 39, 49));

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

  MOCKPP_ASSERT_TRUE(123 == mvo.visitable5(1, 1, 1, 1, 1));
  mvo.verify();
}


void VisitableMockObject_5_test::test_controller_constraint_ext()
{
  MyVisitableMockObject_5ex mvo(MOCKPP_PCHAR("mvo"));
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, abc_visitable5, Ext1) abc1_ctr5 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, abc_visitable5, Ext2) abc2_ctr5 (&mvo);
  MOCKPP_CONTROLLER_FOR_EXT(MyVisitableMockObject_5ex, cba_visitable5, Ext1) cba_ctr5 (&mvo);

  abc1_ctr5.setDefaultReturnValue(123);
  abc2_ctr5.setDefaultReturnValue(456);
  cba_ctr5.setDefaultReturnValue(321);

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

  abc2_ctr5.addResponseValue(4, MOCKPP_NS::eq<CA>(10),
                                MOCKPP_NS::eq<CB>(20),
                                MOCKPP_NS::eq<CC>(30),
                                MOCKPP_NS::eq<CD>(40),
                                MOCKPP_NS::eq<CF>(50));

  cba_ctr5.addResponseValue(2, MOCKPP_NS::eq<CE>(10),
                               MOCKPP_NS::eq<CD>(20),
                               MOCKPP_NS::eq<CC>(30),
                               MOCKPP_NS::eq<CB>(40),
                               MOCKPP_NS::eq<CA>(50));

  mvo.activate();

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CE(50)),
                         MOCKPP_PCHAR("mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CE(50))"),
                         int,
                         2);
#endif

  MOCKPP_ASSERT_TRUE(4 == mvo.abc_visitable5(CA(10), CB(20), CC(30), CD(40), CF(50)));

  MOCKPP_ASSERT_TRUE(2 == mvo.cba_visitable5(CE(10), CD(20), CC(30), CB(40), CA(50)));

  MOCKPP_ASSERT_TRUE(123 == mvo.abc_visitable5(CA(11), CB(21), CC(31), CD(41), CE(51)));
  MOCKPP_ASSERT_TRUE(456 == mvo.abc_visitable5(CA(11), CB(21), CC(31), CD(41), CF(51)));
  MOCKPP_ASSERT_TRUE(321 == mvo.cba_visitable5(CE(11), CD(21), CC(31), CB(41), CA(51)));

  mvo.verify();
}




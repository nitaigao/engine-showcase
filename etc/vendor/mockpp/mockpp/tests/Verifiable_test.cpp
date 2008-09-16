/***************************************************************************
             Verifiable_test.cpp  -  unit tests for Verifiable class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Verifiable_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <mockpp/VerifiableList.h>


class Verifiable_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Verifiable_test );

  CPPUNIT_TEST(test_parent);
  CPPUNIT_TEST(test_add);
  CPPUNIT_TEST(test_add_remove);
  CPPUNIT_TEST(test_clear);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_clear();
  MOCKPP_TESTMETHOD_DECL void test_add();
  MOCKPP_TESTMETHOD_DECL void test_add_remove();
  MOCKPP_TESTMETHOD_DECL void test_parent();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Verifiable_test );

MOCKPP_CXXTEST(Verifiable_test, test_parent);
MOCKPP_CXXTEST(Verifiable_test, test_add);
MOCKPP_CXXTEST(Verifiable_test, test_add_remove);
MOCKPP_CXXTEST(Verifiable_test, test_clear);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Verifiable_test::test_parent);
MOCKPP_BOOST_TEST(Verifiable_test::test_add);
MOCKPP_BOOST_TEST(Verifiable_test::test_add_remove);
MOCKPP_BOOST_TEST(Verifiable_test::test_clear);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Verifiable_test);

#endif // frameworks


template <class T>
class MyExpectationValue : public MOCKPP_NS::ExpectationValue<T>
{
  public:
    MyExpectationValue(const MOCKPP_NS::String &name, MOCKPP_NS::VerifiableList *parent)
      : MOCKPP_NS::ExpectationValue<T>(name, parent)
    {}

    // make public for test purposes
    MOCKPP_NS::Verifiable* getParent()
    {
#ifdef _MSC_VER
       return Verifiable::getParent();
#else
       return MOCKPP_NS::Verifiable::getParent();
#endif
    }
};


class MyVerifiableList : public MOCKPP_NS::VerifiableList
{
  public:

    MyVerifiableList()
      : MOCKPP_NS::VerifiableList(MOCKPP_PCHAR("verList"), 0)
    {}

    // make public for test purposes
    virtual void verify()
    {
    }

    // make public for test purposes
    virtual void reset()
    {
    }

    // make public for test purposes
    MOCKPP_NS::VerifiableList *getParent() const
    {
#ifdef _MSC_VER
      return Verifiable::getParent();
#else
      return MOCKPP_NS::Verifiable::getParent();
#endif
    }

    // make public for test purposes
    unsigned numVerifiables() const
    {
#ifdef _MSC_VER
      return VerifiableList::numVerifiables();
#else
      return MOCKPP_NS::VerifiableList::numVerifiables();
#endif
    }

    // make public for test purposes
    void clearVer()
    {
#ifdef _MSC_VER
      VerifiableList::clearVerifiables();
#else
      MOCKPP_NS::VerifiableList::clearVerifiables();
#endif
    }

    // make public for test purposes
    MOCKPP_NS::Verifiable *getVerifiable(unsigned idx)
    {
#ifdef _MSC_VER
      return VerifiableList::getVerifiable(idx);
#else
      return MOCKPP_NS::VerifiableList::getVerifiable(idx);
#endif
    }

    bool hasVerifiable(MOCKPP_NS::Verifiable *vf) const
    {
#ifdef _MSC_VER
      return VerifiableList::hasVerifiable(vf);
#else
      return MOCKPP_NS::VerifiableList::hasVerifiable(vf);
#endif
    }

    void addVerifiable(MOCKPP_NS::Verifiable *vf)
    {
#ifdef _MSC_VER
      VerifiableList::addVerifiable(vf);
#else
      MOCKPP_NS::VerifiableList::addVerifiable(vf);
#endif
    }

    void removeVerifiable(MOCKPP_NS::Verifiable *vf)
    {
#ifdef _MSC_VER
      VerifiableList::removeVerifiable(vf);
#else
      MOCKPP_NS::VerifiableList::removeVerifiable(vf);
#endif
    }
};


void Verifiable_test::test_parent()
{
  typedef MyExpectationValue<int> evi_t;

  MyVerifiableList root;
  evi_t lev1a(MOCKPP_PCHAR("lev1a"), &root);

  MOCKPP_ASSERT_TRUE(&root == lev1a.getParent());
  MOCKPP_ASSERT_TRUE(0 == root.getParent());
}


void Verifiable_test::test_add()
{
  typedef MyExpectationValue<int> evi_t;

  MyVerifiableList root;
  MOCKPP_ASSERT_TRUE(0 == root.numVerifiables());
  {
    evi_t lev1a(MOCKPP_PCHAR("lev1a"), &root);
    MOCKPP_ASSERT_TRUE(1 == root.numVerifiables());

    evi_t lev1b(MOCKPP_PCHAR("lev1b"), &root);
    MOCKPP_ASSERT_TRUE(2 == root.numVerifiables());

    MOCKPP_ASSERT_TRUE(&lev1a == root.getVerifiable(0));
    MOCKPP_ASSERT_TRUE(&lev1b == root.getVerifiable(1));
  }
  MOCKPP_ASSERT_TRUE(0 == root.numVerifiables());
}


void Verifiable_test::test_clear()
{
  typedef MyExpectationValue<int> evi_t;

  MyVerifiableList root;
  evi_t lev1a(MOCKPP_PCHAR("lev1a"), &root);
  MOCKPP_ASSERT_TRUE(1 == root.numVerifiables());

  evi_t lev1b(MOCKPP_PCHAR("lev1b"), &root);
  MOCKPP_ASSERT_TRUE(2 == root.numVerifiables());
  root.clearVer();
  MOCKPP_ASSERT_TRUE(0 == root.numVerifiables());
}


void Verifiable_test::test_add_remove()
{
  typedef MyExpectationValue<int> evi_t;

  MyVerifiableList root;
  evi_t lev1a(MOCKPP_PCHAR("lev1a"), 0);
  evi_t lev1b(MOCKPP_PCHAR("lev1a"), 0);
  MOCKPP_ASSERT_TRUE(0 == lev1b.numVerifiables());

  root.addVerifiable(&lev1a);
  MOCKPP_ASSERT_TRUE(true == root.hasVerifiable(&lev1a));
  MOCKPP_ASSERT_TRUE(false == root.hasVerifiable(&lev1b));
  root.addVerifiable(&lev1b);
  MOCKPP_ASSERT_TRUE(2 == root.numVerifiables());
  root.removeVerifiable(&lev1a);
  MOCKPP_ASSERT_TRUE(1 == root.numVerifiables());
  MOCKPP_ASSERT_TRUE(false == root.hasVerifiable(&lev1a));
  MOCKPP_ASSERT_TRUE(true == root.hasVerifiable(&lev1b));
}




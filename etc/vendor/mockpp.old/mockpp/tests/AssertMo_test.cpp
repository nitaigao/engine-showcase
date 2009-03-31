/***************************************************************************
             AssertMo_test.cpp  -  unit tests for AssertMo class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: AssertMo_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/util/AssertMo.h>
#include <mockpp/Verifiable.h>
#include <mockpp/util/NotImplementedException.h>


class AssertMo_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( AssertMo_test );

 CPPUNIT_TEST(test_A_includes);
 CPPUNIT_TEST(test_A_excludes);
 CPPUNIT_TEST(test_A_startsWith);

#ifdef MOCKPP_UNICODE
 CPPUNIT_TEST(test_W_includes);
 CPPUNIT_TEST(test_W_excludes);
 CPPUNIT_TEST(test_W_startsWith);
#endif

 CPPUNIT_TEST(test_verifyFails);
 CPPUNIT_TEST(test_macros);

 CPPUNIT_TEST(test_throwing);

 CPPUNIT_TEST(fail_A_includes);
 CPPUNIT_TEST(fail_A_excludes);
 CPPUNIT_TEST(fail_A_startsWith);

#ifdef MOCKPP_UNICODE
 CPPUNIT_TEST(fail_W_includes);
 CPPUNIT_TEST(fail_W_excludes);
 CPPUNIT_TEST(fail_W_startsWith);
#endif

 CPPUNIT_TEST(fail_notImpl);

CPPUNIT_TEST_SUITE_END();

#endif

 public:

  MOCKPP_TESTMETHOD_DECL void test_throwing();
  MOCKPP_TESTMETHOD_DECL void test_A_includes();
  MOCKPP_TESTMETHOD_DECL void test_A_excludes();
  MOCKPP_TESTMETHOD_DECL void test_A_startsWith();

#ifdef MOCKPP_UNICODE
  MOCKPP_TESTMETHOD_DECL void test_W_includes();
  MOCKPP_TESTMETHOD_DECL void test_W_excludes();
  MOCKPP_TESTMETHOD_DECL void test_W_startsWith();
#endif

  MOCKPP_TESTMETHOD_DECL void test_verifyFails();
  MOCKPP_TESTMETHOD_DECL void test_macros();

  MOCKPP_TESTMETHOD_DECL void fail_A_includes();
  MOCKPP_TESTMETHOD_DECL void fail_A_excludes();
  MOCKPP_TESTMETHOD_DECL void fail_A_startsWith();

#ifdef MOCKPP_UNICODE
  MOCKPP_TESTMETHOD_DECL void fail_W_includes();
  MOCKPP_TESTMETHOD_DECL void fail_W_excludes();
  MOCKPP_TESTMETHOD_DECL void fail_W_startsWith();
#endif

  MOCKPP_TESTMETHOD_DECL void fail_notImpl();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( AssertMo_test );

MOCKPP_CXXTEST(AssertMo_test, test_A_includes);
MOCKPP_CXXTEST(AssertMo_test, test_A_excludes);
MOCKPP_CXXTEST(AssertMo_test, test_A_startsWith);

#ifdef MOCKPP_UNICODE
MOCKPP_CXXTEST(AssertMo_test, test_W_includes);
MOCKPP_CXXTEST(AssertMo_test, test_W_excludes);
MOCKPP_CXXTEST(AssertMo_test, test_W_startsWith);
#endif

MOCKPP_CXXTEST(AssertMo_test, test_verifyFails);
MOCKPP_CXXTEST(AssertMo_test, test_macros);

MOCKPP_CXXTEST(AssertMo_test, test_throwing);

MOCKPP_CXXTEST(AssertMo_test, fail_A_includes);
MOCKPP_CXXTEST(AssertMo_test, fail_A_excludes);
MOCKPP_CXXTEST(AssertMo_test, fail_A_startsWith);

#ifdef MOCKPP_UNICODE
MOCKPP_CXXTEST(AssertMo_test, fail_W_includes);
MOCKPP_CXXTEST(AssertMo_test, fail_W_excludes);
MOCKPP_CXXTEST(AssertMo_test, fail_W_startsWith);
#endif

MOCKPP_CXXTEST(AssertMo_test, fail_notImpl);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(AssertMo_test::test_A_includes);
MOCKPP_BOOST_TEST(AssertMo_test::test_A_excludes);
MOCKPP_BOOST_TEST(AssertMo_test::test_A_startsWith);

#ifdef MOCKPP_UNICODE
MOCKPP_BOOST_TEST(AssertMo_test::test_W_includes);
MOCKPP_BOOST_TEST(AssertMo_test::test_W_excludes);
MOCKPP_BOOST_TEST(AssertMo_test::test_W_startsWith);
#endif

MOCKPP_BOOST_TEST(AssertMo_test::test_verifyFails);
MOCKPP_BOOST_TEST(AssertMo_test::test_macros);

MOCKPP_BOOST_TEST(AssertMo_test::test_throwing);

MOCKPP_BOOST_TEST(AssertMo_test::fail_A_includes);
MOCKPP_BOOST_TEST(AssertMo_test::fail_A_excludes);
MOCKPP_BOOST_TEST(AssertMo_test::fail_A_startsWith);

#ifdef MOCKPP_UNICODE
MOCKPP_BOOST_TEST(AssertMo_test::fail_W_includes);
MOCKPP_BOOST_TEST(AssertMo_test::fail_W_excludes);
MOCKPP_BOOST_TEST(AssertMo_test::fail_W_startsWith);
#endif

MOCKPP_BOOST_TEST(AssertMo_test::fail_notImpl);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (AssertMo_test);

#endif // frameworks



void AssertMo_test::test_macros()
{
  bool thrown;

  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_INCLUDES_MESSAGE(MOCKPP_PCHAR("includes abc xyz"), MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("xyz") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("includes abc xyz") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_INCLUDES(MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("xyz") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
// use \042 instead of \" because m$vc is too stupid to escape correctly
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("MOCKPP_PCHAR(\042abc\042) <in> MOCKPP_PCHAR(\042xyz\042)") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EXCLUDES_MESSAGE(MOCKPP_PCHAR("excludes abc abc"), MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("abc") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("excludes abc abc") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EXCLUDES(MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("abc") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
// use \042 instead of \" because m$vc is too stupid to escape correctly
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("MOCKPP_PCHAR(\042abc\042) <!in> MOCKPP_PCHAR(\042abc\042)") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_STARTSWITH_MESSAGE(MOCKPP_PCHAR("starts with abc xyz"), MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("xyz") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("starts with abc xyz") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_STARTSWITH(MOCKPP_PCHAR("abc"), MOCKPP_PCHAR("xyz") );
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
// use \042 instead of \" because m$vc is too stupid to escape correctly
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("MOCKPP_PCHAR(\042abc\042) <starts> MOCKPP_PCHAR(\042xyz\042)") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void AssertMo_test::test_A_includes()
{
  MOCKPP_ASSERT_INCLUDES(MOCKPP_STL::string("yLongS"), MOCKPP_STL::string("aVeryLongString"));

  MOCKPP_ASSERT_INCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString yLongS"),
                           MOCKPP_STL::string("yLongS"), MOCKPP_STL::string("aVeryLongString"));
}


#ifdef MOCKPP_UNICODE

void AssertMo_test::test_W_includes()
{
  MOCKPP_ASSERT_INCLUDES(MOCKPP_STL::basic_string<wchar_t>(L"yLongS"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));

  MOCKPP_ASSERT_INCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString yLongS"),
                           MOCKPP_STL::basic_string<wchar_t>(L"yLongS"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
}


void AssertMo_test::test_W_excludes()
{
  MOCKPP_ASSERT_EXCLUDES(MOCKPP_STL::basic_string<wchar_t>(L"abcdef"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));

  MOCKPP_ASSERT_EXCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                           MOCKPP_STL::basic_string<wchar_t>(L"abcdef"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
}


void AssertMo_test::test_W_startsWith()
{
  MOCKPP_ASSERT_STARTSWITH(MOCKPP_STL::basic_string<wchar_t>(L"aVeryLong"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));

  MOCKPP_ASSERT_STARTSWITH_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                             MOCKPP_STL::basic_string<wchar_t>(L"aVeryLong"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
}

#endif


void AssertMo_test::test_A_excludes()
{
  MOCKPP_ASSERT_EXCLUDES(MOCKPP_STL::string("abcdef"), MOCKPP_STL::string("aVeryLongString"));

  MOCKPP_ASSERT_EXCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                           MOCKPP_STL::string("abcdef"), MOCKPP_STL::string("aVeryLongString"));
}


void AssertMo_test::test_A_startsWith()
{
  MOCKPP_ASSERT_STARTSWITH(MOCKPP_STL::string("aVeryLong"), MOCKPP_STL::string("aVeryLongString"));

  MOCKPP_ASSERT_STARTSWITH_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                             MOCKPP_STL::string("aVeryLong"), MOCKPP_STL::string("aVeryLongString"));
}


void AssertMo_test::fail_A_includes()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_INCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString yLongS"),
                             MOCKPP_STL::string("abcdef"), MOCKPP_STL::string("aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


#ifdef MOCKPP_UNICODE

void AssertMo_test::fail_W_excludes()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EXCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                             MOCKPP_STL::basic_string<wchar_t>(L"aVeryLong"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void AssertMo_test::fail_W_includes()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_INCLUDES_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString yLongS"),
                             MOCKPP_STL::basic_string<wchar_t>(L"abcdef"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void AssertMo_test::fail_W_startsWith()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_STARTSWITH_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                               MOCKPP_STL::basic_string<wchar_t>(L"abcdef"), MOCKPP_STL::basic_string<wchar_t>(L"aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}

#endif


void AssertMo_test::fail_A_excludes()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EXCLUDES(MOCKPP_STL::string("aVeryLong"), MOCKPP_STL::string("aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void AssertMo_test::fail_A_startsWith()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_STARTSWITH_MESSAGE(MOCKPP_PCHAR("assertIncludes aVeryLongString abcdef"),
                               MOCKPP_STL::string("abcdef"), MOCKPP_STL::string("aVeryLongString"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void AssertMo_test::fail_notImpl()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_NOT_IMPLEMENTED(MOCKPP_PCHAR("not implemented"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


class MyVerifiable : public MOCKPP_NS::Verifiable
{
  public:
    MyVerifiable()
      : MOCKPP_NS::Verifiable(MOCKPP_PCHAR("myVer"), 0)
    {}

    virtual void reset()
    {
    }

    virtual void verify()
    {
      MOCKPP_NS::assertionFailed(__LINE__, __FILE__,
                                          MOCKPP_PCHAR("verify with AssertionFailedError"));
    }
};


void AssertMo_test::test_verifyFails()
{
  MyVerifiable mv;
  MOCKPP_ASSERT_VERIFYFAILS(&mv);
}


#ifndef MOCKPP_NO_EXCEPTIONS
static void do_a_throw(unsigned val)
{
  MOCKPP_THROW((unsigned) val);
}
#endif

void AssertMo_test::test_throwing()
{
  unsigned ret = 0;
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING(do_a_throw(123),
                         MOCKPP_PCHAR("throw 123"),
                         unsigned,
                         123);
#endif

  ret = 0;
#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_COND(do_a_throw(321),
                              MOCKPP_PCHAR("throw 321"),
                              unsigned,
                              (ret = exception_object) == 321);
  MOCKPP_ASSERT_TRUE(ret == 321);
#endif

#ifndef MOCKPP_NO_EXCEPTIONS
  MOCKPP_ASSERT_THROWING_EQUALS(do_a_throw(321),
                                MOCKPP_PCHAR("throw 321"),
                                unsigned,
                                exception_object, 321u);
#endif

}



/***************************************************************************
             Assert_test.cpp  -  unit tests for Assert class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Assert_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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

#include <mockpp/compat/Asserter.h>


class Assert_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Assert_test );

 CPPUNIT_TEST(test_FalseTrue);
 CPPUNIT_TEST(test_boundary);

 CPPUNIT_TEST(test_equalsDouble);
 CPPUNIT_TEST(test_equalsFloat);
 CPPUNIT_TEST(test_equalsLong);
 CPPUNIT_TEST(test_equalsBool);
 CPPUNIT_TEST(test_equalsChar);
 CPPUNIT_TEST(test_equalsInt);
 CPPUNIT_TEST(test_equalsShort);
 CPPUNIT_TEST(test_equalsAString);

#ifdef MOCKPP_UNICODE
 CPPUNIT_TEST(test_equalsWString);
 CPPUNIT_TEST(test_equalsPWChar);
#endif

 CPPUNIT_TEST(test_equalsPChar);
 CPPUNIT_TEST(test_macros);

 CPPUNIT_TEST( fail_fail);
 CPPUNIT_TEST( fail_failmsg);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void fail_fail();
  MOCKPP_TESTMETHOD_DECL void fail_failmsg();

  MOCKPP_TESTMETHOD_DECL void test_FalseTrue();
  MOCKPP_TESTMETHOD_DECL void test_boundary();

  MOCKPP_TESTMETHOD_DECL void test_equalsPChar();
  MOCKPP_TESTMETHOD_DECL void test_equalsAString();
#ifdef MOCKPP_UNICODE
  MOCKPP_TESTMETHOD_DECL void test_equalsPWChar();
  MOCKPP_TESTMETHOD_DECL void test_equalsWString();
#endif

  MOCKPP_TESTMETHOD_DECL void test_equalsDouble();
  MOCKPP_TESTMETHOD_DECL void test_equalsFloat();
  MOCKPP_TESTMETHOD_DECL void test_equalsLong();
  MOCKPP_TESTMETHOD_DECL void test_equalsBool();
  MOCKPP_TESTMETHOD_DECL void test_equalsChar();
  MOCKPP_TESTMETHOD_DECL void test_equalsInt();
  MOCKPP_TESTMETHOD_DECL void test_equalsShort();
  MOCKPP_TESTMETHOD_DECL void test_macros();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Assert_test );

MOCKPP_CXXTEST(Assert_test, test_FalseTrue);
MOCKPP_CXXTEST(Assert_test, test_boundary);

MOCKPP_CXXTEST(Assert_test, test_equalsDouble);
MOCKPP_CXXTEST(Assert_test, test_equalsFloat);
MOCKPP_CXXTEST(Assert_test, test_equalsLong);
MOCKPP_CXXTEST(Assert_test, test_equalsBool);
MOCKPP_CXXTEST(Assert_test, test_equalsChar);
MOCKPP_CXXTEST(Assert_test, test_equalsInt);
MOCKPP_CXXTEST(Assert_test, test_equalsShort);
MOCKPP_CXXTEST(Assert_test, test_equalsAString);

#ifdef MOCKPP_UNICODE
MOCKPP_CXXTEST(Assert_test, test_equalsWString);
MOCKPP_CXXTEST(Assert_test, test_equalsPWChar);
#endif

MOCKPP_CXXTEST(Assert_test, test_equalsPChar);
MOCKPP_CXXTEST(Assert_test, test_macros);

MOCKPP_CXXTEST(Assert_test,  fail_fail);
MOCKPP_CXXTEST(Assert_test,  fail_failmsg);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Assert_test::test_FalseTrue);
MOCKPP_BOOST_TEST(Assert_test::test_boundary);

MOCKPP_BOOST_TEST(Assert_test::test_equalsDouble);
MOCKPP_BOOST_TEST(Assert_test::test_equalsFloat);
MOCKPP_BOOST_TEST(Assert_test::test_equalsLong);
MOCKPP_BOOST_TEST(Assert_test::test_equalsBool);
MOCKPP_BOOST_TEST(Assert_test::test_equalsChar);
MOCKPP_BOOST_TEST(Assert_test::test_equalsInt);
MOCKPP_BOOST_TEST(Assert_test::test_equalsShort);
MOCKPP_BOOST_TEST(Assert_test::test_equalsAString);

#ifdef MOCKPP_UNICODE
MOCKPP_BOOST_TEST(Assert_test::test_equalsWString);
MOCKPP_BOOST_TEST(Assert_test::test_equalsPWChar);
#endif

MOCKPP_BOOST_TEST(Assert_test::test_equalsPChar);
MOCKPP_BOOST_TEST(Assert_test::test_macros);

MOCKPP_BOOST_TEST(Assert_test:: fail_fail);
MOCKPP_BOOST_TEST(Assert_test:: fail_failmsg);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Assert_test);

#endif // frameworks



void Assert_test::test_macros()
{
  bool thrown;

  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("equals 1==2"), 1, 2);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("equals 1==2") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS(123, 321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("invalid: 123 == 321") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("range 1 +- 2 == 4"), 1, 4, 2);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("range 1 +- 2 == 4") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA(123, 321, 3);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("invalid: 123 +- 3 == 321") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_BOUNDARY(123, 321, 789);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("invalid: 123 < 789 < 321") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_BOUNDARY_MESSAGE(MOCKPP_PCHAR("123 321 789"), 123, 321, 789);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("123 321 789") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("condition true == false"), true == false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("condition true == false") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_TRUE( true == false ); // some spaces AROUND the expression
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("!true: true == false") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_FALSE_MESSAGE(MOCKPP_PCHAR("condition true != false"), true != false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("condition true != false") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_FALSE( true != false ); // some spaces AROUND the expression
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch (MOCKPP_NS::AssertionFailedError &ae)
  {
    thrown = true;
    MOCKPP_ASSERT_TRUE(MOCKPP_PCHAR("!false: true != false") == ae.getMessage());
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsAString()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"123\""),
                               MOCKPP_STL::string("123"), MOCKPP_STL::string("123"));

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_STL::string("123"), MOCKPP_STL::string("123"));


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"321\""),
                                 MOCKPP_STL::string("123"), MOCKPP_STL::string("321"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_STL::string("123"), MOCKPP_STL::string("321"));
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

void Assert_test::test_equalsWString()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"123\""),
                               MOCKPP_STL::basic_string<wchar_t>(L"123"), MOCKPP_STL::basic_string<wchar_t>(L"123"));

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_STL::basic_string<wchar_t>(L"123"), MOCKPP_STL::basic_string<wchar_t>(L"123"));


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"321\""),
                                 MOCKPP_STL::basic_string<wchar_t>(L"123"), MOCKPP_STL::basic_string<wchar_t>(L"321"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_STL::basic_string<wchar_t>(L"123"), MOCKPP_STL::basic_string<wchar_t>(L"321"));
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

void Assert_test::test_equalsPChar()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"123\""),
                               "123", "123");

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), "123", "123");


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"321\""),
                                 "123", "321");
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), "123", "321");
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

void Assert_test::test_equalsPWChar()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"123\""),
                               L"123", L"123");

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), L"123", L"123");


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals \"123\" == \"321\""),
                                 L"123", L"321");
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), L"123", L"321");
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

void Assert_test::test_equalsLong()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 12345678 == 12345678"),
                               (long) 12345678, (long) 12345678);

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (long) -12345678, (long) -12345678);

  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 12345678 == 876543321"),
                                 (long) 12345678, (long) 87654321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);

  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (long) -12345678, (long) -87654321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsShort()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 1234 == 1234"),
                               (short) 1234, (short) 1234);

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (short) -1234, (short) -1234);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (short) -1234, (short) -4321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 1234 != 4321"),
                                 (short) -1234, (short) -4321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsInt()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 1234 == 1234"),
                               (int) 1234, (int) 1234);

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (int) -1234, (int) -1234);

  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 1234 == 4321"),
                                 (int) 1234, (int) 4321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (int) 1234, (int) 4321);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsChar()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 'x' == 'x'"),
                               MOCKPP_CHAR('x'), MOCKPP_CHAR('x'));

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_CHAR('x'), MOCKPP_CHAR('x'));


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals 'x' == 'u'"),
                                 MOCKPP_CHAR('x'), MOCKPP_CHAR('u'));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), MOCKPP_CHAR('x'), MOCKPP_CHAR('u'));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsBool()
{
  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals true == true"),
                               (bool)true, (bool)true);

  MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (bool)true, (bool)true);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR("assertEquals true == false"),
                                 (bool)true, (bool)false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_EQUALS_MESSAGE(MOCKPP_PCHAR(""), (bool)true, (bool)false);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsDouble()
{
  // should fail when instantiated with template and long/int!
  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.3 +- 0.4 == 12.6"),
                               (double)12.3, (double)12.6, (double)0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.6 +- 0.4 == 12.3"),
                               (double)12.6, (double)12.3, (double)0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals -12.6 +- -0.4 == -12.3"),
                               (double)-12.6, (double)-12.3, (double)-0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals -12.6 +- 0.4 == -12.3"),
                               (double)-12.6, (double)-12.3, (double)0.4);

  MOCKPP_ASSERT_DELTA((double)12.5, (double)12.7, (double)0.6);

  MOCKPP_ASSERT_DELTA((double)12.5, (double)12.1, (double)0.6);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.5 +- 0.5 == 13.1"),
                                 (double)12.5, (double)13.1, (double)0.5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA((double)12.5, (double)13.1, (double)-0.5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_equalsFloat()
{
  // should fail when instantiated with template and long/int!
  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.3 +- 0.4 == 12.6"),
                               (float)12.3, (float)12.6, (float)0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.6 +- 0.4 == 12.3"),
                               (float)12.6, (float)12.3, (float)0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals -12.6 +- -0.4 == -12.3"),
                               (float)-12.6, (float)-12.3, (float)-0.4);

  MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals -12.6 +- 0.4 == -12.3"),
                               (float)-12.6, (float)-12.3, (float)0.4);

  MOCKPP_ASSERT_DELTA((float)12.5, (float)12.7, (float)0.5);

  MOCKPP_ASSERT_DELTA((float)12.5, (float)12.1, (float)0.5);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA_MESSAGE(MOCKPP_PCHAR("assertEquals 12.5 +- 0.5 == 11.9"),
                                 (float)12.5, (float)11.9, (float)0.5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_DELTA((float)12.5, (float)11.9, (float)0.5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_FalseTrue()
{
  MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("assertTrue 1 == 1"), 1 == 1);

  MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR(""), 1 == 1);

  MOCKPP_ASSERT_FALSE_MESSAGE(MOCKPP_PCHAR("assertFalse 1 != 1"), 1 != 1);

  MOCKPP_ASSERT_FALSE_MESSAGE(MOCKPP_PCHAR(""), 1 != 1);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR("assertTrue 1 != 1"), 1 != 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_TRUE_MESSAGE(MOCKPP_PCHAR(""), 1 != 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_FALSE_MESSAGE(MOCKPP_PCHAR("assertTrue 1 == 1"), 1 == 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_FALSE_MESSAGE(MOCKPP_PCHAR(""), 1 == 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::fail_fail()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_FAIL(MOCKPP_PCHAR(""));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::fail_failmsg()
{
  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_FAIL(MOCKPP_PCHAR("fail with msg"));
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Assert_test::test_boundary()
{
  MOCKPP_ASSERT_BOUNDARY_MESSAGE(MOCKPP_PCHAR("assertBoundary 2 4 3"), 2, 4, 3);

  MOCKPP_ASSERT_BOUNDARY(2, 4, 2);
  MOCKPP_ASSERT_BOUNDARY(2, 4, 3);
  MOCKPP_ASSERT_BOUNDARY(2, 4, 4);


  bool thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_BOUNDARY(2, 4, 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);


  thrown = false;
  MOCKPP_TRY
  {
    MOCKPP_ASSERT_BOUNDARY(2, 4, 5);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}





/***************************************************************************
             Formatter_test.cpp  -  unit tests for Formatter class
                             -------------------
    begin                : Fri Dez 27 2002
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

  $Id: Formatter_test.cpp 1438 2008-03-08 20:22:43Z ewald-arnold $

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
#include <cstdio>
//#include <iostream>

#include "SelectFramework.h"

#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/AssertionFailedError.h>


class Formatter_test MOCKPP_DERIVE_PUBLIC_UNITFRAMEWORK
{
 public:

#if defined (MOCKPP_USE_CXXTEST)

#elif defined(MOCKPP_USE_BOOSTTEST)

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE( Formatter_test );

 CPPUNIT_TEST(test_fmt_int);
 CPPUNIT_TEST(test_fmt_bool);
 CPPUNIT_TEST(test_fmt_uint);
 CPPUNIT_TEST(test_fmt_long);
 CPPUNIT_TEST(test_fmt_ulong);
 CPPUNIT_TEST(test_fmt_String);
 CPPUNIT_TEST(test_fmt_Char);
 CPPUNIT_TEST(test_fmt_float);
 CPPUNIT_TEST(test_fmt_double);

 CPPUNIT_TEST(test_ostreamed);
 CPPUNIT_TEST(test_number_long_range);
 CPPUNIT_TEST(test_number_ulong_range);
 CPPUNIT_TEST(test_number_ulong_oct);
 CPPUNIT_TEST(test_number_ulong_hex);
 CPPUNIT_TEST(test_number_long_hex);
 CPPUNIT_TEST(test_number_min_long_hex);
 CPPUNIT_TEST(test_number_min_long_dec);
 CPPUNIT_TEST(test_number_max_ulong_dec);
 CPPUNIT_TEST(test_number_max_long_dec);
 CPPUNIT_TEST(test_number_double);

 CPPUNIT_TEST(fail_base_long_37);
 CPPUNIT_TEST(fail_base_ulong_37);
 CPPUNIT_TEST(fail_base_long_1);
 CPPUNIT_TEST(fail_base_ulong_1);
 CPPUNIT_TEST(fail_format);

CPPUNIT_TEST_SUITE_END();

#endif

 public:
  MOCKPP_TESTMETHOD_DECL void fail_base_long_37();
  MOCKPP_TESTMETHOD_DECL void fail_base_ulong_37();
  MOCKPP_TESTMETHOD_DECL void fail_base_long_1();
  MOCKPP_TESTMETHOD_DECL void fail_base_ulong_1();
  MOCKPP_TESTMETHOD_DECL void fail_format();

  MOCKPP_TESTMETHOD_DECL void test_number_double();
  MOCKPP_TESTMETHOD_DECL void test_number_long_hex();
  MOCKPP_TESTMETHOD_DECL void test_number_ulong_hex();
  MOCKPP_TESTMETHOD_DECL void test_number_max_long_dec();
  MOCKPP_TESTMETHOD_DECL void test_number_max_ulong_dec();
  MOCKPP_TESTMETHOD_DECL void test_number_min_long_hex();
  MOCKPP_TESTMETHOD_DECL void test_number_min_long_dec();
  MOCKPP_TESTMETHOD_DECL void test_number_ulong_oct();
  MOCKPP_TESTMETHOD_DECL void test_number_long_range();
  MOCKPP_TESTMETHOD_DECL void test_number_ulong_range();

  MOCKPP_TESTMETHOD_DECL void test_ostreamed();
  MOCKPP_TESTMETHOD_DECL void test_fmt_int();
  MOCKPP_TESTMETHOD_DECL void test_fmt_bool();
  MOCKPP_TESTMETHOD_DECL void test_fmt_uint();
  MOCKPP_TESTMETHOD_DECL void test_fmt_long();
  MOCKPP_TESTMETHOD_DECL void test_fmt_ulong();
  MOCKPP_TESTMETHOD_DECL void test_fmt_String();
  MOCKPP_TESTMETHOD_DECL void test_fmt_Char();
  MOCKPP_TESTMETHOD_DECL void test_fmt_float();
  MOCKPP_TESTMETHOD_DECL void test_fmt_double();
};


#if defined (MOCKPP_USE_CXXTEST)

MOCKPP_CXXTEST_SUITE_REGISTRATION( Formatter_test );

MOCKPP_CXXTEST(Formatter_test, test_fmt_int);
MOCKPP_CXXTEST(Formatter_test, test_fmt_bool);
MOCKPP_CXXTEST(Formatter_test, test_fmt_uint);
MOCKPP_CXXTEST(Formatter_test, test_fmt_long);
MOCKPP_CXXTEST(Formatter_test, test_fmt_ulong);
MOCKPP_CXXTEST(Formatter_test, test_fmt_String);
MOCKPP_CXXTEST(Formatter_test, test_fmt_Char);
MOCKPP_CXXTEST(Formatter_test, test_fmt_float);
MOCKPP_CXXTEST(Formatter_test, test_fmt_double);

MOCKPP_CXXTEST(Formatter_test, test_ostreamed);
MOCKPP_CXXTEST(Formatter_test, test_number_long_range);
MOCKPP_CXXTEST(Formatter_test, test_number_ulong_range);
MOCKPP_CXXTEST(Formatter_test, test_number_ulong_oct);
MOCKPP_CXXTEST(Formatter_test, test_number_ulong_hex);
MOCKPP_CXXTEST(Formatter_test, test_number_long_hex);
MOCKPP_CXXTEST(Formatter_test, test_number_min_long_hex);
MOCKPP_CXXTEST(Formatter_test, test_number_min_long_dec);
MOCKPP_CXXTEST(Formatter_test, test_number_max_ulong_dec);
MOCKPP_CXXTEST(Formatter_test, test_number_max_long_dec);
MOCKPP_CXXTEST(Formatter_test, test_number_double);

MOCKPP_CXXTEST(Formatter_test, fail_base_long_37);
MOCKPP_CXXTEST(Formatter_test, fail_base_ulong_37);
MOCKPP_CXXTEST(Formatter_test, fail_base_long_1);
MOCKPP_CXXTEST(Formatter_test, fail_base_ulong_1);
MOCKPP_CXXTEST(Formatter_test, fail_format);

#elif defined(MOCKPP_USE_BOOSTTEST)

MOCKPP_BOOST_TEST(Formatter_test::test_fmt_int);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_bool);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_uint);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_long);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_ulong);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_String);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_Char);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_float);
MOCKPP_BOOST_TEST(Formatter_test::test_fmt_double);

MOCKPP_BOOST_TEST(Formatter_test::test_ostreamed);
MOCKPP_BOOST_TEST(Formatter_test::test_number_long_range);
MOCKPP_BOOST_TEST(Formatter_test::test_number_ulong_range);
MOCKPP_BOOST_TEST(Formatter_test::test_number_ulong_oct);
MOCKPP_BOOST_TEST(Formatter_test::test_number_ulong_hex);
MOCKPP_BOOST_TEST(Formatter_test::test_number_long_hex);
MOCKPP_BOOST_TEST(Formatter_test::test_number_min_long_hex);
MOCKPP_BOOST_TEST(Formatter_test::test_number_min_long_dec);
MOCKPP_BOOST_TEST(Formatter_test::test_number_max_ulong_dec);
MOCKPP_BOOST_TEST(Formatter_test::test_number_max_long_dec);
MOCKPP_BOOST_TEST(Formatter_test::test_number_double);

MOCKPP_BOOST_TEST(Formatter_test::fail_base_long_37);
MOCKPP_BOOST_TEST(Formatter_test::fail_base_ulong_37);
MOCKPP_BOOST_TEST(Formatter_test::fail_base_long_1);
MOCKPP_BOOST_TEST(Formatter_test::fail_base_ulong_1);
MOCKPP_BOOST_TEST(Formatter_test::fail_format);

#elif defined(MOCKPP_USE_CPPUNIT)

CPPUNIT_TEST_SUITE_REGISTRATION (Formatter_test);

#endif // frameworks



void Formatter_test::fail_format()
{
  MOCKPP_NS::String fmt = MOCKPP_PCHAR("dummy %1 dummy");
  bool thrown = false;
  MOCKPP_TRY
  {
    fmt << 1 << 2;
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Formatter_test::fail_base_long_37()
{
  bool thrown = false;
  MOCKPP_TRY
  {
   /*MOCKPP_NS::String hexstr =*/ MOCKPP_NS::number((long)12, 37);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Formatter_test::fail_base_ulong_37()
{
  bool thrown = false;
  MOCKPP_TRY
  {
   /*MOCKPP_NS::String hexstr = */MOCKPP_NS::number((unsigned long)12, 37);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Formatter_test::fail_base_long_1()
{
  bool thrown = false;
  MOCKPP_TRY
  {
   /*MOCKPP_NS::String hexstr =*/ MOCKPP_NS::number((long)12, 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Formatter_test::fail_base_ulong_1()
{
  bool thrown = false;
  MOCKPP_TRY
  {
  /*MOCKPP_NS::String hexstr =*/ MOCKPP_NS::number((unsigned long)12, 1);
  }
#ifndef MOCKPP_NO_EXCEPTIONS
  catch(MOCKPP_NS::AssertionFailedError &)
  {
    thrown = true;
  }
#endif
  MOCKPP_ASSERT_TRUE(thrown == true);
}


void Formatter_test::test_number_double()
{
  MOCKPP_NS::String str = MOCKPP_NS::number((double) 1.2345678, 3);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("1.23"), str);

  str = MOCKPP_NS::number((double) 1.2345678, 6);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("1.23457"), str); // is rounded !!
}


void Formatter_test::test_number_ulong_oct()
{
  MOCKPP_NS::String str = MOCKPP_NS::number((long)12345678, 8);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("57060516"), str);
}

void Formatter_test::test_number_min_long_dec()
{
#ifdef HAVE_LIMITS
  long ul = MOCKPP_STD_NS::numeric_limits<long>::min();
#else
  long ul = INT_MIN;
#endif

  MOCKPP_NS::String str = MOCKPP_NS::number((long)123, 16);

  str = MOCKPP_NS::number(ul);  // base 10
  char buff[100];
#ifdef HAVE_SPRINTF_S
  sprintf_s(buff, "%ld", ul );
#else
  MOCKPP_STD_NS::sprintf(buff, "%ld", ul );
#endif

#ifdef MOCKPP_UNICODE
   MOCKPP_NS::String traditional = MOCKPP_NS::getUnicode(buff);
#else
   MOCKPP_NS::String traditional(buff);
#endif

  MOCKPP_ASSERT_EQUALS(traditional, str);
}


void Formatter_test::test_number_min_long_hex()
{
#ifndef UPS_NO_LONG_LONG

#ifdef HAVE_LIMITS
  long long ull = MOCKPP_STD_NS::numeric_limits<long long>::min();
#else
  long long ull = INT_MIN;
#endif

  MOCKPP_NS::String str = MOCKPP_NS::number((unsigned long)123, 16);
  str = MOCKPP_NS::number(ull, 16);
//  MOCKPP_STD_NS::cout << MOCKPP_NS::getLatin1(str).c_str() << MOCKPP_STD_NS::endl;
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("-8000000000000000"), str); // differs from sprintf(..)

#endif
}


void Formatter_test::test_number_max_long_dec()
{
#ifndef UPS_NO_LONG_LONG

#ifdef HAVE_LIMITS
  long long ul = MOCKPP_STD_NS::numeric_limits<long long>::max();
#else
  long long ul = INT_MAX;
#endif

  MOCKPP_NS::String str = MOCKPP_NS::number(ul);  // base 10
  char buff[100];
#ifdef HAVE_SPRINTF_S
  sprintf_s(buff, "%lld", ul );
#else
  MOCKPP_STD_NS::sprintf(buff, "%lld", ul );
#endif

#ifdef MOCKPP_UNICODE
   MOCKPP_NS::String traditional = MOCKPP_NS::getUnicode(buff);
#else
   MOCKPP_NS::String traditional(buff);
#endif

  MOCKPP_ASSERT_EQUALS(traditional, str);
#endif
}


void Formatter_test::test_number_max_ulong_dec()
{
#ifndef UPS_NO_LONG_LONG

#ifdef HAVE_LIMITS
  unsigned long long ul = MOCKPP_STD_NS::numeric_limits<unsigned long long>::max();
#else
  unsigned long long ul = UINT_MAX;
#endif

  MOCKPP_NS::String str = MOCKPP_NS::number(ul);  // base 10
  char buff[100];
#ifdef HAVE_SPRINTF_S
  sprintf_s(buff, "%llu", ul );
#else
  MOCKPP_STD_NS::sprintf(buff, "%llu", ul );
#endif

#ifdef MOCKPP_UNICODE
   MOCKPP_NS::String traditional = MOCKPP_NS::getUnicode(buff);
#else
   MOCKPP_NS::String traditional(buff);
#endif

  MOCKPP_ASSERT_EQUALS(traditional, str);

#endif
}


void Formatter_test::test_number_long_hex()
{
  MOCKPP_NS::String hexstr = MOCKPP_NS::number((long)-0x12345678, 16);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("-12345678"), hexstr);

  hexstr = MOCKPP_NS::number((long)0x12345678, 16);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("12345678"), hexstr);

  hexstr = MOCKPP_NS::number((long)-1, 16);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("-1"), hexstr);
}


void Formatter_test::test_number_ulong_hex()
{
  MOCKPP_NS::String hexstr = MOCKPP_NS::number((unsigned long)0x12345678, 16);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("12345678"), hexstr);

  hexstr = MOCKPP_NS::number((unsigned long)0xffffffff, 16);
  MOCKPP_ASSERT_EQUALS(MOCKPP_PCSTRING("ffffffff"), hexstr); // lower case
}


void Formatter_test::test_number_long_range()
{
  for (unsigned i = 2; i <= 36; ++i)
    /*MOCKPP_NS::String hexstr =*/ MOCKPP_NS::number((long)12, i);
}


void Formatter_test::test_number_ulong_range()
{
  for (unsigned i = 2; i <= 36; ++i)
    /*MOCKPP_NS::String hexstr =*/ MOCKPP_NS::number((unsigned long)12, i);
}


void Formatter_test::test_fmt_int()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0"));
  fmt << (int) 0
      << (int) 1
      << (int) 2
      << (int) 3
      << (int) 4
      << (int) 5
      << (int) 6
      << (int) 7
      << (int) 8
      << (int) 9;

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
}


void Formatter_test::test_fmt_bool()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%1%0"));
  fmt << false
      << true;

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("truefalse"));
}


void Formatter_test::test_fmt_uint()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0"));
  fmt << (unsigned int) 0
      << (unsigned int) 1
      << (unsigned int) 2
      << (unsigned int) 3
      << (unsigned int) 4
      << (unsigned int) 5
      << (unsigned int) 6
      << (unsigned int) 7
      << (unsigned int) 8
      << (unsigned int) 9;

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
}


void Formatter_test::test_fmt_long()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0"));
  fmt << (long) 0
      << (long) 1
      << (long) 2
      << (long) 3
      << (long) 4
      << (long) 5
      << (long) 6
      << (long) 7
      << (long) 8
      << (long) 9;

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
}


void Formatter_test::test_fmt_ulong()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0"));
  fmt << (unsigned long) 0
      << (unsigned long) 1
      << (unsigned long) 2
      << (unsigned long) 3
      << (unsigned long) 4
      << (unsigned long) 5
      << (unsigned long) 6
      << (unsigned long) 7
      << (unsigned long) 8
      << (unsigned long) 9;

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
}


void Formatter_test::test_fmt_String()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0"));
  fmt << MOCKPP_PCHAR("0")
      << MOCKPP_PCHAR("1")
      << MOCKPP_PCHAR("2")
      << MOCKPP_PCHAR("3")
      << MOCKPP_PCHAR("4")
      << MOCKPP_PCHAR("5")
      << MOCKPP_PCHAR("6")
      << MOCKPP_PCHAR("7")
      << MOCKPP_PCHAR("8")
      << MOCKPP_PCHAR("9");

  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
}


void Formatter_test::test_fmt_Char()
{
  MOCKPP_NS::String fmt;

  fmt = MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0");
  fmt << (signed char)('0')
      << (signed char)('1')
      << (signed char)('2')
      << (signed char)('3')
      << (signed char)('4')
      << (signed char)('5')
      << (signed char)('6')
      << (signed char)('7')
      << (signed char)('8')
      << (signed char)('9');
#ifndef MOCKPP_MAKE_CHAR_READABLE
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
#else
# ifdef __BORLANDC__
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{" "\x22" "9" "\x22" ", 0x39}{" "\x22" "8" "\x22" ", 0x38}{" "\x22" "7" "\x22" ", 0x37}{" "\x22" "6" "\x22" ", 0x36}{" "\x22" "5" "\x22" ", 0x35}{" "\x22" "4" "\x22" ", 0x34}{" "\x22" "3" "\x22" ", 0x33}{" "\x22" "2" "\x22" ", 0x32}{" "\x22" "1" "\x22" ", 0x31}{" "\x22" "0" "\x22" ", 0x30}"));
# else
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{\"9\", 0x39}{\"8\", 0x38}{\"7\", 0x37}{\"6\", 0x36}{\"5\", 0x35}{\"4\", 0x34}{\"3\", 0x33}{\"2\", 0x32}{\"1\", 0x31}{\"0\", 0x30}"));
# endif
#endif

  fmt = MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0");
  fmt << (unsigned char)('0')
      << (unsigned char)('1')
      << (unsigned char)('2')
      << (unsigned char)('3')
      << (unsigned char)('4')
      << (unsigned char)('5')
      << (unsigned char)('6')
      << (unsigned char)('7')
      << (unsigned char)('8')
      << (unsigned char)('9');
#ifndef MOCKPP_MAKE_CHAR_READABLE
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
#else
# ifdef __BORLANDC__
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{" "\x22" "9" "\x22" ", 0x39}{" "\x22" "8" "\x22" ", 0x38}{" "\x22" "7" "\x22" ", 0x37}{" "\x22" "6" "\x22" ", 0x36}{" "\x22" "5" "\x22" ", 0x35}{" "\x22" "4" "\x22" ", 0x34}{" "\x22" "3" "\x22" ", 0x33}{" "\x22" "2" "\x22" ", 0x32}{" "\x22" "1" "\x22" ", 0x31}{" "\x22" "0" "\x22" ", 0x30}"));
# else
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{\"9\", 0x39}{\"8\", 0x38}{\"7\", 0x37}{\"6\", 0x36}{\"5\", 0x35}{\"4\", 0x34}{\"3\", 0x33}{\"2\", 0x32}{\"1\", 0x31}{\"0\", 0x30}"));
# endif
#endif

  fmt = MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0");
  fmt << (char)('0')
      << (char)('1')
      << (char)('2')
      << (char)('3')
      << (char)('4')
      << (char)('5')
      << (char)('6')
      << (char)('7')
      << (char)('8')
      << (char)('9');
#ifndef MOCKPP_MAKE_CHAR_READABLE
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
#else
# ifdef __BORLANDC__
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{" "\x22" "9" "\x22" ", 0x39}{" "\x22" "8" "\x22" ", 0x38}{" "\x22" "7" "\x22" ", 0x37}{" "\x22" "6" "\x22" ", 0x36}{" "\x22" "5" "\x22" ", 0x35}{" "\x22" "4" "\x22" ", 0x34}{" "\x22" "3" "\x22" ", 0x33}{" "\x22" "2" "\x22" ", 0x32}{" "\x22" "1" "\x22" ", 0x31}{" "\x22" "0" "\x22" ", 0x30}"));
# else
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{\"9\", 0x39}{\"8\", 0x38}{\"7\", 0x37}{\"6\", 0x36}{\"5\", 0x35}{\"4\", 0x34}{\"3\", 0x33}{\"2\", 0x32}{\"1\", 0x31}{\"0\", 0x30}"));
# endif
#endif

  fmt = MOCKPP_PCHAR("%9%8%7%6%5%4%3%2%1%0");
  fmt << MOCKPP_CHAR('0')
      << MOCKPP_CHAR('1')
      << MOCKPP_CHAR('2')
      << MOCKPP_CHAR('3')
      << MOCKPP_CHAR('4')
      << MOCKPP_CHAR('5')
      << MOCKPP_CHAR('6')
      << MOCKPP_CHAR('7')
      << MOCKPP_CHAR('8')
      << MOCKPP_CHAR('9');
#ifndef MOCKPP_MAKE_CHAR_READABLE
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("9876543210"));
#else
# ifdef __BORLANDC__
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{" "\x22" "9" "\x22" ", 0x39}{" "\x22" "8" "\x22" ", 0x38}{" "\x22" "7" "\x22" ", 0x37}{" "\x22" "6" "\x22" ", 0x36}{" "\x22" "5" "\x22" ", 0x35}{" "\x22" "4" "\x22" ", 0x34}{" "\x22" "3" "\x22" ", 0x33}{" "\x22" "2" "\x22" ", 0x32}{" "\x22" "1" "\x22" ", 0x31}{" "\x22" "0" "\x22" ", 0x30}"));
# else
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("{\"9\", 0x39}{\"8\", 0x38}{\"7\", 0x37}{\"6\", 0x36}{\"5\", 0x35}{\"4\", 0x34}{\"3\", 0x33}{\"2\", 0x32}{\"1\", 0x31}{\"0\", 0x30}"));
# endif
#endif
}


void Formatter_test::test_fmt_float()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("==%0=="));
  fmt << (float) 1.2;
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("==1.2=="));
}


void Formatter_test::test_fmt_double()
{
  MOCKPP_NS::String fmt (MOCKPP_PCHAR("==%0=="));
  fmt << (double) 1.2;
  MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("==1.2=="));
}

class Formatter_Dummy
{
  unsigned u;
};


#ifdef MOCKPP_UNICODE

MOCKPP_STD_NS::basic_ostream<wchar_t> &
  operator<< (MOCKPP_STD_NS::basic_ostream<wchar_t> &os, const Formatter_Dummy &/*fd*/)
{
  os << "Formatter_Dummy-wchar_t";
  return os;
}

MOCKPP_OWSTREAMABLE(Formatter_Dummy)

#else // MOCKPP_UNICODE

MOCKPP_STD_NS::basic_ostream<char> &
  operator<< (MOCKPP_STD_NS::basic_ostream<char> &os, const Formatter_Dummy &/*fd*/)
{
  os << "Formatter_Dummy-char";
  return os;
}

MOCKPP_OSTREAMABLE(Formatter_Dummy)

#endif // MOCKPP_UNICODE


void Formatter_test::test_ostreamed()
{

#ifndef MOCKPP_DISABLE_OSTREAMED

  Formatter_Dummy fd;

#ifdef MOCKPP_UNICODE

  {
    MOCKPP_NS::String fmt (MOCKPP_PCHAR("1==%0=="));
    fmt << MOCKPP_NS::owstreamed(fd);
    MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("1==Formatter_Dummy-wchar_t=="));
  }

  {
    MOCKPP_NS::String fmt (MOCKPP_PCHAR("2==%0=="));
    fmt << fd;
    MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("2==Formatter_Dummy-wchar_t=="));
  }

#else // MOCKPP_UNICODE

  {
    MOCKPP_NS::String fmt (MOCKPP_PCHAR("1==%0=="));
    fmt << MOCKPP_NS::ostreamed(fd);
    MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("1==Formatter_Dummy-char=="));
  }

  {
    MOCKPP_NS::String fmt (MOCKPP_PCHAR("2==%0=="));
    fmt << fd;
    MOCKPP_ASSERT_EQUALS(fmt, MOCKPP_PCSTRING("2==Formatter_Dummy-char=="));
  }

#endif // MOCKPP_UNICODE

#endif // MOCKPP_DISABLE_OSTREAMED

}



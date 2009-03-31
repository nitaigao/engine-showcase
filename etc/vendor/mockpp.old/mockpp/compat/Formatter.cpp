/** @file
    @brief    support formatting of strings

 $Id: Formatter.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Wed Dez 25 2002
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 **/


#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h>  // always first header

#include <cstdio>
#include <cctype>

#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START

#undef MOCKPP_ENABLE_DEFAULT_FORMATTER // one never knows..


MOCKPP_ANON_NS_START

/** Transform a signed scalar number into a string
  * @internal
  */
template <typename T, typename uT>
MOCKPP_API_IMPL(MOCKPP_NS::String) number_stpl( T n, unsigned base )
{
  if ( base < 2 || base > 36 )
  {
    MOCKPP_NS::String fmt = mockpp_i18n( MOCKPP_PCHAR( "Conversion of T to String: base %1 not allowed" ) );
    fmt << number( ( uT ) base, 10 );
    MOCKPP_NS::assertionFailed( __LINE__, __FILE__, fmt );
  }

  MOCKPP_NS::Char charbuf[ sizeof(T) * 65 * sizeof( MOCKPP_NS::Char ) ];
  MOCKPP_NS::Char *buf = ( MOCKPP_NS::Char* ) charbuf;
  MOCKPP_NS::Char *p = &buf[ sizeof(T) * 64 ];
  *p = MOCKPP_CHAR( '\0' );
  int len = 0;
  bool neg;
  if ( n < 0 )
  {
    neg = true;
#ifdef HAVE_LIMITS
    if ( n == MOCKPP_STD_NS::numeric_limits<T>::min() )
#else
    if ( n == INT_MIN )
#endif
    {
      MOCKPP_NS::String s1, s2;
      s1 = number( n / ( T ) base, base );
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
      s2 = number( ( unsigned T ) ( -( n + ( T ) base ) ) % base, base );
#else
      s2 = number( ( -( n + ( T ) base ) ) % base, base );
#endif
      return s1 + s2;
    }
    n = -n;
  }
  else
  {
    neg = false;
  }

  do
  {
    *--p = MOCKPP_PCHAR( "0123456789abcdefghijklmnopqrstuvwxyz" ) [ ( ( int ) ( n % base ) ) ];
    n /= base;
    len++;
  }
  while ( n );

  if ( neg )
  {
    *--p = MOCKPP_CHAR( '-' );
    len++;
  }
  return p;
}

MOCKPP_NS_END


MOCKPP_API_IMPL(MOCKPP_NS::String) number( long n, unsigned base )
{
  return number_stpl<long, unsigned long>(n, base);
}


#ifndef UPS_NO_LONG_LONG

MOCKPP_API_IMPL(MOCKPP_NS::String) number( long long n, unsigned base )
{
  return number_stpl<long long, unsigned long long>(n, base);
}

#endif

MOCKPP_ANON_NS_START

/** Transform an unsigned scalar number into a string
  * @internal
  */
template <typename T>
MOCKPP_API_IMPL(MOCKPP_NS::String) number_utpl(T n, unsigned base )
{
  if ( base < 2 || base > 36 )
  {
    MOCKPP_NS::String fmt = mockpp_i18n( MOCKPP_PCHAR( "Conversion of unsigned T to String: base %1 not allowed" ) );
    fmt << number( ( T ) base, 10 );
    MOCKPP_NS::assertionFailed( __LINE__, __FILE__, fmt );
  }

  MOCKPP_NS::Char charbuf[ sizeof(T) * 65 * sizeof( MOCKPP_NS::Char ) ];
  MOCKPP_NS::Char *buf = ( MOCKPP_NS::Char* ) charbuf;
  MOCKPP_NS::Char *p = &buf[ sizeof(T) * 64 ];
  int len = 0;
  *p = MOCKPP_CHAR( '\0' );
  do
  {
    *--p = MOCKPP_PCHAR( "0123456789abcdefghijklmnopqrstuvwxyz" ) [ ( ( int ) ( n % base ) ) ];
    n /= base;
    len++;
  }
  while ( n );

  return p;
}

MOCKPP_NS_END


MOCKPP_API_IMPL(MOCKPP_NS::String) number( unsigned long n, unsigned base )
{
  return number_utpl<unsigned long>(n, base);
}


#ifndef UPS_NO_LONG_LONG

MOCKPP_API_IMPL(MOCKPP_NS::String) number( unsigned long long n, unsigned base )
{
  return number_utpl<unsigned long long>(n, base);
}

#endif

MOCKPP_API_IMPL(MOCKPP_NS::String) number( double n, unsigned prec )
{
  if ( prec > 99 )
    prec = 99;

  char format[ 20 ];
  char buf[ 120 ];
  char *fs = format;

  *fs++ = '%';
  if ( prec != 0 )
  {
    *fs++ = '.';
    if ( prec >= 10 )
    {
      *fs++ = prec / 10 + '0';
      *fs++ = prec % 10 + '0';
    }
    else
      *fs++ = prec + '0';      //   "%.<prec>lg"
    *fs++ = 'l';
  }
  *fs++ = 'g';
  *fs = '\0';
#if defined HAVE_SPRINTF_S
  sprintf_s( buf, format, n );
#else
  MOCKPP_STD_NS::sprintf( buf, format, n );
#endif

#ifdef MOCKPP_UNICODE
  return MOCKPP_NS::getUnicode( buf );
#else
  return MOCKPP_NS::String( buf );
#endif
}


/** Helper function.
  * @internal
  */
static bool findArg( MOCKPP_NS::String &fmt, int& pos, int& len )
{
  MOCKPP_NS::Char lowest = 0;
  for ( unsigned int i = 0; i < fmt.length(); i++ )
  {
    if ( fmt[ i ] == MOCKPP_CHAR( '%' ) && i + 1 < fmt.length() )
    {
      MOCKPP_NS::Char dig = fmt[ i + 1 ];
      if ( dig >= MOCKPP_CHAR( '0' ) && dig <= MOCKPP_CHAR( '9' ) )
      {
        if ( !lowest || dig < lowest )
        {
          lowest = dig;
          pos = i;
          len = 2;
        }
      }
    }
  }
  return lowest != 0;
}


MOCKPP_API_IMPL(String &) replaceFormatterString(MOCKPP_NS::String &formatter, const String &repdata)
{
  int pos, len;

  if ( !findArg(formatter, pos, len ) )
    fail(__LINE__, __FILE__, MOCKPP_PCHAR("No %-placeholder found to insert \'")
                                                             + repdata
                                                             + MOCKPP_PCHAR("\' into \'")
                                                             + formatter
                                                             + MOCKPP_PCHAR("\'"));
  else
    formatter.replace( pos, len, repdata);

  return formatter;
}


/** Transform a character into a human readable string.
  * @internal
  */
static String charToReadable(unsigned x)
{
  String text;
#ifndef MOCKPP_MAKE_CHAR_READABLE
  text += x;
#else
#if (defined (_MSC_VER) && (_MSC_VER <= 1300)) || (defined (__GNUC__) && (__GNUC__ < 3))
  if (!isalnum(x))
#else
  if (!MOCKPP_STD_NS::isalnum(x))
#endif
  {
    text += MOCKPP_PCHAR("0x") + number((unsigned long)x, 16);
  }
  else
  {
    text += MOCKPP_PCHAR("{\"");
    text += x;
    text += MOCKPP_PCHAR("\", ");
    text += MOCKPP_PCHAR("0x") + number((unsigned long)x, 16);
    text += MOCKPP_PCHAR("}");
  }
#endif
  return text;
}


MOCKPP_NS_END


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const int &repdata )
{
  formatter << MOCKPP_NS::number( ( long ) repdata );
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const bool &repdata )
{
  formatter << (repdata ? MOCKPP_PCHAR( "true" )
                        : MOCKPP_PCHAR( "false" ));
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const unsigned int &repdata )
{
  formatter << MOCKPP_NS::number( ( unsigned long ) repdata );
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const unsigned long &repdata )
{
  formatter << MOCKPP_NS::number( repdata );
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const signed char &repdata )
{
  formatter << MOCKPP_NS::charToReadable(repdata);
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const char &repdata )
{
  formatter << MOCKPP_NS::charToReadable(repdata);
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const unsigned char &repdata )
{
  formatter << MOCKPP_NS::charToReadable(repdata);
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const char *repdata )
{
  formatter << MOCKPP_STL::string(repdata);
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const float &repdata )
{
  formatter << MOCKPP_NS::number( repdata );
  return formatter;
}


#ifdef MOCKPP_UNICODE

MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const wchar_t &repdata )
{
  formatter << MOCKPP_NS::charToReadable(repdata);
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const wchar_t * repdata )
{
  formatter << MOCKPP_NS::String(repdata);
  return formatter;
}

#endif


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const long &repdata )
{
  formatter << MOCKPP_NS::number( repdata );
  return formatter;
}


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const double &repdata )
{
  formatter << MOCKPP_NS::number( repdata );
  return formatter;
}


#ifdef MOCKPP_UNICODE

MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << ( MOCKPP_NS::String &formatter, const MOCKPP_STL::string &str )
{
  formatter << MOCKPP_NS::getUnicode( str );
  return formatter;
}

#else

// MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const MOCKPP_STL::basic_string<wchar_t> &str)
// {
//   formatter << MOCKPP_NS::getLatin1(str);
//   return formatter;
// }

#endif


MOCKPP_API_IMPL(MOCKPP_NS::String &) operator << (MOCKPP_NS::String &formatter, const MOCKPP_NS::String &repdata)
{
  return MOCKPP_NS::replaceFormatterString(formatter, repdata);
}



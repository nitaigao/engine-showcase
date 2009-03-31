/** @file
    @brief    Support formatting of strings

  $Id: Formatter.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_FORMATTER_H
#define MOCKPP_FORMATTER_H

#include <mockpp/mockpp.h>  // always first header


MOCKPP_NS_START


/** Converts a long value into its string representation.
  * @ingroup grp_helper
  * @param n          the actual value
  * @param base       the conversion base from 2 to 36
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String) number(long n, unsigned base = 10);

#ifndef UPS_NO_LONG_LONG

/** Converts a longlong value into its string representation.
  * @ingroup grp_helper
  * @param n          the actual value
  * @param base       the conversion base from 2 to 36
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String) number(long long n, unsigned base = 10);

#endif

/** Converts an unsigned long value into its string representation.
  * @ingroup grp_helper
  * @param n          the actual value
  * @param base       the conversion base from 2 to 36
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String) number(unsigned long n, unsigned base = 10);


#ifndef UPS_NO_LONG_LONG

/** Converts an unsigned long value into its string representation.
  * @ingroup grp_helper
  * @param n          the actual value
  * @param base       the conversion base from 2 to 36
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String) number(unsigned long long n, unsigned base = 10);

#endif

/** Converts an double value into its string representation.
  * @ingroup grp_helper
  * @param n          the actual value
  * @param prec       the desired precision (amout of significant digits).
  *                   0 = minimum required, no trailing zeros.
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String) number(double n, unsigned prec = 0);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << "world"  gives "hello world"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(String &) replaceFormatterString(String &formatter, const String &repdata);


MOCKPP_NS_END


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const int &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << true  gives "hello 1"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const bool &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const unsigned int &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String & 
    operator) << (MOCKPP_NS::String &formatter, const unsigned long &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 'A'  gives "hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const signed char &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 'A'  gives "hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const unsigned char &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 'A'  gives "hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const char &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 'A'  gives "hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const char * repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 1.234  gives "hello 1.234"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const float &repdata);

#ifdef MOCKPP_UNICODE

/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * L"hello %1" << L'A'  gives L"hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const wchar_t &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * L"hello %1" << L'A'  gives L"hello A"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &)
    operator << (MOCKPP_NS::String &formatter, const wchar_t * repdata);

#endif // MOCKPP_UNICODE


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const long &repdata);


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << "world"  gives "hello world"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const MOCKPP_NS::String &repdata);


#ifdef MOCKPP_UNICODE

/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << "world"  gives "hello world"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const MOCKPP_STL::string &repdata);

#else // MOCKPP_UNICODE

/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * L"hello %1" << L"world"  gives L"hello world"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
// MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const MOCKPP_STL::basic_string<wchar_t> &repdata);

#endif // MOCKPP_UNICODE


/** Replace a positional %-parameter with a value.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 1.234  gives "hello 1.234"
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param repdata    value to replace
  * @return the resulting string
  */
  MOCKPP_API_DECL(MOCKPP_NS::String &) 
    operator << (MOCKPP_NS::String &formatter, const double &repdata);


#ifdef MOCKPP_ENABLE_DEFAULT_FORMATTER

/** Fallback output: replace a positional %-parameter with a values typeid name.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @ingroup grp_helper
  * @note Only meaningful if RTTI is actually enabled at compile time.
  * @param formatter  target string with position parameter
  * @param o          value by reference to output
  * @return the resulting string
  */
  template <typename T>
  MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const T &o)
  {
    MOCKPP_NS::String s =   MOCKPP_PCHAR("[")
#ifdef MOCKPP_NO_RTTI
                       + MOCKPP_GET_STRING("[rtti-disabled]")
#else
                       + MOCKPP_GET_STRING(typeid(o).name())
#endif
                       + MOCKPP_PCHAR("&]");
    formatter << s;
#ifdef __BORLANDC__
    MOCKPP_UNUSED(o);
#endif
    return formatter;
}


/** Fallback output: replace a positional %-parameter with a values typeid name.
  * Replaces the lowest %-parameter with the string representation of the data.
  * "hello %1" << 9  gives "hello 9"
  * @note Only meaningful if RTTI is actually enabled at compile time.
  * @ingroup grp_helper
  * @param formatter  target string with position parameter
  * @param po         value by pointer to output
  * @return the resulting string
  */
template <typename T>
MOCKPP_NS::String & operator << (MOCKPP_NS::String &formatter, const T * &po)
{
  MOCKPP_NS::String s =   MOCKPP_PCHAR("[")
#ifdef MOCKPP_NO_RTTI
                     + MOCKPP_GET_STRING("[rtti-disabled]")
#else
                     + MOCKPP_GET_STRING(typeid(*po).name())
#endif
                     + MOCKPP_PCHAR("*]");
  formatter << s;
#ifdef __BORLANDC__
  MOCKPP_UNUSED(po);
#endif
  return formatter;
}


#endif // MOCKPP_ENABLE_DEFAULT_FORMATTER


#ifndef MOCKPP_DISABLE_OSTREAMED

#include <sstream>


MOCKPP_NS_START


/** Outputter which reuses an existing operator<<(std::basic_ostream<wchar_t>&, T).
  * @param  t  data type for output
  */
template <class T>
MOCKPP_NS::String ostreamed(const T& t)
{
  MOCKPP_STD_NS::basic_ostringstream<char> oss;
  oss << t;
#ifdef MOCKPP_UNICODE
# ifdef MOCKPP_ALTERNATIVE_STL
  return MOCKPP_NS::getUnicode(oss.str().c_str());
# else
  return MOCKPP_NS::getUnicode(oss.str());
# endif
#else
# ifdef MOCKPP_ALTERNATIVE_STL
  return oss.str().c_str();
# else
  return oss.str();
# endif
#endif
}


MOCKPP_NS_END


/** Outputter which reuses an existing operator<<(std::basic_ostream<char>&, T).
  * @param  T  data type for output
  */
#define MOCKPP_OSTREAMABLE(T) \
MOCKPP_NS::String & \
  operator << (MOCKPP_NS::String &formatter, const T &o) \
{ \
  formatter << MOCKPP_NS::ostreamed(o); \
  return formatter; \
}


#ifdef MOCKPP_UNICODE


MOCKPP_NS_START

/** Outputter which reuses an existing operator<<(std::basic_ostream<wchar_t>&, T).
  * @param  t  data type for output
  */
template <class T>
MOCKPP_NS::String owstreamed(const T& t)
{
  MOCKPP_STD_NS::basic_ostringstream<wchar_t> oss;
  oss << t;
#ifndef MOCKPP_ALTERNATIVE_STL
  return oss.str();
#else
  return oss.str().c_str();
#endif
}

MOCKPP_NS_END

/** Outputter which implements a local (no exports of any kind)
  * forwarder to an existing \c MOCKPP_NS::owstream(T)
  * @param  T  data type for output
  */
#define MOCKPP_OWSTREAMABLE(T) \
MOCKPP_NS::String & \
  operator << (MOCKPP_NS::String &formatter, const T &o) \
{ \
  formatter << MOCKPP_NS::owstreamed(o); \
  return formatter; \
}


#endif // MOCKPP_UNICODE


#endif // MOCKPP_DISABLE_OSTREAMED

#endif // MOCKPP_FORMATTER_H


/** @file
    @brief  Matches an invocation count of at least x

  $Id: InvokeAtLeastMatcher.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 26 2004
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
#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/InvokeAtLeastMatcher.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 InvokeAtLeastMatcher::InvokeAtLeastMatcher( int count )
  : expectedCount(count)
{
}


MOCKPP_API_IMPL0 InvokeAtLeastMatcher::~InvokeAtLeastMatcher()
{
}


MOCKPP_API_IMPL(bool) InvokeAtLeastMatcher::matches( )
{
  return true;
}


MOCKPP_API_IMPL(void) InvokeAtLeastMatcher::verify()
{
  String fmt(MOCKPP_PCHAR("expected method was not invoked often enough, expected ")
              MOCKPP_PCHAR("at least %1 times, was invoked %2 times"));
  fmt << expectedCount << getInvocationCount();

  assertTrue(__LINE__, __FILE__, fmt,
              getInvocationCount() >= expectedCount);
}


MOCKPP_API_IMPL(bool) InvokeAtLeastMatcher::hasDescription()
{
    return true;
}


MOCKPP_API_IMPL(String) InvokeAtLeastMatcher::describeTo( String &buffer ) const
{
  String fmt = MOCKPP_PCHAR("expected at least %1 times, invoked %2 times");
  fmt << expectedCount << getInvocationCount();
  buffer += fmt;
  return buffer;
}


MOCKPP_NS_END


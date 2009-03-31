/** @file
    @brief  Base class for invocation count matchers

  $Id: InvokedRecorder.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

   *
   *  Original Java Sources:  Copyright (c) 2000-2004, jMock.org
   *

 **/

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/InvokedRecorder.h>


MOCKPP_NS_START


MOCKPP_API_IMPL0 InvokedRecorder::InvokedRecorder()
  : invocationCount(0)
{
}


MOCKPP_API_IMPL0 InvokedRecorder::~InvokedRecorder()
{
}


MOCKPP_API_IMPL(int) InvokedRecorder::getInvocationCount() const
{
    return invocationCount;
}


MOCKPP_API_IMPL(bool) InvokedRecorder::hasBeenInvoked() const
{
    return invocationCount > 0;
}


MOCKPP_API_IMPL(bool) InvokedRecorder::matches( )
{
  return true;
}


MOCKPP_API_IMPL(void) InvokedRecorder::incInvoked( )
{
  invocationCount++;
}


MOCKPP_API_IMPL(bool) InvokedRecorder::hasDescription()
{
  return false;
}


MOCKPP_API_IMPL(String) InvokedRecorder::describeTo( String &buffer ) const
{
  return buffer;
}


MOCKPP_API_IMPL(void) InvokedRecorder::verify()
{
  // always verifies
}


MOCKPP_API_IMPL(void) InvokedRecorder::verifyHasBeenInvoked() const
{
  assertTrue(__LINE__, __FILE__, MOCKPP_PCHAR("expected method was not invoked"), hasBeenInvoked() );
}


MOCKPP_API_IMPL(void) InvokedRecorder::verifyHasBeenInvokedExactly( int expectedCount ) const
{
  String fmt (MOCKPP_PCHAR("expected method was not invoked the expected")
              MOCKPP_PCHAR(" number of times: expected ")
              MOCKPP_PCHAR("%1 times, was invoked %2 times"));
  fmt << expectedCount  << invocationCount;
  assertTrue(__LINE__, __FILE__, fmt, invocationCount == expectedCount );
}


MOCKPP_NS_END

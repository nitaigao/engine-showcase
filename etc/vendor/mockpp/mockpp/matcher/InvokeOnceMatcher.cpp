/** @file
    @brief  Matches an invocation count of exact 1

  $Id: InvokeOnceMatcher.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#include <mockpp/matcher/InvokeOnceMatcher.h>


MOCKPP_NS_START


MOCKPP_API_IMPL(bool) InvokeOnceMatcher::matches( )
{
  return !hasBeenInvoked();
}


MOCKPP_API_IMPL(void) InvokeOnceMatcher::verify()
{
    verifyHasBeenInvoked();
}


MOCKPP_API_IMPL(bool) InvokeOnceMatcher::hasDescription()
{
    return true;
}


MOCKPP_API_IMPL(String) InvokeOnceMatcher::describeTo( String &buffer ) const
{
    buffer += MOCKPP_PCHAR("expected once");
    if (hasBeenInvoked())
      buffer += MOCKPP_PCHAR(" and has been invoked");
    return buffer;
}


MOCKPP_NS_END


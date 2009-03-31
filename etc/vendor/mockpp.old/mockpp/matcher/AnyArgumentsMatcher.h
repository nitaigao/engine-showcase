/** @file
    @brief Matches any arguments

  $Id: AnyArgumentsMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ANYARGUMENTSMATCHER_H
#define MOCKPP_ANYARGUMENTSMATCHER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/StatelessInvocationMatcher.h>


MOCKPP_NS_START

/** Matches any arguments.
  * @ingroup grp_matcher
  * @see mockpp::any
  */
template <typename I> // Invocation
class AnyArgumentsMatcher : public StatelessInvocationMatcher<I>
{
  public:

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data (only dummy data)
    * @return always true
    */
    virtual bool matches( const I &invocation )
    {
        MOCKPP_UNUSED(invocation);
        return true;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
        buffer += MOCKPP_PCHAR("(any arguments)");
        return buffer;
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ANYARGUMENTSMATCHER_H

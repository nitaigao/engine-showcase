/** @file
  * @internal
    @brief  Base class for matchers with non-changing state

  $Id: StatelessInvocationMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_STATELESSINVOCATIONMATCHER_H
#define MOCKPP_STATELESSINVOCATIONMATCHER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/InvocationMatcher.h>


MOCKPP_NS_START


/** Base for invocation matcher with no-chaning state.
  * @internal
  */
template <typename I> // Invocation
class StatelessInvocationMatcher : public InvocationMatcher<I>
{
  public:

  /** Increments the invocation count
    * @param  invocation  the invocation data
    */
    virtual void incInvoked( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
        // Do nothing because state cannot change
    }

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
        // Nothing to verify because state cannot change
    }

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription()
    {
      return true;
    }
};


MOCKPP_NS_END


#endif // MOCKPP_STATELESSINVOCATIONMATCHER_H


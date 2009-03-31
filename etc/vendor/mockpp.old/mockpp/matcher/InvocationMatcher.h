/** @file
    @internal
    @brief  Base class for invocation matchers

  $Id: InvocationMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

#ifndef MOCKPP_INVOCATIONMATCHER_H
#define MOCKPP_INVOCATIONMATCHER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/Verifiable.h>

#include <mockpp/SelfDescribing.h>

#include <mockpp/util/AutoPointer.h>


MOCKPP_NS_START


/** Base for invocation matchers. Matchers form a sub-expectation of a
  * chained expectation. Matchers are mainly intended to check the invocation
  * count.
  * @defgroup grp_matcher Matchers
  */
template <typename I> // Invocation
class InvocationMatcher : public Verifiable,
                          public SelfDescribing
{
  public:

    typedef AutoPointer<InvocationMatcher<I> > AP;  //!< internal shorthand

  /** Constructs the object
    */
    InvocationMatcher()
      : Verifiable (MOCKPP_PCHAR("InvocationMatcher"), 0)
    {}

  /** Destroys the object
    */
    virtual ~InvocationMatcher()
    {}

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data
    * @return true:  the invocation matches
    */
    virtual bool matches( const I &invocation ) = 0;

  /** Increments the invocation count
    * @param  invocation  the invocation data
    */
    virtual void incInvoked( const I &invocation ) = 0;

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription() = 0;

  /**
    * Sets all internal data to the state after construction.
    * Only a dummy as matchers don't have data to reset.
    */
    virtual void reset()
    {}
};


MOCKPP_NS_END


#endif // MOCKPP_INVOCATIONMATCHER_H


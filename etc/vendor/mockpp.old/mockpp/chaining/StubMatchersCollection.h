/** @file
    @internal
    @brief  Base class for a collection of stubs for matches

 $Id: StubMatchersCollection.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_STUBMATCHERSCOLLECTION_H
#define MOCKPP_STUBMATCHERSCOLLECTION_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/StubHolder.h>

#include <mockpp/matcher/MatcherHolder.h>


MOCKPP_NS_START


/** Container of matchers for stubs.
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class StubMatchersCollection
{
  public:

  /** Destroys the object
    */
    virtual ~StubMatchersCollection()
    {}

  /** Sets the objects name
    * @param name  object name
    */
    virtual void setName( const String &name ) = 0;

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual void addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Sets the stub data for the invocation.
    * @param stub  the stub object
    */
    virtual void setStub( const StubHolder<R, I> &stub ) = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_STUBMATCHERSCOLLECTION_H


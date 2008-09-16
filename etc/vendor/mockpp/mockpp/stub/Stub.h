/** @file
    @internal
    @brief  Base class for stubs

  $Id: Stub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 24 2004
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

#ifndef MOCKPP_STUB_H
#define MOCKPP_STUB_H

#include <mockpp/mockpp.h> // always first

#include <memory>

#include <mockpp/util/AutoPointer.h>

#include <mockpp/SelfDescribing.h>
#include <mockpp/chaining/Invocation.h>


MOCKPP_NS_START


/** An object that stubs the behaviour of an invoked method invocation on behalf of an
  * object. A stub is also part of a chained expectation.
  * @defgroup grp_stub Stubs
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class Stub : public SelfDescribing
{
  public:

    typedef AutoPointer<Stub<R, I> > AP;  //!< internal shorthand
    typedef R ReturnType;                 //!< internal shorthand
    typedef I InvocationType;             //!< internal shorthand

  /** Destroys the object. Also ensure virtual destructor.
    */
    virtual ~Stub()
    {
    }

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    virtual R invoke( const I &invocation ) = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_STUB_H

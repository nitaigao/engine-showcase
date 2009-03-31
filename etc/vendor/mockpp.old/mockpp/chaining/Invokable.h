/** @file
    @internal
    @brief  Handle Invocations of objects

  $Id: Invokable.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
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

#ifndef MOCKPP_INVOKABLE_H
#define MOCKPP_INVOKABLE_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/Verifiable.h>

#include <mockpp/util/AutoPointer.h>

#include <mockpp/SelfDescribing.h>
#include <mockpp/chaining/Invocation.h>


MOCKPP_NS_START


/** Helper class to handle invocations
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class Invokable : public Verifiable,
                  public SelfDescribing
{
  public:

    typedef AutoPointer<Invokable<R,I> > AP;  //!< internal shorthand


  /** Constructs the object
    */
    Invokable ()
     : Verifiable(MOCKPP_PCHAR("Invokable"), 0)
    {
    }

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data
    * @return true: the invocation matches
    */
    virtual bool matches( const I &invocation ) = 0;

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    virtual R invoke( const I &invocation )  = 0;

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription() = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOKABLE_H

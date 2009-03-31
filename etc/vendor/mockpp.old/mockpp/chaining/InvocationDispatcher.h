/** @file
    @internal
    @brief  Base class for invocation dispatchers

  $Id: InvocationDispatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_INVOCATIONDISPATCHER_H
#define MOCKPP_INVOCATIONDISPATCHER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/Verifiable.h>
#include <mockpp/SelfDescribing.h>

#include <mockpp/stub/StubHolder.h>

#include <mockpp/chaining/Invokable.h>


MOCKPP_NS_START


/** Dispatch invokables
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class InvocationDispatcher : public Verifiable,
                             public SelfDescribing
{
  public:

  /** Constructs the dispatcher
    */
    InvocationDispatcher()
     : Verifiable(MOCKPP_PCHAR("InvocationDispatcher"), 0)
    {
    }

    typedef AutoPointer<InvocationDispatcher<R, I> > AP;  //!< internal shorthand

  /** Dispatches the invokable
    * @param invocation  the invocation data
    */
    virtual R dispatch( const I &invocation ) = 0;

  /** Sets the default stub for the object
    * @param newDefaultStub  pointer to the default stub
    */
    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &newDefaultStub ) = 0;

  /** Adds an invokable
    * @param invokable  pointer to the invocable
    */
    virtual void add( typename Invokable<R,I>::AP invokable ) = 0;

  /** Frees all internal data.
    * @internal
    */
    virtual void reset() = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOCATIONDISPATCHER_H

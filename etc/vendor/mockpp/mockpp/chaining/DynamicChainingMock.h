/** @file
    @internal
    @brief  Base class for chaining mocks

  $Id: DynamicChainingMock.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_DYNAMICCHAININGMOCK_H
#define MOCKPP_DYNAMICCHAININGMOCK_H


#include <mockpp/mockpp.h> // always first

#include MOCKPP_VECTOR_H

#include <mockpp/Verifiable.h>

#include <mockpp/chaining/Invokable.h>

#include <mockpp/stub/StubHolder.h>


MOCKPP_NS_START


/** Interface for chaining mocks.
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class DynamicChainingMock : public Verifiable
{
  public:

    typedef AutoPointer<DynamicChainingMock<R,I> > AP; //!< internal shorthand

  /** Constructs the chaining mock
    * @param  name                 pointer to invocation dispatcher
    * @param  parent               pointer to parent verifiable
    */
    DynamicChainingMock (const String &name, VerifiableList *parent = 0)
      : Verifiable(name, parent)
    {
    }

  /** Transform the object's state to a human readable string.
    * @return string representation
    */
    virtual String toString() const = 0;

  /** Sets the default stub for the object
    * @param newDefaultStub  pointer to the default stub
    */
    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &newDefaultStub ) = 0;

  /** Adds an invokable to the mock object
    * @param invokable  pointer to the default stub
    */
    virtual void addInvokable( typename Invokable<R, I>::AP invokable ) = 0;

  /** Clears all internal states
    */
    virtual void reset() = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_DYNAMICCHAININGMOCK_H


/** @file
    @internal
    @brief  Base class for invocation count matchers

  $Id: InvokedRecorder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_INVOKEDRECORDER_H
#define MOCKPP_INVOKEDRECORDER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/TypelessMatcher.h>

#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Base for objects that match invocation counts.
  * @internal
  */
class MOCKPP_API_DECL0 InvokedRecorder : public TypelessMatcher
{
  public:

    typedef AutoPointer<InvokedRecorder> AP;  //!< internal shorthand

  /** Creates the object
    */
    InvokedRecorder();

  /** Destroys the object
    */
    virtual ~InvokedRecorder();

  /** Gets the current invocation count
    * @return the invocation count
    */
    int getInvocationCount() const;

  /** Tests if the method has been invoked at least once.
    * @return true: object has been invoked
    */
    bool hasBeenInvoked() const;

  /** Checks if an invocation matches the expectaton
    * @return true: the invocation matches
    */
    virtual bool matches( );

  /** Increments the invocation count
    */
    virtual void incInvoked( );

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription();

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const;

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify();

  /** Verifys that the object has been invoked at least once.
    * An AssertionFailedError is throw if it has never been invoked.
    */
    void verifyHasBeenInvoked() const;

  /** Verifys that the object has been invoked the given number.
    * An AssertionFailedError is throw if the count does not match.
    */
    void verifyHasBeenInvokedExactly( int expectedCount ) const;

  private:

    int invocationCount;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOKEDRECORDER_H


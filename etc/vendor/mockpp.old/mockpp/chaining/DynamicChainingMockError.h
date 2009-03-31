/** @file
    @internal
    @brief   Exception for jMock errors

 $Id: DynamicChainingMockError.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_DYNAMICCHAININGMOCKERROR_H
#define MOCKPP_DYNAMICCHAININGMOCKERROR_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/DynamicChainingMock.h>
#include <mockpp/chaining/InvocationDispatcher.h>
#include <mockpp/chaining/Invocation.h>

#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


/** Helper class to handle chaining mock errors.
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class DynamicChainingMockError : public AssertionFailedError
{
  public:

  /** Creates the error object
    * @param in_dynamicMock  the mock object
    * @param in_invocation   the invocation data
    * @param in_dispatcher   the according disapatcher
    * @param in_message      additional error text
    */
    DynamicChainingMockError( DynamicChainingMock<R, I> *in_dynamicMock,
                              const I &in_invocation,
                              InvocationDispatcher<R, I> *in_dispatcher,
                              const String &in_message )
        : AssertionFailedError( __LINE__, __FILE__, in_message )
        , dynamicMock( in_dynamicMock )
        , invocation( in_invocation )
        , dispatcher( in_dispatcher )

    {}


  /** Destroys the object
    */
    virtual ~DynamicChainingMockError() throw()
    {}


  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    String writeTo( String &buffer ) const
    {
      buffer += dynamicMock->toString();
      buffer += MOCKPP_PCHAR( ": " );
      buffer += AssertionFailedError::getMessage();
      buffer += MOCKPP_PCHAR( "\n" );
      buffer += MOCKPP_PCHAR( "Invoked: " );
      buffer = invocation.describeTo( buffer );
      buffer += MOCKPP_PCHAR( "\n" );
      buffer += MOCKPP_PCHAR( "Allowed: " );
      buffer = dispatcher->describeTo( buffer );
      return buffer;
    }


  /** Returns the error data
    * @return error string
    */
    virtual String getMessage() const
    {
      String s;
      return writeTo(s);
    }

  private:

    DynamicChainingMock<R, I>*   dynamicMock;
    I                            invocation;
    InvocationDispatcher<R, I>*  dispatcher;
};


MOCKPP_NS_END


#endif // MOCKPP_DYNAMICCHAININGMOCKERROR_H



/** @file
    @brief  Stub always throwing an AssertionFailedError

  $Id: TestFailureStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_TESTFAILURESTUB_H
#define MOCKPP_TESTFAILURESTUB_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/Stub.h>
#include <mockpp/compat/AssertionFailedError.h>


MOCKPP_NS_START


/** Stub that always fails upon invocation.
  * @ingroup grp_stub
  * @see mockpp::never
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class TestFailureStub : public Stub<R, I>
{
  public:

  /** Creates the test failure stub
    * @param  in_errorMessage  string for error message
    */
    TestFailureStub( const String &in_errorMessage )
      : errorMessage(in_errorMessage)
    {}

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    virtual R invoke( const I &invocation )
    {
        MOCKPP_UNUSED(invocation);
        assertionFailed(__LINE__, __FILE__, errorMessage);
        return R();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      buffer += MOCKPP_PCHAR("fails the test and reports \"")
                + errorMessage
                + MOCKPP_PCHAR("\"");
      return buffer;
    }

  private:

    const String errorMessage;
};


MOCKPP_NS_END


#endif // MOCKPP_TESTFAILURESTUB_H


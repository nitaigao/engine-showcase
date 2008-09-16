/** @file
    @brief Base class for application-specific stubs

  $Id: CustomStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_CUSTOMSTUB_H
#define MOCKPP_CUSTOMSTUB_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/Stub.h>


MOCKPP_NS_START


/** A stub to implement application-specific stubs.
  * @ingroup grp_stub
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class CustomStub : public Stub<R, I>
{
  public:

  /** Creates the object
    * @param in_description  a string describing the stub
    */
    CustomStub( const String &in_description )
      : description(in_description)
    {}

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      return internalDescribeTo(buffer);
    }

  protected:

  /** Appends the description of this object to the buffer.
    * Needed for compilers that can't invoke \c CustumStub<>::describeTo() directly.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    String internalDescribeTo( String &buffer ) const  // msvc6 hack
    {
      buffer += description;
      return buffer;
    }

  private:

    const String description;
};


MOCKPP_NS_END


#endif // MOCKPP_CUSTOMSTUB_H

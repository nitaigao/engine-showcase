/** @file
    @brief  Stub for void methods

  $Id: VoidStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_VOIDSTUB_H
#define MOCKPP_VOIDSTUB_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/TypelessStub.h>


MOCKPP_NS_START


/** A stub for void methods. Not really useful but needed for compatibility.
  * May also be useful to express this property within an expectation.
  * @ingroup grp_stub
  * @see mockpp::onConsecutiveCalls
  */
class MOCKPP_API_DECL0 VoidStub : public TypelessStub<void>
{
  public:

  /** Mock the invocation without passing parameters.
    * @return return value from mocked invocation
    */
    virtual void typelessInvoke( );

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const;
};


MOCKPP_NS_END


#endif // MOCKPP_VOIDSTUB_H


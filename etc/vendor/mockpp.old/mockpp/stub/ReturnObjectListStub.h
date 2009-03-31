/** @file
    @brief  Stub returning a list of defined values

  $Id: ReturnObjectListStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 19 2006
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

#ifndef MOCKPP_RETURNOBJECTLISTSTUB_H
#define MOCKPP_RETURNOBJECTLISTSTUB_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/Formatter.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/stub/TypelessStub.h>
#include <mockpp/ReturnObjectList.h>


MOCKPP_NS_START

/** Stub returning a list of defined values.
  * This object is also a \c Verfiable since there might be the need
  * to verify the use of all objects fed to it in the end.
  * @ingroup grp_stub
  * @see mockpp::returnValue
  */
template <typename R>  // Returntype
class ReturnObjectListStub : public TypelessStub<R>
                           , public Verifiable
{
  public:

  /** Creates the object.
    * @param result  the return value
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ReturnObjectListStub( const R &result, const String &name = MOCKPP_PCHAR(""), VerifiableList *parent = 0  )
      : Verifiable (name, parent)
      , returnObjects(MOCKPP_PCHAR("ReturnObjectListStub/returnObjects"), 0)
    {
      addObjectToReturn(result);
    }

  /** Creates the object based on a list of return values.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    template <class I>
    ReturnObjectListStub( I items, I end, const String &name = MOCKPP_PCHAR(""), VerifiableList *parent = 0 )
      : Verifiable (name, parent)
      , returnObjects(MOCKPP_PCHAR("ReturnObjectListStub/returnObjects"), 0)
    {
      addObjectToReturn(items, end);
    }

  /** Throws an AssertionFailedException if any expectations have not been met.
    */
    virtual void verify()
    {
      returnObjects.verify();
    }

  /**
    * Sets all internal data to the state after construction.
    */
    virtual void reset()
    {
      returnObjects.reset();
    }

  /** Adds another return value.
    * @param retArg   the next value to return
    */
    void addObjectToReturn( const R &retArg )
    {
      returnObjects.addObjectToReturn(retArg);
    }

  /** Add a sequence of next objects to the end of the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ReturnObjectListStub& addObjectToReturn(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addObjectToReturn(*items);
      return *this;
    }

  /** Mock the invocation without passing parameters.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
        return returnObjects.nextReturnObject();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR("returns %1");
      fmt << returnObjects.toString();
      buffer += fmt;
      return buffer;
    }

  private:

    mutable ReturnObjectList<R> returnObjects;
};


MOCKPP_NS_END


#endif // MOCKPP_RETURNOBJECTLISTSTUB_H



/** @file
    @brief  Stub throwing a defined exception

  $Id: ThrowStub.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_THROWSTUB_H
#define MOCKPP_THROWSTUB_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/Throwable.h>
#include <mockpp/stub/TypelessStub.h>


MOCKPP_NS_START


/** Base for stubs throwing exceptions.
  * @ingroup grp_stub
  * @see mockpp::throwException
  */
template <typename R>  // Returntype
class ThrowStubBase : public TypelessStub<R>
{
  public:

  /** Creates the object
    * @param  in_throwable   pointer to the object to throw
    */
    ThrowStubBase( Throwable *in_throwable )
      : throwable(in_throwable)
    {}

  /** Creates the object
    * @param  in_throwable   the object to throw
    */
    ThrowStubBase( ThrowableItem &in_throwable )
      : throwable(in_throwable)
    {}

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      buffer += MOCKPP_PCHAR("throws <")
#ifdef MOCKPP_NO_RTTI
                + MOCKPP_GET_STRING("[rtti-disabled]")
#else
                + MOCKPP_GET_STRING(typeid(throwable).name())
#endif
                + MOCKPP_PCHAR(">");
      return buffer;
    }

  protected:

    ThrowableItem throwable;  //!< internal shared member
};


/** A stub throwing exceptions.
  */
template <typename R>  // Returntype
class ThrowStub : public ThrowStubBase<R>
{
  public:

  /** Creates the object
    * @param  in_throwable   pointer to the object to throw
    */
    ThrowStub( Throwable *in_throwable )
      : ThrowStubBase<R>(in_throwable)
    {}

  /** Creates the object
    * @param  in_throwable   the object to throw
    */
    ThrowStub( ThrowableItem &in_throwable )
      : ThrowStubBase<R>(in_throwable)
    {}

  /** Mock the invocation without passing parameters.
    * Actually the method never returns but the excection is thrown instead.
    * @return return value from mocked invocation
    */
    virtual R typelessInvoke()
    {
        this->throwable.get()->throw_me();
        return R();  // avoids warnings
    }
};



/** A stub throwing exceptions.
  */
template<>
class ThrowStub<void> : public ThrowStubBase<void>
{
  public:

  /** Creates the object
    * @param  in_throwable   pointer to the object to throw
    */
    ThrowStub( Throwable *in_throwable )
      : ThrowStubBase<void>(in_throwable)
    {}

  /** Creates the object
    * @param  in_throwable   the object to throw
    */
    ThrowStub( ThrowableItem &in_throwable )
      : ThrowStubBase<void>(in_throwable)
    {}

  /** Mock the invocation without passing parameters.
    * Actually the method never returns but the excection is thrown instead.
    * @return return value from mocked invocation
    */
    virtual void typelessInvoke()
    {
        this->throwable.get()->throw_me();
    }
};


MOCKPP_NS_END


#endif // MOCKPP_THROWSTUB_H


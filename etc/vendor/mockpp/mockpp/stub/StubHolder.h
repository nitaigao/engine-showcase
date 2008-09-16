/** @file
    @internal
    @brief  Container helper for Stub objects.

  $Id: StubHolder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Oct 24 2004
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

 **/

#ifndef MOCKPP_STUBHOLDER_H
#define MOCKPP_STUBHOLDER_H

#include <mockpp/util/AutoPointer.h>

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/Stub.h>
#include <mockpp/stub/TypelessStub.h>


MOCKPP_NS_START


/** Proxy class to conveniently move Stubs into Stub related methods.
  * @internal
  * @attention Not intended for common use since this class is actually a liar.
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class StubHolder
{
  public:

  /** Creates the object.
    * @param  in_stub  pointer to the stub
    */
    StubHolder(const AutoPointer<TypelessStub<R> > &in_stub)
      : stub(new TypelessStubAdapter<R, I>(in_stub))
    {}

  /** Creates the object.
    * @param  in_stub  pointer to the stub
    */
    StubHolder(TypelessStub<R> *in_stub)
      : stub(new TypelessStubAdapter<R, I>(in_stub))
    {}

  /** Creates the object.
    * @param  in_stub  pointer to the stub
    */
    StubHolder(Stub<R, I> *in_stub)
      : stub(in_stub)
    {}

#ifndef _MSC_VER

	/** Creates the object.
    * @param  in_stub  pointer to the stub
    */
    StubHolder(const AutoPointer<Stub<R, I> > &in_stub)
      : stub(in_stub)
    {}

#endif

  /** Returns the stub object.
    * @attention After this method is called this object becomes invalid.
    * @return the stub object
    */
    Stub<R, I>* release() const  // we we actually lie a bit
    {
      return stub.release();
    }

  /** Returns the stub object.
    * @attention After this method is called this object becomes invalid.
    * @return the stub object
    */
    operator AutoPointer<Stub<R, I> > () const  // we we actually lie a bit
    {
      return stub;
    }

  private:

    mutable typename Stub<R, I>::AP   stub;   // we we actually lie a bit
};


MOCKPP_NS_END


#endif // MOCKPP_STUBHOLDER_H

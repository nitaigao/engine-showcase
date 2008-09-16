/** @file
    @internal
    @brief  Build a stub

  $Id: StubBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_STUBBUILDER_H
#define MOCKPP_STUBBUILDER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/builder/IdentityBuilder.h>

#include <mockpp/stub/VoidStub.h>
#include <mockpp/stub/StubHolder.h>


MOCKPP_NS_START


/** Builder class for stub related purposes.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,  // Returntype
          typename I>  // Invocation
class StubBuilder : public IdentityBuilder
{
  public:

#ifdef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness

  /** Indicates that the method is \c void.
    * @return the builder object
    */
    IdentityBuilder& isVoid()
    {
      setStub( new VoidStub());
      return *this;
    }

#endif

  /** Sets the stub return data for the invocation.
    * @ingroup grp_chainer
    * @param stubAction  data to return
    * @return the builder object
    */
    IdentityBuilder& will( const StubHolder<R, I> &stubAction )
    {
      setStub( stubAction );
      return *this;
    }

  /** Sets the stub data for the invocation.
    * @param stubAction  the stub object
    */
    virtual void setStub( const StubHolder<R, I> &stubAction ) = 0;
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation for builder class for void stub related purposes.
  * @ingroup grp_chainer
  * @internal
  */
template <typename I>  // Invocation
class StubBuilder<void, I> : public IdentityBuilder
{
  public:

  /** Indicates that the method is \c void.
    * @return the builder object
    */
    IdentityBuilder& isVoid()
    {
      setStub( new VoidStub());
      return *this;
    }

  /** Sets the stub return data for the invocation.
    * @ingroup grp_chainer
    * @param stubAction  data to return
    * @return the builder object
    */
    IdentityBuilder& will( const StubHolder<void, I> &stubAction )
    {
      setStub( stubAction );
      return *this;
    }

  /** Sets the stub data for the invocation.
    * @param stubAction  the stub object
    */
    virtual void setStub( const StubHolder<void, I> &stubAction ) = 0;
};


#endif


MOCKPP_NS_END


#endif // MOCKPP_STUBBUILDER_H


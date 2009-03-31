/** @file
    @brief  Visitable Mock Method with 0 parameters.
            Generated with gen_visitablemethod_N.pl.

  $Id: VisitableMockMethod0.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 22 2005
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

#ifndef MOCKPP_VisitableMockMethod0_H
#define MOCKPP_VisitableMockMethod0_H

#include <mockpp/mockpp.h>

#include <mockpp/visiting/VisitableMockObject.h>
#include <mockpp/visiting/VisitableMockMethod.h>


MOCKPP_NS_START


/** Common stuff to set up visitable mock method expectations with 0 parameters.
  * @ingroup grp_controller
  */
template <typename R>
class VisitableMockMethod0Common : public VisitableMockReturningMethodBase<R>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod0Common(const String &name, VisitableMockObject *parent)
      : VisitableMockReturningMethodBase<R>(name, parent)
    {
    }

  protected:

  /** Perform the internals to verify a mocked method.
    */
    void forward_param() const
    {
      if (!this->getVisitableMockObject()->isActivated() )
      {
        this->getVisitableMockObject()->addExpectedMethod(this->getMethodIdentifier());
      }
      else
      {
        this->getVisitableMockObject()->addActualMethod(this->getMethodIdentifier());
        this->throwAvailableException();
      }
    }
  public:

};


/** Set up visitable mock method expectations with 0 parameters.
  * @ingroup grp_controller
  */
template <typename R>
class VisitableMockMethod0
  : public VisitableMockMethod0Common<R>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod0(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod0Common<R>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    */
    R forward() const
    {
      this->forward_param();

      if (this->getVisitableMockObject()->isActivated() )
      {
        return this->determineReturnValue();
      }
      else
        return R(); // only dummy value to make the compiler happy
    }
};


/** Set up visitable mock method expectations with 0 parameters.
  * Partial specialisation for a void return value.
  * @ingroup grp_controller
  */
template <>
class VisitableMockMethod0<void>
   : public VisitableMockMethod0Common<void>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent Visitable mock object
    */
    VisitableMockMethod0(const String &name, VisitableMockObject *parent)
      : VisitableMockMethod0Common<void>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    */
    void forward() const
    {
       this->forward_param();
    }
};


MOCKPP_NS_END


#endif // MOCKPP_VisitableMockMethod0_H


/** @file
    @brief  Negates the result of another constraint

  $Id: IsNot.h 1445 2008-05-16 19:08:55Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
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

#ifndef MOCKPP_ISNOT_H
#define MOCKPP_ISNOT_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/ConstraintHolder.h>


MOCKPP_NS_START


/** Calculates the logical negation of a constraint.
  * @ingroup grp_constraint
  * @see mockpp::ne
  */
template <typename T>
class IsNot : public Constraint<T>
{
  public:

  /** Constructs the object
    * @param in_constraint   the constraint which will be negated
    */
    IsNot( const ConstraintHolder<T> &in_constraint )
      : constraint(in_constraint)
    {
    }

  /** Destroys the object
    */
    virtual ~IsNot( )
    {}

  /** Evaluates the constraint
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const T &arg ) const
    {
      return !constraint->eval(arg);
    }

  /** Evaluates the constraint at the end.
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool verify( const T &arg ) const
    {
      return !constraint->verify(arg);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
       buffer += MOCKPP_PCHAR("not ");
       return constraint->describeTo(buffer);
    }

  private:

    const typename Constraint<T>::AP constraint;
};


MOCKPP_NS_END


#endif // MOCKPP_ISNOT_H


/** @file
    @internal
    @brief  Container helper for constraint objects.

  $Id: ConstraintHolder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_CONSTRAINTHOLDER_H
#define MOCKPP_CONSTRAINTHOLDER_H

#include <mockpp/util/AutoPointer.h>

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/constraint/TypelessConstraint.h>


MOCKPP_NS_START


/** Proxy class to conveniently move constraints into constraint sets.
  * @internal
  * @attention Not intended for common use since this class is actually a liar.
  */
template <typename T>
class ConstraintHolder
{
  public:

  /** Constructs the object.
    * @param in_constraint  pointer to the constraint
    */
    ConstraintHolder(Constraint<T> *in_constraint)
      : constraint(in_constraint)
    {}

  /** Constructs the object.
    * @param in_constraint  pointer to the typeless constraint
    */
    ConstraintHolder(const typename TypelessConstraint::AP &in_constraint)
      : constraint(new TypelessConstraintAdapter<T>(in_constraint))
    {}

  /** Constructs the object.
    * @param in_constraint  pointer to the typeless constraint
    */
    ConstraintHolder(TypelessConstraint *in_constraint)
      : constraint(new TypelessConstraintAdapter<T>(in_constraint))
    {}

  /** Constructs the object.
    * @param in_constraint  pointer to the constraint
    */
    ConstraintHolder(const typename Constraint<T>::AP &in_constraint)
      : constraint(in_constraint)
    {}

  /** Returns the constraint object.
    * @attention After this method is called this object becomes invalid.
    * @return the costraint object
    */
#if defined(__BORLANDC__) && (__BORLANDC__ >= 560)
    operator typename AutoPointer<Constraint<T> > () const  // we actually lie a bit
#else
    operator typename Constraint<T>::AP () const
#endif
    {
      return constraint;
    }

  private:

    mutable typename Constraint<T>::AP   constraint;   // we actually lie a bit
};


MOCKPP_NS_END


#endif // MOCKPP_CONSTRAINTHOLDER_H

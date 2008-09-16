/** @file
    @brief  Checks if the argument is a derived object

  $Id: IsInstanceOf.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Aug 21 2004
   copyright            : (C) 2002-2008 by Ewald Arnold
   email                : mockpp at ewald-arnold dot de

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
    by the Free Software Foundation; either version 2 of the License,
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

#ifndef MOCKPP_ISINSTANCEOF_H
#define MOCKPP_ISINSTANCEOF_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>


MOCKPP_NS_START


#ifndef MOCKPP_NO_RTTI

/** Tests whether the value is derived from the given class.
  * A base class is needed since dynamic_cast checks for an existing traversal
  * from \c derived to \c base.
  * @ingroup grp_constraint
  * @see mockpp::isA
  */
template <typename BASE,
          typename DERIVED>
class IsInstanceOf : public Constraint<BASE*>
{
    typedef BASE*  pBASE;

  public:

  /** Evaluates the constraint
    * @param obj the object against which the constraint is evaluated.
    * @return true:  obj meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const pBASE &obj ) const
    {
      return dynamic_cast<const DERIVED*>(obj) != 0;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
        buffer += MOCKPP_PCHAR("derived from ")
               + MOCKPP_GET_STRING(typeid(DERIVED).name());
        return buffer;
    }
};

#endif // MOCKPP_NO_RTTI

MOCKPP_NS_END


#endif // MOCKPP_ISINSTANCEOF_H

/** @file
    @brief  Base class for triggered or defered constraints

  $Id: TriggeredConstraint.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 01 2006
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

#ifndef MOCKPP_TRIGGERED_CONSTRAINT_H
#define MOCKPP_TRIGGERED_CONSTRAINT_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>


MOCKPP_NS_START


/** A constraint which is triggered or defered. Constraints are evaluated
  * before the mock methodis invoked.
  * over a method parameter.
  * @ingroup grp_constraint
  */
template <typename T>
class TriggeredConstraint : public  Constraint<T>
{
  public:

  /** Triggers the constraint
    */
    virtual void trigger() = 0;
};


MOCKPP_NS_END


#endif // MOCKPP_TRIGGERED_CONSTRAINT_H

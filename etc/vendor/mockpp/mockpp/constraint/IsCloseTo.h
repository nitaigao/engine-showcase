/** @file
    @brief  Checks if the value is close to a reference

  $Id: IsCloseTo.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ISCLOSETO_H
#define MOCKPP_ISCLOSETO_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Is the value of a number equal to a value within some range of
  * acceptable deviation?
  * @ingroup grp_constraint
  * @see mockpp::eq( const T &operand, const T &deviation )
  */
template <typename NumberType>
class IsCloseTo : public Constraint<NumberType>
{
  public:

  /** Constructs the object
    * @param in_value      the value for the comparison
    * @param in_deviation  the allowed deviation
    */
    IsCloseTo( const NumberType &in_value, const NumberType &in_deviation )
      : value(in_value)
      , deviation(in_deviation)
    {}

  /** Evaluates the constraint
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const NumberType &arg ) const
    {
       // don't use std::abs to better support user types
       return    (arg <= value + deviation && arg >= value - deviation)  // positive deviation
              || (arg <= value - deviation && arg >= value + deviation); // negative deviation
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR("a numeric value within +-%1 of %2");
      fmt << deviation << value;
      buffer += fmt;
      return buffer;
    }

  private:

    const NumberType value;
    const NumberType deviation;
};


MOCKPP_NS_END


#endif // MOCKPP_ISCLOSETO_H

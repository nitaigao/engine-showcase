/** @file
    @brief  Checks if the argument is equal to the referenced value

 $Id: IsEqual.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ISEQUAL_H
#define MOCKPP_ISEQUAL_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Default comparison function for \c IsEqual.
  * The default implementation compares the values of the two
  * objects.
  * @ingroup grp_constraint
  * @see mockpp::IsEqual
  * @param left  left operand
  * @param right right operand
  * @return true if both values are equal
  */
template <class T>
bool isEqualComparison(const T &left, const T &right)
{
  return left == right;
}


/** Is the value equal to another value, as tested by the
  * invoked method? Basically the same as \c IsSame which compares
  * by reference and a specializable comparison template.
  * @ingroup grp_constraint
  * @see mockpp::eq( const T& op )
  * @see mockpp::IsSame
  */
template <typename T>
class IsEqual : public Constraint<T>
{
  public:

  /** Constructs the object
    * @param equalArg   the value for the comparison
    */
    IsEqual( const T &equalArg )
        : ref( equalArg )
    {}

  /** Destroys the object
    */
    virtual ~IsEqual()
    {}

  /** Evaluates the constraint
    * @param arg the object against which the constraint is evaluated.
    * @return true:  o meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const T &arg ) const
    {
      return isEqualComparison(arg, ref);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR( "equalTo %1" );
      fmt << ref;
      buffer += fmt;
      return buffer;
    }

  private:

    const T ref;
};


MOCKPP_NS_END


#endif // MOCKPP_ISEQUAL_H

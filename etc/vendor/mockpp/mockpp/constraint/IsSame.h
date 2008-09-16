/** @file
   @brief  Checks if the argument is the same as the referenced value

 $Id: IsSame.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ISSAME_H
#define MOCKPP_ISSAME_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


/** Default comparison function for \c IsSame.
  * The default implementation compares the adresses of the two
  * objects.
  * @ingroup grp_constraint
  * @see mockpp::IsSame
  * @param left  left operand
  * @param right right operand
  * @return true if both operands are equal
  */
template <class T>
bool isSameComparison(const T &left, const T &right)
{
  return &left == &right;
}


/** Is the value the same object as another value?
  * Basically the same as \c IsEqual which compares with a value.
  * \c IsSame compares by reference and a comparison template which can be
  * specialized.
  * @ingroup grp_constraint
  * @see mockpp::same
  * @see mockpp::IsEqual
  * @see mockpp::isSameComparison
  */
template <typename T>
class IsSame : public Constraint<T>
{
  public:

  /** Constructs the object
    * @param object  the object to compare with
    */
    IsSame( const T &object )
        : ref( object )
    {}

  /** Destroys the object
    */
    virtual ~IsSame( )
    {}

  /** Evaluates the constraint
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const T &arg ) const
    {
      return isSameComparison(arg, ref);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR( "sameAs %1" );
      fmt << ref;
      buffer += fmt;
      return buffer;
    }

  private:

    const T &ref;
};


MOCKPP_NS_END


#endif // MOCKPP_ISSAME_H

/** @file
    @brief  Logical and of two constraints

 $Id: And.h 1449 2008-05-22 14:57:05Z ewald-arnold $

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

#ifndef MOCKPP_AND_H
#define MOCKPP_AND_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/ConstraintHolder.h>


MOCKPP_NS_START


/** Calculates the logical conjunction of two constraints. Evaluation is
  * shortcut by default, so that the second constraint is not called if the first
  * constraint returns <code>false</code>.
  */
template <typename T>
class And : public Constraint<T>
{
  public:

  /** Constructs the object
    * @param in_left   first operand
    * @param in_right  second operand
    * @param in_short  @li true: don't evaluate \c right if \c left is \c true (c++ standard behaviour)
    *                  @li false: alwyas evaluate \c right even if \c left is \c true
    */
    And( const ConstraintHolder<T> &in_left,
         const ConstraintHolder<T> &in_right,
         bool  in_short = true )
        : left( in_left )
        , right( in_right )
        , shortcut(in_short)
    {}

  /** Destroys the object
    */
    virtual ~And()
    {}

  /** Evaluates the constraint
    * @param o the object against which the constraint is evaluated.
    * @return true:  o meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const T &o ) const
    {
      if (shortcut)
        return left->eval( o ) && right->eval( o );

      else
      {
        const bool res_left = left->eval( o );
        const bool res_right = right->eval( o );

        return res_left && res_right;
      }
    }

  /** Evaluates the constraint at the end.
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool verify( const T &arg ) const
    {
      if (shortcut)
        return left->verify(arg) && right->verify(arg);

      else
      {
        const bool res_left = left->verify( arg );
        const bool res_right = right->verify( arg );

        return res_left && res_right;
      }
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      buffer += MOCKPP_PCHAR( "(" );
      buffer = left->describeTo( buffer );
      buffer += MOCKPP_PCHAR( " && " );
      buffer = right->describeTo( buffer );
      buffer += MOCKPP_PCHAR( ")" );
      return buffer;
    }

  private:

    const typename Constraint<T>::AP left;
    const typename Constraint<T>::AP right;
    bool  shortcut;
};


MOCKPP_NS_END


#endif // MOCKPP_AND_H

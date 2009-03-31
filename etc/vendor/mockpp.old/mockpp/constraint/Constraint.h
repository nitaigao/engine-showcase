/** @file
    @brief  Base class for argument constraints

  $Id: Constraint.h 1439 2008-05-09 13:33:16Z ewald-arnold $

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

#ifndef MOCKPP_CONSTRAINT_H
#define MOCKPP_CONSTRAINT_H

#include <mockpp/util/AutoPointer.h>

#include <mockpp/mockpp.h> // always first

#include <mockpp/SelfDescribing.h>


MOCKPP_NS_START


/** A constraint over acceptable values.
  * Constraints are part of a chained expectation and form a sub-expectation
  * over a method parameter.
  * @defgroup grp_constraint Parameter Constraints
  */
template <typename T>
class Constraint : public SelfDescribing
{
  public:

    typedef AutoPointer<Constraint<T> > AP;  //!< internal shorthand

  /** Destroys the object
    */
    virtual ~Constraint()
    {}

  /** Evaluates the constraint
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval( const T &arg ) const = 0;

  /** Evaluates the constraint at the end.
    * @param arg the object against which the constraint is evaluated.
    * @return true:  arg meets the constraint,
    * @return false if it does not.
    */
    virtual bool verify( const T &arg ) const
    {
      return this->eval(arg);
    }
};


MOCKPP_NS_END


#endif // MOCKPP_CONSTRAINT_H

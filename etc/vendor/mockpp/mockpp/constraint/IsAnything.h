/** @file
    @brief  Constraint which is always true

 $Id: IsAnything.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ISANYTHING_H
#define MOCKPP_ISANYTHING_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/TypelessConstraint.h>


MOCKPP_NS_START


/** A constraint that always returns <code>true</code>.
  * @ingroup grp_constraint
  * @see mockpp::any
  */
class MOCKPP_API_DECL0 IsAnything : public TypelessConstraint
{
  public:

  /** Constructs the object.
    */
    IsAnything();

  /** Constructs the object by name.
    */
    IsAnything( const String &in_description );

  /** Destroys the object
    */
    virtual ~IsAnything();

  /** Evaluates the constraint
    * @return true:  the object meets the constraint,
    * @return false if it does not.
    */
    virtual bool typelessEval( ) const;

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const;

  private:

    String description;
};


MOCKPP_NS_END


#endif //MOCKPP_ISANYTHING_H

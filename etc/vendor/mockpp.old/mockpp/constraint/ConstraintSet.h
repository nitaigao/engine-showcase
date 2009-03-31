/** @file
    @internal
    @brief  A set of argument constrains

 $Id: ConstraintSet.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

 **/

#ifndef MOCKPP_CONSTRAINTSET_H
#define MOCKPP_CONSTRAINTSET_H

#include <mockpp/constraint/ConstraintHolder.h>

#include <mockpp/compat/Formatter.h>

#include <mockpp/mockpp.h> // always first


MOCKPP_NS_START

/** A set of constraints for a method without arguments
  * @internal
  */
class MOCKPP_API_DECL0 ConstraintSet0
{
  public:

  /** Destroys the object
    */
    virtual ~ConstraintSet0();

  /** Tests if the constraint set matches the invocation
    * @param   invocation   the invocation data
    * @return true: the set matches
    */
    template <typename I> // Invocation
    bool matches( const I & invocation )
    {
      MOCKPP_UNUSED(invocation);
      return true;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const;
};



MOCKPP_NS_END


#endif // MOCKPP_CONSTRAINTSET_H


#include "ConstraintSetN.h"

/** @file
    @internal
    @brief  Build matcher for arguments

 $Id: ArgumentsMatchBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Sun Aug 22 2004
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

#ifndef MOCKPP_ARGUMENTSMATCHBUILDER_H
#define MOCKPP_ARGUMENTSMATCHBUILDER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/builder/MatchBuilder.h>

#include <mockpp/constraint/ConstraintSet.h>
#include <mockpp/constraint/ConstraintHolder.h>

#include <mockpp/matcher/NoArgumentsMatcher.h>
#include <mockpp/matcher/AnyArgumentsMatcher.h>
#include <mockpp/matcher/MatcherHolder.h>


MOCKPP_NS_START


/** Build a matcher for an invocation with no arguments.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class ArgumentsMatchBuilder0 : public MatchBuilder<R,I>
{
  public:

    typedef R ReturnType;                      //!< internal shorthand
    typedef I InvocationType;                  //!< internal shorthand
    typedef ConstraintSet0 ConstraintSetType;  //!< internal shorthand

  /** Indicate a matcher for \c no arguments.
    * @ingroup grp_chainer
    * @return the builder object
    */
    MatchBuilder<R, I>& with()
    {
      return withNoArguments();
    }

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I>& addMatcher( const MatcherHolder<I> &matcher ) = 0;

  /** Indicate a matcher for \c no arguments.
    * @return the builder object
    */
    MatchBuilder<R, I>& withNoArguments()
    {
      return addMatcher( new NoArgumentsMatcher<I>() );
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ARGUMENTSMATCHBUILDER_H


#include "ArgumentsMatchBuilderN.h"



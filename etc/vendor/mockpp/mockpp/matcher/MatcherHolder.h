/** @file
    @internal
    @brief  Container helper for Matcher objects.

  $Id: MatcherHolder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Oct 24 2004
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

#ifndef MOCKPP_MATCHERHOLDER_H
#define MOCKPP_MATCHERHOLDER_H

#include <mockpp/util/AutoPointer.h>

#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/InvocationMatcher.h>
#include <mockpp/matcher/TypelessMatcher.h>


MOCKPP_NS_START


/** Proxy class to conveniently move Matchers into matcher related methods.
  * @internal
  * @attention Not intended for common use since this class is actually a liar.
  */
template <typename I>   // Invocation
class MatcherHolder
{
  public:

  /** Constructs the object.
    * @param  in_matcher  pointer to the matcher
    */
    MatcherHolder(InvocationMatcher<I> *in_matcher)
      : matcher(in_matcher)
    {
    }

  /** Constructs the object.
    * @param  in_matcher  pointer to the typeless matcher
    */
    MatcherHolder(const typename TypelessMatcher::AP &in_matcher)
      : matcher(new TypelessMatcherAdapter<I>(in_matcher))
    {
    }

  /** Constructs the object.
    * @param  in_matcher  pointer to the typeless matcher
    */
    MatcherHolder(TypelessMatcher*in_matcher)
      : matcher(new TypelessMatcherAdapter<I>(in_matcher))
    {
    }

  /** Constructs the object.
    * @param  in_matcher  pointer to the matcher
    */
    MatcherHolder(const typename InvocationMatcher<I>::AP &in_matcher)
      : matcher(in_matcher)
    {
    }

  /** Returns the matcher object.
    * @attention After this method is called this object becomes invalid.
    * @return the matcher object
    */
#if defined(__BORLANDC__) && (__BORLANDC__ >= 560)
    operator typename AutoPointer<InvocationMatcher<I> > () const  // we actually lie a bit
#else
    operator typename InvocationMatcher<I>::AP () const
#endif
    {
      return matcher;
    }

  private:

    mutable typename InvocationMatcher<I>::AP   matcher;   // we actually lie a bit
};


MOCKPP_NS_END


#endif // MOCKPP_MATCHERHOLDER_H

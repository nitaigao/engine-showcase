/** @file
    @brief  Matches an invocation after another invocation

  $Id: InvokedAfterMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 26 2004
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

#ifndef MOCKPP_INVOKEDAFTERMATCHER_H
#define MOCKPP_INVOKEDAFTERMATCHER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/StatelessInvocationMatcher.h>
#include <mockpp/matcher/InvokedRecorder.h>


MOCKPP_NS_START


/** Has the method been invoked after another?
  * @ingroup grp_matcher
  * @see mockpp::MatchBuilder::after
  */
template <typename I> // Invocation
class InvokedAfterMatcher : public StatelessInvocationMatcher<I>
{
  public:

  /** Constructs the object
    * @param  in_priorCallRecorder     pointer to the invcation call recorder
    * @param  in_priorCallDescription  string describing the prior call
    */
    InvokedAfterMatcher( InvokedRecorder *in_priorCallRecorder,
                         const String &in_priorCallDescription )
      :  priorCallRecorder(in_priorCallRecorder)
      ,  priorCallDescription(in_priorCallDescription)
    {
    }

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data (only dummy data)
    * @return true: the invocation matches
    */
    virtual bool matches( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      return priorCallRecorder->hasBeenInvoked();
    }

  /** Increments the invocation count
    * @param  invocation  the invocation data
    */
    virtual void incInvoked( const I &invocation )
    {
      if (!matches(invocation))
        assertionFailed(__LINE__, __FILE__,
                                   MOCKPP_PCHAR("called out of order; should be called after ")
                                   + priorCallDescription);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      buffer += MOCKPP_PCHAR("after ") + priorCallDescription;
      if( priorCallRecorder->hasBeenInvoked() )
        buffer += MOCKPP_PCHAR(" (invoked)");
      else
        buffer += MOCKPP_PCHAR(" (not invoked)");

      return buffer;
    }

  private:

    InvokedRecorder    *priorCallRecorder;
    String              priorCallDescription;

};


MOCKPP_NS_END


#endif // MOCKPP_INVOKEDAFTERMATCHER_H


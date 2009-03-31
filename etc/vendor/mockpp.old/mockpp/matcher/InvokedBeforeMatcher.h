/** @file
    @brief  Matches an invocation before another invocation

  $Id: InvokedBeforeMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

 **/

#ifndef MOCKPP_INVOKEDBEFOREMATCHER_H
#define MOCKPP_INVOKEDBEFOREMATCHER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/matcher/StatelessInvocationMatcher.h>
#include <mockpp/matcher/InvokedRecorder.h>


MOCKPP_NS_START


/** Has the method been invoked before another?
  * @ingroup grp_matcher
  * @see mockpp::MatchBuilder::before
  */
template <typename I> // Invocation
class InvokedBeforeMatcher : public StatelessInvocationMatcher<I>
{
  public:

  /** Constructs the object
    * @param  in_subsequentCallRecorder     pointer to the invcation call recorder
    * @param  in_subsequentCallDescription  string describing the subsequent call
    */
    InvokedBeforeMatcher( InvokedRecorder *in_subsequentCallRecorder,
                          const String &in_subsequentCallDescription )
      :  subsequentCallRecorder(in_subsequentCallRecorder)
      ,  subsequentCallDescription(in_subsequentCallDescription)
    {}

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data (only dummy data)
    * @return true: the invocation matches
    */
    virtual bool matches( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      return !subsequentCallRecorder->hasBeenInvoked();
    }

  /** Increments the invocation count
    * @param  invocation  the invocation data
    */
    virtual void incInvoked( const I &invocation )
    {
      if (!matches(invocation))
        assertionFailed(__LINE__, __FILE__,
                                   MOCKPP_PCHAR("called out of order; should be called before ")
                                     + subsequentCallDescription);
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      buffer += MOCKPP_PCHAR("before ") + subsequentCallDescription;
      if( subsequentCallRecorder->hasBeenInvoked() )
        buffer += MOCKPP_PCHAR(" (invoked)");
      else
        buffer += MOCKPP_PCHAR(" (not invoked)");

      return buffer;
    }

  private:

    InvokedRecorder    *subsequentCallRecorder;
    String              subsequentCallDescription;

};


MOCKPP_NS_END


#endif // MOCKPP_INVOKEDBEFOREMATCHER_H


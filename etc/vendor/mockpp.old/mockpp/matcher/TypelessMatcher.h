/** @file
    @internal
    @brief  Base class for typeless invocation matchers

  $Id: TypelessMatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

 **/

#ifndef MOCKPP_TYPELESSMATCHER_H
#define MOCKPP_TYPELESSMATCHER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/Invocation.h>

#include <mockpp/Verifiable.h>
#include <mockpp/SelfDescribing.h>

#include <mockpp/matcher/InvocationMatcher.h>


MOCKPP_NS_START


/** Base for matchers regardless of method parameter values.
  * @internal
  */
class MOCKPP_API_DECL0 TypelessMatcher : public Verifiable,
                        public SelfDescribing
{
  public:

    typedef AutoPointer<TypelessMatcher> AP;  //!< internal shorthand

  /** Creates the object
    */
    TypelessMatcher();

  /** Destroys the object
    */
    virtual ~TypelessMatcher();

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription() = 0;

  /** Checks if an invocation matches the expectaton
    * @return the description
    */
    virtual bool matches( ) = 0;

  /** Increments the invocation count
    */
    virtual void incInvoked( ) = 0;

  /** Sets all internal data to the state after construction.
    * Only a dummy as matchers don't have data to reset.
    */
    virtual void reset();
};


/** Adapter for the "translation" to typeless matcher objects.
  * @internal
  */
template <typename I>
class TypelessMatcherAdapter : public InvocationMatcher<I>
{
  public:

  /** Creates the object
    * @param  tm  pointer to matcher object
    */
    TypelessMatcherAdapter(TypelessMatcher::AP tm)
      : matcher(tm)
    {
    }

  /** Checks if an invocation matches the expectaton
    * @param  invocation  the invocation data (only dummy data)
    * @return true: the invocation matches
    */
    virtual bool matches( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      return matcher->matches();
    }

  /** Increments the invocation count
    * @param  invocation  the invocation data
    */
    virtual void incInvoked( const I &invocation )
    {
      MOCKPP_UNUSED(invocation);
      matcher->incInvoked();
    }

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      matcher->verify();
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      return matcher->describeTo(buffer);
    }

  /** Does the object provide meaningful description via describeTo() ?.
    * @return true: description is available
    */
    virtual bool hasDescription()
    {
      return matcher->hasDescription();
    }

  private:

    const TypelessMatcher::AP matcher;
};


MOCKPP_NS_END


#endif // MOCKPP_TYPELESSMATCHER_H


/** @file
    @brief  Passes a value back via a reference (Outbound value).

 $Id: TriggeredOutbound.h 1448 2008-05-17 18:13:56Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 01 2006
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

 */

#ifndef MOCKPP_TriggeredOutbound_H
#define MOCKPP_TriggeredOutbound_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/TriggeredConstraint.h>
#include <mockpp/compat/Formatter.h>
#include <mockpp/ReturnObjectList.h>


MOCKPP_NS_START


/** Passes a value back via a reference (Outbound value).
  * The reference is set in method \c eval and filled with the according value
  * in \c trigger. The reference is set to zero immediately after using it.
  * For that reason \c eval and \c trigger always come in pairs to avoid using
  * a dangling pointer from a temporary or deleted variable.
  * @warning Be extremely careful when using \c TriggeredOutbound in conjunction
  * with other contraints using \c And or \c Or as side effects might occur.
  * @ingroup grp_constraint
  */
template <typename T>
class TriggeredOutbound : public TriggeredConstraint<T>
{
  public:

  /** Constructs the object
    * @param retArg   the first value to return
    */
    TriggeredOutbound( const T &retArg )
      : returnObjects(MOCKPP_PCHAR("TriggeredOutbound/returnObjects"), 0)
      , back_ref(0)
    {
      addOutboundObject(retArg);
    }

  /** Constructs the object base on a list of values.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    */
    template <class I>
    TriggeredOutbound( I items, I end )
      : returnObjects(MOCKPP_PCHAR("TriggeredOutbound/returnObjects"), 0)
      , back_ref(0)
    {
      addOutboundObject(items, end);
    }

  /** Adds another return value.
    * @param retArg   the next value to return
    */
    void addOutboundObject( const T &retArg )
    {
      returnObjects.addObjectToReturn(retArg);
    }

  /** Add a sequence of next objects to the end of the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    TriggeredOutbound& addOutboundObject(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addOutboundObject(*items);
      return *this;
    }

  /** Destroys the object
    */
    virtual ~TriggeredOutbound()
    {}

  /** Evaluates the constraint.
    * The value is not actually evaluated but the adresse is stored for later use.
    * @param arg the object which is to pass the value later with \c trigger().
    * @return always \c true
    */
    virtual bool eval( const T &arg ) const
    {
      back_ref = const_cast<T*>(&arg);
      return true;
    }

  /** Evaluates the constraint at the end.
    * @param arg the object which is passed the value.
    * @return true:  All values must have been consumed.
    * @return false:  At least one object is still available
    */
    virtual bool verify( const T & /*arg*/ ) const
    {
      return returnObjects.hasMoreObjects() == 0;
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const
    {
      String fmt = MOCKPP_PCHAR("stores %1");
      fmt << returnObjects.toString();
      buffer += fmt;
      return buffer;
    }

  /** Triggers the defered constraint.
    * Stores the next available object into the variable.
    */
    virtual void trigger()
    {
      MOCKPP_ASSERT_TRUE(back_ref != 0);
     *back_ref = returnObjects.nextReturnObject();
      back_ref = 0; // use reference only ONCE
    }

  private:

    mutable ReturnObjectList<T>   returnObjects;
    mutable T                    *back_ref;
};


MOCKPP_NS_END


#endif // MOCKPP_TriggeredOutbound_H

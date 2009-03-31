/** @file
    @internal
    @brief  Base class for invocations

$Id: Invocation.h 1437 2008-03-08 20:12:53Z ewald-arnold $

***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 24 2004
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

#ifndef MOCKPP_INVOKATION_H
#define MOCKPP_INVOKATION_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/SelfDescribing.h>

#include <mockpp/constraint/ConstraintSet.h>

#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


#ifdef MOCKPP_USE_INVOCATION_EQUALS

/** Default comparison function for the various \c Invocation classes.
  * The default implementation compares the values of the two
  * objects.
  * @see mockpp::Invocation
  * @param left  left operand
  * @param right right operand
  * @return true if both values are equal
  */
template <class T>
bool invocationComparison(const T &left, const T &right)
{
  return left == right;
}

#endif // MOCKPP_USE_INVOCATION_EQUALS


/** Base class for objects holding invocations
  * @internal
  */
class MOCKPP_API_DECL0 InvocationBase : public SelfDescribing
{
  public:

  /** Destroys the object
    */
    virtual ~InvocationBase();
};


/** Hold information about an invocation dispatched
  * to a Mock object.
  * @internal
  */
class MOCKPP_API_DECL0 Invocation : public InvocationBase
{
  public:

  /** Constructs the object
    * @param  in_name  name of he object
    */
    Invocation( const String &in_name);

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The current content of the buffer data
    */
    virtual String describeTo( String &buffer ) const;

  /** Returns the method name
    * @return the method name
    */
    String getMethodName() const;

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const = 0;

  private:

    String    name;
};


/////////////////////////////////////////////////////////////////////


/** Hold invocations without parameters
  * @internal
  */
class MOCKPP_API_DECL0 Invocation0 : public Invocation
{
  public:

    enum { numParams = 0 };

    typedef ConstraintSet0 ConstraintSetType; //!< internal shorthand

  /** Constructs the invocation
    * @param name the method name
    */
    Invocation0(const String &name);

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const Invocation0 &other ) const;

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const;

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOKATION_H

#include "InvocationN.h"


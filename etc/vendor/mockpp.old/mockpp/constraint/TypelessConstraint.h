/** @file
    @internal
    @brief  Base class for typeless argument constraints

  $Id: TypelessConstraint.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_TYPELESSCONSTRAINT_H
#define MOCKPP_TYPELESSCONSTRAINT_H

#include <mockpp/util/AutoPointer.h>

#include <mockpp/mockpp.h> // always first

#include <mockpp/constraint/Constraint.h>


MOCKPP_NS_START


/** A constraint where actual values are ignored.
  * @internal
  */
class MOCKPP_API_DECL0 TypelessConstraint : public SelfDescribing
{
  public:

    typedef AutoPointer<TypelessConstraint> AP;  //!< internal shorthand

  /** Destroys the object
    */
    virtual ~TypelessConstraint();

  /** Evaluates the constraint without actual arguments.
    * @return <code>true</code> if the invocation meets the constraint,
    * @return <code>false</code> if it does not.
    */
    virtual bool typelessEval() const = 0;
};


/** An adapter for the "translation" typeless constrains
  * @internal
  */
template <typename T>
class TypelessConstraintAdapter : public Constraint<T>
{
  public:

  /** Creates the object
    * @param   tc  pointer to the constraint
    */
    TypelessConstraintAdapter(typename TypelessConstraint::AP tc)
      : constraint(tc)
    {
    }

  /** Destroys the object
    */
    virtual ~TypelessConstraintAdapter()
    {
    }

  /** Appends the description of this object to the buffer.
    * @param buffer The buffer that the description is appended to.
    * @return The buffer passed to the object.
    */
    virtual String describeTo( String &buffer ) const
    {
      return constraint->describeTo(buffer);
    }

  /** Evaluates the constraint
    * @param o  The object against which the constraint is evaluated.
	*             Actually only a dummy value for syntactical reasons.
    * @return true:  the underlying expression meets the constraint,
    * @return false if it does not.
    */
    virtual bool eval(const T& o) const
    {
      MOCKPP_UNUSED(o);
      return constraint->typelessEval();
    }

  private:

    const typename TypelessConstraint::AP constraint;
};


MOCKPP_NS_END


#endif // MOCKPP_TYPELESSCONSTRAINT_H

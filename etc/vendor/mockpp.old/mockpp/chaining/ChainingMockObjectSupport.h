/** @file
    @brief  Simplify handling of some constraints

 $Id: ChainingMockObjectSupport.h 1449 2008-05-22 14:57:05Z ewald-arnold $

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

#ifndef MOCKPP_CHAININGMOCKOBJECTSUPPORT_H
#define MOCKPP_CHAININGMOCKOBJECTSUPPORT_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/Throwable.h>

#include <mockpp/util/AutoPointer.h>

#include <mockpp/constraint/And.h>
#include <mockpp/constraint/Or.h>
#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsSame.h>
#include <mockpp/constraint/IsNot.h>
#include <mockpp/constraint/IsCloseTo.h>
#include <mockpp/constraint/StringContains.h>
#include <mockpp/constraint/StringStartsWith.h>
#include <mockpp/constraint/StringEndsWith.h>
#include <mockpp/constraint/IsInstanceOf.h>
#include <mockpp/constraint/IsGreaterThan.h>
#include <mockpp/constraint/IsGreaterOrEqual.h>
#include <mockpp/constraint/IsLessThan.h>
#include <mockpp/constraint/IsLessOrEqual.h>
#include <mockpp/constraint/OutBound.h>

#include <mockpp/chaining/CoreMock.h>

#include <mockpp/stub/ThrowStub.h>
#include <mockpp/stub/ReturnStub.h>
#include <mockpp/stub/RandomStub.h>
#include <mockpp/stub/ReturnObjectListStub.h>

#include <mockpp/matcher/TypelessMatcher.h>


MOCKPP_NS_START

/**
  * @defgroup grp_constraint_abbrev Abbreviated Parameter Constraints
  * @see mockpp::Constraint
  * Provides convenience functions that create contraint objects.
  * Use of these functions should also help to increase readability
  * if you import them via some appriopriate \c using directive.
  */

/**
  * @defgroup grp_stub_abbrev Abbreviated Stubs
  * @see mockpp::Stub
  * Provides convenience functions that create stub objects.
  * Use of these functions should also help to increase readability
  * if you import them via some appriopriate \c using directive.
  */

/**
  * @defgroup grp_matcher_abbrev Abbreviated Matchers
  * @see mockpp::InvocationMatcher
  * Provides convenience functions that create matcher objects.
  * Use of these functions should also help to increase readability
  * if you import them via some appriopriate \c using directive.
  */

/** Creates a constraint that matches any invocation
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsAnything
  * @return the new constraint
  */
MOCKPP_API_DECL(TypelessConstraint::AP) any();


/** Creates a constraint that never matches an invocation
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsNothing
  * @return the new constraint
  */
MOCKPP_API_DECL(TypelessConstraint::AP) nothing();


/** Passes a value back via a reference (outbound value).
  * @ingroup grp_constraint_abbrev
  * @see mockpp::OutBound
  * @param  op  object to return at invoationwith
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
outBound( const T& op )
{
  return new OutBound<T>( op );
}


/** Creates a constraint that tests for equality
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsEqual
  * @param  op  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
eq( const T& op )
{
  return new IsEqual<T>( op );
}


/** Creates a constraint that tests for non-equality
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsNot
  * @param  op  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
ne( const T& op )
{
  return new IsNot<T>( new IsEqual<T>( op ) );
}


/** Creates a constraint that tests for near-equality
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsCloseTo
  * @param  operand     object to compare with
  * @param  deviation   amount of allowed deviation
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
eq( const T &operand, const T &deviation )
{
  return new IsCloseTo<T>( operand, deviation );
}


/** Creates a less-or-equal constraint
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsLessOrEqual
  * @param  value  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
le( const T &value )
{
  return new IsLessOrEqual<T>( value );
}


/** Creates a less-than constraint
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsLessThan
  * @param  value  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
lt( const T &value )
{
  return new IsLessThan<T>( value );
}


/** Creates a greater-than constraint
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsGreaterThan
  * @param  value  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
gt( const T &value )
{
  return new IsGreaterThan<T>( value );
}


/** Creates a greater-or-equal constraint
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsGreaterOrEqual
  * @param  value  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
ge( const T &value )
{
  return new IsGreaterOrEqual<T>( value );
}


/** Creates a constraint that checks if an object is the same
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsSame
  * @param  operand  object to compare with
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
same( const T &operand )
{
  return new IsSame<T>( operand );
}


#ifndef MOCKPP_NO_RTTI

/** Creates a constraint that checks if an object is derived from a base class.
  * The base class is needed since dynamic_cast checks for an existing traversal
  * from \c derived to \c base.
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsInstanceOf
  * @return the new constraint
  */
template <typename ROOT,
          typename DERIVED>
typename Constraint<ROOT*>::AP
isA( const DERIVED & /*operandClass*/ )
{
  return new IsInstanceOf<ROOT, DERIVED>();
}

/** Creates a constraint that checks if an object is derived from a base class.
  * The base class is needed since dynamic_cast checks for an existing traversal
  * from \c derived to \c base.
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsInstanceOf
  * @return the new constraint
  */
template <typename ROOT,
          typename DERIVED>
typename Constraint<ROOT*>::AP
isA()
{
  return new IsInstanceOf<ROOT, DERIVED>();
}

#endif // MOCKPP_NO_RTTI


/** Creates a constraint that checks for the occurence of a substring
  * @ingroup grp_constraint_abbrev
  * @see mockpp::StringContains
  * @param  substring   the substring
  * @return the new constraint
  */
template <typename S>  // MOCKPP_STL::basic_string<char_type>
typename Constraint<S>::AP
stringContains( const S &substring )
{
  return new StringContains<S>( substring );
}


/** Creates a constraint that checks for the occurence of a starting substring
  * @ingroup grp_constraint_abbrev
  * @see mockpp::StringStartsWith
  * @param  substring   the substring
  * @return the new constraint
  */
template <typename S>  // MOCKPP_STL::basic_string<char_type>
typename Constraint<S>::AP
startsWith( const S &substring )
{
  return new StringStartsWith<S>( substring );
}


/** Creates a constraint that checks for the occurence of a trailing substring
  * @ingroup grp_constraint_abbrev
  * @see mockpp::StringEndsWith
  * @param  substring   the substring
  * @return the new constraint
  */
template <typename S>  // MOCKPP_STL::basic_string<char_type>
typename Constraint<S>::AP
endsWith( const S &substring )
{
  return new StringEndsWith<S>( substring );
}


/** Creates a constraint that checks for the occurence of a substring
  * @ingroup grp_constraint_abbrev
  * @see mockpp::StringContains
  * @param  substring   the substring
  * @return the new constraint
  */
template <typename PCS> // pointer to const char_type
typename Constraint<MOCKPP_STL::basic_string<PCS> >::AP
stringContains( const PCS *substring )
{
  return new StringContains<MOCKPP_STL::basic_string<PCS> >( substring );
}


/** Creates a constraint that negates another
  * @ingroup grp_constraint_abbrev
  * @see mockpp::IsNot
  * @param  c  the other constraint
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
logic_not( const ConstraintHolder<T> &c )
{
  return new IsNot<T>( c );
}


/** Creates a constraint to logically-and two other constraints
  * @ingroup grp_constraint_abbrev
  * @see mockpp::And
  * @param  left      the first constraint
  * @param  right     the second constraint
  * @param  shortcut  @li true: don't evaluate \c right if \c left is \c true (c++ standard behaviour)
  *                   @li false: alwyas evaluate \c right even if \c left is \c true
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
logic_and( const ConstraintHolder<T> &left,
           const ConstraintHolder<T> &right,
           bool shortcut = true )
{
  return new And<T>( left, right, shortcut );
}


/** Creates a constraint to logically-or two other constraints
  * @ingroup grp_constraint_abbrev
  * @see mockpp::Or
  * @param  left      the first constraint
  * @param  right     the second constraint
  * @param  shortcut  @li true: don't evaluate \c right if \c left is \c true (c++ standard behaviour)
  *                   @li false: alwyas evaluate \c right even if \c left is \c true
  * @return the new constraint
  */
template <typename T>
typename Constraint<T>::AP
logic_or( const ConstraintHolder<T> &left,
          const ConstraintHolder<T> &right,
          bool shortcut = true )
{
  return new Or<T>( left, right, shortcut );
}


/** Creates a stub returning a value
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnStub
  * @param  o  the value to return
  * @return the new stub
  */
template <typename T>   // Invocation
typename TypelessStub<T>::AP returnValue( const T &o )
{
  return new ReturnStub<T>( o );
}


/** Creates a stub returning a random value
  * @ingroup grp_stub_abbrev
  * @see mockpp::RandomStub
  * @param  max  the maximal value to return. 0 is implicit lowest.
  * @return the new stub
  */
template <typename R, typename T>   // Invocation
typename TypelessStub<R>::AP randomValue( const T &max )
{
  return new RandomStub<R, T>( max );
}


/** Creates a stub returning a random value
  * @ingroup grp_stub_abbrev
  * @see mockpp::RandomStub
  * @param  min  the minimal value to return
  * @param  max  the maximal value to return
  * @return the new stub
  */
template <typename R, typename T>   // Invocation
typename TypelessStub<R>::AP randomValue( const T &min, const T &max )
{
  return new RandomStub<R, T>( min, max );
}


/** Creates a stub returning a value
  * @ingroup grp_stub_abbrev
  * @see mockpp::ReturnObjectListStub
  * @param start    start iterator
  * @param end      terminating iterator (note: one element "behind" as always with STL)
  * @return the new stub
  */
template <typename T,
          typename I >   // Invocation
typename TypelessStub<T>::AP returnValue( I start, I end )
{
  return new ReturnObjectListStub<T>( start, end );
}


/** Creates a stub returning a void stub
  * @ingroup grp_stub_abbrev
  * @see mockpp::VoidStub
  * @return the new stub
  */
MOCKPP_API_DECL(TypelessStub<void>::AP) isVoid( );


/** Creates a stub throwing a value
  * @ingroup grp_stub_abbrev
  * @see mockpp::ThrowStub
  * @param  val  the value to throw
  * @return the new stub
  */
template <typename R,   // ReturnType
          typename T>   // Invocation
typename TypelessStub<R>::AP throwException( const T &val )
{
  return new ThrowStub<R>( make_throwable(val) );
}


/** Creates a matcher to verify a single invocation
  * @ingroup grp_matcher_abbrev
  * @see mockpp::InvokeOnceMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) once();


/** Creates a matcher to verify a minimum invocation count of one.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::InvokeAtLeastOnceMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) atLeastOnce();


/** Creates a matcher to verify an arbitrary invocation count.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::UnlimitedMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) unlimited();


/** Creates a matcher to verify a maximal invocation count.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::InvokeAtMostMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) atMost( int expectedCount );


/** Creates a matcher to verify a minimal invocation count.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::InvokeAtLeastMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) atLeast(int expectedCount);


/** Creates a matcher to verify an exact invocation count.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::InvokeCountMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) exactly( int expectedCount );


/** Creates a matcher to verify that the invactation never occured.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::TestFailureMatcher
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher>) never();


/** Creates a matcher to verify that the invactation never occured.
  * @ingroup grp_matcher_abbrev
  * @see mockpp::TestFailureMatcher
  * @param  errorMessage   additional info for error message
  * @return the new matcher
  */
MOCKPP_API_DECL(AutoPointer<TypelessMatcher> never)( const String &errorMessage );


MOCKPP_NS_END


#include "OnConsecutiveCalls.h"
#include "ReturnValueAndTrigger.h"


/** Imports the abbreviations into the global namespace to enhance redability
  * by omitting namespace prefix.
  */
#ifdef MOCKPP_IMPORT_ABBREVIATED
using MOCKPP_NS::any;
using MOCKPP_NS::nothing;
using MOCKPP_NS::eq;
using MOCKPP_NS::ne;
using MOCKPP_NS::le;
using MOCKPP_NS::ge;
using MOCKPP_NS::gt;
using MOCKPP_NS::lt;
using MOCKPP_NS::same;
#ifndef MOCKPP_NO_RTTI
using MOCKPP_NS::isA;
#endif
using MOCKPP_NS::stringContains;
using MOCKPP_NS::startsWith;
using MOCKPP_NS::endsWith;
using MOCKPP_NS::logic_not;
using MOCKPP_NS::logic_and;
using MOCKPP_NS::logic_or;
using MOCKPP_NS::returnValue;
using MOCKPP_NS::returnValueAndTrigger;
using MOCKPP_NS::trigger;
using MOCKPP_NS::onConsecutiveCalls;
using MOCKPP_NS::throwException;
using MOCKPP_NS::once;
using MOCKPP_NS::atLeastOnce;
using MOCKPP_NS::atMost;
using MOCKPP_NS::atLeast;
using MOCKPP_NS::exactly;
using MOCKPP_NS::never;
using MOCKPP_NS::isVoid;
using MOCKPP_NS::unlimited;
using MOCKPP_NS::outBound;
#endif


#endif // MOCKPP_CHAININGMOCKOBJECTSUPPORT_H

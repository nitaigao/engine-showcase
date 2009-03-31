/** @file
    @internal
    @brief  Build invocation mockers

 $Id: InvocationMockerBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_INVOCATIONBUILDER_H
#define MOCKPP_INVOCATIONBUILDER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/builder/BuilderNamespace.h>
#include <mockpp/builder/IdentityBuilder.h>
#include <mockpp/builder/MatchBuilder.h>

#include <mockpp/stub/StubHolder.h>

#include <mockpp/matcher/ArgumentsMatcher.h>
#include <mockpp/matcher/MatcherHolder.h>

#include <mockpp/chaining/StubMatchersCollection.h>


MOCKPP_NS_START

/** Builder class for invocation related purposes.
  * @ingroup grp_chainer
  * @internal
  */
template <typename AMB>  // ArgumentsMatchBuilder
class InvocationMockerBuilder : public AMB
{
    typedef typename AMB::ReturnType R;
    typedef typename AMB::InvocationType I;
    typedef typename AMB::ConstraintSetType MOCKPP_CS;

  public:

  /** Constructs the invocation mocker builder
    * @param in_mocker           pointer to matcher object
    * @param in_builderNamespace pointer to builder namespace
    * @return
    */
    InvocationMockerBuilder( StubMatchersCollection<R, I> *in_mocker,
                             BuilderNamespace *in_builderNamespace )
      : mocker( in_mocker )
      , builderNamespace( in_builderNamespace )
    {}

  /** Adds another matcher.
    * @param customMatcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I> &match( const MatcherHolder<I> &customMatcher )
    {
      return addMatcher( customMatcher );
    }

  /** Adds another expectation.
    * @param expectation the matcher object
    * @return the builder object
    */
    IdentityBuilder &expect( const MatcherHolder<I> &expectation )
    {
      return addMatcher( expectation );
    }

  /** Sets the identifier for the expectation.
    * @ingroup grp_chainer
    * @param invocationID  identifier for the expectation
    */
    void id( const String &invocationID )
    {
      mocker->setName( invocationID );
      builderNamespace->registerUniqueID( invocationID, new MatchBuilderAdapter<R, I>(this) );
    }

  /** Sets the stub data for the invocation.
    * @param stubAction  the stub object
    */
    virtual void setStub( const StubHolder<R, I> &stubAction )
    {
      mocker->setStub( stubAction );
    }

  protected:

  /** Indicate a matcher for a set of arguments.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I> &with_cs( const MOCKPP_CS &constraintset )
    {
      return internalWith_cs( constraintset);
    }

  /** Indicate a matcher for a set of arguments.
    * Needed for compilers that can't invoke \c InvocationMockerBuilder<>::with_cs() directly.
    * @param constraintset   constraint set
    * @return the builder object
    */
    virtual MatchBuilder<R, I> &internalWith_cs( const MOCKPP_CS &constraintset )
    {
      return addMatcher( new ArgumentsMatcher<I>( constraintset ) );
    }

  /** Gets the builder namespace.
    * @return pointer to builder namspace
    */
    virtual BuilderNamespace *getBuilderNamespace()
    {
      return builderNamespace;
    }

  /** Gets the matcher object
    * @return pointer to the matcher object
    */
    virtual StubMatchersCollection<R, I> *getMocker()
    {
      return mocker;
    }

  private:

  /** Adds another matcher.
    * @param matcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder<R, I> &addMatcher( const MatcherHolder<I> &matcher ) //    virtual InvocationMockerBuilder &addMatcher( const MatcherHolder<I> &matcher )
    {
      mocker->addMatcher( matcher );
      return *this;
    }

    StubMatchersCollection<R, I> *mocker;
    BuilderNamespace             *builderNamespace;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOCATIONBUILDER_H

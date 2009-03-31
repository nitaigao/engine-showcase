/** @file
    @internal
    @brief   Build a match

 $Id: MatchBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_MATCHBUILDER_H
#define MOCKPP_MATCHBUILDER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/compat/AssertionFailedError.h>

#include <mockpp/chaining/StubMatchersCollection.h>

#include <mockpp/builder/StubBuilder.h>
#include <mockpp/builder/BuilderNamespace.h>

#include <mockpp/matcher/InvokedRecorder.h>
#include <mockpp/matcher/InvokedAfterMatcher.h>
#include <mockpp/matcher/InvokedBeforeMatcher.h>
#include <mockpp/matcher/MatcherHolder.h>


MOCKPP_NS_START


/** Base for a helper class for convenient handling of match builders.
  * @internal
  */
class MatchBuilderAdapterBase
{
  public:

  /** Returns the internal builder
    * @return pointer to builder
    */
    virtual void *getBuilder() const = 0;

  /** Destructs the adapter
    */
    virtual ~MatchBuilderAdapterBase()
    {}

  /** Adds another match to the builder
    * @param    customMatcher   pointer to matcher object
    */
    virtual void match( TypelessMatcher::AP customMatcher ) = 0;
};


/** Builder class for matcher related purposes.
  * @ingroup grp_chainer
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class MatchBuilder : public StubBuilder<R, I>
{
  public:

    typedef AutoPointer<MatchBuilder<R, I> > AP;  //!< internal shorthand

  /** Adds another matcher.
    * @ingroup grp_chainer
    * @param customMatcher the matcher object
    * @return the builder object
    */
    virtual MatchBuilder &match( const MatcherHolder<I> &customMatcher ) = 0;

  /** Adds a matcher indicating subsequent calls on the same mock object.
    * @ingroup grp_chainer
    * @param priorCallID identifier of the prior call
    * @return the builder object
    */
    MatchBuilder<R, I>& after( const String &priorCallID )
    {
      this->setupOrderingAfterMatchers( *getBuilderNamespace(), priorCallID, priorCallID );
      return *this;
    }

  /** Adds a matcher indicating prior calls on the same mock object.
    * @ingroup grp_chainer
    * @param subsequentCallID identifier of the prior call on another object
    * @return the builder object
    */
    MatchBuilder<R, I>& before( const String &subsequentCallID )
    {
      this->setupOrderingBeforeMatchers( *getBuilderNamespace(), subsequentCallID, subsequentCallID );
      return *this;
    }

  /** Adds a matcher indicating subsequent calls compared to another mock object.
    * @ingroup grp_chainer
    * @param otherMock   pointer to the other mock object
    * @param priorCallID identifier of the prior call on another object
    * @return the builder object
    */
    MatchBuilder<R, I>& after( BuilderNamespace &otherMock, const String &priorCallID )
    {
      this->setupOrderingAfterMatchers( otherMock, priorCallID, priorCallID + MOCKPP_PCHAR( " on " ) + otherMock.getIdent() );
      return *this;
    }

  /** Adds a matcher indicating prior calls compared to another mock object.
    * @ingroup grp_chainer
    * @param otherMock        pointer to the other mock object
    * @param subsequentCallID identifier of the prior call on another object
    * @return the builder object
    */
    MatchBuilder<R, I>& before( BuilderNamespace &otherMock, const String &subsequentCallID )
    {
      this->setupOrderingBeforeMatchers( otherMock, subsequentCallID, subsequentCallID + MOCKPP_PCHAR( " on " ) + otherMock.getIdent() );
      return *this;
    }

  protected:

  /** Gets the builder namespace.
    * @return pointer to builder namspace
    */
    virtual BuilderNamespace *getBuilderNamespace() = 0;

  /** Gets the matcher object
    * @return pointer to the matcher object
    */
    virtual StubMatchersCollection<R, I> * getMocker() = 0;

  private:

  /** Adds a matcher indicating subsequent calls compared to another mock object.
    * @param idTable              pointer to the other mock object
    * @param priorCallID          identifier of the prior call on another object
    * @param priorCallDescription description for the action
    * @return the builder object
    */
    void setupOrderingAfterMatchers( BuilderNamespace &idTable,
                                     const String &priorCallID,
                                     const String &priorCallDescription )
    {
      MatchBuilderAdapterBase * priorCallBuilder = idTable.lookupID( priorCallID );

      typename InvokedRecorder::AP priorCallRecorder ( new InvokedRecorder );

      getMocker() ->addMatcher( new InvokedAfterMatcher<I>( priorCallRecorder.get(),
                                priorCallDescription ) );
      if (priorCallBuilder != 0)
#if defined(_MSC_VER) && (_MSC_VER <= 1300)
        priorCallBuilder->match( (TypelessMatcher*)priorCallRecorder.release() );
#else
        priorCallBuilder->match(priorCallRecorder);
#endif
      else
        idTable.addPendingRecorder(priorCallID, priorCallRecorder);
    }

  /** Adds a matcher indicating prior calls compared to another mock object.
    * @param idTable                   pointer to the other mock object
    * @param subsequentCallID          identifier of the prior call on another object
    * @param subsequentCallDescription description for the action
    * @return the builder object
    */
    void setupOrderingBeforeMatchers( BuilderNamespace &idTable,
                                      const String &subsequentCallID,
                                      const String &subsequentCallDescription )
    {
      MatchBuilderAdapterBase * subsequentCallBuilder = idTable.lookupID( subsequentCallID );

      typename InvokedRecorder::AP subsequentCallRecorder ( new InvokedRecorder );

      getMocker() ->addMatcher( new InvokedBeforeMatcher<I>( subsequentCallRecorder.get(),
                                subsequentCallDescription ) );
      if (subsequentCallBuilder != 0)
        subsequentCallBuilder->match( subsequentCallRecorder );
      else
        idTable.addPendingRecorder(subsequentCallID, subsequentCallRecorder);
    }
};


/** A helper class for convenient handling of match builders.
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class MatchBuilderAdapter : public MatchBuilderAdapterBase
{
  public:

  /** Constructs a match builder adapter
    * @param   in_builder        pointer to actual match builder
    * @param   in_shouldDelete   true: delete in_builder in destructor
    */
    MatchBuilderAdapter(MatchBuilder<R, I> *in_builder, bool in_shouldDelete = false)
      : builder(in_builder)
      , shouldDelete(in_shouldDelete)
    {}

  /** Destructs the adapter
    */
    virtual ~MatchBuilderAdapter()
    {
      if (shouldDelete)
        delete builder;
      builder = 0;
    }

  /** Adds another match to the builder
    * @param    customMatcher   pointer to matcher object
    */
    virtual void match( typename TypelessMatcher::AP customMatcher )
    {
      builder->match (new TypelessMatcherAdapter<I>(customMatcher));
    }

  /** Returns the internal builder
    * @return pointer to builder
    */
    virtual void *getBuilder() const
    {
      return builder;
    }

  private:

    MatchBuilder<R, I> *builder;
    bool                shouldDelete;
};


MOCKPP_NS_END


#endif // MOCKPP_MATCHBUILDER_H

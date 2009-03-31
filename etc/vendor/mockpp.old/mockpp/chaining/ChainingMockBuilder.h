/** @file
    @brief  Build chaining mock objects

    $Id: ChainingMockBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_CHAININGMOCKBUILDER_H
#define MOCKPP_CHAININGMOCKBUILDER_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_MAP_H

#include <mockpp/chaining/DynamicChainingMock.h>
#include <mockpp/chaining/InvocationMocker.h>

#include <mockpp/builder/BuilderNamespace.h>
#include <mockpp/builder/MatchBuilder.h>
#include <mockpp/builder/InvocationMockerBuilder.h>

#include <mockpp/matcher/MatcherHolder.h>


MOCKPP_NS_START


/** Build chaining mocks
  * @ingroup grp_chainer
  */
template <typename AMB>    // ArgumentsMatchBuilder
class ChainingMockBuilder : public DynamicChainingMock < typename AMB::ReturnType,
                                                         typename AMB::InvocationType >
{
    typedef typename AMB::ReturnType     R;  //!< internal shorthand
    typedef typename AMB::InvocationType I;  //!< internal shorthand

  public:

  /** Construct the object.
    * @param in_coreMock              pointer to core mock
    * @param in_buildernamespace      builder names
    * @param name                     mock name
    * @return return value from mocked invocation
    */
    ChainingMockBuilder( DynamicChainingMock<R, I> *in_coreMock,
                         BuilderNamespace *in_buildernamespace,
                         const String &name /*, VerifiableList *parent = 0*/ )
      : DynamicChainingMock<R, I>(name /*, parent*/)
      , coreMock( in_coreMock )
      , buildernamespace(in_buildernamespace)
    {}

  /** Transform the object's state to a human readable string.
    * @return string representation
    */
    String toString() const
    {
      return coreMock->toString();
    }

  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      coreMock->verify();
    }

  /** Adds an invokable to the mock object
    * @param invokable  pointer to the default stub
    */
    virtual void addInvokable( typename Invokable<R, I>::AP invokable )
    {
      coreMock->addInvokable( invokable );
    }

  /** Stubs an invocation.
    * @ingroup grp_chainer
    * @return the temporary builder object
    */
    InvocationMockerBuilder<AMB>& stubs()
    {
      return addNewInvocationMocker();
    }

  /** Stubs an invocation.
    * @ingroup grp_chainer
    * @param customMatcher   pointer to matcher data
    * @return the temporary builder object
    */
    InvocationMockerBuilder<AMB>& stubs( const MatcherHolder<I> &customMatcher )
    {
      return expects(customMatcher); // @todo call expects??
    }

  /** Expects an invocation.
    * @ingroup grp_chainer
    * @param expectation   pointer to matcher data
    * @return the temporary builder object
    */
    InvocationMockerBuilder<AMB>& expects( const MatcherHolder<I> &expectation )
    {
      InvocationMockerBuilder<AMB>& builder = addNewInvocationMocker();
      builder.match( expectation );
      return builder;
    }

  /** Sets the default stub for the object
    * @ingroup grp_chainer
    * @param newDefaultStub  pointer to the default stub
    */
    virtual void setDefaultStub( const MOCKPP_NS::StubHolder<R, I> &newDefaultStub )
    {
      coreMock->setDefaultStub( newDefaultStub );
    }

  /** Clears all internal states
    */
    void reset()
    {
      buildernamespace->reset();
      coreMock->reset();
    }

  /** Searches a match builder.
    * @param id    name of the builder
    * @return the match builder
    */
    MatchBuilderAdapterBase *lookupID( const String &id ) const
    {
      MatchBuilderAdapterBase *tmb = buildernamespace->lookupID(id);
      if ( tmb == 0 )
        assertionFailed( __LINE__, __FILE__,
                                    MOCKPP_PCHAR( "no expected invocation named '" )
                                    + id
                                    + MOCKPP_PCHAR( "'" ) );
      return tmb;
    }

  /** Registers a match builder.
    * @param id       name of the builder
    * @param builder  the match builder
    */
    void registerUniqueID( const String &id, MatchBuilderAdapterBase *builder )
    {
      buildernamespace->registerUniqueID(id, builder);
    }

  private:

  /** Adds a new invocation mocker
    * @return the temporary builder object
    */
    InvocationMockerBuilder<AMB>& addNewInvocationMocker()
    {
      InvocationMocker<R, I> *mocker =
        new InvocationMocker<R, I>( new typename InvocationMocker<R, I>::DefaultDescriber() );
      addInvokable( mocker ); // owner of mocker!

      InvocationMockerBuilder<AMB> *builder = new InvocationMockerBuilder<AMB>( mocker, buildernamespace );
      buildernamespace->storeBuilder(new MatchBuilderAdapter<R, I>(builder, true));
      return *builder;
    }

    DynamicChainingMock<R, I>      *coreMock;
    BuilderNamespace               *buildernamespace;
};


MOCKPP_NS_END


#endif // MOCKPP_CHAININGMOCKBUILDER_H


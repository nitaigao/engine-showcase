/***************************************************************************
         MyMatchBuilder.h  -  helper classes for common use in tests
                             -------------------
    begin                : Wed Nov 17 2004
    copyright            : (C) 2002-2008 by Ewald Arnold
    email                : mockpp at ewald-arnold dot de

   $Id: MyMatchBuilder.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/


template <typename R,    // Returntype
          typename I>    // Invocation
class MyStubMatchersCollection : public MOCKPP_NS::StubMatchersCollection<R, I>
{
  public:

    virtual ~MyStubMatchersCollection()
    {}

    virtual void setName( const MOCKPP_NS::String &/*name*/ )
    {}

    virtual void addMatcher( const MOCKPP_NS::MatcherHolder<I> &matcher )
    {
      last_matcher = matcher;
    }

    virtual void setStub( const MOCKPP_NS::StubHolder<R, I> &stub )
    {
      last_stub = stub;
    }

    typename MOCKPP_NS::InvocationMatcher<I>::AP last_matcher;
    typename MOCKPP_NS::Stub<R, I>::AP            last_stub;
};


template <typename R,   // Returntype
          typename I>   // Invocation
class MyMatchBuilder : public MOCKPP_NS::MatchBuilder<R, I>
{
  public:

    virtual ~MyMatchBuilder()
    {}

    virtual void id( const MOCKPP_NS::String &/*id*/ )
    {}

    virtual MOCKPP_NS::MatchBuilder<R, I> &match( const MOCKPP_NS::MatcherHolder<I> &customMatcher )
    {
      last_match = customMatcher;
      return *this;
    }

    virtual void setStub( const MOCKPP_NS::StubHolder<R, I> &/*stubAction*/ )
    {}

    virtual MOCKPP_NS::BuilderNamespace * getBuilderNamespace()
    {
      return & bn;
    }

    virtual MOCKPP_NS::StubMatchersCollection<R, I> * getMocker()
    {
      return & mocker;
    }

    typename MOCKPP_NS::InvocationMatcher<I>::AP last_match;

    MyStubMatchersCollection<R, I> mocker;
    MyBuilderNamespace bn;
};

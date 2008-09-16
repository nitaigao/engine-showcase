/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_consecutive_N.pl.

  $Id: OnConsecutiveCalls.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Apr 7 2006
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

#ifndef MOCKPP_ONCONSECUTIVECALLS_H
#define MOCKPP_ONCONSECUTIVECALLS_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/stub/StubSequence.h>
#include <mockpp/stub/TypelessStubSequence.h>


MOCKPP_NS_START


/** Creates a sequence of 1 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 1 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 1 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 1 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 2 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 2 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 2 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 2 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 3 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 3 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 3 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 3 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 4 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 4 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 4 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 4 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 5 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 5 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 5 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 5 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 6 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                           , AutoPointer<Stub<R, I> > stub6
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 6 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                           , Stub<R, I> *stub6
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 6 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                                , AutoPointer<TypelessStub<R> > stub6
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 6 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                                , TypelessStub<R> *stub6
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 7 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                           , AutoPointer<Stub<R, I> > stub6
                                           , AutoPointer<Stub<R, I> > stub7
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 7 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                           , Stub<R, I> *stub6
                                           , Stub<R, I> *stub7
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 7 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                                , AutoPointer<TypelessStub<R> > stub6
                                                , AutoPointer<TypelessStub<R> > stub7
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 7 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                                , TypelessStub<R> *stub6
                                                , TypelessStub<R> *stub7
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 8 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                           , AutoPointer<Stub<R, I> > stub6
                                           , AutoPointer<Stub<R, I> > stub7
                                           , AutoPointer<Stub<R, I> > stub8
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 8 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                           , Stub<R, I> *stub6
                                           , Stub<R, I> *stub7
                                           , Stub<R, I> *stub8
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 8 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                                , AutoPointer<TypelessStub<R> > stub6
                                                , AutoPointer<TypelessStub<R> > stub7
                                                , AutoPointer<TypelessStub<R> > stub8
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 8 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                                , TypelessStub<R> *stub6
                                                , TypelessStub<R> *stub7
                                                , TypelessStub<R> *stub8
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 9 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                           , AutoPointer<Stub<R, I> > stub6
                                           , AutoPointer<Stub<R, I> > stub7
                                           , AutoPointer<Stub<R, I> > stub8
                                           , AutoPointer<Stub<R, I> > stub9
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  vec.push_back( stub9.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 9 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                           , Stub<R, I> *stub6
                                           , Stub<R, I> *stub7
                                           , Stub<R, I> *stub8
                                           , Stub<R, I> *stub9
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  vec.push_back( stub9);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 9 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                                , AutoPointer<TypelessStub<R> > stub6
                                                , AutoPointer<TypelessStub<R> > stub7
                                                , AutoPointer<TypelessStub<R> > stub8
                                                , AutoPointer<TypelessStub<R> > stub9
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  vec.push_back( stub9.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 9 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                                , TypelessStub<R> *stub6
                                                , TypelessStub<R> *stub7
                                                , TypelessStub<R> *stub8
                                                , TypelessStub<R> *stub9
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  vec.push_back( stub9);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 10 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @param  stub10  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  AutoPointer<Stub<R, I> > stub1
                                           , AutoPointer<Stub<R, I> > stub2
                                           , AutoPointer<Stub<R, I> > stub3
                                           , AutoPointer<Stub<R, I> > stub4
                                           , AutoPointer<Stub<R, I> > stub5
                                           , AutoPointer<Stub<R, I> > stub6
                                           , AutoPointer<Stub<R, I> > stub7
                                           , AutoPointer<Stub<R, I> > stub8
                                           , AutoPointer<Stub<R, I> > stub9
                                           , AutoPointer<Stub<R, I> > stub10
                                          )
{
  MOCKPP_STL::vector<Stub<R, I>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  vec.push_back( stub9.release());
  vec.push_back( stub10.release());
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 10 stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::StubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @param  stub10  the next stub
  * @return the new stub
  */
template <typename R,  // ReturnType
          typename I>  // Invocation
typename Stub<R, I>::AP onConsecutiveCalls(  Stub<R, I> *stub1
                                           , Stub<R, I> *stub2
                                           , Stub<R, I> *stub3
                                           , Stub<R, I> *stub4
                                           , Stub<R, I> *stub5
                                           , Stub<R, I> *stub6
                                           , Stub<R, I> *stub7
                                           , Stub<R, I> *stub8
                                           , Stub<R, I> *stub9
                                           , Stub<R, I> *stub10
                                          )
{
  MOCKPP_STL::vector<Stub<I, R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  vec.push_back( stub9);
  vec.push_back( stub10);
  return new StubSequence<R, I>( vec.begin(), vec.end() );
}


/** Creates a sequence of 10 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @param  stub10  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  AutoPointer<TypelessStub<R> > stub1
                                                , AutoPointer<TypelessStub<R> > stub2
                                                , AutoPointer<TypelessStub<R> > stub3
                                                , AutoPointer<TypelessStub<R> > stub4
                                                , AutoPointer<TypelessStub<R> > stub5
                                                , AutoPointer<TypelessStub<R> > stub6
                                                , AutoPointer<TypelessStub<R> > stub7
                                                , AutoPointer<TypelessStub<R> > stub8
                                                , AutoPointer<TypelessStub<R> > stub9
                                                , AutoPointer<TypelessStub<R> > stub10
                                               )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1.release());
  vec.push_back( stub2.release());
  vec.push_back( stub3.release());
  vec.push_back( stub4.release());
  vec.push_back( stub5.release());
  vec.push_back( stub6.release());
  vec.push_back( stub7.release());
  vec.push_back( stub8.release());
  vec.push_back( stub9.release());
  vec.push_back( stub10.release());
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


/** Creates a sequence of 10 typeless stubs.
  * @ingroup grp_stub_abbrev
  * @see mockpp::TypelessStubSequence
  * @param  stub1  first stub
  * @param  stub2  the next stub
  * @param  stub3  the next stub
  * @param  stub4  the next stub
  * @param  stub5  the next stub
  * @param  stub6  the next stub
  * @param  stub7  the next stub
  * @param  stub8  the next stub
  * @param  stub9  the next stub
  * @param  stub10  the next stub
  * @return the new stub
  */
template <typename R>  // ReturnType
typename TypelessStub<R>::AP onConsecutiveCalls(  TypelessStub<R> *stub1
                                                , TypelessStub<R> *stub2
                                                , TypelessStub<R> *stub3
                                                , TypelessStub<R> *stub4
                                                , TypelessStub<R> *stub5
                                                , TypelessStub<R> *stub6
                                                , TypelessStub<R> *stub7
                                                , TypelessStub<R> *stub8
                                                , TypelessStub<R> *stub9
                                                , TypelessStub<R> *stub10
                                          )
{
  MOCKPP_STL::vector<TypelessStub<R>*> vec;
  vec.push_back( stub1);
  vec.push_back( stub2);
  vec.push_back( stub3);
  vec.push_back( stub4);
  vec.push_back( stub5);
  vec.push_back( stub6);
  vec.push_back( stub7);
  vec.push_back( stub8);
  vec.push_back( stub9);
  vec.push_back( stub10);
  return new TypelessStubSequence<R>( vec.begin(), vec.end() );
}


MOCKPP_NS_END


#endif // MOCKPP_ONCONSECUTIVECALLS_H


/** @file
    @internal
    @brief  Invocations last-in-first-out

 $Id: LIFOInvocationDispatcher.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_LIFOINVOCATIONDISPATCHER_H
#define MOCKPP_LIFOINVOCATIONDISPATCHER_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/AbstractInvocationDispatcher.h>


MOCKPP_NS_START


/** Dispatch invokables Last-In-First-Out
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class LIFOInvocationDispatcher : public AbstractInvocationDispatcher<R, I>
{
  protected:

    typedef typename AbstractInvocationDispatcher<R, I>::ReverseIterator Iterator;  //!< internal shorthand

  /** Dispatches the invokable
    * @param invocation  the invocation data
    */
    virtual R dispatch( const I &invocation )
    {
      Iterator it = this->invokables.rbegin();
      Iterator end = this->invokables.rend();

      for (  /**/; it != end; ++it )
      {
        if ( ( *it ) ->matches( invocation ) )
          return ( *it ) ->invoke( invocation );
      }

      return this->defaultStub->invoke( invocation );
    }
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation to dispatch void invokables Last-In-First-Out
  * @internal
  */
template <typename I>   // Invocation
class LIFOInvocationDispatcher<void, I> : public AbstractInvocationDispatcher<void, I>
{
  protected:

    typedef typename AbstractInvocationDispatcher<void, I>::ReverseIterator Iterator;  //!< internal shorthand

  /** Dispatches the invokable
    * @param invocation  the invocation data
    */
    virtual void dispatch( const I &invocation )
    {
      Iterator it = this->invokables.rbegin();
      Iterator end = this->invokables.rend();
      for (  /**/; it != end; ++it )
      {
        if ( ( *it ) ->matches( invocation ) )
        {
          ( *it ) ->invoke( invocation );
          return;
        }
      }

      this->defaultStub->invoke( invocation );
    }
};


#define MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_IMPL(I)
#define MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_DECL(I)


#else // MOCKPP_PTI_WEAKNESS Partial Template Instantiation Weakness


#define MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_DECL(I) \
template<> \
void MOCKPP_NS::LIFOInvocationDispatcher<void, I >::dispatch( const I &invocation );


#define MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_IMPL(I) \
MOCKPP_NS_START \
template<> \
class LIFOInvocationDispatcher<void, I> : public AbstractInvocationDispatcher<void, I> \
{ \
 protected: \
  typedef AbstractInvocationDispatcher<void, I>::ReverseIterator Iterator; \
  \
  void dispatch( const I &invocation )  \
  { \
    Iterator it = this->invokables.rbegin(); \
    Iterator end = this->invokables.rend(); \
    for (  /**/; it != end; ++it ) \
    { \
      if ( ( *it ) ->matches( invocation ) ) \
      { \
        ( *it ) ->invoke( invocation ); \
        return; \
      } \
    } \
  \
    this->defaultStub->invoke( invocation ); \
  } \
}; \
MOCKPP_NS_END


#endif


MOCKPP_NS_END

/** @def MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_DECL
  * Declare a specialized class for MOCKPP_NS::LIFOInvocationDispatcher.
  * Neccessary for compilers with weak template capabilities.
  */

/** @def MOCKPP_LIFOINVOCATIONDISPATCHER_PTI_IMPL
  * Implement a specialized class for MOCKPP_NS::LIFOInvocationDispatcher.
  * Neccessary for compilers with weak template capabilities.
  */

#endif // MOCKPP_LIFOINVOCATIONDISPATCHER_H


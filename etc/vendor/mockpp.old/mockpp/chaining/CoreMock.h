/** @file
    @internal
    @brief  The core of a chaining mock object

 $Id: CoreMock.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_COREMOCK_H
#define MOCKPP_COREMOCK_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/chaining/AbstractDynamicChainingMock.h>


MOCKPP_NS_START


/** Helper class for chaining mocks.
  * @internal
  */
template <typename R,   // Returntype
          typename I>   // Invocation
class CoreMock : public AbstractDynamicChainingMock<R, I>
{
  public:

  /** Construct the object.
    * @param in_invocationDispatcher  pointer to invocation dispatcher
    * @param name                     mock name
    * @param parent                   pointer to parent verifiable
    * @return return value from mocked invocation
    */
    CoreMock( typename InvocationDispatcher<R, I>::AP in_invocationDispatcher,
              const String &name,
              VerifiableList *parent)
      : AbstractDynamicChainingMock<R, I>( in_invocationDispatcher, name, parent )
    {}

  /** Construct the object.
    * @param  name                 mock name
    * @param  parent               pointer to parent verifiable
    * @return return value from mocked invocation
    */
    CoreMock( const String &name, VerifiableList *parent)
      : AbstractDynamicChainingMock<R, I>( name, parent )
    {}

  /** Mock the invocation
    * @param invocation  invocation data
    * @return return value from mocked invocation
    */
    R invoke( const I &invocation )
    {
      return mockInvocation( invocation );
    }
};


#ifndef MOCKPP_PTI_WEAKNESS // Partial Template Instantiation Weakness


/** Partial specialisation of a helper chaining mock for void methods.
  * @internal
  */
template <typename I>   // Invocation
class CoreMock<void, I> : public AbstractDynamicChainingMock<void, I>
{
  public:

  /** Construct the object.
    * @param  in_invocationDispatcher  pointer to invocation dispatcher
    * @param  name                     mock name
    * @param  parent                   pointer to parent verifiable
    * @return return value from mocked invocation
    */
    CoreMock( typename InvocationDispatcher<void, I>::AP in_invocationDispatcher,
              const String &name,
              VerifiableList *parent)
      : AbstractDynamicChainingMock<void, I>( in_invocationDispatcher, name, parent )
    {}

  /** Construct the object.
    * @param  name                 mock name
    * @param  parent               pointer to parent verifiable
    * @return return value from mocked invocation
    */
    CoreMock( const String &name, VerifiableList *parent)
      : AbstractDynamicChainingMock<void, I>( name, parent)
    {}

  /** Mock the invocation
    * @param invocation  invocation data
    */
    void invoke( const I &invocation )
    {
      mockInvocation( invocation );
    }
};


#define MOCKPP_COREMOCK_PTI_IMPL(I)
#define MOCKPP_COREMOCK_PTI_DECL(I)


#else // MOCKPP_PTI_WEAKNESS Partial Template Instantiation Weakness


#define MOCKPP_COREMOCK_PTI_DECL(I) \
template<> \
void MOCKPP_NS::CoreMock<void, I >;


#define MOCKPP_COREMOCK_PTI_IMPL(I) \
MOCKPP_NS_START \
template<> \
class CoreMock<void, I> : public AbstractDynamicChainingMock<void, I> \
{ \
 public: \
  CoreMock<void, I>( InvocationDispatcher<void, I>::AP in_invocationDispatcher, \
                     const String &name, \
                     VerifiableList *parent) \
   : AbstractDynamicChainingMock<void, I>( in_invocationDispatcher, name, parent ) \
  {} \
  \
  CoreMock<void, I>( const String &name, VerifiableList *parent) \
      : AbstractDynamicChainingMock<void, I>( name, parent) \
  {} \
  \
  void invoke( const I &invocation ) \
  { \
    mockInvocation( invocation ); \
  } \
}; \
MOCKPP_NS_END

#endif


MOCKPP_NS_END

/** @def MOCKPP_COREMOCK_PTI_DECL
  * Declare a specialized class for MOCKPP_NS::CoreMock.
  * Neccessary for compilers with weak template capabilities.
  */

/** @def MOCKPP_COREMOCK_PTI_IMPL
  * Implement a specialized class for MOCKPP_NS::CoreMock.
  * Neccessary for compilers with weak template capabilities.
  */

#endif // MOCKPP_COREMOCK_H

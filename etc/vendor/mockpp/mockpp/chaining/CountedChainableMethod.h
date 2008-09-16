/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_countchainable_N.pl.

  $Id: CountedChainableMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sun Oct 30 2005
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

#ifndef MOCKPP_COUNTEDCHAINABLEMETHOD_H
#define MOCKPP_COUNTEDCHAINABLEMETHOD_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/CountParameters.h>

#include <mockpp/chaining/ChainableMockMethod0.h>
#include <mockpp/chaining/ChainableMockMethod1.h>
#include <mockpp/chaining/ChainableMockMethod2.h>
#include <mockpp/chaining/ChainableMockMethod3.h>
#include <mockpp/chaining/ChainableMockMethod4.h>
#include <mockpp/chaining/ChainableMockMethod5.h>
#include <mockpp/chaining/ChainableMockMethod6.h>


MOCKPP_NS_START


#ifndef MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with meta programming.
  * @internal
  */
template<unsigned params>
class SwitchChainable
{
};


/** Helper class to select the method mock with 0 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<0>
{
  public:

  /** Helper class with an embeded type to the the method mock with 0 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod0<RT> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 1 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<1>
{
  public:

  /** Helper class with an embeded type to the the method mock with 1 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod1<RT, P1> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 2 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<2>
{
  public:

  /** Helper class with an embeded type to the the method mock with 2 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod2<RT, P1, P2> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 3 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<3>
{
  public:

  /** Helper class with an embeded type to the the method mock with 3 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod3<RT, P1, P2, P3> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 4 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<4>
{
  public:

  /** Helper class with an embeded type to the the method mock with 4 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod4<RT, P1, P2, P3, P4> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 5 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<5>
{
  public:

  /** Helper class with an embeded type to the the method mock with 5 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod5<RT, P1, P2, P3, P4, P5> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 6 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchChainable<6>
{
  public:

  /** Helper class with an embeded type to the the method mock with 6 parameters.
    */
    template<  typename RT
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef ChainableMockMethod6<RT, P1, P2, P3, P4, P5, P6> ChainableMockMethodType; //!< internal shorthand
    };
};


/** Method mock class to select the actual class with the desired number of parameters.
  * Works with meta programming.
  */
template<   typename RT
          , typename P1 = NoParameter
          , typename P2 = NoParameter
          , typename P3 = NoParameter
          , typename P4 = NoParameter
          , typename P5 = NoParameter
          , typename P6 = NoParameter>
class ChainableMockMethod
  : public SwitchChainable<CountParameters<RT, P1, P2, P3, P4, P5, P6>::value >
#ifdef MOCKPP_NO_TEMPLATE_HINT
             ::Method<RT, P1, P2, P3, P4, P5, P6>
#else
             ::template Method<RT, P1, P2, P3, P4, P5, P6>
#endif
               ::ChainableMockMethodType
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod(const String &name, ChainableMockObject *parent )
      : SwitchChainable<CountParameters<RT, P1, P2, P3, P4, P5, P6>::value >
#ifdef MOCKPP_NO_TEMPLATE_HINT
          ::Method<RT, P1, P2, P3, P4, P5, P6>
#else
          ::template Method<RT, P1, P2, P3, P4, P5, P6>
#endif
            ::ChainableMockMethodType(name, parent)
    {
    }
};


#else // MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with partial specialisation.
  * @internal
  */
template<   typename RT
          , typename P1 = NoParameter
          , typename P2 = NoParameter
          , typename P3 = NoParameter
          , typename P4 = NoParameter
          , typename P5 = NoParameter
          , typename P6 = NoParameter
          , typename P7 = NoParameter> // one more parameter to enabled full specialisation
class ChainableMockMethod
{
};


/** Method mock class to select the actual class with 0 parameters.
  * Works with partial specialisation.
  */
template<typename RT>
class ChainableMockMethod<RT, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
  : public ChainableMockMethod0<RT>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod0<RT>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 1 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1>
class ChainableMockMethod<RT, P1, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
  : public ChainableMockMethod1<RT, P1>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod1<RT, P1>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 2 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1, typename P2>
class ChainableMockMethod<RT, P1, P2, NoParameter, NoParameter, NoParameter, NoParameter>
  : public ChainableMockMethod2<RT, P1, P2>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, P2, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod2<RT, P1, P2>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 3 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1, typename P2, typename P3>
class ChainableMockMethod<RT, P1, P2, P3, NoParameter, NoParameter, NoParameter>
  : public ChainableMockMethod3<RT, P1, P2, P3>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, P2, P3, NoParameter, NoParameter, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod3<RT, P1, P2, P3>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 4 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1, typename P2, typename P3, typename P4>
class ChainableMockMethod<RT, P1, P2, P3, P4, NoParameter, NoParameter>
  : public ChainableMockMethod4<RT, P1, P2, P3, P4>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, P2, P3, P4, NoParameter, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod4<RT, P1, P2, P3, P4>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 5 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
class ChainableMockMethod<RT, P1, P2, P3, P4, P5, NoParameter>
  : public ChainableMockMethod5<RT, P1, P2, P3, P4, P5>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, P2, P3, P4, P5, NoParameter>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod5<RT, P1, P2, P3, P4, P5>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 6 parameters.
  * Works with partial specialisation.
  */
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ChainableMockMethod<RT, P1, P2, P3, P4, P5, P6>
  : public ChainableMockMethod6<RT, P1, P2, P3, P4, P5, P6>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod<RT, P1, P2, P3, P4, P5, P6>
          (const String &name, ChainableMockObject *parent )
      : ChainableMockMethod6<RT, P1, P2, P3, P4, P5, P6>(name, parent)
    {
    }
};


#endif // MOCKPP_COUNTED_WEAKNESS


MOCKPP_NS_END


#endif // MOCKPP_COUNTEDCHAINABLEMETHOD_H


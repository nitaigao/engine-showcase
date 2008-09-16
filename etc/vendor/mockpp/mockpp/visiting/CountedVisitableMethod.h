/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_countvisitable.pl.

  $Id: CountedVisitableMethod.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_COUNTEDVISITABLEMETHOD_H
#define MOCKPP_COUNTEDVISITABLEMETHOD_H

#include <mockpp/CountParameters.h>

#include <mockpp/visiting/VisitableMockMethod0.h>
#include <mockpp/visiting/VisitableMockMethod1.h>
#include <mockpp/visiting/VisitableMockMethod2.h>
#include <mockpp/visiting/VisitableMockMethod3.h>
#include <mockpp/visiting/VisitableMockMethod4.h>
#include <mockpp/visiting/VisitableMockMethod5.h>
#include <mockpp/visiting/VisitableMockMethod6.h>


MOCKPP_NS_START


#ifndef MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with meta programming.
  * @internal
  */
template<unsigned params>
class SwitchVisitable
{
};


/** Helper class to select the method mock with 0 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<0>
{
  public:

  /** Helper class with an embeded type to the the method mock with 0 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod0<R> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 1 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<1>
{
  public:

  /** Helper class with an embeded type to the the method mock with 1 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod1<R, P1> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 2 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<2>
{
  public:

  /** Helper class with an embeded type to the the method mock with 2 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod2<R, P1, P2> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 3 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<3>
{
  public:

  /** Helper class with an embeded type to the the method mock with 3 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod3<R, P1, P2, P3> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 4 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<4>
{
  public:

  /** Helper class with an embeded type to the the method mock with 4 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod4<R, P1, P2, P3, P4> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 5 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<5>
{
  public:

  /** Helper class with an embeded type to the the method mock with 5 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod5<R, P1, P2, P3, P4, P5> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Helper class to select the method mock with 6 parameters.
  * Works with meta programming.
  * @internal
  */
template<>
class SwitchVisitable<6>
{
  public:

  /** Helper class with an embeded type to the the method mock with 6 parameters.
    */
    template<  typename R
             , typename P1
             , typename P2
             , typename P3
             , typename P4
             , typename P5
             , typename P6>
    class Method
    {
      public:

        typedef VisitableMockMethod6<R, P1, P2, P3, P4, P5, P6> VisitableMockMethodType; //!< internal shorthand
    };
};


/** Method mock class to select the actual class with the desired number of parameters.
  * Works with meta programming.
  */
template<   typename R
          , typename P1 = NoParameter
          , typename P2 = NoParameter
          , typename P3 = NoParameter
          , typename P4 = NoParameter
          , typename P5 = NoParameter
          , typename P6 = NoParameter>
class VisitableMockMethod
  : public SwitchVisitable<CountParameters<R, P1, P2, P3, P4, P5, P6>::value>
#ifdef MOCKPP_NO_TEMPLATE_HINT
             ::Method<R, P1, P2, P3, P4, P5, P6>
#else
             ::template Method<R, P1, P2, P3, P4, P5, P6>
#endif
               ::VisitableMockMethodType
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod(const String &name, VisitableMockObject *parent )
      : SwitchVisitable<CountParameters<R, P1, P2, P3, P4, P5, P6>::value >
#ifdef MOCKPP_NO_TEMPLATE_HINT
          ::Method<R, P1, P2, P3, P4, P5, P6>
#else
          ::template Method<R, P1, P2, P3, P4, P5, P6>
#endif
            ::VisitableMockMethodType(name, parent)
    {
    }
};


#else // MOCKPP_COUNTED_WEAKNESS


/** Helper class to select the method mock with the correct number of parameters.
  * Works with partial specialisation.
  * @internal
  */
template<   typename R
          , typename P1 = NoParameter
          , typename P2 = NoParameter
          , typename P3 = NoParameter
          , typename P4 = NoParameter
          , typename P5 = NoParameter
          , typename P6 = NoParameter
          , typename P7 = NoParameter>
class VisitableMockMethod
{
};


/** Method mock class to select the actual class with 0 parameters.
  * Works with partial specialisation.
  */
template<typename R>
class VisitableMockMethod<R, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
  : public VisitableMockMethod0<R>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod0<R>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 1 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1>
class VisitableMockMethod<R, P1, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
  : public VisitableMockMethod1<R, P1>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, NoParameter, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod1<R, P1>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 2 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1, typename P2>
class VisitableMockMethod<R, P1, P2, NoParameter, NoParameter, NoParameter, NoParameter>
  : public VisitableMockMethod2<R, P1, P2>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, P2, NoParameter, NoParameter, NoParameter, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod2<R, P1, P2>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 3 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1, typename P2, typename P3>
class VisitableMockMethod<R, P1, P2, P3, NoParameter, NoParameter, NoParameter>
  : public VisitableMockMethod3<R, P1, P2, P3>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, P2, P3, NoParameter, NoParameter, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod3<R, P1, P2, P3>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 4 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1, typename P2, typename P3, typename P4>
class VisitableMockMethod<R, P1, P2, P3, P4, NoParameter, NoParameter>
  : public VisitableMockMethod4<R, P1, P2, P3, P4>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, P2, P3, P4, NoParameter, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod4<R, P1, P2, P3, P4>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 5 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
class VisitableMockMethod<R, P1, P2, P3, P4, P5, NoParameter>
  : public VisitableMockMethod5<R, P1, P2, P3, P4, P5>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, P2, P3, P4, P5, NoParameter>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod5<R, P1, P2, P3, P4, P5>(name, parent)
    {
    }
};


/** Method mock class to select the actual class with 6 parameters.
  * Works with partial specialisation.
  */
template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class VisitableMockMethod<R, P1, P2, P3, P4, P5, P6>
  : public VisitableMockMethod6<R, P1, P2, P3, P4, P5, P6>
{
  public:

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    VisitableMockMethod<R, P1, P2, P3, P4, P5, P6>
          (const String &name, VisitableMockObject *parent )
      : VisitableMockMethod6<R, P1, P2, P3, P4, P5, P6>(name, parent)
    {
    }
};


#endif // MOCKPP_COUNTED_WEAKNESS


MOCKPP_NS_END


#endif // MOCKPP_COUNTEDVISITABLEMETHOD_H


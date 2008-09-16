/** @file
    @internal NOT INTENDED FOR PUBLIC INCLUSION
    @brief    Generated with gen_invocation_N.pl.

  $Id: InvocationN.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Aug 24 2004
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

#ifndef MOCKPP_INVOCATION_N_H
#define MOCKPP_INVOCATION_N_H

//#include <mockpp/chaining/Invocation.h>


MOCKPP_NS_START



/** Hold invocations with 1 parameters
  * @internal
  */
template <typename P1>
class Invocation1 : public Invocation
{
  public:

    enum { numParams = 1 };

    typedef ConstraintSet1<P1> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    */
    Invocation1(  const String &objname
                , const P1 &in_param1)
      : Invocation( objname )
      , param1( in_param1 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation1<P1> &other ) const
    {
      return invocationComparison(param1, other.param1);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1");
      fmt << param1;
      return fmt;
    }

  private:

    const P1 & param1;
};


/** Hold invocations with 2 parameters
  * @internal
  */
template <typename P1, typename P2>
class Invocation2 : public Invocation
{
  public:

    enum { numParams = 2 };

    typedef ConstraintSet2<P1, P2> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand
    typedef P2 T2Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    * @param in_param2  parameter 2
    */
    Invocation2(  const String &objname
                , const P1 &in_param1
                , const P2 &in_param2)
      : Invocation( objname )
      , param1( in_param1 )
      , param2( in_param2 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation2<P1, P2> &other ) const
    {
      return invocationComparison(param1, other.param1)
             && invocationComparison(param2, other.param2);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 2
    */
    const P2 & getParameter2() const
    {
      return param2;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1, %2");
      fmt << param1
          << param2;
      return fmt;
    }

  private:

    const P1 & param1;
    const P2 & param2;
};


/** Hold invocations with 3 parameters
  * @internal
  */
template <typename P1, typename P2, typename P3>
class Invocation3 : public Invocation
{
  public:

    enum { numParams = 3 };

    typedef ConstraintSet3<P1, P2, P3> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand
    typedef P2 T2Type;   //!< internal shorthand
    typedef P3 T3Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    * @param in_param2  parameter 2
    * @param in_param3  parameter 3
    */
    Invocation3(  const String &objname
                , const P1 &in_param1
                , const P2 &in_param2
                , const P3 &in_param3)
      : Invocation( objname )
      , param1( in_param1 )
      , param2( in_param2 )
      , param3( in_param3 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation3<P1, P2, P3> &other ) const
    {
      return invocationComparison(param1, other.param1)
             && invocationComparison(param2, other.param2)
             && invocationComparison(param3, other.param3);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 2
    */
    const P2 & getParameter2() const
    {
      return param2;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 3
    */
    const P3 & getParameter3() const
    {
      return param3;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1, %2, %3");
      fmt << param1
          << param2
          << param3;
      return fmt;
    }

  private:

    const P1 & param1;
    const P2 & param2;
    const P3 & param3;
};


/** Hold invocations with 4 parameters
  * @internal
  */
template <typename P1, typename P2, typename P3, typename P4>
class Invocation4 : public Invocation
{
  public:

    enum { numParams = 4 };

    typedef ConstraintSet4<P1, P2, P3, P4> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand
    typedef P2 T2Type;   //!< internal shorthand
    typedef P3 T3Type;   //!< internal shorthand
    typedef P4 T4Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    * @param in_param2  parameter 2
    * @param in_param3  parameter 3
    * @param in_param4  parameter 4
    */
    Invocation4(  const String &objname
                , const P1 &in_param1
                , const P2 &in_param2
                , const P3 &in_param3
                , const P4 &in_param4)
      : Invocation( objname )
      , param1( in_param1 )
      , param2( in_param2 )
      , param3( in_param3 )
      , param4( in_param4 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation4<P1, P2, P3, P4> &other ) const
    {
      return invocationComparison(param1, other.param1)
             && invocationComparison(param2, other.param2)
             && invocationComparison(param3, other.param3)
             && invocationComparison(param4, other.param4);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 2
    */
    const P2 & getParameter2() const
    {
      return param2;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 3
    */
    const P3 & getParameter3() const
    {
      return param3;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 4
    */
    const P4 & getParameter4() const
    {
      return param4;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1, %2, %3, %4");
      fmt << param1
          << param2
          << param3
          << param4;
      return fmt;
    }

  private:

    const P1 & param1;
    const P2 & param2;
    const P3 & param3;
    const P4 & param4;
};


/** Hold invocations with 5 parameters
  * @internal
  */
template <typename P1, typename P2, typename P3, typename P4, typename P5>
class Invocation5 : public Invocation
{
  public:

    enum { numParams = 5 };

    typedef ConstraintSet5<P1, P2, P3, P4, P5> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand
    typedef P2 T2Type;   //!< internal shorthand
    typedef P3 T3Type;   //!< internal shorthand
    typedef P4 T4Type;   //!< internal shorthand
    typedef P5 T5Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    * @param in_param2  parameter 2
    * @param in_param3  parameter 3
    * @param in_param4  parameter 4
    * @param in_param5  parameter 5
    */
    Invocation5(  const String &objname
                , const P1 &in_param1
                , const P2 &in_param2
                , const P3 &in_param3
                , const P4 &in_param4
                , const P5 &in_param5)
      : Invocation( objname )
      , param1( in_param1 )
      , param2( in_param2 )
      , param3( in_param3 )
      , param4( in_param4 )
      , param5( in_param5 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation5<P1, P2, P3, P4, P5> &other ) const
    {
      return invocationComparison(param1, other.param1)
             && invocationComparison(param2, other.param2)
             && invocationComparison(param3, other.param3)
             && invocationComparison(param4, other.param4)
             && invocationComparison(param5, other.param5);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 2
    */
    const P2 & getParameter2() const
    {
      return param2;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 3
    */
    const P3 & getParameter3() const
    {
      return param3;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 4
    */
    const P4 & getParameter4() const
    {
      return param4;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 5
    */
    const P5 & getParameter5() const
    {
      return param5;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1, %2, %3, %4, %5");
      fmt << param1
          << param2
          << param3
          << param4
          << param5;
      return fmt;
    }

  private:

    const P1 & param1;
    const P2 & param2;
    const P3 & param3;
    const P4 & param4;
    const P5 & param5;
};


/** Hold invocations with 6 parameters
  * @internal
  */
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class Invocation6 : public Invocation
{
  public:

    enum { numParams = 6 };

    typedef ConstraintSet6<P1, P2, P3, P4, P5, P6> ConstraintSetType;   //!< internal shorthand

    typedef P1 T1Type;   //!< internal shorthand
    typedef P2 T2Type;   //!< internal shorthand
    typedef P3 T3Type;   //!< internal shorthand
    typedef P4 T4Type;   //!< internal shorthand
    typedef P5 T5Type;   //!< internal shorthand
    typedef P6 T6Type;   //!< internal shorthand

  /** Constructs the object
    * @param  objname   name of the object
    * @param in_param1  parameter 1
    * @param in_param2  parameter 2
    * @param in_param3  parameter 3
    * @param in_param4  parameter 4
    * @param in_param5  parameter 5
    * @param in_param6  parameter 6
    */
    Invocation6(  const String &objname
                , const P1 &in_param1
                , const P2 &in_param2
                , const P3 &in_param3
                , const P4 &in_param4
                , const P5 &in_param5
                , const P6 &in_param6)
      : Invocation( objname )
      , param1( in_param1 )
      , param2( in_param2 )
      , param3( in_param3 )
      , param4( in_param4 )
      , param5( in_param5 )
      , param6( in_param6 )
    {
    }

#ifdef MOCKPP_USE_INVOCATION_EQUALS

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    virtual bool equals( const Invocation6<P1, P2, P3, P4, P5, P6> &other ) const
    {
      return invocationComparison(param1, other.param1)
             && invocationComparison(param2, other.param2)
             && invocationComparison(param3, other.param3)
             && invocationComparison(param4, other.param4)
             && invocationComparison(param5, other.param5)
             && invocationComparison(param6, other.param6);
    }

  /** Check if the object equals another invocation
    * @param other  the other invocation
    * @return  true: objects are equal
    */
    bool equals( const InvocationBase &other ) const
    {
      MOCKPP_UNUSED(other);
      return false;
    }

#endif // MOCKPP_USE_INVOCATION_EQUALS

  /** Returns an invocation parameter
    * @return  the invocation parameter 1
    */
    const P1 & getParameter1() const
    {
      return param1;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 2
    */
    const P2 & getParameter2() const
    {
      return param2;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 3
    */
    const P3 & getParameter3() const
    {
      return param3;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 4
    */
    const P4 & getParameter4() const
    {
      return param4;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 5
    */
    const P5 & getParameter5() const
    {
      return param5;
    }

  /** Returns an invocation parameter
    * @return  the invocation parameter 6
    */
    const P6 & getParameter6() const
    {
      return param6;
    }

  /** Returns a description of the parameters
    * @return the description
    */
    virtual String describeParameters() const
    {
      String fmt = MOCKPP_PCHAR("%1, %2, %3, %4, %5, %6");
      fmt << param1
          << param2
          << param3
          << param4
          << param5
          << param6;
      return fmt;
    }

  private:

    const P1 & param1;
    const P2 & param2;
    const P3 & param3;
    const P4 & param4;
    const P5 & param5;
    const P6 & param6;
};


MOCKPP_NS_END


#endif // MOCKPP_INVOCATION_N_H


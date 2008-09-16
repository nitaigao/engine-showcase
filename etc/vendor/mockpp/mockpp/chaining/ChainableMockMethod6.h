/** @file
    @brief  Chainable Mock Method with 6 parameters.
            Generated with gen_chainablemethod_N.pl.

  $Id: ChainableMockMethod6.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Oct 2 2005
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

#ifndef MOCKPP_ChainableMockMethod6_H
#define MOCKPP_ChainableMockMethod6_H

#include <mockpp/mockpp.h>

#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainableMockMethod.h>


MOCKPP_NS_START


/** Common stuff to set up chainable mock method expectations with 6 parameters.
  * @ingroup grp_chainer
  * @internal
  */
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ChainableMockMethod6Common
  : public ChainableMockMethodCommon
  , public ChainingMockBuilder <ArgumentsMatchBuilder6<RT, Invocation6<P1, P2, P3, P4, P5, P6> > >
{
  public:

    typedef Invocation6<P1, P2, P3, P4, P5, P6>                InvocationType;  //!< internal shorthand

    typedef CoreMock<RT, InvocationType>                      CoreMockType;   //!< internal shorthand
    typedef RT                                                ReturnType;     //!< internal shorthand

    typedef ChainingMockBuilder <ArgumentsMatchBuilder6<ReturnType, InvocationType> > Builder; //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod6Common(const String &name, ChainableMockObject *parent )
      : ChainableMockMethodCommon(name, parent)
      , Builder(&coremock, this->getChainableMockObject(), this->getMethodName())
      , coremock(name + MOCKPP_PCHAR("/coreMock"), this)
    {
    }

  protected:

  /** Returns the underlying coremock.
    * @return reference to the mock object
    */
    CoreMockType & getCoreMock() const
    {
      return coremock;
    }

  private:

    mutable CoreMockType    coremock;
};


/** Set up a chainable mock method expectations with 6 parameters.
  * @ingroup grp_chainer
  */
template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ChainableMockMethod6
  : public ChainableMockMethod6Common<RT, P1, P2, P3, P4, P5, P6>
{
  public:

    typedef typename ChainableMockMethod6Common<RT, P1, P2, P3, P4, P5, P6>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod6(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod6Common<RT, P1, P2, P3, P4, P5, P6>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param p5 mock method parameter 5
    * @param p6 mock method parameter 6
    * @return the calculated value
    */
    RT forward(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6) const
    {
      InvocationType inv(this->getMethodName(), p1, p2, p3, p4, p5, p6);
      return this->getCoreMock().invoke(inv);
    }
};


/** Set up a chainable mock method expectations with 6 parameters.
  * Partial specialisation for a void return value.
  * @ingroup grp_chainer
  */
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ChainableMockMethod6<void, P1, P2, P3, P4, P5, P6>
  : public ChainableMockMethod6Common<void, P1, P2, P3, P4, P5, P6>
{
  public:
 
    typedef typename ChainableMockMethod6Common<void, P1, P2, P3, P4, P5, P6>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod6(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod6Common<void, P1, P2, P3, P4, P5, P6>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @param p3 mock method parameter 3
    * @param p4 mock method parameter 4
    * @param p5 mock method parameter 5
    * @param p6 mock method parameter 6
    */
    void forward(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5, const P6 &p6) const
    {
      InvocationType inv(this->getMethodName(), p1, p2, p3, p4, p5, p6);
      this->getCoreMock().invoke(inv);
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ChainableMockMethod6_H


/** @file
    @brief  Chainable Mock Method with 2 parameters.
            Generated with gen_chainablemethod_N.pl.

  $Id: ChainableMockMethod2.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ChainableMockMethod2_H
#define MOCKPP_ChainableMockMethod2_H

#include <mockpp/mockpp.h>

#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainableMockMethod.h>


MOCKPP_NS_START


/** Common stuff to set up chainable mock method expectations with 2 parameters.
  * @ingroup grp_chainer
  * @internal
  */
template <typename RT, typename P1, typename P2>
class ChainableMockMethod2Common
  : public ChainableMockMethodCommon
  , public ChainingMockBuilder <ArgumentsMatchBuilder2<RT, Invocation2<P1, P2> > >
{
  public:

    typedef Invocation2<P1, P2>                InvocationType;  //!< internal shorthand

    typedef CoreMock<RT, InvocationType>                      CoreMockType;   //!< internal shorthand
    typedef RT                                                ReturnType;     //!< internal shorthand

    typedef ChainingMockBuilder <ArgumentsMatchBuilder2<ReturnType, InvocationType> > Builder; //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod2Common(const String &name, ChainableMockObject *parent )
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


/** Set up a chainable mock method expectations with 2 parameters.
  * @ingroup grp_chainer
  */
template <typename RT, typename P1, typename P2>
class ChainableMockMethod2
  : public ChainableMockMethod2Common<RT, P1, P2>
{
  public:

    typedef typename ChainableMockMethod2Common<RT, P1, P2>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod2(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod2Common<RT, P1, P2>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    * @return the calculated value
    */
    RT forward(const P1 &p1, const P2 &p2) const
    {
      InvocationType inv(this->getMethodName(), p1, p2);
      return this->getCoreMock().invoke(inv);
    }
};


/** Set up a chainable mock method expectations with 2 parameters.
  * Partial specialisation for a void return value.
  * @ingroup grp_chainer
  */
template <typename P1, typename P2>
class ChainableMockMethod2<void, P1, P2>
  : public ChainableMockMethod2Common<void, P1, P2>
{
  public:
 
    typedef typename ChainableMockMethod2Common<void, P1, P2>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod2(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod2Common<void, P1, P2>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @param p2 mock method parameter 2
    */
    void forward(const P1 &p1, const P2 &p2) const
    {
      InvocationType inv(this->getMethodName(), p1, p2);
      this->getCoreMock().invoke(inv);
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ChainableMockMethod2_H


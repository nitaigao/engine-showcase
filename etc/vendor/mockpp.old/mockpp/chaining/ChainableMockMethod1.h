/** @file
    @brief  Chainable Mock Method with 1 parameters.
            Generated with gen_chainablemethod_N.pl.

  $Id: ChainableMockMethod1.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_ChainableMockMethod1_H
#define MOCKPP_ChainableMockMethod1_H

#include <mockpp/mockpp.h>

#include <mockpp/chaining/ChainableMockObject.h>
#include <mockpp/chaining/ChainableMockMethod.h>


MOCKPP_NS_START


/** Common stuff to set up chainable mock method expectations with 1 parameters.
  * @ingroup grp_chainer
  * @internal
  */
template <typename RT, typename P1>
class ChainableMockMethod1Common
  : public ChainableMockMethodCommon
  , public ChainingMockBuilder <ArgumentsMatchBuilder1<RT, Invocation1<P1> > >
{
  public:

    typedef Invocation1<P1>                InvocationType;  //!< internal shorthand

    typedef CoreMock<RT, InvocationType>                      CoreMockType;   //!< internal shorthand
    typedef RT                                                ReturnType;     //!< internal shorthand

    typedef ChainingMockBuilder <ArgumentsMatchBuilder1<ReturnType, InvocationType> > Builder; //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod1Common(const String &name, ChainableMockObject *parent )
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


/** Set up a chainable mock method expectations with 1 parameters.
  * @ingroup grp_chainer
  */
template <typename RT, typename P1>
class ChainableMockMethod1
  : public ChainableMockMethod1Common<RT, P1>
{
  public:

    typedef typename ChainableMockMethod1Common<RT, P1>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod1(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod1Common<RT, P1>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    * @return the calculated value
    */
    RT forward(const P1 &p1) const
    {
      InvocationType inv(this->getMethodName(), p1);
      return this->getCoreMock().invoke(inv);
    }
};


/** Set up a chainable mock method expectations with 1 parameters.
  * Partial specialisation for a void return value.
  * @ingroup grp_chainer
  */
template <typename P1>
class ChainableMockMethod1<void, P1>
  : public ChainableMockMethod1Common<void, P1>
{
  public:
 
    typedef typename ChainableMockMethod1Common<void, P1>::InvocationType   InvocationType;  //!< internal shorthand

  /** Constructs the mock object.
    * @param name    human readable description about the expectation
    * @param parent  parent chainable mock object
    */
    ChainableMockMethod1(const String &name, ChainableMockObject *parent )
      : ChainableMockMethod1Common<void, P1>(name, parent)
    {
    }

  /** Actually verifies the mocked method.
    * Must be called by the client code.
    * @param p1 mock method parameter 1
    */
    void forward(const P1 &p1) const
    {
      InvocationType inv(this->getMethodName(), p1);
      this->getCoreMock().invoke(inv);
    }
};


MOCKPP_NS_END


#endif // MOCKPP_ChainableMockMethod1_H


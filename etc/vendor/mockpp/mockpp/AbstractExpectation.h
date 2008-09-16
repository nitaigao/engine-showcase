/** @file
    @brief    Base class for expectations

  $Id: AbstractExpectation.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Thu Dec 19 2002
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

#ifndef MOCKPP_ABSTRACTEXPECTATION_H
#define MOCKPP_ABSTRACTEXPECTATION_H

#include <mockpp/mockpp.h> // always first

#include <mockpp/Expectation.h>
#include <mockpp/compat/Asserter.h>
#include <mockpp/compat/Formatter.h>


MOCKPP_NS_START


class VerifiableList;

/** Base class for all expectations.
  */
template <class T>
class AbstractExpectation : public Expectation
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    AbstractExpectation(const String &name, VerifiableList *parent)
     : Expectation(name, parent)
    {
      clearFailOnVerify();
      clearHasExpectations();
    }


  /**
    * Tests if any expectations have been set on this object.
    * @return true: there are expectations
    */
    bool hasExpectations() const
    {
      return myHasExpectations;
    }


  /**
    * If an incorrect actual value is set, defer reporting this as a failure until verify()
    * is called on this object.
    */
    void setFailOnVerify()
    {
      myFailureModeIsImmediate = false;
    }


  /**
    * If an incorrect actual value is set, report this immediately
    */
    void clearFailOnVerify()
    {
      myFailureModeIsImmediate = true;
    }


  /**
    * Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual() = 0;


  /**
    * Verifies the contained expectation.
    * If this fails an AssertionFailedError is thrown.
    */
    virtual void verify() = 0;

  protected:

  /**
    * Asserts that two values are equal.
    * @param message         a human readable description about the cause
    * @param expectedValue   the expected value
    * @param actualValue     the actual value
    */
    void assertEquals(
        const String &message,
        const T &expectedValue,
        const T &actualValue) const
    {
      if (!hasExpectations() )
        return;

      if (expectedValue == actualValue)
        return;

      if (message.length() == 0)
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 assertion failed.\nExpected: %2\nReceived: %3"));
        fmt << getVerifiableName() << expectedValue << actualValue;
        MOCKPP_FAIL(fmt);
      }
      else
        MOCKPP_FAIL(message);
    }


  /**
    * Tests if any actuals should be checked immediately when set.
    * Otherwise they are checked when verify() is called.
    * @return true: check immediately
    */
    bool shouldCheckImmediately() const
    {
      return myFailureModeIsImmediate && myHasExpectations;
    }


  /**
    * Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    void setHasExpectations()
    {
      myHasExpectations = true;
    }

  /** Tells the object having no expectations yet.
    */
    void clearHasExpectations()
    {
      myHasExpectations = false;
    }

  private:

    bool     myFailureModeIsImmediate;
    bool     myHasExpectations;
};


MOCKPP_NS_END


#endif // MOCKPP_ABSTRACTEXPECTATION_H

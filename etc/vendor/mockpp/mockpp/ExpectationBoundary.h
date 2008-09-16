/** @file
    @brief    Check for expected value boundaries

  $Id: ExpectationBoundary.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 **************************************************************************/

/**************************************************************************

   begin                : Thu Dec 17 2002
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

#ifndef MOCKPP_EXPECTATIONBOUNDARY_H
#define MOCKPP_EXPECTATIONBOUNDARY_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>
#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


/** A class to verify that an actual value lies between two boundary values.
  * Since it uses operator<() and operator==() for verification
  * it can handle all objects that implement those operators not just numerics.
  * \ingroup grp_basic_mo
  */
template <class T>
class ExpectationBoundary : public AbstractExpectation<T>
{
  private:

    T    lower_bound;
    T    upper_bound;
    T    actualValue;
    bool haveActualValue;
    bool expectNothing;

  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationBoundary(const String &name, VerifiableList *parent = 0)
     : AbstractExpectation<T>(name, parent),
       expectNothing(false)
    {
        clearActual();
    }


  /** Sets the actual value.
    * @param value  the actual value
    */
    void setActual(const T &value )
    {
      actualValue = value;
      haveActualValue = true;
      if (this->shouldCheckImmediately())
      {
        verify();
      }
    }


  /** Sets the expectation value boundaries.
    * @param lower  lowest value
    * @param upper  highest value
    */
    void setExpected(const T &lower, const T &upper )
    {
      MOCKPP_ASSERT_TRUE(lower <= upper);
      lower_bound = lower;
      upper_bound = upper;
      expectNothing = false;
      this->setHasExpectations();
    }


#if MOCKPP_BOUNDARY_DELTA != 0

  /** Sets the expectation value boundaries.
    * Differs from the normal setExpected() method only in the parameters.
    * Since this method is not activated by default, you must add the
    * following lines to your source code before \#include'ing ExpectationBoundery.h
    * to activate it.
    * <pre>
    *  \#undef  MOCKPP_BOUNDARY_DELTA  // after inclusion of mockpp.h!
    *  \#define MOCKPP_BOUNDARY_DELTA 1
    * </pre>
    * Additionally operator+() and operator-() must exist for your
    * expected objects, that's the reason why it is disabled by default.
    * @param median   expected value
    * @param delta    allowed difference range
    */
    void setExpectedDelta(const T &median, const T &delta )
    {
      T lower = median - delta;
      T upper = median + delta;
      if (lower <= upper)
        setExpected(lower, upper);
      else
        setExpected(upper, lower);
    }
#endif


  /**
    * Resets the internal state completely.
    */
    virtual void reset()
    {
      this->clearFailOnVerify();
      clearActual();
      clearExpectation();
      expectNothing = false;
    }


  /**
    * Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual()
    {
      haveActualValue = false;
      lower_bound = 0;
      upper_bound = 0;
      actualValue = 0;
    }


  /**
    * Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    virtual void setExpectNothing()
    {
      expectNothing = true;
      clearActual();
      this->setHasExpectations();
    }


  /** Verify that the expected value is within the allowed bounds.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      if( expectNothing )
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
        fmt << this->getVerifiableName();

        MOCKPP_ASSERT_FALSE_MESSAGE(fmt, haveActualValue );
      }
      else
      {
        if (!this->hasExpectations() )
          return;

        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected a value."));
        fmt << this->getVerifiableName();
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, haveActualValue );

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 received a value of %2 which is less than the expected value of %3."));
        fmt << this->getVerifiableName() << actualValue << lower_bound;
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, lower_bound <= actualValue );

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 received a value of %2 which is greater than the expected value of %3."));
        fmt << this->getVerifiableName() << actualValue << upper_bound;
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, actualValue <= upper_bound);
      }
    }

  protected:

  /**
    * Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      this->clearHasExpectations();
    }
};


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONBOUNDARY_H

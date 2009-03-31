/** @file
    @brief    Check for expected values

  $Id: ExpectationValue.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

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

#ifndef MOCKPP_EXPECTATIONVALUE_H
#define MOCKPP_EXPECTATIONVALUE_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>

#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


/** A class to verify the equality of expected and actual value.
  * Attention: This class is not intended for float types! It just does not work!
  *            Use ExpectationBoundary instead.
  * \ingroup grp_basic_mo
  */
template <class T>
class ExpectationValue : public AbstractExpectation<T>
{
  public:


  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationValue(const String &name, VerifiableList *parent = 0)
     : AbstractExpectation<T>(name, parent),
       expectNothing(false),
       haveActualValue(false)
    {
    }


  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual()
    {
        haveActualValue = false;
    }


  /** Resets the internal state completely.
    */
    virtual void reset()
    {
      this->clearFailOnVerify();
      clearActual();
      clearExpectation();
    }


  /** Sets the actual value.
    * @param value  the actual value
    */
    void setActual(const T &value)
    {
      myActualValue = value;
      haveActualValue = true;
      if (this->shouldCheckImmediately())
      {
        verify();
      }
    }


  /** Sets the expectation value.
    * @param value  target value
    */
    void setExpected(const T &value)
    {
        myExpectedValue = value;
        expectNothing = false;
        this->setHasExpectations();
    }


  /** Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    virtual void setExpectNothing()
    {
        expectNothing = true;
        this->setHasExpectations();
    }


  /** Verify that the expected and actual value are equal.
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

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive the expected value. %2 != %3."));
        fmt << this->getVerifiableName() << myExpectedValue << myActualValue;

        this->assertEquals(fmt, myExpectedValue, myActualValue);
      }
    }

  protected:

  /** Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      expectNothing = false;
      this->clearHasExpectations();
    }

  private:

    T    myExpectedValue;
    T    myActualValue;
    bool expectNothing;
    bool haveActualValue;
};


#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef MOCKP_NO_PREVENT_FLOAT_EXPECTATION

// Hopefully prevent people with this from using these dangerous
// template instantiations:
//
// ==> 1/3 is not the same as 0.3333333 so the following classes won't work!

/** Dummy
  */
template<>
class ExpectationValue<float>
{
  public:
  /** Dummy
    */
    void dummy();

  private:
    ExpectationValue<float>();
    void setExpected(const float &aValue);
};

/** Dummy
  */
template<>
class ExpectationValue<double>
{
  public:
  /** Dummy
    */
    void dummy();

  private:
    ExpectationValue<double>();
    void setExpected(const double &aValue);
};

/** Dummy
  */
template<>
class ExpectationValue<long double>
{
  public:
  /** Dummy
    */
    void dummy();

  private:
    ExpectationValue<long double>();
    void setExpected(const long double &aValue);
};


#endif // MOCKP__NO_PREVENT_FLOAT_EXPECTATION

#endif // DOXYGEN_SHOULD_SKIP_THIS

MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONVALUE_H

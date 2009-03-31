/** @file
    @brief    Check list of expectations

  $Id: ExpectationList.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Dec 21 2002
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

#ifndef MOCKPP_EXPECTATIONLIST_H
#define MOCKPP_EXPECTATIONLIST_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H
#include MOCKPP_VECTOR_H

#include <mockpp/AbstractExpectationCollection.h>
#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


/** A class to verify lists of expectations, one after the other as they occur.
  * \ingroup grp_basic_mo
  */
template <class T>
class ExpectationList : public AbstractExpectationCollection<T>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationList(const String &name, VerifiableList *parent = 0)
      : AbstractExpectationCollection<T>(name, parent),
        expectNothing(false),
        haveActualValue(false)
    {
    }


  /**
    * Verify that the expected values equal the expected ones.
    * Note: in a list there can be any number of elements of a given value and the order
    * of the elements must match exactly.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      if( expectNothing)
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

        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, haveActualValue);

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive the expected item list."));
        fmt << this->getVerifiableName();

        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, actualItems == expectedItems);
      }
    }


  /** Adds another actual value to the list.
    * @param actualItem  new value
    */
    void addActual(const T &actualItem)
    {
      actualItems.push_back(actualItem);
      haveActualValue = true;

      if (this->shouldCheckImmediately())
        checkImmediateValue(actualItem);
    }


  /** Adds a sequence of actual values to the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    */
    template <class I>
    void addActual(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addActual(*items);
    }


  /** Adds another expectation value to the list.
    * @param expectedItem  new value
    * @return reference to itself for chaining
    */
    ExpectationList& addExpected(const T &expectedItem)
    {
      expectedItems.push_back(expectedItem);
      expectNothing = false;
      this->setHasExpectations();
      return *this;
    }


  /** Copy one item from expectation list to actuals at same position.
    * Useful after recovering from an error at another place.
    */
    void balanceActual()
    {
      if (actualItems.size() < expectedItems.size())
      {
        haveActualValue = true;
        actualItems.push_back(expectedItems[actualItems.size()]);
      }
    }


  /** Adds a sequence of expectation values to the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ExpectationList& addExpected(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addExpected(*items);
      return *this;
    }


  /**
    * Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual()
    {
      haveActualValue = false;
      actualItems.clear();
    }


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
    * Returns the amount of expectation values.
    * @return number of expectation values
    */
    unsigned size() const
    {
      return expectedItems.size();
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
      clearExpectation();
      this->setHasExpectations();
    }


  protected:

  /**
    * Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      this->clearHasExpectations();
      expectedItems.clear();
    }


  /**
    * Checks if the actual value matches the expectation.
    * Note: in a list there can be any number of elements of a given value and the order
    * of the elements must match exactly.
    * If it fails, an AssertionFailedError is thrown
    */
    void checkImmediateValue(const T &actualItem) const
    {
      unsigned itemsize = actualItems.size();
      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 had different item sizes.\n")
                        MOCKPP_PCHAR("Expected %2 items but received %3 when adding %4."));
      fmt << this->getVerifiableName() << expectedItems.size() << itemsize << actualItem;

      MOCKPP_ASSERT_TRUE_MESSAGE(fmt, expectedItems.size() >= itemsize);

      fmt = mockpp_i18n(MOCKPP_PCHAR("%1 added item[%2] does not match. %3 != %4."));
      fmt << this->getVerifiableName() << itemsize << expectedItems[(itemsize - 1)] << actualItem;

      this->assertEquals(fmt, actualItem, expectedItems[(itemsize - 1)]);
    }

  private:

    MOCKPP_STL::vector<T> actualItems;
    MOCKPP_STL::vector<T> expectedItems;
    bool           expectNothing;
    bool           haveActualValue;
};


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONLIST_H

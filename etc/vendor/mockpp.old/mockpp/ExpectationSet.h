/** @file
    @brief    Check sets of expectations

  $Id: ExpectationSet.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_EXPECTATIONSET_H
#define MOCKPP_EXPECTATIONSET_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H
#include MOCKPP_SET_H

#include <mockpp/AbstractExpectationCollection.h>
#include <mockpp/util/AssertMo.h>


MOCKPP_NS_START


/** A class to verify sets of expectations: each expected object must occur at
  * least once.
  * \ingroup grp_basic_mo
  */
template <class T>
class ExpectationSet : public AbstractExpectationCollection<T>
{

  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationSet(const String &name, VerifiableList *parent = 0)
      : AbstractExpectationCollection<T>(name, parent),
        expectNothing(false),
        haveActualValue(false)
    {
    }


  /** Adds another actual value to the list.
    * @param actualItem  new value
    */
    void addActual(const T &actualItem)
    {
      actualItems.insert(actualItem);
      haveActualValue = true;

      if (this->shouldCheckImmediately())
        checkImmediateValue(actualItem);
    }


  /** Adds a sequence of actual values to the set.
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
    ExpectationSet& addExpected(const T &expectedItem)
    {
      expectNothing = false;
      expectedItems.insert(expectedItem);
      this->setHasExpectations();
      return *this;
    }


  /** Adds a sequence of expectation values to the set.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ExpectationSet& addExpected(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addExpected(*items);
      return *this;
    }


  /**
    * Verify that the expected values equal the expected ones.
    * Note: in a set there is always only one element of a given value and the order
    * of the elements is not relevant.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void verify()
    {
      if( expectNothing)
      {
        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
        fmt << getVerifiableName();
        MOCKPP_ASSERT_FALSE_MESSAGE(fmt, haveActualValue );
      }
      else
      {
        if (!this->hasExpectations() )
          return;

        String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected a value."));
        fmt << getVerifiableName();

        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, haveActualValue );

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive the expected number of collection items."));
        fmt << getVerifiableName();

        MOCKPP_ASSERT_TRUE_MESSAGE(fmt,actualItems.size() == expectedItems.size());

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive the expected item set."));
        fmt << getVerifiableName();

        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, actualItems == expectedItems);
      }
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
      actualItems.clear();
    }


    AbstractExpectationCollection<T>::getVerifiableName;

  protected:

  /**
    * Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      expectedItems.clear();
      this->clearHasExpectations();
    }


  /**
    * Checks if the actual value matches the expectation.
    * Note: in a set there is always only one element of a given value and the order
    * of the elements is not relevant.
    * If it fails, an AssertionFailedError is thrown
    */
    virtual void checkImmediateValue(const T &actualItem) const
    {
      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive an expected item.\nUnexpected: %2"));
      fmt << getVerifiableName() << actualItem;

      MOCKPP_ASSERT_TRUE_MESSAGE(fmt,
          MOCKPP_STL::find(expectedItems.begin(), expectedItems.end(), actualItem) != expectedItems.end());
    }

  private:

    MOCKPP_STL::set<T> actualItems;
    MOCKPP_STL::set<T> expectedItems;
    bool        expectNothing;
    bool        haveActualValue;
};



MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONSET_H


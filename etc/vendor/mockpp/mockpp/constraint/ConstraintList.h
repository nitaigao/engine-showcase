/** @file
    @brief    Check list of constraints

  $Id: ConstraintList.h 1439 2008-05-09 13:33:16Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Sat Feb 26 2005
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

#ifndef MOCKPP_ConstraintList_H
#define MOCKPP_ConstraintList_H

#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H
#include MOCKPP_VECTOR_H

#include <mockpp/AbstractExpectationCollection.h>

#include <mockpp/util/AssertMo.h>

#include <mockpp/constraint/Constraint.h>
#include <mockpp/constraint/ConstraintHolder.h>
#include <mockpp/constraint/IsEqual.h>
#include <mockpp/constraint/IsAnything.h>


MOCKPP_NS_START


/** A class to verify lists of constraints, one after the other as they occur.
  * This class is similar to ExpectationList but does not compare exactly but based on
  * one of the constraint classes.
  * \ingroup grp_basic_mo
  */
template <class T>
class ConstraintList : public AbstractExpectationCollection<T>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ConstraintList(const String &name, VerifiableList *parent = 0)
      : AbstractExpectationCollection<T>(name, parent),
        expectNothing(false),
        haveActualValue(false)
    {
    }

  /** Destroys the expectation.
    */
    virtual ~ConstraintList()
    {
      reset();
    }

    typedef bool (Constraint<T>::*Checker)(const T &arg ) const;

  /** Verify that the expected values equal the expected ones.
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

        fmt = mockpp_i18n(MOCKPP_PCHAR("%1 did not receive the expected amount of values."));
        fmt << this->getVerifiableName();
        MOCKPP_ASSERT_TRUE_MESSAGE(fmt, actualItems.size() == constraints.size());

        for (unsigned i = 0; i < actualItems.size(); ++i)
          checkImmediateValue(actualItems[i], i, &Constraint<T>::verify);
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


  /** Adds another constraint to the list.
    * @param constraint   pointer to new constraint
    * @return reference to itself for chaining
    */
    ConstraintList& addExpected(const ConstraintHolder<T> &constraint)
    {
      typename Constraint<T>::AP cons (constraint);
      constraints.push_back(cons.release());
      expectNothing = false;
      this->setHasExpectations();
      return *this;
    }


  /** Adds another exact expectation value to the list.
    * @param expectedItem  new value
    * @return reference to itself for chaining
    */
    ConstraintList& addExpected(const T &expectedItem)
    {
      const ConstraintHolder<T> h = new IsEqual<T>(expectedItem);
      return addExpected(h);
    }


  /** Simulate one correct item regarding the expectation list.
    * Useful after recovering from an error at another place.
    */
    void balanceActual()
    {
      if (actualItems.size() < constraints.size())
      {
        haveActualValue = true;
        delete constraints[actualItems.size()];
        constraints[actualItems.size()] = new TypelessConstraintAdapter<T>(new IsAnything);
        actualItems.push_back(T());
      }
    }


  /** Adds a sequence of expectation values to the list.
    * @param items    start iterator
    * @param end      terminating iterator (note: one element "behind" as always with STL)
    * @return reference to itself for chaining
    */
    template <class I>
    ConstraintList& addExpected(I items, I end)
    {
      for ( /* -- */; items != end; ++items)
        addExpected(T(*items));
      return *this;
    }


  /** Resets the internal state to reflect that there is no actual value set.
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


  /** Returns the amount of expectation values.
    * @return number of expectation values
    */
    unsigned size() const
    {
      return constraints.size();
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
      clearExpectation();
      this->setHasExpectations();
    }


  protected:

  /** Clears the expectation list.
    */
    virtual void clearExpectation()
    {
      this->clearHasExpectations();
      for (unsigned i =0; i < constraints.size(); ++i)
        delete constraints[i];
      constraints.clear();
    }


  /** Checks if the actual value matches the expectation.
    * Note: in a list there can be any number of elements of a given value and the order
    * of the elements must match exactly.
    * If it fails, an AssertionFailedError is thrown
    * @param actualItem  actual item to verify
    */
    void checkImmediateValue(const T &actualItem) const
    {
      checkImmediateValue(actualItem, actualItems.size()-1, &Constraint<T>::eval);
    }


  /** Checks if the actual value matches the expectation.
    * Note: in a list there can be any number of elements of a given value and the order
    * of the elements must match exactly.
    * If it fails, an AssertionFailedError is thrown
    * @param actualItem  actual item to verify
    * @param pos         current index
    */
    void checkImmediateValue(const T &actualItem, unsigned pos, Checker checker) const
    {
      String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 had different item sizes.\n")
                        MOCKPP_PCHAR("Expected %2 items but received %3 when adding %4."));
      fmt << this->getVerifiableName() << constraints.size() << (pos+1) << actualItem;

      MOCKPP_ASSERT_TRUE_MESSAGE(fmt, constraints.size() > pos);

      String s;
      fmt = mockpp_i18n(MOCKPP_PCHAR("%1 added constraint[%2] does not match: <%3> != %4"));
      fmt << this->getVerifiableName() << pos << constraints[pos]->describeTo(s) << actualItem;
      MOCKPP_ASSERT_TRUE_MESSAGE(fmt, (constraints[pos]->*checker)(actualItem));
    }

  private:

    ConstraintList<T> (const ConstraintList<T> &);    // forbid due to internal pointers
    ConstraintList<T>& operator=(ConstraintList<T>&);

    MOCKPP_STL::vector<T>                 actualItems;
    MOCKPP_STL::vector<Constraint<T>*>    constraints;
    bool                                  expectNothing;
    bool                                  haveActualValue;
};


MOCKPP_NS_END


#endif // MOCKPP_ConstraintList_H

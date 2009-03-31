/** @file
    @brief    Check range of of expected calls

  $Id: ExpectationCounter.h 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#ifndef MOCKPP_EXPECTATIONCOUNTER_H
#define MOCKPP_EXPECTATIONCOUNTER_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>


MOCKPP_NS_START


/** A class to verify the expected count of an event.
  * \ingroup grp_basic_mo
  */
class MOCKPP_API_DECL0 ExpectationCounter : public AbstractExpectation<unsigned>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent verifiable
    */
    ExpectationCounter(const String &name, VerifiableList *parent = 0);

  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual();


  /** Resets the internal state completely.
    */
    virtual void reset();

  /** Increments the actual number of calls by 1.
    */
    void inc();


  /** Sets the exact number of expected calls.
    * @param  expectedCalls   number of expected calls
    */
    void setExpected(unsigned  expectedCalls);


  /** Sets the range of expected calls.
    * @param  expectedMinCalls   lower bound of expected calls
    * @param  expectedMaxCalls   upper bound of expected calls
    */
    void setExpected(unsigned expectedMinCalls, unsigned expectedMaxCalls);


  /** Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    void setExpectNothing();


  /** Gets the number of actual calls.
    * @return   counter value.
    */
    unsigned getActual() const;


   /** Verify that the amount of calls is within the allowed bounds.
     * If it fails, an AssertionFailedError is thrown
     */
    virtual void verify();

  protected:

  /** Clears the expectation list.
    */
    virtual void clearExpectation();

  private:

    unsigned myExpectedMinCalls;
    unsigned myExpectedMaxCalls;
    unsigned myActualCalls;
    bool     expectNothing;
};


MOCKPP_NS_END


#endif // MOCKPP_EXPECTATIONCOUNTER_H

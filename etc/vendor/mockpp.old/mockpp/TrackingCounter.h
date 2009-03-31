/** @file
    @brief    Track occurrences based on a shared counter

  $Id: TrackingCounter.h 1437 2008-03-08 20:12:53Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************

   begin                : Fri Feb 18 2005
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

#ifndef MOCKPP_TrackingCounter_H
#define MOCKPP_TrackingCounter_H


#include <mockpp/mockpp.h> // always first

#include <mockpp/AbstractExpectation.h>

#include MOCKPP_VECTOR_H


MOCKPP_NS_START


/** A base class to keep track of occurrences.
  * There a groups with related counter objects which are all based on a
  * shared counter value.
  * \ingroup grp_basic_mo
  * \ingroup grp_poor_mo
  */
class MOCKPP_API_DECL0 TrackingCounterBase : public AbstractExpectation<unsigned>
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent TrackingCounterClient
    */
    TrackingCounterBase(const String &name, VerifiableList *parent = 0);

  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual() = 0;

  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void reset();

  /** Sets the exact number of expected value.
    * @param  expectedValue   number of expected value
    */
    void setExpected(unsigned  expectedValue);

  /** Sets the range of expected value.
    * @param  expectedMinValue   lower bound of expected value
    * @param  expectedMaxValue   upper bound of expected value
    */
    void setExpected(unsigned expectedMinValue, unsigned expectedMaxValue);

  /** Tell the object to expect nothing to happen to it, perhaps because the test is exercising
    * the handling of an error. The Expectation will fail if any actual values are set.
    *
    * Note that this is not the same as not setting any expectations, in which case verify()
    * will do nothing.
    */
    void setExpectNothing();

  /** Gets the number of actual Value.
    * @return   counter value.
    */
    virtual unsigned getActual() const = 0;

  protected:

  /** Gets the expected minimal count.
    * @return   counter value.
    */
    unsigned getExpectedMinValue() const;

  /** Gets the expected maximal count.
    * @return   counter value.
    */
    unsigned getExpectedMaxValue() const;

  /** Clears the expectation list.
    */
    virtual void clearExpectation();

  /** Tests if the object expects nothing.
    */
    virtual bool isExpectNothing() const;

  private:

    unsigned myExpectedMinVal;
    unsigned myExpectedMaxVal;
    bool     expectNothing;
};


class TrackingCounterClient;


/** The master counter from which clients obtain the current occurence count.
  * \ingroup grp_basic_mo
  * \ingroup grp_poor_mo
  */
class MOCKPP_API_DECL0 TrackingCounterMaster : public TrackingCounterBase
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param parent  parent TrackingCounterClient
    */
    TrackingCounterMaster(const String &name, VerifiableList *parent = 0);

   /** Verify that the amount of Value is within the allowed bounds.
     * If it fails, an AssertionFailedError is thrown
     */
    virtual void verify();

  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual();

  /** Gets the number of actual Value.
    * @return   counter value.
    */
    virtual unsigned getActual() const;

  /** Gets the number of registered clients.
    * @return number of clients
    */
    unsigned numClients () const;

  protected:

    friend class TrackingCounterClient;

  /** Adds another TrackingCounterClient object to the list.
    * @param vf   pointer to the TrackingCounterClient object
    */
    virtual void addClient (TrackingCounterClient* vf);

  /** Removes a TrackingCounterClient object from the list.
    * @param vf   pointer to the TrackingCounterClient object
    */
    void removeClient (TrackingCounterClient* vf);

  /** Increments the counter value by 1 and returns it.
    */
    unsigned increment();

  private:

    TrackingCounterMaster (const TrackingCounterMaster &);    // forbid
    TrackingCounterMaster& operator=(TrackingCounterMaster&);

    MOCKPP_STL::vector<TrackingCounterClient*>  clients;
    unsigned myActualValue;
};


/** The client counter to track occurrences based on a shared counter.
  * This class stores all the actual master counter values when invoked.
  * \ingroup grp_basic_mo
  * \ingroup grp_poor_mo
  */
class MOCKPP_API_DECL0 TrackingCounterClient : public TrackingCounterBase
{
  public:

  /** Constructs the expectation.
    * @param name    human readable description about the expectation
    * @param master  the according master counter
    */
    TrackingCounterClient(const String &name, TrackingCounterMaster &master);

  /** Destructs the expectation.
    */
    virtual ~TrackingCounterClient();

  /** Stores and returns the current master count value.
    */
    unsigned inc();

  /** Stores and returns the current master count value.
    * Replacement for \see inc().
    */
    unsigned operator++ ()
    {
      return inc();
    }

  /** Stores and returns the current master count value.
    * Replacement for \see inc().
    */
    unsigned operator++ (int)
    {
      return inc();
    }

   /** Verify that the amount of Value is within the allowed bounds.
     * If it fails, an AssertionFailedError is thrown
     */
    virtual void verify();

  /** Resets the internal state to reflect that there is no actual value set.
    */
    virtual void clearActual();

  /** Gets the number of actual Value.
    * @return   counter value.
    */
    virtual unsigned getActual() const;

  /** Gets the number of counter transfers from the master.
    */
    unsigned numTags() const;

  /** Gets the value of a conter transfer.
    */
    unsigned getTag(unsigned index) const;

 private:

    TrackingCounterClient (const TrackingCounterClient &);    // forbid
    TrackingCounterClient& operator=(TrackingCounterClient&);

    MOCKPP_STL::vector<unsigned>   countervalues;
    TrackingCounterMaster  *master;
    unsigned                myActualValue;
};


MOCKPP_NS_END


#endif // MOCKPP_TrackingCounter_H

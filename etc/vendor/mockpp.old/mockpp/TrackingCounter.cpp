/** @file
    @brief    Track occurrences based on a shared counter

  $Id: TrackingCounter.cpp 1437 2008-03-08 20:12:53Z ewald-arnold $

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

#define MOCKPP_NEED_EXPORTS
#include <mockpp/mockpp.h> // always first

#include MOCKPP_ALGORITHM_H

#include <mockpp/TrackingCounter.h>
#include <mockpp/VerifiableList.h>

#include <mockpp/compat/Asserter.h>

#include <mockpp/compat/Formatter.h>

MOCKPP_NS_START


MOCKPP_API_IMPL0
TrackingCounterBase::TrackingCounterBase(const String &name, VerifiableList *parent)
  : AbstractExpectation<unsigned>(name, parent)
{
  expectNothing = false;
  myExpectedMinVal = 0;
  myExpectedMaxVal = 0;
}


MOCKPP_API_IMPL(unsigned) TrackingCounterBase::getExpectedMinValue() const
{
  return myExpectedMinVal;
}


MOCKPP_API_IMPL(unsigned) TrackingCounterBase::getExpectedMaxValue() const
{
  return myExpectedMaxVal;
}


MOCKPP_API_IMPL(void) TrackingCounterBase::clearExpectation()
{
  clearHasExpectations();
}


MOCKPP_API_IMPL(void) TrackingCounterBase::reset()
{
  clearActual();
  clearExpectation();
  this->clearHasExpectations();
}


MOCKPP_API_IMPL(void)
TrackingCounterBase::setExpected(unsigned expectedMinValue, unsigned expectedMaxValue)
{
  expectNothing = false;
  MOCKPP_ASSERT_TRUE(expectedMinValue <= expectedMaxValue);
  myExpectedMinVal = expectedMinValue;
  myExpectedMaxVal = expectedMaxValue;
  setHasExpectations();
}


MOCKPP_API_IMPL(void) TrackingCounterBase::setExpected(unsigned expectedValue)
{
  setExpected(expectedValue, expectedValue);
}


MOCKPP_API_IMPL(bool) TrackingCounterBase::isExpectNothing() const
{
  return expectNothing;
}


MOCKPP_API_IMPL(void) TrackingCounterBase::setExpectNothing()
{
  myExpectedMinVal = 0;
  myExpectedMaxVal = 0;
  setHasExpectations();
  expectNothing = true;
}


//////////////////////////////////////////////


MOCKPP_API_IMPL0 TrackingCounterMaster::TrackingCounterMaster(const String &name, VerifiableList *parent)
  : TrackingCounterBase(name, parent)
{
  myActualValue = 0;
}


MOCKPP_API_IMPL(void) TrackingCounterMaster::verify()
{
  for (unsigned i = 0; i < clients.size(); ++i)
    clients[i]->verify();

  if (!this->hasExpectations() )
    return;

  if( isExpectNothing() )
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
    fmt << this->getVerifiableName();

    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualValue == 0 );
  }
  else
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was tagged %2 times which is less than the expected count of %3."));
    fmt << getVerifiableName() << myActualValue << getExpectedMinValue();
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, getExpectedMinValue() <= myActualValue);

    fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was tagged %2 times which is more than the expected count of %3."));
    fmt << getVerifiableName() << myActualValue << getExpectedMaxValue();
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, getExpectedMaxValue() >= myActualValue);
  }
}


MOCKPP_API_IMPL(unsigned) TrackingCounterMaster::numClients () const
{
  return clients.size();
}


MOCKPP_API_IMPL(void) TrackingCounterMaster::addClient (TrackingCounterClient* vf)
{
  if (vf != 0)
    clients.push_back(vf);
}


MOCKPP_API_IMPL(void) TrackingCounterMaster::removeClient(TrackingCounterClient* vf)
{
  MOCKPP_STL::vector<TrackingCounterClient*>::iterator it =
    MOCKPP_STL::find (clients.begin(), clients.end(), vf);

  if(it != clients.end())
     clients.erase(it);
}


MOCKPP_API_IMPL(unsigned) TrackingCounterMaster::getActual() const
{
  return myActualValue;
}


MOCKPP_API_IMPL(void) TrackingCounterMaster::clearActual()
{
  myActualValue = 0;
  for (unsigned i = 0; i < clients.size(); ++i)
    clients[i]->clearActual();
}


MOCKPP_API_IMPL(unsigned) TrackingCounterMaster::increment()
{
  myActualValue++;
  if (shouldCheckImmediately())
  {
     String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was tagged %2 times but should not be called more than %3 times."));

     fmt << getVerifiableName() << myActualValue << getExpectedMaxValue();
     MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualValue <= getExpectedMaxValue());
  }

  return myActualValue;
}


//////////////////////////////////////////////


MOCKPP_API_IMPL0 TrackingCounterClient::TrackingCounterClient(const String &name, TrackingCounterMaster &in_master)
  : TrackingCounterBase(name, in_master.getParent())
{
  master = &in_master;
  master->addClient(this);
  clearActual();
}


MOCKPP_API_IMPL0 TrackingCounterClient::~TrackingCounterClient()
{
  master->removeClient(this);
}


MOCKPP_API_IMPL(void) TrackingCounterClient::clearActual()
{
  myActualValue = 0;
  countervalues.clear();
}


MOCKPP_API_IMPL(unsigned) TrackingCounterClient::getActual() const
{
  return myActualValue;
}


MOCKPP_API_IMPL(unsigned) TrackingCounterClient::getTag(unsigned index) const
{
  MOCKPP_ASSERT_TRUE(index < countervalues.size());
  return countervalues[index];
}


MOCKPP_API_IMPL(unsigned) TrackingCounterClient::numTags() const
{

  return countervalues.size();
}


MOCKPP_API_IMPL(void) TrackingCounterClient::verify()
{
  if (!this->hasExpectations() )
    return;

  if( isExpectNothing() )
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 expected no value."));
    fmt << this->getVerifiableName();

    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualValue == 0 );
  }
  else
  {
    String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was last tagged with %2 which is less than the expected count of %3."));
    fmt << getVerifiableName() << myActualValue << getExpectedMinValue();
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, getExpectedMinValue() <= myActualValue);

    fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was last tagged with %2 which is more than the expected count of %3."));
    fmt << getVerifiableName() << myActualValue << getExpectedMaxValue();
    MOCKPP_ASSERT_TRUE_MESSAGE(fmt, getExpectedMaxValue() >= myActualValue);
  }
}


MOCKPP_API_IMPL(unsigned) TrackingCounterClient::inc()
{
  myActualValue = master->increment();
  countervalues.push_back(myActualValue);
  if (shouldCheckImmediately())
  {
     String fmt = mockpp_i18n(MOCKPP_PCHAR("%1 was tagged with %2 but should not be tagged more than %3 times."));

     fmt << getVerifiableName() << myActualValue << getExpectedMaxValue();
     MOCKPP_ASSERT_TRUE_MESSAGE(fmt, myActualValue <= getExpectedMaxValue());
  }
  return myActualValue;
}


MOCKPP_NS_END
